/*
 * Copyright 2016 Google Inc.
 *
 * Based on Linux Kernel TPM driver by
 * Peter Huewe <peter.huewe@infineon.com>
 * Copyright (C) 2011 Infineon Technologies
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/*
 * cr50 is a TPM 2.0 capable device that requries special
 * handling for the I2C interface.
 *
 * - Use an interrupt for transaction status instead of hardcoded delays
 * - Must use write+wait+read read protocol
 * - All 4 bytes of status register must be read/written at once
 * - Burst count max is 63 bytes, and burst count behaves
 *   slightly differently than other I2C TPMs
 * - When reading from FIFO the full burstcnt must be read
 *   instead of just reading header and determining the remainder
 */

#include <arch/early_variables.h>
#include <commonlib/endian.h>
#include <stdint.h>
#include <string.h>
#include <types.h>
#include <delay.h>
#include <console/console.h>
#include <device/i2c.h>
#include <endian.h>
#include <timer.h>
#include "tpm.h"

#if IS_ENABLED(CONFIG_ARCH_X86)
#include <arch/acpi.h>
#endif

#define CR50_MAX_BUFSIZE	63
#define CR50_TIMEOUT_LONG_MS	2000	/* Long timeout while waiting for TPM */
#define CR50_TIMEOUT_SHORT_MS	2	/* Short timeout during transactions */
#define CR50_TIMEOUT_NOIRQ_MS	20	/* Timeout for TPM ready without IRQ */
#define CR50_TIMEOUT_IRQ_MS	100	/* Timeout for TPM ready with IRQ */
#define CR50_DID_VID		0x00281ae0L

struct tpm_inf_dev {
	int bus;
	unsigned int addr;
	uint8_t buf[CR50_MAX_BUFSIZE + sizeof(uint8_t)];
};

static struct tpm_inf_dev g_tpm_dev CAR_GLOBAL;

/* Wait for interrupt to indicate the TPM is ready */
static int cr50_i2c_wait_tpm_ready(struct tpm_chip *chip)
{
	struct stopwatch sw;

	if (!chip->vendor.irq_status) {
		/* Fixed delay if interrupt not supported */
		mdelay(CR50_TIMEOUT_NOIRQ_MS);
		return 0;
	}

	stopwatch_init_msecs_expire(&sw, CR50_TIMEOUT_IRQ_MS);

	while (!chip->vendor.irq_status(chip->vendor.irq))
		if (stopwatch_expired(&sw))
			return -1;

	return 0;
}

/* Clear pending interrupts */
static void cr50_i2c_clear_tpm_irq(struct tpm_chip *chip)
{
	if (chip->vendor.irq_status)
		chip->vendor.irq_status(chip->vendor.irq);
}

/*
 * cr50_i2c_read() - read from TPM register
 *
 * @chip: TPM chip information
 * @addr: register address to read from
 * @buffer: provided by caller
 * @len: number of bytes to read
 *
 * 1) send register address byte 'addr' to the TPM
 * 2) wait for TPM to indicate it is ready
 * 3) read 'len' bytes of TPM response into the provided 'buffer'
 *
 * Return -1 on error, 0 on success.
 */
static int cr50_i2c_read(struct tpm_chip *chip, uint8_t addr,
			 uint8_t *buffer, size_t len)
{
	struct tpm_inf_dev *tpm_dev = car_get_var_ptr(&g_tpm_dev);

	if (tpm_dev->addr == 0)
		return -1;

	/* Clear interrupt before starting transaction */
	cr50_i2c_clear_tpm_irq(chip);

	/* Send the register address byte to the TPM */
	if (i2c_write_raw(tpm_dev->bus, tpm_dev->addr, &addr, 1)) {
		printk(BIOS_ERR, "%s: Address write failed\n", __func__);
		return -1;
	}

	/* Wait for TPM to be ready with response data */
	if (cr50_i2c_wait_tpm_ready(chip) < 0)
		return -1;

	/* Read response data from the TPM */
	if (i2c_read_raw(tpm_dev->bus, tpm_dev->addr, buffer, len)) {
		printk(BIOS_ERR, "%s: Read response failed\n", __func__);
		return -1;
	}

	return 0;
}

/*
 * cr50_i2c_write() - write to TPM register
 *
 * @chip: TPM chip information
 * @addr: register address to write to
 * @buffer: data to write
 * @len: number of bytes to write
 *
 * 1) prepend the provided address to the provided data
 * 2) send the address+data to the TPM
 * 3) wait for TPM to indicate it is done writing
 *
 * Returns -1 on error, 0 on success.
 */
