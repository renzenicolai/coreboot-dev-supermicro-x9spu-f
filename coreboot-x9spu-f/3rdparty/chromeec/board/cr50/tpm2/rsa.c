/* Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "CryptoEngine.h"
#include "Global.h"
#include "Hierarchy_fp.h"

#include "dcrypto.h"
#include "trng.h"

#include <assert.h>

TPM2B_BYTE_VALUE(4);
TPM2B_BYTE_VALUE(32);

static void reverse_tpm2b(TPM2B *b)
{
	reverse(b->buffer, b->size);
}

static int check_key(const RSA_KEY *key)
{
	if (key->publicKey->size & 0x3)
		/* Only word-multiple sizes supported. */
		return 0;
	return 1;
}

static int check_encrypt_params(TPM_ALG_ID padding_alg, TPM_ALG_ID hash_alg,
				enum padding_mode *padding,
				enum hashing_mode *hashing)
{
	/* Initialize hashing for all padding types */
	*hashing = HASH_SHA1;

	if (padding_alg == TPM_ALG_RSAES) {
		*padding = PADDING_MODE_PKCS1;
	} else if (padding_alg == TPM_ALG_OAEP) {
		/* Only SHA1 and SHA256 supported with OAEP. */
		if (hash_alg == TPM_ALG_SHA256)
			*hashing = HASH_SHA256;
		else if (hash_alg != TPM_ALG_SHA1)
			/* Unsupported hash algorithm. */
			return 0;
		*padding = PADDING_MODE_OAEP;
	} else if (padding_alg == TPM_ALG_NULL) {
		*padding = PADDING_MODE_NULL;
	} else {
		return 0;  /* Unsupported padding mode. */
	}
	return 1;
}

static int check_sign_params(TPM_ALG_ID padding_alg, TPM_ALG_ID hash_alg,
			enum padding_mode *padding,
			enum hashing_mode *hashing)
{
	if (padding_alg == TPM_ALG_RSASSA ||
		padding_alg == TPM_ALG_RSAPSS) {
		if (hash_alg == TPM_ALG_SHA1)
			*hashing = HASH_SHA1;
		else if (hash_alg == TPM_ALG_SHA256)
			*hashing = HASH_SHA256;
		else if (hash_alg == ALG_SHA384_VALUE &&
			 padding_alg == TPM_ALG_RSASSA)
			*hashing = HASH_SHA384;
		else if (hash_alg == ALG_SHA512_VALUE &&
			 padding_alg == TPM_ALG_RSASSA)
			*hashing = HASH_SHA512;
#if defined(SUPPORT_PADDING_ONLY_RSASSA) && SUPPORT_PADDING_ONLY_RSASSA == YES
		else if (hash_alg == TPM_ALG_NULL &&
			 padding_alg == TPM_ALG_RSASSA)
			*hashing = HASH_NULL;
#endif
		else
			return 0;
		if (padding_alg == TPM_ALG_RSASSA)
			*padding = PADDING_MODE_PKCS1;
		else
			*padding = PADDING_MODE_PSS;
	} else {
		return 0;
	}
	return 1;
}

CRYPT_RESULT _cpri__EncryptRSA(uint32_t *out_len, uint8_t *out,
			RSA_KEY *key, TPM_ALG_ID padding_alg,
			uint32_t in_len, uint8_t *in,
			TPM_ALG_ID hash_alg, const char *label)
{
	struct RSA rsa;
	enum padding_mode padding;
	enum hashing_mode hashing;
	int result;

	if (!check_key(key))
		return CRYPT_FAIL;
	if (!check_encrypt_params(padding_alg, hash_alg, &padding, &hashing))
		return CRYPT_FAIL;

	reverse_tpm2b(key->publicKey);
	rsa.e = key->exponent;
	rsa.N.dmax = key->publicKey->size / sizeof(uint32_t);
	rsa.N.d = (struct access_helper *) &key->publicKey->buffer;
	rsa.d.dmax = 0;
	rsa.d.d = NULL;

	result = DCRYPTO_rsa_encrypt(&rsa, out, out_len, in, in_len, padding,
				hashing, label);

	reverse_tpm2b(key->publicKey);

	if (result)
		return CRYPT_SUCCESS;
	else
		return CRYPT_FAIL;
}

CRYPT_RESULT _cpri__DecryptRSA(uint32_t *out_len, uint8_t *out,
			RSA_KEY *key, TPM_ALG_ID padding_alg,
			uint32_t in_len, uint8_t *in,
			TPM_ALG_ID hash_alg, const char *label)
{
	struct RSA rsa;
	enum padding_mode padding;
	enum hashing_mode hashing;
	int result;

	if (!check_key(key))
		return CRYPT_FAIL;
	if (!check_encrypt_params(padding_alg, hash_alg, &padding, &hashing))
		return CRYPT_FAIL;

	reverse_tpm2b(key->publicKey);
	reverse_tpm2b(key->privateKey);

	rsa.e = key->exponent;
	rsa.N.dmax = key->publicKey->size / sizeof(uint32_t);
	rsa.N.d = (struct access_helper *) &key->publicKey->buffer;
	rsa.d.dmax = key->privateKey->size / sizeof(uint32_t);
	rsa.d.d = (struct access_helper *) &key->privateKey->buffer;

	result = DCRYPTO_rsa_decrypt(&rsa, out, out_len, in, in_len, padding,
				hashing, label);

	reverse_tpm2b(key->publicKey);
	reverse_tpm2b(key->privateKey);

	if (result)
		return CRYPT_SUCCESS;
	else
		return CRYPT_FAIL;
}

CRYPT_RESULT _cpri__SignRSA(uint32_t *out_len, uint8_t *out,
			RSA_KEY *key, TPM_ALG_ID padding_alg,
			TPM_ALG_ID hash_alg, uint32_t in_len, uint8_t *in)
{
	struct RSA rsa;
	enum padding_mode padding;
	enum hashing_mode hashing;
	int result;

	if (!check_key(key))
		return CRYPT_FAIL;
	if (!check_sign_params(padding_alg, hash_alg, &padding, &hashing))
		return CRYPT_FAIL;

	reverse_tpm2b(key->publicKey);
	reverse_tpm2b(key->privateKey);

	rsa.e = key->exponent;
	rsa.N.dmax = key->publicKey->size / sizeof(uint32_t);
	rsa.N.d = (struct access_helper *) &key->publicKey->buffer;
	rsa.d.dmax = key->privateKey->size / sizeof(uint32_t);
	rsa.d.d = (struct access_helper *) &key->privateKey->buffer;

	/* TPM2 wrapper function fails to initialize out_len! */
	*out_len = key->publicKey->size;
	result = DCRYPTO_rsa_sign(&rsa, out, out_len, in, in_len,
				padding, hashing);

	reverse_tpm2b(key->publicKey);
	reverse_tpm2b(key->privateKey);

	if (result)
		return CRYPT_SUCCESS;
	else
		return CRYPT_FAIL;
}

CRYPT_RESULT _cpri__ValidateSignatureRSA(
	RSA_KEY *key, TPM_ALG_ID padding_alg, TPM_ALG_ID hash_alg,
	uint32_t digest_len, uint8_t *digest, uint32_t sig_len,
	uint8_t *sig, uint16_t salt_len)
{
	struct RSA rsa;
	enum padding_mode padding;
	enum hashing_mode hashing;
	int result;

	if (!check_key(key))
		return CRYPT_FAIL;
	if (!check_sign_params(padding_alg, hash_alg, &padding, &hashing))
		return CRYPT_FAIL;

	reverse_tpm2b(key->publicKey);

	rsa.e = key->exponent;
	rsa.N.dmax = key->publicKey->size / sizeof(uint32_t);
	rsa.N.d = (struct access_helper *) &key->publicKey->buffer;
	rsa.d.dmax = 0;
	rsa.d.d = NULL;

	result = DCRYPTO_rsa_verify(&rsa, digest, digest_len, sig, sig_len,
				padding, hashing);

	reverse_tpm2b(key->publicKey);

	if (result)
		return CRYPT_SUCCESS;
	else
		return CRYPT_FAIL;
}

