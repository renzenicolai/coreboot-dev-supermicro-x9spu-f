/* Copyright (c) 2013 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * High-level firmware wrapper API - entry points for kernel selection
 */

#include "sysincludes.h"

#include "2sysincludes.h"
#include "2common.h"
#include "2rsa.h"
#include "gbb_access.h"
#include "gbb_header.h"
#include "load_kernel_fw.h"
#include "region.h"
#include "rollback_index.h"
#include "utility.h"
#include "vb2_common.h"
#include "vboot_api.h"
#include "vboot_audio.h"
#include "vboot_common.h"
#include "vboot_display.h"
#include "vboot_kernel.h"
#include "vboot_nvstorage.h"

/* Global variables */
static VbNvContext vnc;
static struct RollbackSpaceFwmp fwmp;

#ifdef CHROMEOS_ENVIRONMENT
/* Global variable accessors for unit tests */

VbNvContext *VbApiKernelGetVnc(void)
{
	return &vnc;
}

struct RollbackSpaceFwmp *VbApiKernelGetFwmp(void)
{
	return &fwmp;
}
#endif

/**
 * Set recovery request (called from vboot_api_kernel.c functions only)
 */
static void VbSetRecoveryRequest(uint32_t recovery_request)
{
	VBDEBUG(("VbSetRecoveryRequest(%d)\n", (int)recovery_request));
	VbNvSet(&vnc, VBNV_RECOVERY_REQUEST, recovery_request);
}

static void VbSetRecoverySubcode(uint32_t recovery_request)
{
	VBDEBUG(("VbSetRecoverySubcode(%d)\n", (int)recovery_request));
	VbNvSet(&vnc, VBNV_RECOVERY_SUBCODE, recovery_request);
}

static void VbNvCommit(void)
{
	VbNvTeardown(&vnc);
	if (vnc.raw_changed)
		VbExNvStorageWrite(vnc.raw);
}

static void VbAllowUsbBoot(void)
{
	VBDEBUG(("%s\n", __func__));
	VbNvSet(&vnc, VBNV_DEV_BOOT_USB, 1);
}

/**
 * Checks GBB flags against VbExIsShutdownRequested() shutdown request to
 * determine if a shutdown is required.
 *
 * Returns true if a shutdown is required and false if no shutdown is required.
 */
static int VbWantShutdown(uint32_t gbb_flags)
{
	uint32_t shutdown_request = VbExIsShutdownRequested();

	/* If desired, ignore shutdown request due to lid closure. */
	if (gbb_flags & GBB_FLAG_DISABLE_LID_SHUTDOWN)
		shutdown_request &= ~VB_SHUTDOWN_REQUEST_LID_CLOSED;

	return !!shutdown_request;
}

static void VbTryLegacy(int allowed)
{
	if (!allowed)
		VBDEBUG(("VbBootDeveloper() - Legacy boot is disabled\n"));
	else if (0 != RollbackKernelLock(0))
		VBDEBUG(("Error locking kernel versions on legacy boot.\n"));
	else
		VbExLegacy();	/* will not return if successful */

	/* If legacy boot fails, beep and return to calling UI loop. */
	VbExBeep(120, 400);
	VbExSleepMs(120);
	VbExBeep(120, 400);
}

/**
 * Attempt loading a kernel from the specified type(s) of disks.
 *
 * If successful, sets p->disk_handle to the disk for the kernel and returns
 * VBERROR_SUCCESS.
 *
 * Returns VBERROR_NO_DISK_FOUND if no disks of the specified type were found.
 *
 * May return other VBERROR_ codes for other failures.
 */
uint32_t VbTryLoadKernel(VbCommonParams *cparams, LoadKernelParams *p,
                         uint32_t get_info_flags)
{
	VbError_t retval = VBERROR_UNKNOWN;
	VbDiskInfo* disk_info = NULL;
	uint32_t disk_count = 0;
	uint32_t i;

	VBDEBUG(("VbTryLoadKernel() start, get_info_flags=0x%x\n",
		 (unsigned)get_info_flags));

	p->disk_handle = NULL;

	/* Find disks */
	if (VBERROR_SUCCESS != VbExDiskGetInfo(&disk_info, &disk_count,
					       get_info_flags))
		disk_count = 0;

	VBDEBUG(("VbTryLoadKernel() found %d disks\n", (int)disk_count));
	if (0 == disk_count) {
		VbSetRecoveryRequest(VBNV_RECOVERY_RW_NO_DISK);
		return VBERROR_NO_DISK_FOUND;
	}

	/* Loop over disks */
	for (i = 0; i < disk_count; i++) {
		VBDEBUG(("VbTryLoadKernel() trying disk %d\n", (int)i));
		/*
		 * Sanity-check what we can. FWIW, VbTryLoadKernel() is always
		 * called with only a single bit set in get_info_flags.
		 *
		 * Ensure 512-byte sectors and non-trivially sized disk (for
		 * cgptlib) and that we got a partition with only the flags we
		 * asked for.
		 */
		if (512 != disk_info[i].bytes_per_lba ||
		    16 > disk_info[i].lba_count ||
		    get_info_flags != (disk_info[i].flags & ~VB_DISK_FLAG_EXTERNAL_GPT)) {
			VBDEBUG(("  skipping: bytes_per_lba=%" PRIu64
				 " lba_count=%" PRIu64 " flags=0x%x\n",
				 disk_info[i].bytes_per_lba,
				 disk_info[i].lba_count,
				 disk_info[i].flags));
			continue;
		}
		p->disk_handle = disk_info[i].handle;
		p->bytes_per_lba = disk_info[i].bytes_per_lba;
		p->gpt_lba_count = disk_info[i].lba_count;
		p->streaming_lba_count = disk_info[i].streaming_lba_count
						?: p->gpt_lba_count;
		p->boot_flags |= disk_info[i].flags & VB_DISK_FLAG_EXTERNAL_GPT
				? BOOT_FLAG_EXTERNAL_GPT : 0;
		retval = LoadKernel(p, cparams);
		VBDEBUG(("VbTryLoadKernel() LoadKernel() = %d\n", retval));

		/*
		 * Stop now if we found a kernel.
		 *
		 * TODO: If recovery requested, should track the farthest we
		 * get, instead of just returning the value from the last disk
		 * attempted.
		 */
		if (VBERROR_SUCCESS == retval)
			break;
	}

	/* If we didn't find any good kernels, don't return a disk handle. */
	if (VBERROR_SUCCESS != retval) {
		VbSetRecoveryRequest(VBNV_RECOVERY_RW_NO_KERNEL);
		p->disk_handle = NULL;
	}

	VbExDiskFreeInfo(disk_info, p->disk_handle);

	/*
	 * Pass through return code.  Recovery reason (if any) has already been
	 * set by LoadKernel().
	 */
	return retval;
}

uint32_t VbTryUsb(VbCommonParams *cparams, LoadKernelParams *p)
{
	uint32_t retval = VbTryLoadKernel(cparams, p, VB_DISK_FLAG_REMOVABLE);
	if (VBERROR_SUCCESS == retval) {
		VBDEBUG(("VbBootDeveloper() - booting USB\n"));
	} else {
		VBDEBUG(("VbBootDeveloper() - no kernel found on USB\n"));
		VbExBeep(250, 200);
		VbExSleepMs(120);
		/*
		 * Clear recovery requests from failed
		 * kernel loading, so that powering off
		 * at this point doesn't put us into
		 * recovery mode.
		 */
		VbSetRecoveryRequest(
			VBNV_RECOVERY_NOT_REQUESTED);
	}
	return retval;
}

#define CONFIRM_KEY_DELAY 20  /* Check confirm screen keys every 20ms */