static int cr50_i2c_write(struct tpm_chip *chip,
			  uint8_t addr, uint8_t *buffer, size_t len)
{
	struct tpm_inf_dev *tpm_dev = car_get_var_ptr(&g_tpm_dev);

	if (tpm_dev->addr == 0)
		return -1;
	if (len > CR50_MAX_BUFSIZE)
		return -1;

	/* Prepend the 'register address' to the buffer */
	tpm_dev->buf[0] = addr;
	memcpy(tpm_dev->buf + 1, buffer, len);

	/* Clear interrupt before starting transaction */
	cr50_i2c_clear_tpm_irq(chip);

	/* Send write request buffer with address */
	if (i2c_write_raw(tpm_dev->bus, tpm_dev->addr, tpm_dev->buf, len + 1)) {
		printk(BIOS_ERR, "%s: Error writing to TPM\n", __func__);
		return -1;
	}

	/* Wait for TPM to be ready */
	return cr50_i2c_wait_tpm_ready(chip);
}

static int check_locality(struct tpm_chip *chip, int loc)
{
	uint8_t mask = TPM_ACCESS_VALID | TPM_ACCESS_ACTIVE_LOCALITY;
	uint8_t buf;

	if (cr50_i2c_read(chip, TPM_ACCESS(loc), &buf, 1) < 0)
		return -1;

	if ((buf & mask) == mask) {
		chip->vendor.locality = loc;
		return loc;
	}

	return -1;
}

static void release_locality(struct tpm_chip *chip, int force)
{
	uint8_t mask = TPM_ACCESS_VALID | TPM_ACCESS_REQUEST_PENDING;
	uint8_t addr = TPM_ACCESS(chip->vendor.locality);
	uint8_t buf;

	if (cr50_i2c_read(chip, addr, &buf, 1) < 0)
		return;

	if (force || (buf & mask) == mask) {
		buf = TPM_ACCESS_ACTIVE_LOCALITY;
		cr50_i2c_write(chip, addr, &buf, 1);
	}

	chip->vendor.locality = 0;
}

static int request_locality(struct tpm_chip *chip, int loc)
{
	uint8_t buf = TPM_ACCESS_REQUEST_USE;
	struct stopwatch sw;

	if (check_locality(chip, loc) >= 0)
		return loc;

	if (cr50_i2c_write(chip, TPM_ACCESS(loc), &buf, 1) < 0)
		return -1;

	stopwatch_init_msecs_expire(&sw, CR50_TIMEOUT_LONG_MS);
	while (check_locality(chip, loc) < 0) {
		if (stopwatch_expired(&sw))
			return -1;
		mdelay(CR50_TIMEOUT_SHORT_MS);
	}
	return loc;
}

/* cr50 requires all 4 bytes of status register to be read */
static uint8_t cr50_i2c_tis_status(struct tpm_chip *chip)
{
	uint8_t buf[4];
	if (cr50_i2c_read(chip, TPM_STS(chip->vendor.locality),
			  buf, sizeof(buf)) < 0) {
		printk(BIOS_ERR, "%s: Failed to read status\n", __func__);
		return 0;
	}
	return buf[0];
}

/* cr50 requires all 4 bytes of status register to be written */
static void cr50_i2c_tis_ready(struct tpm_chip *chip)
{
	uint8_t buf[4] = { TPM_STS_COMMAND_READY };
	cr50_i2c_write(chip, TPM_STS(chip->vendor.locality), buf, sizeof(buf));
	mdelay(CR50_TIMEOUT_SHORT_MS);
}

/* cr50 uses bytes 3:2 of status register for burst count and
 * all 4 bytes must be read */
static int cr50_i2c_wait_burststs(struct tpm_chip *chip, uint8_t mask,
				  size_t *burst, int *status)
{
	uint8_t buf[4];
	struct stopwatch sw;

	stopwatch_init_msecs_expire(&sw, CR50_TIMEOUT_LONG_MS);

	while (!stopwatch_expired(&sw)) {
		if (cr50_i2c_read(chip, TPM_STS(chip->vendor.locality),
				  buf, sizeof(buf)) != 0) {
			mdelay(CR50_TIMEOUT_SHORT_MS);
			continue;
		}

		*status = buf[0];
		*burst = read_le16(&buf[1]);

		/* Check if mask matches and burst is valid */
		if ((*status & mask) == mask &&
		    *burst > 0 && *burst <= CR50_MAX_BUFSIZE)
			return 0;

		mdelay(CR50_TIMEOUT_SHORT_MS);
	}

	printk(BIOS_ERR, "%s: Timeout reading burst and status\n", __func__);
	return -1;
}