CRYPT_RESULT _cpri__TestKeyRSA(TPM2B *d_buf, uint32_t e,
			TPM2B *N_buf, TPM2B *p_buf, TPM2B *q_buf)
{
	struct LITE_BIGNUM N;
	struct LITE_BIGNUM p;
	struct LITE_BIGNUM q;
	struct LITE_BIGNUM d;
	int result;

	if (!p_buf)
		return CRYPT_PARAMETER;
	if (q_buf && p_buf->size != q_buf->size)
		return CRYPT_PARAMETER;
	if (N_buf->size != p_buf->size * 2)
		return CRYPT_PARAMETER;  /* Insufficient output buffer space. */
	if (N_buf->size > RSA_MAX_BYTES)
		return CRYPT_PARAMETER;  /* Unsupported key size. */

	DCRYPTO_bn_wrap(&N, N_buf->buffer, N_buf->size);
	DCRYPTO_bn_wrap(&p, p_buf->buffer, p_buf->size);
	reverse_tpm2b(N_buf);
	reverse_tpm2b(p_buf);
	if (q_buf) {
		DCRYPTO_bn_wrap(&q, q_buf->buffer, q_buf->size);
		reverse_tpm2b(q_buf);
	}
	/* d_buf->size may be uninitialized. */
	DCRYPTO_bn_wrap(&d, d_buf->buffer, N_buf->size);

	result = DCRYPTO_rsa_key_compute(&N, &d, &p, q_buf ? &q : NULL, e);

	reverse_tpm2b(N_buf);
	reverse_tpm2b(p_buf);
	if (q_buf)
		reverse_tpm2b(q_buf);

	if (result) {
		d_buf->size = N_buf->size;
		reverse_tpm2b(d_buf);
		return CRYPT_SUCCESS;
	} else {
		return CRYPT_FAIL;
	}
}

/* Each 1024-bit prime generation attempt fails with probability
 * ~0.5%.  Setting an upper limit on the attempts allows for an
 * application to display a message and then reattempt.
 * TODO(ngm): tweak this value along with performance improvements. */
#define MAX_GENERATE_ATTEMPTS 3

static int generate_prime(struct LITE_BIGNUM *b, TPM_ALG_ID hashing,
			TPM2B *seed, const char *label, TPM2B *extra,
			uint32_t *counter)
{
	TPM2B_4_BYTE_VALUE marshaled_counter = { .t = {4} };
	uint32_t i;

	for (i = 0; i < MAX_GENERATE_ATTEMPTS; i++) {
		UINT32_TO_BYTE_ARRAY(*counter, marshaled_counter.t.buffer);
		_cpri__KDFa(hashing, seed, label, extra, &marshaled_counter.b,
			bn_bits(b), (uint8_t *) b->d, NULL, FALSE);

		(*counter)++;           /* Mark as used. */
		if (DCRYPTO_bn_generate_prime(b))
			return 1;
	}

	return 0;
}

#ifdef CRYPTO_TEST_SETUP
static const uint8_t VERIFY_SEED[32] = {
	0x54, 0xef, 0xe3, 0xe9, 0x1e, 0xfa, 0xad, 0x9b,
	0x18, 0x3f, 0x27, 0x12, 0xfd, 0xe7, 0xfb, 0xc6,
	0x60, 0xcc, 0x34, 0x05, 0x00, 0x7d, 0x21, 0x6e,
	0xc2, 0x1e, 0x78, 0xbe, 0x61, 0xc8, 0x41, 0x99
};
#endif

/* The array below represents the output of ObjectComputeName() when
 * applied to the TPMT_PUBLIC RSA template described in the TPM 2.0
 * EK Credential Profile specification: https://goo.gl/rbE6q7
 */
static const uint8_t TPM2_RSA_EK_NAME_TEMPLATE[] = {
	/* TPM_ALG_SHA256 in big endian. */
	0x00, 0x0b,
	/* SHA256 digest of the default template TPMT_PUBLIC. */
	0x32, 0x50, 0x39, 0x29, 0xa1, 0x28, 0x7e, 0xed,
	0xaa, 0x3e, 0x89, 0xd9, 0x32, 0xf9, 0xb5, 0x1a,
	0x6f, 0x92, 0xab, 0xd0, 0xfa, 0x57, 0x72, 0x1f,
	0xfa, 0x6f, 0xc0, 0x41, 0xe0, 0x4f, 0x74, 0x98
};
BUILD_ASSERT(sizeof(TPM2_RSA_EK_NAME_TEMPLATE) == 2 + SHA256_DIGEST_SIZE);

/* The array below represents the 'name' (corresponding to the
 * parameter extra) used by the CR50 certificate authority when
 * generating the endorsement certificate.
 */
static const uint8_t TPM2_RSA_EK_NAME_CR50[] = {
	0x68, 0xd1, 0xa2, 0x41, 0xfb, 0x27, 0x2f, 0x03,
	0x90, 0xbf, 0xd0, 0x42, 0x8d, 0xad, 0xee, 0xb0,
	0x2b, 0xf4, 0xa1, 0xcd, 0x46, 0xab, 0x6c, 0x39,
	0x1b, 0xa3, 0x1f, 0x51, 0x87, 0x06, 0x8e, 0x6a
};
BUILD_ASSERT(sizeof(TPM2_RSA_EK_NAME_CR50) == SHA256_DIGEST_SIZE);