int VbUserConfirms(VbCommonParams *cparams, uint32_t confirm_flags)
{
	VbSharedDataHeader *shared =
           (VbSharedDataHeader *)cparams->shared_data_blob;
	uint32_t key;
	uint32_t key_flags;
        uint32_t button;
	int rec_button_was_pressed = 0;

	VBDEBUG(("Entering %s(0x%x)\n", __func__, confirm_flags));

	/* Await further instructions */
	while (1) {
		if (VbWantShutdown(cparams->gbb->flags))
			return -1;
		key = VbExKeyboardReadWithFlags(&key_flags);
                button = VbExGetSwitches(VB_INIT_FLAG_REC_BUTTON_PRESSED);
		switch (key) {
		case '\r':
			/* If we require a trusted keyboard for confirmation,
			 * but the keyboard may be faked (for instance, a USB
			 * device), beep and keep waiting.
			 */
			if (confirm_flags & VB_CONFIRM_MUST_TRUST_KEYBOARD &&
			    !(key_flags & VB_KEY_FLAG_TRUSTED_KEYBOARD)) {
				VbExBeep(120, 400);
				break;
                        }

			VBDEBUG(("%s() - Yes (1)\n", __func__));
			return 1;
			break;
		case ' ':
			VBDEBUG(("%s() - Space (%d)\n", __func__,
				 confirm_flags & VB_CONFIRM_SPACE_MEANS_NO));
			if (confirm_flags & VB_CONFIRM_SPACE_MEANS_NO)
				return 0;
			break;
		case 0x1b:
			VBDEBUG(("%s() - No (0)\n", __func__));
			return 0;
			break;
		default:
			/* If the recovery button is physical, and is pressed,
			 * this is also a YES, but must wait for release.
			 */
			if (!(shared->flags & VBSD_BOOT_REC_SWITCH_VIRTUAL)) {
				if (button) {
					VBDEBUG(("%s() - Rec button pressed\n",
						 __func__));
	                                rec_button_was_pressed = 1;
				} else if (rec_button_was_pressed) {
					VBDEBUG(("%s() - Rec button (1)\n",
					 __func__));
					return 1;
				}
			}
			VbCheckDisplayKey(cparams, key, &vnc);
		}
		VbExSleepMs(CONFIRM_KEY_DELAY);
	}

	/* Not reached, but compiler will complain without it */
	return -1;
}

VbError_t test_mockable
VbBootNormal(VbCommonParams *cparams, LoadKernelParams *p)
{
	/* Boot from fixed disk only */
	VBDEBUG(("Entering %s()\n", __func__));
	return VbTryLoadKernel(cparams, p, VB_DISK_FLAG_FIXED);
}

static const char dev_disable_msg[] =
	"Developer mode is disabled on this device by system policy.\n"
	"For more information, see http://dev.chromium.org/chromium-os/fwmp\n"
	"\n";