static int cr50_i2c_tis_recv(struct tpm_chip *chip, uint8_t *buf,
			     size_t buf_len)
{
	size_t burstcnt, current, len, expected;
	uint8_t addr = TPM_DATA_FIFO(chip->vendor.locality);
	uint8_t mask = TPM_STS_VALID | TPM_STS_DATA_AVAIL;
	int status;

	if (buf_len < TPM_HEADER_SIZE)
		goto out_err;

	if (cr50_i2c_wait_burststs(chip, mask, &burstcnt, &status) < 0) {
		printk(BIOS_ERR, "%s: First chunk not available\n", __func__);
		goto out_err;
	}

	/* Read first chunk of burstcnt bytes */
	if (cr50_i2c_read(chip, addr, buf, burstcnt) != 0) {
		printk(BIOS_ERR, "%s: Read failed\n", __func__);
		goto out_err;
	}

	/* Determine expected data in the return buffer */
	expected = read_be32(buf + TPM_RSP_SIZE_BYTE);
	if (expected > buf_len) {
		printk(BIOS_ERR, "%s: Too much data: %zu > %zu\n",
		       __func__, expected, buf_len);
		goto out_err;
	}

	/* Now read the rest of the data */
	current = burstcnt;
	while (current < expected) {
		/* Read updated burst count and check status */
		if (cr50_i2c_wait_burststs(chip, mask, &burstcnt, &status) < 0)
			goto out_err;

		len = min(burstcnt, expected - current);
		if (cr50_i2c_read(chip, addr, buf + current, len) != 0) {
			printk(BIOS_ERR, "%s: Read failed\n", __func__);
			goto out_err;
		}

		current += len;
	}

	/* Ensure TPM is done reading data */
	if (cr50_i2c_wait_burststs(chip, TPM_STS_VALID, &burstcnt, &status) < 0)
		goto out_err;
	if (status & TPM_STS_DATA_AVAIL) {
		printk(BIOS_ERR, "%s: Data still available\n", __func__);
		goto out_err;
	}

	return current;

out_err:
	/* Abort current transaction if still pending */
	if (cr50_i2c_tis_status(chip) & TPM_STS_COMMAND_READY)
		cr50_i2c_tis_ready(chip);
	return -1;
}

static int cr50_i2c_tis_send(struct tpm_chip *chip, uint8_t *buf, size_t len)
{
	int status;
	size_t burstcnt, limit, sent = 0;
	uint8_t tpm_go[4] = { TPM_STS_GO };
	struct stopwatch sw;

	stopwatch_init_msecs_expire(&sw, CR50_TIMEOUT_LONG_MS);

	/* Wait until TPM is ready for a command */
	while (!(cr50_i2c_tis_status(chip) & TPM_STS_COMMAND_READY)) {
		if (stopwatch_expired(&sw)) {
			printk(BIOS_ERR, "%s: Command ready timeout\n",
			       __func__);
			return -1;
		}

		cr50_i2c_tis_ready(chip);
	}

	while (len > 0) {
		uint8_t mask = TPM_STS_VALID;

		/* Wait for data if this is not the first chunk */
		if (sent > 0)
			mask |= TPM_STS_DATA_EXPECT;

		/* Read burst count and check status */
		if (cr50_i2c_wait_burststs(chip, mask, &burstcnt, &status) < 0)
			goto out_err;

		/* Use burstcnt - 1 to account for the address byte
		 * that is inserted by cr50_i2c_write() */
		limit = min(burstcnt - 1, len);
		if (cr50_i2c_write(chip, TPM_DATA_FIFO(chip->vendor.locality),
				   &buf[sent], limit) != 0) {
			printk(BIOS_ERR, "%s: Write failed\n", __func__);
			goto out_err;
		}

		sent += limit;
		len -= limit;
	}

	/* Ensure TPM is not expecting more data */
	if (cr50_i2c_wait_burststs(chip, TPM_STS_VALID, &burstcnt, &status) < 0)
		goto out_err;
	if (status & TPM_STS_DATA_EXPECT) {
		printk(BIOS_ERR, "%s: Data still expected\n", __func__);
		goto out_err;
	}

	/* Start the TPM command */
	if (cr50_i2c_write(chip, TPM_STS(chip->vendor.locality), tpm_go,
			   sizeof(tpm_go)) < 0) {
		printk(BIOS_ERR, "%s: Start command failed\n", __func__);
		goto out_err;
	}
	return sent;

out_err:
	/* Abort current transaction if still pending */
	if (cr50_i2c_tis_status(chip) & TPM_STS_COMMAND_READY)
		cr50_i2c_tis_ready(chip);
	return -1;
}