CRYPT_RESULT _cpri__GenerateKeyRSA(
	TPM2B *N_buf, TPM2B *p_buf, uint16_t num_bits,
	uint32_t e_buf, TPM_ALG_ID hashing, TPM2B *seed,
	const char *label, TPM2B *extra, uint32_t *counter_in)
{
	const char *label_p = "RSA p!";
	const char *label_q = "RSA q!";
	/* Numbers from NIST SP800-57.
	 * Fallback conservatively for keys larger than 2048 bits. */
	const uint32_t security_strength =
		num_bits <= 1024 ? 80 :
		num_bits <= 2048 ? 112 :
		256;

	const uint16_t num_bytes = num_bits / 8;
	uint8_t q_buf[RSA_MAX_BYTES / 2];

	struct LITE_BIGNUM e;
	struct LITE_BIGNUM p;
	struct LITE_BIGNUM q;
	struct LITE_BIGNUM N;

	uint32_t counter = 0;
	TPM2B_32_BYTE_VALUE local_seed = { .t = {32} };
	TPM2B_32_BYTE_VALUE local_extra = { .t = {32} };

	const TPM2B_SEED *endorsement_seed = HierarchyGetPrimarySeed(
		TPM_RH_ENDORSEMENT);

	if (num_bits & 0xF)
		return CRYPT_FAIL;
	if (num_bytes > RSA_MAX_BYTES)
		return CRYPT_FAIL;
	/* Seed size must be at least 2*security_strength per TPM 2.0 spec. */
	if (seed == NULL || seed->size * 8 < 2 * security_strength)
		return CRYPT_FAIL;

	/* When generating the endorsement primary key (based on the
	 * TPM 2.0 standard template, swap in the vendor specific
	 * template instead.
	 */
	if (extra->size == sizeof(TPM2_RSA_EK_NAME_TEMPLATE) &&
		DCRYPTO_equals(extra->buffer, TPM2_RSA_EK_NAME_TEMPLATE,
			       sizeof(TPM2_RSA_EK_NAME_TEMPLATE)) &&
		seed == &endorsement_seed->b) {
		memcpy(local_extra.b.buffer, TPM2_RSA_EK_NAME_CR50,
			sizeof(TPM2_RSA_EK_NAME_CR50));
		extra = &local_extra.b;
	}

	/* Hash down the primary seed for RSA key generation, so that
	 * the derivation tree is distinct from ECC key derivation.
	 */
#ifdef CRYPTO_TEST_SETUP
	if (seed->size == sizeof(VERIFY_SEED) &&
		DCRYPTO_equals(seed->buffer, VERIFY_SEED, seed->size)) {
		/* Test seed has already been hashed down. */
		memcpy(local_seed.t.buffer, seed->buffer, seed->size);
	} else
#endif
	{
		LITE_HMAC_CTX hmac;

		DCRYPTO_HMAC_SHA256_init(&hmac, seed->buffer, seed->size);
		HASH_update(&hmac.hash, "RSA", 4);
		memcpy(local_seed.t.buffer, DCRYPTO_HMAC_final(&hmac),
			local_seed.t.size);
	}

	if (e_buf == 0)
		e_buf = RSA_F4;

	DCRYPTO_bn_wrap(&e, &e_buf, sizeof(e_buf));
	DCRYPTO_bn_wrap(&p, p_buf->buffer, num_bytes / 2);
	DCRYPTO_bn_wrap(&q, q_buf, num_bytes / 2);

	if (label == NULL)
		label = label_p;

	/* The manufacture process uses a counter of 1, whereas the
	 * TPM2.0 library (CryptGenerateKeyRSA) passes in a counter
	 * value of 0.  Having that the counter always start at least
	 * at 1 ensures that endorsement keys are correctly generated.
	 * For non-endorsement keys, the counter value used is
	 * immaterial, as the generation process remains deterministic.
	 */
	if (counter_in != NULL)
		counter = *counter_in;
	counter++;

	if (!generate_prime(&p, hashing, &local_seed.b, label, extra,
			    &counter)) {
		if (counter_in != NULL)
			*counter_in = counter;
		/* TODO(ngm): implement secure memset. */
		memset(local_seed.t.buffer, 0, local_seed.t.size);
		return CRYPT_FAIL;
	}

	if (label == label_p)
		label = label_q;
	if (!generate_prime(&q, hashing, &local_seed.b, label, extra,
			    &counter)) {
		if (counter_in != NULL)
			*counter_in = counter;
		/* TODO(ngm): implement secure memset. */
		memset(local_seed.t.buffer, 0, local_seed.t.size);
		return CRYPT_FAIL;
	}

	if (counter_in != NULL)
		*counter_in = counter;
	N_buf->size = num_bytes;
	p_buf->size = num_bytes / 2;
	DCRYPTO_bn_wrap(&N, N_buf->buffer, num_bytes);
	DCRYPTO_bn_mul(&N, &p, &q);
	reverse_tpm2b(N_buf);
	reverse_tpm2b(p_buf);
	/* TODO(ngm): replace with secure memset. */
	memset(q_buf, 0, sizeof(q_buf));
	memset(local_seed.t.buffer, 0, local_seed.t.size);
	return CRYPT_SUCCESS;
}

#ifdef CRYPTO_TEST_SETUP

#include "extension.h"

enum {
	TEST_RSA_ENCRYPT = 0,
	TEST_RSA_DECRYPT = 1,
	TEST_RSA_SIGN = 2,
	TEST_RSA_VERIFY = 3,
	TEST_RSA_KEYGEN = 4,
	TEST_RSA_KEYTEST = 5,
	TEST_BN_PRIMEGEN = 6,
	TEST_X509_VERIFY = 7,
};

static const TPM2B_PUBLIC_KEY_RSA RSA_768_N = {
	.t = {96, {
			0xb0, 0xdb, 0xed, 0x46, 0xd9, 0x32, 0xf0, 0x7c,
			0xd4, 0x20, 0x23, 0xd2, 0x35, 0x5a, 0x86, 0x17,
			0xdb, 0x24, 0x72, 0x36, 0x33, 0x3b, 0xc2, 0x64,
			0x8b, 0xa4, 0x49, 0x6e, 0x74, 0xfe, 0xfa, 0xd2,
			0x82, 0x0c, 0xc4, 0x12, 0x3a, 0x48, 0x67, 0xe1,
			0x15, 0xcc, 0x94, 0xdf, 0x44, 0x1b, 0x4e, 0xc0,
			0x18, 0xba, 0x46, 0x1b, 0x51, 0x2c, 0xe2, 0x0f,
			0xc0, 0x32, 0x77, 0xed, 0x5f, 0x8b, 0xe5, 0xa3,
			0x00, 0xe6, 0x3c, 0x2d, 0xa7, 0x10, 0x89, 0x53,
			0xa8, 0x2b, 0x33, 0x74, 0x38, 0xf7, 0x36, 0x00,
			0xfd, 0xdd, 0x5b, 0xbd, 0x7b, 0xc1, 0x7c, 0xe1,
			0x75, 0x90, 0x2b, 0x78, 0x2d, 0x39, 0x85, 0x69
		}
	}
};

static const TPM2B_PUBLIC_KEY_RSA RSA_768_D = {
	.t = {96, {
			0xae, 0xad, 0xb9, 0x50, 0x25, 0x8c, 0x1b, 0x5c,
			0x9f, 0x42, 0xd3, 0x3e, 0x76, 0x75, 0xdf, 0x45,
			0x46, 0xab, 0x5b, 0xa6, 0xce, 0xb9, 0x72, 0x49,
			0x4e, 0x66, 0xc8, 0x24, 0x31, 0xa7, 0xf9, 0x61,
			0xdb, 0x12, 0xf2, 0xc1, 0x32, 0x11, 0x7b, 0x90,
			0x23, 0xb0, 0xb9, 0x45, 0x3f, 0x06, 0x5d, 0xa2,
			0xd7, 0x35, 0x0f, 0xdd, 0xfc, 0x03, 0xdf, 0x8d,
			0x91, 0x6b, 0x83, 0xf9, 0x59, 0xee, 0x67, 0x1e,
			0x1a, 0x20, 0x9e, 0x8b, 0xf8, 0xf6, 0xe2, 0xb2,
			0xf5, 0x29, 0x71, 0x4c, 0x22, 0x54, 0xcf, 0x7e,
			0x97, 0xbc, 0x70, 0x24, 0xdd, 0x6d, 0x52, 0xfe,
			0x17, 0xd9, 0xd6, 0x41, 0x7b, 0x76, 0x40, 0x01
		}
	}
};

static const TPM2B_PUBLIC_KEY_RSA RSA_768_P = {
	.t = {48, {
			0xd6, 0x09, 0x64, 0xc8, 0xf3, 0x5c, 0x02, 0xc7,
			0xc6, 0x47, 0x4e, 0x7f, 0x43, 0x9d, 0x31, 0x46,
			0x7a, 0x33, 0x85, 0xa0, 0xa4, 0x16, 0xea, 0x22,
			0x7b, 0xcd, 0x64, 0x9b, 0x50, 0xec, 0xa7, 0x2f,
			0x7e, 0xcf, 0xeb, 0x69, 0x29, 0x34, 0x8e, 0xb7,
			0xb5, 0xb3, 0xba, 0x7f, 0x9b, 0x01, 0x7d, 0x69
		}
	}
};