VbError_t VbBootDeveloper(VbCommonParams *cparams, LoadKernelParams *p)
{
	GoogleBinaryBlockHeader *gbb = cparams->gbb;
	VbSharedDataHeader *shared =
		(VbSharedDataHeader *)cparams->shared_data_blob;

	uint32_t allow_usb = 0;
	uint32_t allow_legacy = 0;
	uint32_t disable_dev_boot = 0;
	uint32_t use_usb = 0;
	uint32_t use_legacy = 0;
	uint32_t default_boot = 0;
	uint32_t ctrl_d_pressed = 0;

	VbAudioContext *audio = 0;

	VBDEBUG(("Entering %s()\n", __func__));

	/* Check if USB booting is allowed */
	VbNvGet(&vnc, VBNV_DEV_BOOT_USB, &allow_usb);
	VbNvGet(&vnc, VBNV_DEV_BOOT_LEGACY, &allow_legacy);

	/* Check if the default is to boot using disk, usb, or legacy */
	VbNvGet(&vnc, VBNV_DEV_DEFAULT_BOOT, &default_boot);

	if(default_boot == VBNV_DEV_DEFAULT_BOOT_USB)
		use_usb = 1;
	if(default_boot == VBNV_DEV_DEFAULT_BOOT_LEGACY)
		use_legacy = 1;

	/* Handle GBB flag override */
	if (gbb->flags & GBB_FLAG_FORCE_DEV_BOOT_USB)
		allow_usb = 1;
	if (gbb->flags & GBB_FLAG_FORCE_DEV_BOOT_LEGACY)
		allow_legacy = 1;
	if (gbb->flags & GBB_FLAG_DEFAULT_DEV_BOOT_LEGACY) {
		use_legacy = 1;
		use_usb = 0;
	}

	/* Handle FWMP override */
	if (fwmp.flags & FWMP_DEV_ENABLE_USB)
		allow_usb = 1;
	if (fwmp.flags & FWMP_DEV_ENABLE_LEGACY)
		allow_legacy = 1;
	if (fwmp.flags & FWMP_DEV_DISABLE_BOOT) {
		if (gbb->flags & GBB_FLAG_FORCE_DEV_SWITCH_ON) {
			VBDEBUG(("%s() - FWMP_DEV_DISABLE_BOOT rejected by "
				 "FORCE_DEV_SWITCH_ON\n",
				 __func__));
		} else {
			disable_dev_boot = 1;
		}
	}

	/* If dev mode is disabled, only allow TONORM */
	while (disable_dev_boot) {
		VBDEBUG(("%s() - dev_disable_boot is set.\n", __func__));
		VbDisplayScreen(cparams, VB_SCREEN_DEVELOPER_TO_NORM, 0, &vnc);
		VbExDisplayDebugInfo(dev_disable_msg);

		/* Ignore space in VbUserConfirms()... */
		switch (VbUserConfirms(cparams, 0)) {
		case 1:
			VBDEBUG(("%s() - leaving dev-mode.\n", __func__));
			VbNvSet(&vnc, VBNV_DISABLE_DEV_REQUEST, 1);
			VbDisplayScreen(cparams,
					VB_SCREEN_TO_NORM_CONFIRMED,
					0, &vnc);
			VbExSleepMs(5000);
			return VBERROR_REBOOT_REQUIRED;
		case -1:
			VBDEBUG(("%s() - shutdown requested\n", __func__));
			return VBERROR_SHUTDOWN_REQUESTED;
		default:
			/* Ignore user attempt to cancel */
			VBDEBUG(("%s() - ignore cancel TONORM\n", __func__));
		}
	}

	/* Show the dev mode warning screen */
	VbDisplayScreen(cparams, VB_SCREEN_DEVELOPER_WARNING, 0, &vnc);

	/* Get audio/delay context */
	audio = VbAudioOpen(cparams);

	/* We'll loop until we finish the delay or are interrupted */
	do {
		uint32_t key;

		if (VbWantShutdown(gbb->flags)) {
			VBDEBUG(("VbBootDeveloper() - shutdown requested!\n"));
			VbAudioClose(audio);
			return VBERROR_SHUTDOWN_REQUESTED;
		}

		key = VbExKeyboardRead();
		switch (key) {
		case 0:
			/* nothing pressed */
			break;
		case '\r':
			/* Only disable virtual dev switch if allowed by GBB */
			if (!(gbb->flags & GBB_FLAG_ENTER_TRIGGERS_TONORM))
				break;
		case ' ':
			/* See if we should disable virtual dev-mode switch. */
			VBDEBUG(("%s shared->flags=0x%x\n",
				 __func__, shared->flags));
			if (shared->flags & VBSD_HONOR_VIRT_DEV_SWITCH &&
			    shared->flags & VBSD_BOOT_DEV_SWITCH_ON) {
				/* Stop the countdown while we go ask... */
				VbAudioClose(audio);
				if (gbb->flags & GBB_FLAG_FORCE_DEV_SWITCH_ON) {
					/*
					 * TONORM won't work (only for
					 * non-shipping devices).
					 */
					VBDEBUG(("%s() - TONORM rejected by "
						 "FORCE_DEV_SWITCH_ON\n",
						 __func__));
					VbExDisplayDebugInfo(
						"WARNING: TONORM prohibited by "
						"GBB FORCE_DEV_SWITCH_ON.\n\n");
					VbExBeep(120, 400);
					break;
				}
				VbDisplayScreen(cparams,
						VB_SCREEN_DEVELOPER_TO_NORM,
						0, &vnc);
				/* Ignore space in VbUserConfirms()... */
				switch (VbUserConfirms(cparams, 0)) {
				case 1:
					VBDEBUG(("%s() - leaving dev-mode.\n",
						 __func__));
					VbNvSet(&vnc, VBNV_DISABLE_DEV_REQUEST,
						1);
					VbDisplayScreen(
						cparams,
						VB_SCREEN_TO_NORM_CONFIRMED,
						0, &vnc);
					VbExSleepMs(5000);
					return VBERROR_REBOOT_REQUIRED;
				case -1:
					VBDEBUG(("%s() - shutdown requested\n",
						 __func__));
					return VBERROR_SHUTDOWN_REQUESTED;
				default:
					/* Stay in dev-mode */
					VBDEBUG(("%s() - stay in dev-mode\n",
						 __func__));
					VbDisplayScreen(
						cparams,
						VB_SCREEN_DEVELOPER_WARNING,
						0, &vnc);
					/* Start new countdown */
					audio = VbAudioOpen(cparams);
				}
			} else {
				/*
				 * No virtual dev-mode switch, so go directly
				 * to recovery mode.
				 */
				VBDEBUG(("%s() - going to recovery\n",
					 __func__));
				VbSetRecoveryRequest(
					VBNV_RECOVERY_RW_DEV_SCREEN);
				VbAudioClose(audio);
				return VBERROR_LOAD_KERNEL_RECOVERY;
			}
			break;
		case 0x04:
			/* Ctrl+D = dismiss warning; advance to timeout */
			VBDEBUG(("VbBootDeveloper() - "
				 "user pressed Ctrl+D; skip delay\n"));
			ctrl_d_pressed = 1;
			goto fallout;
			break;
		case 0x0c:
			VBDEBUG(("VbBootDeveloper() - "
				 "user pressed Ctrl+L; Try legacy boot\n"));
			VbTryLegacy(allow_legacy);
			break;

		case VB_KEY_CTRL_ENTER:
			/*
			 * The Ctrl-Enter is special for Lumpy test purpose;
			 * fall through to Ctrl+U handler.
			 */
		case 0x15:
			/* Ctrl+U = try USB boot, or beep if failure */
			VBDEBUG(("VbBootDeveloper() - "
				 "user pressed Ctrl+U; try USB\n"));
			if (!allow_usb) {
				VBDEBUG(("VbBootDeveloper() - "
					 "USB booting is disabled\n"));
				VbExDisplayDebugInfo(
					"WARNING: Booting from external media "
					"(USB/SD) has not been enabled. Refer "
					"to the developer-mode documentation "
					"for details.\n");
				VbExBeep(120, 400);
				VbExSleepMs(120);
				VbExBeep(120, 400);
			} else {
				/*
				 * Clear the screen to show we get the Ctrl+U
				 * key press.
				 */
				VbDisplayScreen(cparams, VB_SCREEN_BLANK, 0,
						&vnc);
				if (VBERROR_SUCCESS == VbTryUsb(cparams, p)) {
					VbAudioClose(audio);
					return VBERROR_SUCCESS;
				} else {
					/* Show dev mode warning screen again */
					VbDisplayScreen(
						cparams,
						VB_SCREEN_DEVELOPER_WARNING,
						0, &vnc);
				}
			}
			break;
		default:
			VBDEBUG(("VbBootDeveloper() - pressed key %d\n", key));
			VbCheckDisplayKey(cparams, key, &vnc);
			break;
		}
	} while(VbAudioLooping(audio));

 fallout:

	/* If defaulting to legacy boot, try that unless Ctrl+D was pressed */
	if (use_legacy && !ctrl_d_pressed) {
		VBDEBUG(("VbBootDeveloper() - defaulting to legacy\n"));
		VbTryLegacy(allow_legacy);
	}

	if ((use_usb && !ctrl_d_pressed) && allow_usb) {
		if (VBERROR_SUCCESS == VbTryUsb(cparams, p)) {
			VbAudioClose(audio);
			return VBERROR_SUCCESS;
		}
	}

	/* Timeout or Ctrl+D; attempt loading from fixed disk */
	VBDEBUG(("VbBootDeveloper() - trying fixed disk\n"));
	VbAudioClose(audio);
	return VbTryLoadKernel(cparams, p, VB_DISK_FLAG_FIXED);
}

/* Delay in recovery mode */
#define REC_DISK_DELAY       1000     /* Check disks every 1s */
#define REC_KEY_DELAY        20       /* Check keys every 20ms */
#define REC_MEDIA_INIT_DELAY 500      /* Check removable media every 500ms */