static void cr50_vendor_init(struct tpm_chip *chip)
{
	memset(&chip->vendor, 0, sizeof(struct tpm_vendor_specific));
	chip->vendor.req_complete_mask = TPM_STS_DATA_AVAIL | TPM_STS_VALID;
	chip->vendor.req_complete_val = TPM_STS_DATA_AVAIL | TPM_STS_VALID;
	chip->vendor.req_canceled = TPM_STS_COMMAND_READY;
	chip->vendor.status = &cr50_i2c_tis_status;
	chip->vendor.recv = &cr50_i2c_tis_recv;
	chip->vendor.send = &cr50_i2c_tis_send;
	chip->vendor.cancel = &cr50_i2c_tis_ready;
	chip->vendor.irq = CONFIG_DRIVER_TPM_I2C_IRQ;

	/*
	 * Interrupts are not supported this early in firmware,
	 * use use an arch-specific method to query for interrupt status.
	 */
	if (chip->vendor.irq > 0) {
#if IS_ENABLED(CONFIG_ARCH_X86)
		/* Query GPE status for interrupt */
		chip->vendor.irq_status = &acpi_get_gpe;
#else
		chip->vendor.irq = -1;
#endif
	}

	if (chip->vendor.irq <= 0)
		printk(BIOS_WARNING,
		       "%s: No IRQ, will use %ums delay for TPM ready\n",
		       __func__, CR50_TIMEOUT_NOIRQ_MS);
}

int tpm_vendor_probe(unsigned bus, uint32_t addr)
{
	struct tpm_inf_dev *tpm_dev = car_get_var_ptr(&g_tpm_dev);
	struct tpm_chip probe_chip;
	struct stopwatch sw;
	uint8_t buf = 0;
	int ret;
	long sw_run_duration = CR50_TIMEOUT_LONG_MS;

	tpm_dev->bus = bus;
	tpm_dev->addr = addr;

	cr50_vendor_init(&probe_chip);

	/* Wait for TPM_ACCESS register ValidSts bit to be set */
	stopwatch_init_msecs_expire(&sw, sw_run_duration);
	do {
		ret = cr50_i2c_read(&probe_chip, TPM_ACCESS(0), &buf, 1);
		if (!ret && (buf & TPM_STS_VALID)) {
			sw_run_duration = stopwatch_duration_msecs(&sw);
			break;
		}
		mdelay(CR50_TIMEOUT_SHORT_MS);
	} while (!stopwatch_expired(&sw));

	printk(BIOS_INFO,
	       "%s: ValidSts bit %s(%d) in TPM_ACCESS register after %ld ms\n",
	       __func__, (buf & TPM_STS_VALID) ? "set" : "clear",
	       (buf & TPM_STS_VALID) >> 7, sw_run_duration);

	/* Claim failure if the ValidSts (bit 7) is clear */
	if (!(buf & TPM_STS_VALID))
		return -1;

	return 0;
}

int tpm_vendor_init(struct tpm_chip *chip, unsigned bus, uint32_t dev_addr)
{
	struct tpm_inf_dev *tpm_dev = car_get_var_ptr(&g_tpm_dev);
	uint32_t vendor;

	if (dev_addr == 0) {
		printk(BIOS_ERR, "%s: missing device address\n", __func__);
		return -1;
	}

	tpm_dev->bus = bus;
	tpm_dev->addr = dev_addr;

	cr50_vendor_init(chip);

	if (request_locality(chip, 0) != 0)
		return -1;

	/* Read four bytes from DID_VID register */
	if (cr50_i2c_read(chip, TPM_DID_VID(0), (uint8_t *)&vendor, 4) < 0)
		goto out_err;

	if (vendor != CR50_DID_VID) {
		printk(BIOS_DEBUG, "Vendor ID 0x%08x not recognized\n", vendor);
		goto out_err;
	}

	printk(BIOS_DEBUG, "cr50 TPM 2.0 (i2c %u:0x%02x irq %d id 0x%x)\n",
	       bus, dev_addr, chip->vendor.irq, vendor >> 16);

	chip->is_open = 1;
	return 0;

out_err:
	release_locality(chip, 1);
	return -1;
}

void tpm_vendor_cleanup(struct tpm_chip *chip)
{
	release_locality(chip, 1);
}