static const TPM2B_PUBLIC_KEY_RSA RSA_768_Q = {
	.t = {48, {
			0xd3, 0x88, 0x92, 0x2d, 0xd5, 0xc6, 0x29, 0xf4,
			0xf0, 0x2e, 0x61, 0xf0, 0x60, 0xad, 0xa9, 0x46,
			0x11, 0xa9, 0x0c, 0x69, 0x14, 0x31, 0x09, 0x36,
			0x8b, 0x70, 0x1b, 0x11, 0x9b, 0x26, 0x39, 0x34,
			0x34, 0xfd, 0xf1, 0x9a, 0x89, 0x51, 0x63, 0x0a,
			0xc6, 0x60, 0x0b, 0xba, 0x18, 0x8e, 0xc8, 0x01
		}
	}
};

static const TPM2B_PUBLIC_KEY_RSA RSA_1024_N = {
	.t = {128, {
			0xdf, 0x4e, 0xaf, 0x73, 0x45, 0x94, 0x98, 0x34,
			0x30, 0x7e, 0x26, 0xad, 0x40, 0x83, 0xf9, 0x17,
			0x21, 0xb0, 0x4e, 0x1b, 0x0d, 0x6a, 0x44, 0xce,
			0x4e, 0x3e, 0x2e, 0x72, 0x4c, 0x97, 0xdf, 0x89,
			0x8a, 0x39, 0x10, 0x25, 0xae, 0x20, 0x4c, 0xf2,
			0x3b, 0x20, 0xb2, 0xa5, 0x10, 0xdd, 0xb2, 0x6b,
			0x62, 0x4e, 0xa6, 0x9f, 0x92, 0x4a, 0xd9, 0x86,
			0x97, 0xcc, 0x70, 0x20, 0x3b, 0x6a, 0x32, 0x63,
			0xca, 0x7f, 0x59, 0xfb, 0x57, 0xb6, 0xa9, 0x99,
			0xe9, 0xd0, 0x2e, 0x0f, 0x1c, 0xd4, 0x7d, 0x8b,
			0xa0, 0xbd, 0x0f, 0xd2, 0xd5, 0x3b, 0x1f, 0x11,
			0xb4, 0x6a, 0x94, 0xcf, 0x4f, 0x0a, 0x2b, 0x44,
			0xe7, 0xfa, 0x6b, 0x24, 0x91, 0xb4, 0x82, 0x1f,
			0xf6, 0x75, 0xb6, 0x91, 0xc5, 0xa0, 0xf6, 0x2f,
			0xd5, 0xff, 0x10, 0x73, 0x9b, 0x34, 0xf6, 0x7a,
			0x88, 0x23, 0xa9, 0x42, 0x3c, 0xa8, 0x24, 0x91
		}
	}
};

static const TPM2B_PUBLIC_KEY_RSA RSA_1024_D = {
	.t = {128, {
			0x9a, 0x6d, 0x85, 0xf4, 0x07, 0xa8, 0x6d, 0x61,
			0x9a, 0x2f, 0x83, 0x7b, 0xc8, 0xe3, 0xfb, 0x7c,
			0xbd, 0xb5, 0x79, 0x2e, 0x48, 0x26, 0xb7, 0x92,
			0x9c, 0x95, 0x6f, 0xf5, 0x67, 0x76, 0x98, 0x06,
			0x3b, 0xea, 0x9e, 0x7a, 0x10, 0x63, 0x12, 0x13,
			0x6a, 0x44, 0x80, 0x86, 0x9a, 0x95, 0x56, 0x6f,
			0xe0, 0xba, 0x57, 0x8c, 0x7e, 0xd4, 0xf8, 0x7d,
			0x95, 0xb8, 0xb1, 0xc9, 0xf8, 0x8c, 0xc6, 0x6e,
			0xe5, 0x7b, 0xa0, 0xaf, 0xa0, 0x4e, 0x4e, 0x84,
			0xd7, 0x97, 0xb9, 0x5a, 0xdd, 0x32, 0xe5, 0x2b,
			0xe5, 0x80, 0xb3, 0xb2, 0xbf, 0x56, 0xff, 0x01,
			0xdc, 0xe6, 0xa6, 0x6c, 0x4a, 0x81, 0x1d, 0x8f,
			0xea, 0x4b, 0xed, 0x24, 0x08, 0xf4, 0x67, 0xaf,
			0x0d, 0xf2, 0xfd, 0x37, 0x3f, 0x31, 0x25, 0xfa,
			0xee, 0x35, 0xb0, 0xdb, 0x66, 0x11, 0xff, 0x49,
			0xe1, 0xe5, 0xff, 0x1b, 0xcc, 0xc3, 0x0e, 0x09
		}
	}
};

static const TPM2B_PUBLIC_KEY_RSA RSA_1024_P = {
	.t = {64, {
			0xf9, 0x5e, 0x79, 0x65, 0x43, 0x70, 0x40, 0x83,
			0x50, 0x0a, 0xbb, 0x61, 0xb3, 0x87, 0x7b, 0x24,
			0x8f, 0x2a, 0x03, 0x5b, 0xb5, 0x4b, 0x94, 0x94,
			0x67, 0xaa, 0x98, 0xd6, 0x14, 0x40, 0x90, 0x3c,
			0xa4, 0x0d, 0x6d, 0x58, 0x31, 0xc5, 0x42, 0xf1,
			0x2d, 0x15, 0x0e, 0xe7, 0xcd, 0xe6, 0x3e, 0xca,
			0xd8, 0x94, 0x37, 0xaa, 0x4c, 0xd6, 0xf3, 0x21,
			0x2e, 0xa4, 0xfe, 0x1d, 0x79, 0x44, 0xd7, 0xb3
		}
	}
};

static const TPM2B_PUBLIC_KEY_RSA RSA_1024_Q = {
	.t = {64, {
			0xe5, 0x3e, 0xcd, 0x4b, 0x97, 0xc5, 0x96, 0x39,
			0x70, 0x97, 0x3a, 0x10, 0xa9, 0xc3, 0x35, 0x0a,
			0xd6, 0x2b, 0xf5, 0x12, 0x8d, 0xb2, 0xc0, 0x0b,
			0x1c, 0x5f, 0xa0, 0x0b, 0x86, 0x83, 0xa7, 0x90,
			0xe9, 0xf8, 0x16, 0x92, 0x9f, 0xce, 0x13, 0x4c,
			0x14, 0xe8, 0x9e, 0x4c, 0x24, 0xef, 0xff, 0x58,
			0x22, 0x06, 0xf9, 0xcf, 0xfd, 0x19, 0xb7, 0x23,
			0xf9, 0xe3, 0xb3, 0xe3, 0x7a, 0x9b, 0xb0, 0xab
		}
	}
};