VbError_t VbBootRecovery(VbCommonParams *cparams, LoadKernelParams *p)
{
	VbSharedDataHeader *shared =
		(VbSharedDataHeader *)cparams->shared_data_blob;
	uint32_t retval;
	uint32_t key;
	int i;

	VBDEBUG(("VbBootRecovery() start\n"));

	/*
	 * If the dev-mode switch is off and the user didn't press the recovery
	 * button (recovery was triggerred automatically), show 'broken' screen.
	 * The user can either only shutdown to abort or hit esc+refresh+power
	 * to initiate recovery as instructed on the screen.
	 */
	if (!(shared->flags & VBSD_BOOT_DEV_SWITCH_ON) &&
	    !(shared->flags & VBSD_BOOT_REC_SWITCH_ON)) {
		/*
		 * We have to save the reason here so that it will survive
		 * coming up three-finger-salute. We're saving it in
		 * VBNV_RECOVERY_SUBCODE to avoid a recovery loop.
		 * If we save the reason in VBNV_RECOVERY_REQUEST, we will come
		 * back here, thus, we won't be able to give a user a chance to
		 * reboot to workaround boot hicups.
		 */
		VBDEBUG(("VbBootRecovery() saving recovery reason (%#x)\n",
				shared->recovery_reason));
		VbSetRecoverySubcode(shared->recovery_reason);
		VbNvCommit();
		VbDisplayScreen(cparams, VB_SCREEN_OS_BROKEN, 0, &vnc);
		VBDEBUG(("VbBootRecovery() waiting for manual recovery\n"));
		while (1) {
			VbCheckDisplayKey(cparams, VbExKeyboardRead(), &vnc);
			if (VbWantShutdown(cparams->gbb->flags))
				return VBERROR_SHUTDOWN_REQUESTED;
			VbExSleepMs(REC_KEY_DELAY);
		}
	}

	/* Loop and wait for a recovery image */
	VBDEBUG(("VbBootRecovery() waiting for a recovery image\n"));
	while (1) {
		VBDEBUG(("VbBootRecovery() attempting to load kernel2\n"));
		retval = VbTryLoadKernel(cparams, p, VB_DISK_FLAG_REMOVABLE);

		/*
		 * Clear recovery requests from failed kernel loading, since
		 * we're already in recovery mode.  Do this now, so that
		 * powering off after inserting an invalid disk doesn't leave
		 * us stuck in recovery mode.
		 */
		VbSetRecoveryRequest(VBNV_RECOVERY_NOT_REQUESTED);

		if (VBERROR_SUCCESS == retval)
			break; /* Found a recovery kernel */

		VbDisplayScreen(cparams, VBERROR_NO_DISK_FOUND == retval ?
				VB_SCREEN_RECOVERY_INSERT :
				VB_SCREEN_RECOVERY_NO_GOOD,
				0, &vnc);

		/*
		 * Scan keyboard more frequently than media, since x86
		 * platforms don't like to scan USB too rapidly.
		 */
		for (i = 0; i < REC_DISK_DELAY; i += REC_KEY_DELAY) {
			key = VbExKeyboardRead();
			/*
			 * We might want to enter dev-mode from the Insert
			 * screen if all of the following are true:
			 *   - user pressed Ctrl-D
			 *   - we can honor the virtual dev switch
			 *   - not already in dev mode
			 *   - user forced recovery mode
			 *   - EC isn't pwned
			 */
			if (key == 0x04 &&
			    shared->flags & VBSD_HONOR_VIRT_DEV_SWITCH &&
			    !(shared->flags & VBSD_BOOT_DEV_SWITCH_ON) &&
			    (shared->flags & VBSD_BOOT_REC_SWITCH_ON) &&
			    VbExTrustEC(0)) {
                                if (!(shared->flags &
				      VBSD_BOOT_REC_SWITCH_VIRTUAL) &&
				    VbExGetSwitches(
					     VB_INIT_FLAG_REC_BUTTON_PRESSED)) {
					/*
					 * Is the recovery button stuck?  In
					 * any case we don't like this.  Beep
					 * and ignore.
					 */
					VBDEBUG(("%s() - ^D but rec switch "
						 "is pressed\n", __func__));
					VbExBeep(120, 400);
					continue;
				}

				/* Ask the user to confirm entering dev-mode */
				VbDisplayScreen(cparams,
						VB_SCREEN_RECOVERY_TO_DEV,
						0, &vnc);
				/* SPACE means no... */
				uint32_t vbc_flags =
					VB_CONFIRM_SPACE_MEANS_NO |
					VB_CONFIRM_MUST_TRUST_KEYBOARD;
				switch (VbUserConfirms(cparams, vbc_flags)) {
				case 1:
					VBDEBUG(("%s() Enabling dev-mode...\n",
						 __func__));
					if (TPM_SUCCESS != SetVirtualDevMode(1))
						return VBERROR_TPM_SET_BOOT_MODE_STATE;
					VBDEBUG(("%s() Reboot so it will take "
						 "effect\n", __func__));
					if (VbExGetSwitches
					    (VB_INIT_FLAG_ALLOW_USB_BOOT))
						VbAllowUsbBoot();
					return VBERROR_REBOOT_REQUIRED;
				case -1:
					VBDEBUG(("%s() - Shutdown requested\n",
						 __func__));
					return VBERROR_SHUTDOWN_REQUESTED;
				default: /* zero, actually */
					VBDEBUG(("%s() - Not enabling "
						 "dev-mode\n", __func__));
					/*
					 * Jump out of the outer loop to
					 * refresh the display quickly.
					 */
					i = 4;
					break;
				}
			} else {
				VbCheckDisplayKey(cparams, key, &vnc);
			}
			if (VbWantShutdown(cparams->gbb->flags))
				return VBERROR_SHUTDOWN_REQUESTED;
			VbExSleepMs(REC_KEY_DELAY);
		}
	}

	return VBERROR_SUCCESS;
}

/**
 * Wrapper around VbExEcProtect() which sets recovery reason on error.
 */
static VbError_t EcProtect(int devidx, enum VbSelectFirmware_t select)
{
	int rv = VbExEcProtect(devidx, select);

	if (rv == VBERROR_EC_REBOOT_TO_RO_REQUIRED) {
		VBDEBUG(("VbExEcProtect() needs reboot\n"));
	} else if (rv != VBERROR_SUCCESS) {
		VBDEBUG(("VbExEcProtect() returned %d\n", rv));
		VbSetRecoveryRequest(VBNV_RECOVERY_EC_PROTECT);
	}
	return rv;
}

static VbError_t EcUpdateImage(int devidx, VbCommonParams *cparams,
			       enum VbSelectFirmware_t select,
			       int *need_update, int in_rw)
{
	VbSharedDataHeader *shared =
		(VbSharedDataHeader *)cparams->shared_data_blob;
	int rv;
	int hash_size;
	int ec_hash_size;
	const uint8_t *hash = NULL;
	const uint8_t *expected = NULL;
	const uint8_t *ec_hash = NULL;
	int expected_size;
	int i;
	int rw_request = select != VB_SELECT_FIRMWARE_READONLY;

	*need_update = 0;
	VBDEBUG(("EcUpdateImage() - "
		 "Check for %s update\n", rw_request ? "RW" : "RO"));

	/* Get current EC hash. */
	rv = VbExEcHashImage(devidx, select, &ec_hash, &ec_hash_size);
	if (rv) {
		VBDEBUG(("EcUpdateImage() - "
			 "VbExEcHashImage() returned %d\n", rv));
		VbSetRecoveryRequest(VBNV_RECOVERY_EC_HASH_FAILED);
		return VBERROR_EC_REBOOT_TO_RO_REQUIRED;
	}
	VBDEBUG(("EC-%s hash: ", rw_request ? "RW" : "RO"));
	for (i = 0; i < ec_hash_size; i++)
		VBDEBUG(("%02x",ec_hash[i]));
	VBDEBUG(("\n"));

	/* Get expected EC hash. */
	rv = VbExEcGetExpectedImageHash(devidx, select, &hash, &hash_size);
	if (rv) {
		VBDEBUG(("EcUpdateImage() - "
			 "VbExEcGetExpectedImageHash() returned %d\n", rv));
		VbSetRecoveryRequest(VBNV_RECOVERY_EC_EXPECTED_HASH);
		return VBERROR_EC_REBOOT_TO_RO_REQUIRED;
	}
	if (ec_hash_size != hash_size) {
		VBDEBUG(("EcUpdateImage() - "
			 "EC uses %d-byte hash, but AP-RW contains %d bytes\n",
			 ec_hash_size, hash_size));
		VbSetRecoveryRequest(VBNV_RECOVERY_EC_HASH_SIZE);
		return VBERROR_EC_REBOOT_TO_RO_REQUIRED;
	}

	VBDEBUG(("Expected hash: "));
	for (i = 0; i < hash_size; i++)
		VBDEBUG(("%02x", hash[i]));
	VBDEBUG(("\n"));
	*need_update = vb2_safe_memcmp(ec_hash, hash, hash_size);

	if (!*need_update)
		return VBERROR_SUCCESS;

	/* Get expected EC image */
	rv = VbExEcGetExpectedImage(devidx, select, &expected, &expected_size);
	if (rv) {
		VBDEBUG(("EcUpdateImage() - "
			 "VbExEcGetExpectedImage() returned %d\n", rv));
		VbSetRecoveryRequest(VBNV_RECOVERY_EC_EXPECTED_IMAGE);
		return VBERROR_EC_REBOOT_TO_RO_REQUIRED;
	}
	VBDEBUG(("EcUpdateImage() - image len = %d\n", expected_size));

	if (in_rw && rw_request) {
		/*
		 * Check if BIOS should also load VGA Option ROM when
		 * rebooting to save another reboot if possible.
		 */
		if ((shared->flags & VBSD_EC_SLOW_UPDATE) &&
		    (shared->flags & VBSD_OPROM_MATTERS) &&
		    !(shared->flags & VBSD_OPROM_LOADED)) {
			VBDEBUG(("EcUpdateImage() - Reboot to "
				 "load VGA Option ROM\n"));
			VbNvSet(&vnc, VBNV_OPROM_NEEDED, 1);
		}

		/*
		 * EC is running the wrong RW image.  Reboot the EC to
		 * RO so we can update it on the next boot.
		 */
		VBDEBUG(("EcUpdateImage() - "
			 "in RW, need to update RW, so reboot\n"));
		return VBERROR_EC_REBOOT_TO_RO_REQUIRED;
	}

	VBDEBUG(("EcUpdateImage() updating EC-%s...\n",
		 rw_request ? "RW" : "RO"));

	if (shared->flags & VBSD_EC_SLOW_UPDATE) {
		VBDEBUG(("EcUpdateImage() - EC is slow. Show WAIT screen.\n"));

		/* Ensure the VGA Option ROM is loaded */
		if ((shared->flags & VBSD_OPROM_MATTERS) &&
		    !(shared->flags & VBSD_OPROM_LOADED)) {
			VBDEBUG(("EcUpdateImage() - Reboot to "
				 "load VGA Option ROM\n"));
			VbNvSet(&vnc, VBNV_OPROM_NEEDED, 1);
			return VBERROR_VGA_OPROM_MISMATCH;
		}

		VbDisplayScreen(cparams, VB_SCREEN_WAIT, 0, &vnc);
	}

	rv = VbExEcUpdateImage(devidx, select, expected, expected_size);
	if (rv != VBERROR_SUCCESS) {
		VBDEBUG(("EcUpdateImage() - "
			 "VbExEcUpdateImage() returned %d\n", rv));

		/*
		 * The EC may know it needs a reboot.  It may need to
		 * unprotect the region before updating, or may need to
		 * reboot after updating.  Either way, it's not an error
		 * requiring recovery mode.
		 *
		 * If we fail for any other reason, trigger recovery
		 * mode.
		 */
		if (rv != VBERROR_EC_REBOOT_TO_RO_REQUIRED)
			VbSetRecoveryRequest(VBNV_RECOVERY_EC_UPDATE);

		return VBERROR_EC_REBOOT_TO_RO_REQUIRED;
	}

	/* Verify the EC was updated properly */
	rv = VbExEcHashImage(devidx, select, &ec_hash, &ec_hash_size);
	if (rv) {
		VBDEBUG(("EcUpdateImage() - "
			 "VbExEcHashImage() returned %d\n", rv));
		VbSetRecoveryRequest(VBNV_RECOVERY_EC_HASH_FAILED);
		return VBERROR_EC_REBOOT_TO_RO_REQUIRED;
	}
	if (hash_size != ec_hash_size) {
		VBDEBUG(("EcUpdateImage() - "
			 "VbExEcHashImage() says size %d, not %d\n",
			 ec_hash_size, hash_size));
		VbSetRecoveryRequest(VBNV_RECOVERY_EC_HASH_SIZE);
		return VBERROR_EC_REBOOT_TO_RO_REQUIRED;
	}
	VBDEBUG(("Updated EC-%s hash: ", rw_request ? "RW" : "RO"));
	for (i = 0; i < ec_hash_size; i++)
		VBDEBUG(("%02x",ec_hash[i]));
	VBDEBUG(("\n"));

	if (vb2_safe_memcmp(ec_hash, hash, hash_size)){
		VBDEBUG(("EcUpdateImage() - "
			 "Failed to update EC-%s\n", rw_request ?
			 "RW" : "RO"));
		VbSetRecoveryRequest(VBNV_RECOVERY_EC_UPDATE);
		return VBERROR_EC_REBOOT_TO_RO_REQUIRED;
	}

	return VBERROR_SUCCESS;
}

VbError_t VbEcSoftwareSync(int devidx, VbCommonParams *cparams)
{
	VbSharedDataHeader *shared =
		(VbSharedDataHeader *)cparams->shared_data_blob;
	enum VbSelectFirmware_t select_rw =
		shared->firmware_index ? VB_SELECT_FIRMWARE_B :
		VB_SELECT_FIRMWARE_A;
	enum VbSelectFirmware_t select_ro = VB_SELECT_FIRMWARE_READONLY;
	int in_rw = 0;
	int ro_try_count = 2;
	int num_tries = 0;
	uint32_t try_ro_sync, recovery_request;
	int rv, updated_rw, updated_ro;

	VBDEBUG(("VbEcSoftwareSync(devidx=%d)\n", devidx));

	/* Determine whether the EC is in RO or RW */
	rv = VbExEcRunningRW(devidx, &in_rw);

	if (shared->recovery_reason) {
		/* Recovery mode; just verify the EC is in RO code */
		if (rv == VBERROR_SUCCESS && in_rw == 1) {
			/*
			 * EC is definitely in RW firmware.  We want it in
			 * read-only code, so preserve the current recovery
			 * reason and reboot.
			 *
			 * We don't reboot on error or unknown EC code, because
			 * we could end up in an endless reboot loop.  If we
			 * had some way to track that we'd already rebooted for
			 * this reason, we could retry only once.
			 */
			VBDEBUG(("VbEcSoftwareSync() - "
				 "want recovery but got EC-RW\n"));
			VbSetRecoveryRequest(shared->recovery_reason);
			return VBERROR_EC_REBOOT_TO_RO_REQUIRED;
		}

		VBDEBUG(("VbEcSoftwareSync() in recovery; EC-RO\n"));
		return VBERROR_SUCCESS;
	}

	/*
	 * Not in recovery.  If we couldn't determine where the EC was,
	 * reboot to recovery.
	 */
	if (rv != VBERROR_SUCCESS) {
		VBDEBUG(("VbEcSoftwareSync() - "
			 "VbExEcRunningRW() returned %d\n", rv));
		VbSetRecoveryRequest(VBNV_RECOVERY_EC_UNKNOWN_IMAGE);
		return VBERROR_EC_REBOOT_TO_RO_REQUIRED;
	}

	/* If AP is read-only normal, EC should be in its RO code also. */
	if (shared->flags & VBSD_LF_USE_RO_NORMAL) {
		/* If EC is in RW code, request reboot back to RO */
		if (in_rw == 1) {
			VBDEBUG(("VbEcSoftwareSync() - "
				 "want RO-normal but got EC-RW\n"));
			return VBERROR_EC_REBOOT_TO_RO_REQUIRED;
		}

		/* Protect the RW flash and stay in EC-RO */
		rv = EcProtect(devidx, select_rw);
		if (rv != VBERROR_SUCCESS)
			return rv;

		rv = VbExEcDisableJump(devidx);
		if (rv != VBERROR_SUCCESS) {
			VBDEBUG(("VbEcSoftwareSync() - "
				 "VbExEcDisableJump() returned %d\n", rv));
			VbSetRecoveryRequest(VBNV_RECOVERY_EC_SOFTWARE_SYNC);
			return VBERROR_EC_REBOOT_TO_RO_REQUIRED;
		}

		VBDEBUG(("VbEcSoftwareSync() in RO-Normal; EC-RO\n"));
		return VBERROR_SUCCESS;
	}

	VBDEBUG(("VbEcSoftwareSync() check for RW update.\n"));

	/* Update the RW Image. */
	rv = EcUpdateImage(devidx, cparams, select_rw, &updated_rw, in_rw);

	if (rv != VBERROR_SUCCESS) {
		VBDEBUG(("VbEcSoftwareSync() - "
			 "EcUpdateImage() returned %d\n", rv));
		return rv;
	}

	/* Tell EC to jump to its RW image */
	if (!in_rw) {
		VBDEBUG(("VbEcSoftwareSync() jumping to EC-RW\n"));
		rv = VbExEcJumpToRW(devidx);
		if (rv != VBERROR_SUCCESS) {
			VBDEBUG(("VbEcSoftwareSync() - "
				 "VbExEcJumpToRW() returned %x\n", rv));

			/*
			 * If the EC booted RO-normal and a previous AP boot
			 * has called VbExEcStayInRO(), we need to reboot the EC
			 * to unlock the ability to jump to the RW firmware.
			 *
			 * All other errors trigger recovery mode.
			 */
			if (rv != VBERROR_EC_REBOOT_TO_RO_REQUIRED)
				VbSetRecoveryRequest(VBNV_RECOVERY_EC_JUMP_RW);

			return VBERROR_EC_REBOOT_TO_RO_REQUIRED;
		}
	}

	VbNvGet(&vnc, VBNV_TRY_RO_SYNC, &try_ro_sync);

	if (!devidx && try_ro_sync &&
	    !(shared->flags & VBSD_BOOT_FIRMWARE_WP_ENABLED)) {
		/* Reset RO Software Sync NV flag */
		VbNvSet(&vnc, VBNV_TRY_RO_SYNC, 0);

		VbNvGet(&vnc, VBNV_RECOVERY_REQUEST, &recovery_request);

		/* Update the RO Image. */
		while (num_tries < ro_try_count) {
			VBDEBUG(("VbEcSoftwareSync() RO Software Sync\n"));

			/* Get expected EC-RO Image. */
			rv = EcUpdateImage(devidx, cparams, select_ro,
					   &updated_ro, in_rw);
			if (rv == VBERROR_SUCCESS) {
				/*
				 * If the RO update had failed, reset the
				 * recovery request.
				 */
				if (num_tries)
					VbSetRecoveryRequest(recovery_request);
				break;
			} else
				VBDEBUG(("VbEcSoftwareSync() - "
					 "EcUpdateImage() returned %d\n", rv));

			num_tries++;
		}
	}
	if (rv != VBERROR_SUCCESS)
		return rv;

	/* Protect RO flash */
	rv = EcProtect(devidx, select_ro);
	if (rv != VBERROR_SUCCESS)
		return rv;

	/* Protect RW flash */
	rv = EcProtect(devidx, select_rw);
	if (rv != VBERROR_SUCCESS)
		return rv;

	rv = VbExEcDisableJump(devidx);
	if (rv != VBERROR_SUCCESS) {
		VBDEBUG(("VbEcSoftwareSync() - "
			"VbExEcDisableJump() returned %d\n", rv));
		VbSetRecoveryRequest(VBNV_RECOVERY_EC_SOFTWARE_SYNC);
		return VBERROR_EC_REBOOT_TO_RO_REQUIRED;
	}

	/*
	 * Reboot to unload VGA Option ROM if:
	 * - RW update was done
	 * - the system is NOT in developer mode
	 * - the system has slow EC update flag set
	 * - the VGA Option ROM was needed and loaded
	 */
	if (updated_rw &&
	    !(shared->flags & VBSD_BOOT_DEV_SWITCH_ON) &&
	    (shared->flags & VBSD_EC_SLOW_UPDATE) &&
	    (shared->flags & VBSD_OPROM_MATTERS) &&
	    (shared->flags & VBSD_OPROM_LOADED)) {
		VBDEBUG(("VbEcSoftwareSync() - Reboot to "
			 "unload VGA Option ROM\n"));
		VbNvSet(&vnc, VBNV_OPROM_NEEDED, 0);
		return VBERROR_VGA_OPROM_MISMATCH;
	}


	return rv;
}