static const TPM2B_PUBLIC_KEY_RSA RSA_2048_N = {
	.t = {256, {
			0x9c, 0xd7, 0x61, 0x2e, 0x43, 0x8e, 0x15, 0xbe,
			0xcd, 0x73, 0x9f, 0xb7, 0xf5, 0x86, 0x4b, 0xe3,
			0x95, 0x90, 0x5c, 0x85, 0x19, 0x4c, 0x1d, 0x2e,
			0x2c, 0xef, 0x6e, 0x1f, 0xed, 0x75, 0x32, 0x0f,
			0x0a, 0xc1, 0x72, 0x9f, 0x0c, 0x78, 0x50, 0xa2,
			0x99, 0x82, 0x53, 0x90, 0xbe, 0x64, 0x23, 0x49,
			0x75, 0x7b, 0x0c, 0xeb, 0x2d, 0x68, 0x97, 0xd6,
			0xaf, 0xb1, 0xaa, 0x2a, 0xde, 0x5e, 0x9b, 0xe3,
			0x06, 0x0d, 0xf2, 0xac, 0xd9, 0xd7, 0x1f, 0x50,
			0x6e, 0xc9, 0x5d, 0xeb, 0xb4, 0xf0, 0xc0, 0x98,
			0x23, 0x04, 0x30, 0x46, 0x10, 0xdc, 0xd4, 0x6b,
			0x57, 0xc7, 0x30, 0xc3, 0x06, 0xdd, 0xaf, 0x51,
			0x6e, 0x40, 0x41, 0xf8, 0x10, 0xde, 0x49, 0x18,
			0x52, 0xb3, 0x18, 0xca, 0x49, 0x50, 0xa8, 0x3a,
			0xcd, 0xb6, 0x94, 0x7b, 0xdb, 0xf1, 0x2d, 0x05,
			0xce, 0x57, 0x0b, 0xbe, 0x38, 0x48, 0xbb, 0xc9,
			0xb1, 0x76, 0x36, 0xb8, 0xa8, 0xcc, 0xe2, 0x07,
			0x5c, 0xc8, 0x7b, 0xcf, 0xcf, 0xf0, 0xfa, 0xa3,
			0xc5, 0xd7, 0x3a, 0x5e, 0xb2, 0xf4, 0xbf, 0xea,
			0xc2, 0xed, 0x51, 0x16, 0xa2, 0x92, 0x9c, 0x36,
			0xa6, 0x86, 0x0e, 0x24, 0xa5, 0x66, 0x15, 0xe7,
			0x97, 0x22, 0x50, 0x04, 0xff, 0xc9, 0x4d, 0xb0,
			0xbc, 0x27, 0x05, 0x5e, 0x2c, 0xf7, 0xef, 0xdc,
			0x5d, 0x58, 0xa1, 0x3b, 0x60, 0x83, 0xb7, 0x8c,
			0xb7, 0xd0, 0x36, 0x6d, 0x55, 0x2e, 0x05, 0x23,
			0x63, 0x74, 0x4a, 0x97, 0x37, 0xa7, 0x78, 0x40,
			0xef, 0x3e, 0x66, 0xfd, 0xba, 0x6e, 0xb3, 0x72,
			0x4a, 0x21, 0x82, 0x1f, 0x33, 0xad, 0x62, 0x0c,
			0xf2, 0x1a, 0xd2, 0x6a, 0xb5, 0xa7, 0xf2, 0x51,
			0x69, 0x1f, 0x38, 0xa5, 0x57, 0x9a, 0xc5, 0x88,
			0x67, 0xe3, 0x11, 0xa6, 0x53, 0x4f, 0xb1, 0xe9,
			0x07, 0x41, 0xde, 0xe8, 0xdf, 0x93, 0xa9, 0x99
		}
	}
};

static const uint8_t RSA_2048_CERT[] = {
	0x30, 0x82, 0x03, 0x5D, 0x30, 0x82, 0x02, 0x45,
	0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x09, 0x00,
	0xCB, 0x9D, 0x38, 0x47, 0x3F, 0x4F, 0x3B, 0xC6,
	0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86,
	0xF7, 0x0D, 0x01, 0x01, 0x0B, 0x05, 0x00, 0x30,
	0x45, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55,
	0x04, 0x06, 0x13, 0x02, 0x41, 0x55, 0x31, 0x13,
	0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0C,
	0x0A, 0x53, 0x6F, 0x6D, 0x65, 0x2D, 0x53, 0x74,
	0x61, 0x74, 0x65, 0x31, 0x21, 0x30, 0x1F, 0x06,
	0x03, 0x55, 0x04, 0x0A, 0x0C, 0x18, 0x49, 0x6E,
	0x74, 0x65, 0x72, 0x6E, 0x65, 0x74, 0x20, 0x57,
	0x69, 0x64, 0x67, 0x69, 0x74, 0x73, 0x20, 0x50,
	0x74, 0x79, 0x20, 0x4C, 0x74, 0x64, 0x30, 0x1E,
	0x17, 0x0D, 0x31, 0x36, 0x30, 0x36, 0x30, 0x38,
	0x32, 0x33, 0x34, 0x35, 0x32, 0x33, 0x5A, 0x17,
	0x0D, 0x31, 0x36, 0x30, 0x37, 0x30, 0x38, 0x32,
	0x33, 0x34, 0x35, 0x32, 0x33, 0x5A, 0x30, 0x45,
	0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04,
	0x06, 0x13, 0x02, 0x41, 0x55, 0x31, 0x13, 0x30,
	0x11, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0C, 0x0A,
	0x53, 0x6F, 0x6D, 0x65, 0x2D, 0x53, 0x74, 0x61,
	0x74, 0x65, 0x31, 0x21, 0x30, 0x1F, 0x06, 0x03,
	0x55, 0x04, 0x0A, 0x0C, 0x18, 0x49, 0x6E, 0x74,
	0x65, 0x72, 0x6E, 0x65, 0x74, 0x20, 0x57, 0x69,
	0x64, 0x67, 0x69, 0x74, 0x73, 0x20, 0x50, 0x74,
	0x79, 0x20, 0x4C, 0x74, 0x64, 0x30, 0x82, 0x01,
	0x22, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48,
	0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01, 0x05, 0x00,
	0x03, 0x82, 0x01, 0x0F, 0x00, 0x30, 0x82, 0x01,
	0x0A, 0x02, 0x82, 0x01, 0x01, 0x00, 0x9C, 0xD7,
	0x61, 0x2E, 0x43, 0x8E, 0x15, 0xBE, 0xCD, 0x73,
	0x9F, 0xB7, 0xF5, 0x86, 0x4B, 0xE3, 0x95, 0x90,
	0x5C, 0x85, 0x19, 0x4C, 0x1D, 0x2E, 0x2C, 0xEF,
	0x6E, 0x1F, 0xED, 0x75, 0x32, 0x0F, 0x0A, 0xC1,
	0x72, 0x9F, 0x0C, 0x78, 0x50, 0xA2, 0x99, 0x82,
	0x53, 0x90, 0xBE, 0x64, 0x23, 0x49, 0x75, 0x7B,
	0x0C, 0xEB, 0x2D, 0x68, 0x97, 0xD6, 0xAF, 0xB1,
	0xAA, 0x2A, 0xDE, 0x5E, 0x9B, 0xE3, 0x06, 0x0D,
	0xF2, 0xAC, 0xD9, 0xD7, 0x1F, 0x50, 0x6E, 0xC9,
	0x5D, 0xEB, 0xB4, 0xF0, 0xC0, 0x98, 0x23, 0x04,
	0x30, 0x46, 0x10, 0xDC, 0xD4, 0x6B, 0x57, 0xC7,
	0x30, 0xC3, 0x06, 0xDD, 0xAF, 0x51, 0x6E, 0x40,
	0x41, 0xF8, 0x10, 0xDE, 0x49, 0x18, 0x52, 0xB3,
	0x18, 0xCA, 0x49, 0x50, 0xA8, 0x3A, 0xCD, 0xB6,
	0x94, 0x7B, 0xDB, 0xF1, 0x2D, 0x05, 0xCE, 0x57,
	0x0B, 0xBE, 0x38, 0x48, 0xBB, 0xC9, 0xB1, 0x76,
	0x36, 0xB8, 0xA8, 0xCC, 0xE2, 0x07, 0x5C, 0xC8,
	0x7B, 0xCF, 0xCF, 0xF0, 0xFA, 0xA3, 0xC5, 0xD7,
	0x3A, 0x5E, 0xB2, 0xF4, 0xBF, 0xEA, 0xC2, 0xED,
	0x51, 0x16, 0xA2, 0x92, 0x9C, 0x36, 0xA6, 0x86,
	0x0E, 0x24, 0xA5, 0x66, 0x15, 0xE7, 0x97, 0x22,
	0x50, 0x04, 0xFF, 0xC9, 0x4D, 0xB0, 0xBC, 0x27,
	0x05, 0x5E, 0x2C, 0xF7, 0xEF, 0xDC, 0x5D, 0x58,
	0xA1, 0x3B, 0x60, 0x83, 0xB7, 0x8C, 0xB7, 0xD0,
	0x36, 0x6D, 0x55, 0x2E, 0x05, 0x23, 0x63, 0x74,
	0x4A, 0x97, 0x37, 0xA7, 0x78, 0x40, 0xEF, 0x3E,
	0x66, 0xFD, 0xBA, 0x6E, 0xB3, 0x72, 0x4A, 0x21,
	0x82, 0x1F, 0x33, 0xAD, 0x62, 0x0C, 0xF2, 0x1A,
	0xD2, 0x6A, 0xB5, 0xA7, 0xF2, 0x51, 0x69, 0x1F,
	0x38, 0xA5, 0x57, 0x9A, 0xC5, 0x88, 0x67, 0xE3,
	0x11, 0xA6, 0x53, 0x4F, 0xB1, 0xE9, 0x07, 0x41,
	0xDE, 0xE8, 0xDF, 0x93, 0xA9, 0x99, 0x02, 0x03,
	0x01, 0x00, 0x01, 0xA3, 0x50, 0x30, 0x4E, 0x30,
	0x1D, 0x06, 0x03, 0x55, 0x1D, 0x0E, 0x04, 0x16,
	0x04, 0x14, 0xDF, 0xA0, 0x28, 0xD1, 0xAF, 0xB0,
	0x55, 0xE3, 0xC1, 0xF1, 0x33, 0x28, 0xED, 0x5C,
	0xDD, 0xC2, 0xBB, 0xBF, 0x22, 0x6E, 0x30, 0x1F,
	0x06, 0x03, 0x55, 0x1D, 0x23, 0x04, 0x18, 0x30,
	0x16, 0x80, 0x14, 0xDF, 0xA0, 0x28, 0xD1, 0xAF,
	0xB0, 0x55, 0xE3, 0xC1, 0xF1, 0x33, 0x28, 0xED,
	0x5C, 0xDD, 0xC2, 0xBB, 0xBF, 0x22, 0x6E, 0x30,
	0x0C, 0x06, 0x03, 0x55, 0x1D, 0x13, 0x04, 0x05,
	0x30, 0x03, 0x01, 0x01, 0xFF, 0x30, 0x0D, 0x06,
	0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01,
	0x01, 0x0B, 0x05, 0x00, 0x03, 0x82, 0x01, 0x01,
	0x00, 0x42, 0x3D, 0x98, 0x0C, 0x98, 0x54, 0xD0,
	0xD7, 0x7B, 0x04, 0x18, 0x5C, 0x8F, 0xD2, 0xC1,
	0xE9, 0xB4, 0xDA, 0x13, 0x76, 0x25, 0xD8, 0x9E,
	0x8D, 0x00, 0x4A, 0x03, 0x89, 0xF3, 0x15, 0xB0,
	0x0A, 0x80, 0x78, 0x5A, 0xB2, 0xAA, 0xBC, 0xE5,
	0x37, 0xF1, 0x4C, 0xAE, 0x34, 0x3B, 0xB1, 0x6B,
	0xD9, 0xF5, 0x8E, 0xA1, 0xFE, 0xC5, 0xED, 0x2E,
	0xA5, 0xD6, 0xA1, 0xDC, 0x13, 0xB7, 0x36, 0xEC,
	0xC5, 0x98, 0x9F, 0xE8, 0xA3, 0x22, 0x66, 0x88,
	0xF2, 0x94, 0x5D, 0x9C, 0x8C, 0x6F, 0xAB, 0x81,
	0x05, 0x3D, 0xE0, 0x9E, 0x5B, 0x03, 0xA9, 0xCA,
	0x54, 0x8F, 0xDC, 0xE2, 0xD6, 0x0E, 0xDA, 0x15,
	0x96, 0xAF, 0x47, 0xA1, 0x99, 0xA8, 0x37, 0xD6,
	0xED, 0xBE, 0x3F, 0x4A, 0x4A, 0x9A, 0xC0, 0x05,
	0x77, 0x6F, 0x1E, 0x62, 0xCB, 0x11, 0x74, 0xDF,
	0x6D, 0xB7, 0xFF, 0x42, 0x77, 0xB3, 0x29, 0x6C,
	0x38, 0x6F, 0xBA, 0xE5, 0x5F, 0xB7, 0x23, 0x2F,
	0x53, 0x19, 0xC0, 0x49, 0x09, 0x18, 0x50, 0x9D,
	0x0F, 0xFE, 0x6C, 0xA4, 0xBD, 0x35, 0x2A, 0xDD,
	0xCF, 0xF8, 0xB6, 0x42, 0x06, 0xE1, 0x53, 0xCA,
	0xC3, 0xF6, 0xA6, 0x70, 0x82, 0x3C, 0x3B, 0x1F,
	0x19, 0x93, 0xBE, 0xC5, 0xB8, 0x11, 0x28, 0xEC,
	0x66, 0xB6, 0xA5, 0x3A, 0x35, 0x82, 0x17, 0x1A,
	0x3C, 0x4E, 0x3E, 0x25, 0xFE, 0x4C, 0xA3, 0x1F,
	0xCA, 0xFB, 0xE6, 0xF8, 0x3B, 0x61, 0xE2, 0x33,
	0x5C, 0x89, 0x66, 0x0F, 0xFB, 0x99, 0xFE, 0xFD,
	0xDB, 0xC2, 0xB7, 0xA8, 0xCF, 0x45, 0xC9, 0xF1,
	0x67, 0xB8, 0xA8, 0x97, 0xF4, 0xA5, 0x90, 0xA1,
	0x99, 0xAA, 0xA6, 0xFF, 0x47, 0x38, 0x13, 0x82,
	0xE4, 0x81, 0x84, 0x36, 0xC3, 0x8C, 0xE5, 0xDD,
	0x7A, 0x70, 0xF8, 0x70, 0x27, 0xFB, 0xCD, 0xC2,
	0xA0, 0xA9, 0xC5, 0x61, 0xA4, 0x3C, 0x9D, 0x1B,
	0x57
};