/* This function is also used by tests */
void VbApiKernelFree(VbCommonParams *cparams)
{
	/* VbSelectAndLoadKernel() always allocates this, tests don't */
	if (cparams->gbb) {
		free(cparams->gbb);
		cparams->gbb = NULL;
	}
	if (cparams->bmp) {
		free(cparams->bmp);
		cparams->bmp = NULL;
	}
}

VbError_t VbSelectAndLoadKernel(VbCommonParams *cparams,
                                VbSelectAndLoadKernelParams *kparams)
{
	VbSharedDataHeader *shared =
		(VbSharedDataHeader *)cparams->shared_data_blob;
	VbError_t retval = VBERROR_SUCCESS;
	LoadKernelParams p;
	uint32_t tpm_status = 0;
	uint32_t battery_cutoff = 0;

	/* Start timer */
	shared->timer_vb_select_and_load_kernel_enter = VbExGetTimer();

	VbExNvStorageRead(vnc.raw);
	VbNvSetup(&vnc);

	/* Fill in params for calls to LoadKernel() */
	memset(&p, 0, sizeof(p));
	p.shared_data_blob = cparams->shared_data_blob;
	p.shared_data_size = cparams->shared_data_size;
	p.gbb_data = cparams->gbb_data;
	p.gbb_size = cparams->gbb_size;
	p.fwmp = &fwmp;
	p.nv_context = &vnc;

	/*
	 * This could be set to NULL, in which case the vboot header
	 * information about the load address and size will be used.
	 */
	p.kernel_buffer = kparams->kernel_buffer;
	p.kernel_buffer_size = kparams->kernel_buffer_size;

	/* Clear output params in case we fail */
	kparams->disk_handle = NULL;
	kparams->partition_number = 0;
	kparams->bootloader_address = 0;
	kparams->bootloader_size = 0;
	kparams->flags = 0;
	memset(kparams->partition_guid, 0, sizeof(kparams->partition_guid));

	cparams->bmp = NULL;
	cparams->gbb = malloc(sizeof(*cparams->gbb));
	retval = VbGbbReadHeader_static(cparams, cparams->gbb);
	if (VBERROR_SUCCESS != retval)
		goto VbSelectAndLoadKernel_exit;

	/* Do EC software sync if necessary */
	if ((shared->flags & VBSD_EC_SOFTWARE_SYNC) &&
	    !(cparams->gbb->flags & GBB_FLAG_DISABLE_EC_SOFTWARE_SYNC)) {
		int oprom_mismatch = 0;

		retval = VbEcSoftwareSync(0, cparams);
		/* Save reboot requested until after possible PD sync */
		if (retval == VBERROR_VGA_OPROM_MISMATCH)
			oprom_mismatch = 1;
		else if (retval != VBERROR_SUCCESS)
			goto VbSelectAndLoadKernel_exit;

#ifdef PD_SYNC
		if (!(cparams->gbb->flags &
		      GBB_FLAG_DISABLE_PD_SOFTWARE_SYNC)) {
			retval = VbEcSoftwareSync(1, cparams);
			if (retval == VBERROR_VGA_OPROM_MISMATCH)
				oprom_mismatch = 1;
			else if (retval != VBERROR_SUCCESS)
				goto VbSelectAndLoadKernel_exit;
		}
#endif

		/* Request reboot to unload VGA Option ROM */
		if (oprom_mismatch) {
			retval = VBERROR_VGA_OPROM_MISMATCH;
			goto VbSelectAndLoadKernel_exit;
		}
	}

	/* EC verification (and possibily updating / jumping) is done */
	retval = VbExEcVbootDone(!!shared->recovery_reason);
	if (retval != VBERROR_SUCCESS)
		goto VbSelectAndLoadKernel_exit;

	/* Check if we need to cut-off battery. This must be done after EC
         * firmware updating and before kernel started. */
	VbNvGet(&vnc, VBNV_BATTERY_CUTOFF_REQUEST, &battery_cutoff);
	if (battery_cutoff) {
		VBDEBUG(("Request to cut-off battery\n"));
		VbNvSet(&vnc, VBNV_BATTERY_CUTOFF_REQUEST, 0);
		VbExEcBatteryCutOff();
		retval = VBERROR_SHUTDOWN_REQUESTED;
		goto VbSelectAndLoadKernel_exit;
	}

	/* Read kernel version from the TPM.  Ignore errors in recovery mode. */
	tpm_status = RollbackKernelRead(&shared->kernel_version_tpm);
	if (0 != tpm_status) {
		VBDEBUG(("Unable to get kernel versions from TPM\n"));
		if (!shared->recovery_reason) {
			VbSetRecoveryRequest(VBNV_RECOVERY_RW_TPM_R_ERROR);
			retval = VBERROR_TPM_READ_KERNEL;
			goto VbSelectAndLoadKernel_exit;
		}
	}
	shared->kernel_version_tpm_start = shared->kernel_version_tpm;

	/* Read FWMP.  Ignore errors in recovery mode. */
	if (cparams->gbb->flags & GBB_FLAG_DISABLE_FWMP) {
		memset(&fwmp, 0, sizeof(fwmp));
		tpm_status = 0;
	} else {
		tpm_status = RollbackFwmpRead(&fwmp);
	}
	if (0 != tpm_status) {
		VBDEBUG(("Unable to get FWMP from TPM\n"));
		if (!shared->recovery_reason) {
			VbSetRecoveryRequest(VBNV_RECOVERY_RW_TPM_R_ERROR);
			retval = VBERROR_TPM_READ_FWMP;
			goto VbSelectAndLoadKernel_exit;
		}
	}

	/* Set up boot flags */
	p.boot_flags = 0;
	if (shared->flags & VBSD_BOOT_DEV_SWITCH_ON)
		p.boot_flags |= BOOT_FLAG_DEVELOPER;

	/* Handle separate normal and developer firmware builds. */
#if defined(VBOOT_FIRMWARE_TYPE_NORMAL)
	/* Normal-type firmware always acts like the dev switch is off. */
	p.boot_flags &= ~BOOT_FLAG_DEVELOPER;
#elif defined(VBOOT_FIRMWARE_TYPE_DEVELOPER)
	/* Developer-type firmware fails if the dev switch is off. */
	if (!(p.boot_flags & BOOT_FLAG_DEVELOPER)) {
		/*
		 * Dev firmware should be signed with a key that only verifies
		 * when the dev switch is on, so we should never get here.
		 */
		VBDEBUG(("Developer firmware called with dev switch off!\n"));
		VbSetRecoveryRequest(VBNV_RECOVERY_RW_DEV_MISMATCH);
		retval = VBERROR_DEV_FIRMWARE_SWITCH_MISMATCH;
		goto VbSelectAndLoadKernel_exit;
	}
#else
	/*
	 * Recovery firmware, or merged normal+developer firmware.  No need to
	 * override flags.
	 */
#endif

	/* Select boot path */
	if (shared->recovery_reason == VBNV_RECOVERY_TRAIN_AND_REBOOT) {
		/* Reboot requested by user recovery code. */
		VBDEBUG(("Reboot requested by user (recovery_reason=%d).\n",
			 shared->recovery_reason));
		retval = VBERROR_REBOOT_REQUIRED;
	} else if (shared->recovery_reason) {
		/* Recovery boot */
		p.boot_flags |= BOOT_FLAG_RECOVERY;
		retval = VbBootRecovery(cparams, &p);
		VbExEcEnteringMode(0, VB_EC_RECOVERY);
		VbDisplayScreen(cparams, VB_SCREEN_BLANK, 0, &vnc);

	} else if (p.boot_flags & BOOT_FLAG_DEVELOPER) {
		/* Developer boot */
		retval = VbBootDeveloper(cparams, &p);
		VbExEcEnteringMode(0, VB_EC_DEVELOPER);
		VbDisplayScreen(cparams, VB_SCREEN_BLANK, 0, &vnc);

	} else {
		/* Normal boot */
		VbExEcEnteringMode(0, VB_EC_NORMAL);
		retval = VbBootNormal(cparams, &p);

		if ((1 == shared->firmware_index) &&
		    (shared->flags & VBSD_FWB_TRIED)) {
			/*
			 * Special cases for when we're trying a new firmware
			 * B.  These are needed because firmware updates also
			 * usually change the kernel key, which means that the
			 * B firmware can only boot a new kernel, and the old
			 * firmware in A can only boot the previous kernel.
			 *
			 * Don't advance the TPM if we're trying a new firmware
			 * B, because we don't yet know if the new kernel will
			 * successfully boot.  We still want to be able to fall
			 * back to the previous firmware+kernel if the new
			 * firmware+kernel fails.
			 *
			 * If we found only invalid kernels, reboot and try
			 * again.  This allows us to fall back to the previous
			 * firmware+kernel instead of giving up and going to
			 * recovery mode right away.  We'll still go to
			 * recovery mode if we run out of tries and the old
			 * firmware can't find a kernel it likes.
			 */
			if (VBERROR_INVALID_KERNEL_FOUND == retval) {
				VBDEBUG(("Trying firmware B, "
					 "and only found invalid kernels.\n"));
				VbSetRecoveryRequest(VBNV_RECOVERY_NOT_REQUESTED);
				goto VbSelectAndLoadKernel_exit;
			}
		} else {
			/* Not trying a new firmware B. */

			/* See if we need to update the TPM. */
			VBDEBUG(("Checking if TPM kernel version needs "
				 "advancing\n"));
			if (shared->kernel_version_tpm >
			    shared->kernel_version_tpm_start) {
				tpm_status = RollbackKernelWrite(
						shared->kernel_version_tpm);
				if (0 != tpm_status) {
					VBDEBUG(("Error writing kernel "
						 "versions to TPM.\n"));
					VbSetRecoveryRequest(VBNV_RECOVERY_RW_TPM_W_ERROR);
					retval = VBERROR_TPM_WRITE_KERNEL;
					goto VbSelectAndLoadKernel_exit;
				}
			}
		}
	}

	if (VBERROR_SUCCESS != retval)
		goto VbSelectAndLoadKernel_exit;

	/* Save disk parameters */
	kparams->disk_handle = p.disk_handle;
	kparams->partition_number = (uint32_t)p.partition_number;
	kparams->bootloader_address = p.bootloader_address;
	kparams->bootloader_size = (uint32_t)p.bootloader_size;
	kparams->flags = p.flags;
	memcpy(kparams->partition_guid, p.partition_guid,
	       sizeof(kparams->partition_guid));

	/* Lock the kernel versions.  Ignore errors in recovery mode. */
	tpm_status = RollbackKernelLock(shared->recovery_reason);
	if (0 != tpm_status) {
		VBDEBUG(("Error locking kernel versions.\n"));
		if (!shared->recovery_reason) {
			VbSetRecoveryRequest(VBNV_RECOVERY_RW_TPM_L_ERROR);
			retval = VBERROR_TPM_LOCK_KERNEL;
			goto VbSelectAndLoadKernel_exit;
		}
	}

 VbSelectAndLoadKernel_exit:

	VbApiKernelFree(cparams);

	VbNvCommit();

	/* Stop timer */
	shared->timer_vb_select_and_load_kernel_exit = VbExGetTimer();

	kparams->kernel_buffer = p.kernel_buffer;
	kparams->kernel_buffer_size = p.kernel_buffer_size;

	VBDEBUG(("VbSelectAndLoadKernel() returning %d\n", (int)retval));

	/* Pass through return value from boot path */
	return retval;
}

VbError_t VbVerifyMemoryBootImage(VbCommonParams *cparams,
				  VbSelectAndLoadKernelParams *kparams,
				  void *boot_image,
				  size_t image_size)
{
	VbError_t retval;
	VbPublicKey* kernel_subkey = NULL;
	uint8_t *kbuf;
	VbKeyBlockHeader *key_block;
	VbSharedDataHeader *shared =
		(VbSharedDataHeader *)cparams->shared_data_blob;
	VbKernelPreambleHeader *preamble;
	uint64_t body_offset;
	int hash_only = 0;
	int dev_switch;
	uint32_t allow_fastboot_full_cap = 0;
	uint8_t *workbuf = NULL;
	struct vb2_workbuf wb;

	if ((boot_image == NULL) || (image_size == 0))
		return VBERROR_INVALID_PARAMETER;

	/* Clear output params in case we fail. */
	kparams->disk_handle = NULL;
	kparams->partition_number = 0;
	kparams->bootloader_address = 0;
	kparams->bootloader_size = 0;
	kparams->flags = 0;
	memset(kparams->partition_guid, 0, sizeof(kparams->partition_guid));

	kbuf = boot_image;

	/* Read GBB Header */
	cparams->bmp = NULL;
	cparams->gbb = malloc(sizeof(*cparams->gbb));
	retval = VbGbbReadHeader_static(cparams, cparams->gbb);
	if (VBERROR_SUCCESS != retval) {
		VBDEBUG(("Gbb read header failed.\n"));
		return retval;
	}

	/*
	 * We don't care verifying the image if:
	 * 1. dev-mode switch is on and
	 * 2a. GBB_FLAG_FORCE_DEV_BOOT_FASTBOOT_FULL_CAP is set, or
	 * 2b. DEV_BOOT_FASTBOOT_FULL_CAP flag is set in NvStorage
	 *
	 * Check only the integrity of the image.
	 */
	dev_switch = shared->flags & VBSD_BOOT_DEV_SWITCH_ON;

	VbExNvStorageRead(vnc.raw);
	VbNvSetup(&vnc);
	VbNvGet(&vnc, VBNV_DEV_BOOT_FASTBOOT_FULL_CAP,
		&allow_fastboot_full_cap);

	if (0 == allow_fastboot_full_cap) {
		allow_fastboot_full_cap = !!(cparams->gbb->flags &
				GBB_FLAG_FORCE_DEV_BOOT_FASTBOOT_FULL_CAP);
	}

	if (dev_switch && allow_fastboot_full_cap) {
		VBDEBUG(("Only performing integrity-check.\n"));
		hash_only = 1;
	} else {
		/* Get recovery key. */
		retval = VbGbbReadRecoveryKey(cparams, &kernel_subkey);
		if (VBERROR_SUCCESS != retval) {
			VBDEBUG(("Gbb Read Recovery key failed.\n"));
			return retval;
		}
	}

	/* If we fail at any step, retval returned would be invalid kernel. */
	retval = VBERROR_INVALID_KERNEL_FOUND;

	/* Allocate work buffer */
	workbuf = (uint8_t *)malloc(VB2_KERNEL_WORKBUF_RECOMMENDED_SIZE);
	if (!workbuf)
		goto fail;
	vb2_workbuf_init(&wb, workbuf, VB2_KERNEL_WORKBUF_RECOMMENDED_SIZE);

	/* Verify the key block. */
	key_block = (VbKeyBlockHeader *)kbuf;
	struct vb2_keyblock *keyblock2 = (struct vb2_keyblock *)kbuf;
	int rv;
	if (hash_only) {
		rv = vb2_verify_keyblock_hash(keyblock2, image_size, &wb);
	} else {
		/* Unpack kernel subkey */
		struct vb2_public_key kernel_subkey2;
		if (VB2_SUCCESS !=
		    vb2_unpack_key(&kernel_subkey2,
				   (struct vb2_packed_key *)kernel_subkey)) {
			VBDEBUG(("Unable to unpack kernel subkey\n"));
			goto fail;
		}
		rv = vb2_verify_keyblock(keyblock2, image_size,
					 &kernel_subkey2, &wb);
	}

	if (VB2_SUCCESS != rv) {
		VBDEBUG(("Verifying key block signature/hash failed.\n"));
		goto fail;
	}

	/* Check the key block flags against the current boot mode. */
	if (!(key_block->key_block_flags &
	      (dev_switch ? KEY_BLOCK_FLAG_DEVELOPER_1 :
	       KEY_BLOCK_FLAG_DEVELOPER_0))) {
		VBDEBUG(("Key block developer flag mismatch.\n"));
		if (hash_only == 0)
			goto fail;
	}

	if (!(key_block->key_block_flags & KEY_BLOCK_FLAG_RECOVERY_1)) {
		VBDEBUG(("Key block recovery flag mismatch.\n"));
		if (hash_only == 0)
			goto fail;
	}

	/* Get key for preamble/data verification from the key block. */
	struct vb2_public_key data_key2;
	if (VB2_SUCCESS != vb2_unpack_key(&data_key2, &keyblock2->data_key)) {
		VBDEBUG(("Unable to unpack kernel data key\n"));
		goto fail;
	}

	/* Verify the preamble, which follows the key block */
	preamble = (VbKernelPreambleHeader *)(kbuf + key_block->key_block_size);
	struct vb2_kernel_preamble *preamble2 =
			(struct vb2_kernel_preamble *)
			(kbuf + key_block->key_block_size);

	if (VB2_SUCCESS != vb2_verify_kernel_preamble(
			preamble2,
			image_size - key_block->key_block_size,
			&data_key2,
			&wb)) {
		VBDEBUG(("Preamble verification failed.\n"));
		goto fail;
	}

	VBDEBUG(("Kernel preamble is good.\n"));

	/* Verify kernel data */
	body_offset = key_block->key_block_size + preamble->preamble_size;
	if (VB2_SUCCESS != vb2_verify_data(
			(const uint8_t *)(kbuf + body_offset),
			image_size - body_offset,
			(struct vb2_signature *)&preamble->body_signature,
			&data_key2, &wb)) {
		VBDEBUG(("Kernel data verification failed.\n"));
		goto fail;
	}

	VBDEBUG(("Kernel is good.\n"));

	/* Fill in output parameters. */
	kparams->kernel_buffer = kbuf + body_offset;
	kparams->kernel_buffer_size = image_size - body_offset;
	kparams->bootloader_address = preamble->bootloader_address;
	kparams->bootloader_size = preamble->bootloader_size;
	if (VbKernelHasFlags(preamble) == VBOOT_SUCCESS)
		kparams->flags = preamble->flags;

	retval = VBERROR_SUCCESS;

fail:
	VbApiKernelFree(cparams);
	if (NULL != kernel_subkey)
		free(kernel_subkey);
	if (NULL != workbuf)
		free(workbuf);
	return retval;
}

VbError_t VbUnlockDevice(void)
{
	VBDEBUG(("%s() Enabling dev-mode...\n", __func__));
	if (TPM_SUCCESS != SetVirtualDevMode(1))
		return VBERROR_TPM_SET_BOOT_MODE_STATE;

	VBDEBUG(("%s() Mode change will take effect on next reboot.\n",
		 __func__));
	return VBERROR_SUCCESS;
}

VbError_t VbLockDevice(void)
{
	VbExNvStorageRead(vnc.raw);
	VbNvSetup(&vnc);

	VBDEBUG(("%s() - Storing request to leave dev-mode.\n",
		 __func__));
	VbNvSet(&vnc, VBNV_DISABLE_DEV_REQUEST,
		1);

	VbNvCommit();

	VBDEBUG(("%s() Mode change will take effect on next reboot.\n",
		 __func__));

	return VBERROR_SUCCESS;
}