static const TPM2B_PUBLIC_KEY_RSA RSA_2048_D = {
	.t  = {256, {
			0x4e, 0x9d, 0x02, 0x1f, 0xdf, 0x4a, 0x8b, 0x89,
			0xbc, 0x8f, 0x14, 0xe2, 0x6f, 0x15, 0x66, 0x5a,
			0x67, 0x70, 0x19, 0x7f, 0xb9, 0x43, 0x56, 0x68,
			0xfb, 0xaa, 0xf3, 0x26, 0xdb, 0xad, 0xdf, 0x6e,
			0x7c, 0xb4, 0xa3, 0xd0, 0x26, 0xbe, 0xf3, 0xa3,
			0xdc, 0x8f, 0xdf, 0x74, 0xf0, 0x89, 0x5e, 0xca,
			0x86, 0x31, 0x2c, 0x33, 0x80, 0xea, 0x29, 0x19,
			0x39, 0xad, 0x32, 0x9f, 0x14, 0x20, 0x95, 0xc0,
			0x40, 0x1b, 0xa3, 0xa4, 0x91, 0xf7, 0xea, 0xc1,
			0x35, 0x16, 0x87, 0x96, 0x0a, 0x76, 0x96, 0x02,
			0x6b, 0xa2, 0xc0, 0xd3, 0x8d, 0xc6, 0x32, 0x4e,
			0xaf, 0x8b, 0xae, 0xdc, 0x42, 0x47, 0xc1, 0x85,
			0x6e, 0x5e, 0x94, 0xf2, 0x52, 0xfa, 0x27, 0xe7,
			0x22, 0x24, 0x94, 0xeb, 0x67, 0xbe, 0x1e, 0xe4,
			0x82, 0x91, 0xde, 0x71, 0x0a, 0xb8, 0x23, 0x1a,
			0x02, 0xe7, 0xcc, 0x82, 0x06, 0xd2, 0x26, 0x15,
			0x54, 0x97, 0x52, 0xcd, 0xf5, 0x3f, 0x6d, 0xc6,
			0xb9, 0x70, 0x30, 0xbe, 0xc5, 0x88, 0xa6, 0xb0,
			0x65, 0x16, 0x9c, 0x4c, 0x84, 0xe2, 0x7a, 0x6e,
			0xe9, 0xc7, 0xbd, 0xcf, 0x45, 0x27, 0xfc, 0x19,
			0xc6, 0x23, 0x1d, 0x2b, 0x88, 0xa2, 0x67, 0x1f,
			0xc2, 0xd6, 0xd3, 0xa0, 0x79, 0xfb, 0xbf, 0xea,
			0x38, 0xa8, 0xdf, 0x4f, 0xbc, 0x9b, 0x8e, 0xee,
			0x04, 0xb7, 0x7c, 0x00, 0xd7, 0x95, 0x1a, 0x03,
			0x82, 0x7a, 0xe8, 0x41, 0xb8, 0xb1, 0xaf, 0x7f,
			0xf1, 0x30, 0x89, 0x56, 0x6d, 0x07, 0x11, 0x55,
			0x79, 0xdd, 0x68, 0x0f, 0x82, 0x08, 0x5c, 0xcc,
			0x24, 0x47, 0x54, 0x68, 0x86, 0xf1, 0xf0, 0x3f,
			0x52, 0x10, 0xad, 0xe4, 0x16, 0x33, 0x16, 0x02,
			0x21, 0x62, 0xe3, 0x2f, 0x5d, 0xeb, 0x22, 0x5b,
			0x64, 0xb4, 0x29, 0x22, 0x74, 0x24, 0x29, 0xa9,
			0x4c, 0x66, 0x84, 0x31, 0xca, 0x99, 0x95, 0xf5
		}
	}
};

static const TPM2B_PUBLIC_KEY_RSA RSA_2048_P = {
	.t  = {128, {
			0xc8, 0x80, 0x6f, 0xf6, 0x2f, 0xfb, 0x49, 0x8b,
			0x77, 0x39, 0xe2, 0x3d, 0x3d, 0x1f, 0x4d, 0xf9,
			0xbb, 0x54, 0x06, 0x0d, 0x71, 0xbf, 0x54, 0xb1,
			0x1e, 0xa2, 0x20, 0x7e, 0xdd, 0xcf, 0x21, 0x16,
			0xe9, 0xc0, 0xba, 0x94, 0x02, 0xd2, 0xa4, 0x2e,
			0x78, 0x3c, 0xfb, 0x64, 0xa0, 0xe7, 0xe9, 0x27,
			0x64, 0x29, 0x19, 0x74, 0xc5, 0x77, 0xbb, 0xe1,
			0x6d, 0xb4, 0x83, 0x1d, 0x43, 0x5a, 0x80, 0x72,
			0xec, 0x3c, 0x32, 0xc3, 0x20, 0x2c, 0xce, 0xf7,
			0xba, 0xf6, 0xc6, 0x0c, 0xf4, 0x56, 0xfd, 0xdf,
			0x21, 0x55, 0xf3, 0xe2, 0x56, 0x25, 0xa6, 0xb3,
			0x96, 0xa4, 0x9c, 0xb8, 0xfd, 0x9c, 0xec, 0x87,
			0xfa, 0xda, 0x2e, 0xa4, 0xf6, 0x0f, 0x14, 0xe6,
			0x81, 0x22, 0x84, 0xe7, 0xc0, 0x1d, 0xd1, 0x3f,
			0xed, 0xb0, 0xba, 0xd8, 0xe4, 0xe9, 0xd4, 0x18,
			0x33, 0xae, 0x29, 0x51, 0x79, 0x79, 0xd1, 0x0f
		}
	}
};

static const TPM2B_PUBLIC_KEY_RSA RSA_2048_Q = {
	.t  = {128, {
			0xc8, 0x41, 0x2a, 0x42, 0xf1, 0x6a, 0x81, 0xac,
			0x06, 0xab, 0xd0, 0xb7, 0xc0, 0xbb, 0xc6, 0x13,
			0xdd, 0xfd, 0x5e, 0x3c, 0x77, 0xfe, 0xc1, 0x2e,
			0x76, 0xf0, 0x94, 0xc0, 0x5d, 0x24, 0x8b, 0x30,
			0x0d, 0xf8, 0x2a, 0xc7, 0x26, 0x78, 0x1b, 0x81,
			0x5a, 0x42, 0x96, 0xad, 0xf7, 0x0e, 0xa4, 0x1b,
			0x2c, 0x8f, 0x38, 0x06, 0x05, 0x8d, 0x98, 0x6e,
			0x37, 0x65, 0xb4, 0x2c, 0x80, 0xe2, 0x38, 0xd5,
			0x79, 0xd2, 0xea, 0x62, 0xf2, 0x32, 0xac, 0x7b,
			0x88, 0x90, 0xc3, 0x4e, 0x9e, 0x53, 0xe5, 0x7e,
			0xef, 0x13, 0xb1, 0xe3, 0xd5, 0x41, 0xd1, 0xa9,
			0x15, 0x04, 0x3c, 0x61, 0x74, 0x5e, 0x1a, 0x00,
			0x5c, 0x8a, 0x8b, 0x17, 0xd5, 0x78, 0xad, 0x5e,
			0xe0, 0xcf, 0x35, 0x63, 0x0a, 0x95, 0x1e, 0x70,
			0xbe, 0x97, 0xf2, 0xd3, 0x78, 0x06, 0x8a, 0x88,
			0x9b, 0x27, 0xc8, 0xb2, 0xb1, 0x3d, 0x8a, 0xd7
		}
	}
};

#define MAX_MSG_BYTES RSA_MAX_BYTES
#define MAX_LABEL_LEN 32

/* 128-byte buffer to hold entropy for generating a
 * 2048-bit RSA key (assuming ~112 bits of security strength,
 * the TPM spec requires a seed of minimum size 28-bytes). */
TPM2B_BYTE_VALUE(128);

static void rsa_command_handler(void *cmd_body,
				size_t cmd_size,
				size_t *response_size_out)
{
	uint8_t *cmd;
	uint8_t op;
	uint8_t padding_alg;
	uint8_t hashing_alg;
	uint16_t key_len;
	uint16_t in_len;
	uint8_t in[MAX_MSG_BYTES];
	uint16_t digest_len;
	uint8_t digest[SHA_DIGEST_MAX_BYTES];
	uint8_t *out = (uint8_t *) cmd_body;
	TPM2B_PUBLIC_KEY_RSA N;
	TPM2B_PUBLIC_KEY_RSA d;
	TPM2B_PUBLIC_KEY_RSA p;
	TPM2B_PUBLIC_KEY_RSA q;
	RSA_KEY key;
	uint32_t *response_size = (uint32_t *) response_size_out;
	TPM2B_PUBLIC_KEY_RSA rsa_d;
	TPM2B_PUBLIC_KEY_RSA rsa_n;

	TPM2B_128_BYTE_VALUE seed;
	uint8_t bn_buf[RSA_MAX_BYTES];
	struct LITE_BIGNUM bn;
	char label[MAX_LABEL_LEN];

	/* This is the SHA-256 hash of the RSA template from the TCG
	 * EK Credential Profile spec.
	 */
	TPM2B_32_BYTE_VALUE RSA_TEMPLATE_EK_EXTRA = {
		.t = {32, {
				0x68, 0xd1, 0xa2, 0x41, 0xfb, 0x27, 0x2f, 0x03,
				0x90, 0xbf, 0xd0, 0x42, 0x8d, 0xad, 0xee, 0xb0,
				0x2b, 0xf4, 0xa1, 0xcd, 0x46, 0xab, 0x6c, 0x39,
				0x1b, 0xa3, 0x1f, 0x51, 0x87, 0x06, 0x8e, 0x6a
			}
		}
	};

	assert(sizeof(size_t) == sizeof(uint32_t));

	/* Command format.
	 *
	 *   OFFSET       FIELD
	 *   0            OP
	 *   1            PADDING
	 *   2            HASHING
	 *   3            MSB KEY LEN
	 *   4            LSB KEY LEN
	 *   5            MSB IN LEN
	 *   6            LSB IN LEN
	 *   7            IN
	 *   7 + IN_LEN   MSB DIGEST LEN
	 *   8 + IN_LEN   LSB DIGEST LEN
	 *   9 + IN_LEN   DIGEST
	 */
	cmd = (uint8_t *) cmd_body;
	op = *cmd++;
	padding_alg = *cmd++;
	hashing_alg = *cmd++;
	key_len = ((uint16_t) (cmd[0] << 8)) | cmd[1];
	cmd += 2;
	in_len = ((uint16_t) (cmd[0] << 8)) | cmd[1];
	cmd += 2;
	if (in_len > sizeof(in)) {
		*response_size = 0;
		return;
	}
	memcpy(in, cmd, in_len);

	/* Make copies of N, and d, as const data is immutable. */
	switch (key_len) {
	case 768:
		N = RSA_768_N;
		d = RSA_768_D;
		p = RSA_768_P;
		q = RSA_768_Q;
		rsa_n.b.size = RSA_768_N.b.size;
		rsa_d.b.size = RSA_768_D.b.size;
		break;
	case 1024:
		N = RSA_1024_N;
		d = RSA_1024_D;
		p = RSA_1024_P;
		q = RSA_1024_Q;
		rsa_n.b.size = RSA_1024_N.b.size;
		rsa_d.b.size = RSA_1024_D.b.size;
		break;
	case 2048:
		N = RSA_2048_N;
		d = RSA_2048_D;
		p = RSA_2048_P;
		q = RSA_2048_Q;
		rsa_n.b.size = RSA_2048_N.b.size;
		rsa_d.b.size = RSA_2048_D.b.size;
		break;
	default:
		*response_size = 0;
		return;
	}
	key.exponent = 65537;
	key.publicKey = &N.b;
	key.privateKey = &d.b;

	switch (op) {
	case TEST_RSA_ENCRYPT:
		if (_cpri__EncryptRSA(
				response_size, out, &key,
				padding_alg, in_len, in, hashing_alg, "")
			!= CRYPT_SUCCESS)
			*response_size = 0;
		return;
	case TEST_RSA_DECRYPT:
		if (_cpri__DecryptRSA(
				response_size, out, &key,
				padding_alg, in_len, in, hashing_alg, "")
			!= CRYPT_SUCCESS)
			*response_size = 0;
		return;
	case TEST_RSA_SIGN:
		if (_cpri__SignRSA(
				response_size, out, &key,
				padding_alg, hashing_alg, in_len, in)
			!= CRYPT_SUCCESS)
			*response_size = 0;
		return;
	case TEST_RSA_VERIFY:
		cmd += in_len;
		digest_len = ((uint16_t) (cmd[0] << 8)) | cmd[1];
		cmd += 2;
		if (digest_len > sizeof(digest)) {
			*response_size = 0;
			return;
		}
		memcpy(digest, cmd, digest_len);

		if (_cpri__ValidateSignatureRSA(
				&key, padding_alg, hashing_alg, digest_len,
				digest, in_len, in, 0)
			!= CRYPT_SUCCESS) {
			*response_size = 0;
		} else {
			*out = 1;
			*response_size = 1;
		}
		return;
	case TEST_RSA_KEYTEST:
		if (_cpri__TestKeyRSA(&rsa_d.b, 65537, &rsa_n.b, &p.b, &q.b)
			!= CRYPT_SUCCESS) {
			*response_size = 0;
			return;
		}
		if (memcmp(rsa_n.b.buffer, key.publicKey->buffer,
				rsa_n.b.size) != 0 ||
			memcmp(rsa_d.b.buffer, key.privateKey->buffer,
					rsa_d.b.size) != 0) {
				*response_size = 0;
				return;
		}

		if (_cpri__TestKeyRSA(&rsa_d.b, 65537, key.publicKey,
					&p.b, NULL) != CRYPT_SUCCESS) {
			*response_size = 0;
			return;
		}
		if (memcmp(rsa_d.b.buffer, key.privateKey->buffer,
				rsa_d.b.size) != 0) {
			*response_size = 0;
			return;
		}
		*out = 1;
		*response_size = 1;
		return;
	case TEST_RSA_KEYGEN:
		if (in_len > MAX_LABEL_LEN - 1) {
			*response_size = 0;
			return;
		}
		N.b.size = sizeof(N.t.buffer);
		p.b.size = sizeof(p.t.buffer);
		seed.b.size = sizeof(VERIFY_SEED);
		memcpy(seed.b.buffer, VERIFY_SEED, sizeof(VERIFY_SEED));
		if (in_len > 0) {
			memcpy(label, in, in_len);
			label[in_len] = '\0';
		}
		if (_cpri__GenerateKeyRSA(
				&N.b, &p.b, key_len, RSA_F4, TPM_ALG_SHA256,
				&seed.b, in_len ? label : NULL,
				&RSA_TEMPLATE_EK_EXTRA.b, NULL)
			!= CRYPT_SUCCESS) {
			*response_size = 0;
		} else {
			memcpy(out, N.b.buffer, N.b.size);
			memcpy(out + N.b.size, p.b.buffer, p.b.size);
			*response_size = N.b.size + p.b.size;
		}
		return;
	case TEST_BN_PRIMEGEN:
		if (in_len > sizeof(bn_buf)) {
			*response_size = 0;
			return;
		}
		DCRYPTO_bn_wrap(&bn, bn_buf, in_len);
		memcpy(bn_buf, in, in_len);
		if (DCRYPTO_bn_generate_prime(&bn)) {
			memcpy(out, bn.d, bn_size(&bn));
			*response_size = bn_size(&bn);
		} else {
			*response_size = 0;
		}
		return;
	case TEST_X509_VERIFY:
	{
		int result;
		struct RSA rsa;

		if (key.publicKey != &N.b) {
			*response_size = 0;
			return;
		}

		reverse_tpm2b(key.publicKey);
		rsa.e = key.exponent;
		rsa.N.dmax = key.publicKey->size / sizeof(uint32_t);
		rsa.N.d = (struct access_helper *) &key.publicKey->buffer;
		rsa.d.dmax = 0;
		rsa.d.d = NULL;

		result = DCRYPTO_x509_verify(RSA_2048_CERT,
					sizeof(RSA_2048_CERT), &rsa);
		reverse_tpm2b(key.publicKey);

		if (!result) {
			*response_size = 0;
			return;
		}
		*out = 0x01;
		*response_size = 1;
	}
	}
}

DECLARE_EXTENSION_COMMAND(EXTENSION_RSA, rsa_command_handler);

#endif   /* CRYPTO_TEST_SETUP */
