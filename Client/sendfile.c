#include "config.h"
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <getopt.h>
#include <unistd.h>
#include <sgx_key_exchange.h>
#include <sgx_report.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include "common.h"
#include "hexutil.h"
#include "crypto.h"
#include "byteorder.h"
#include "protocol.h"
#include "base64.h"
#include "zeromqio.h"
//#include "settings.h"
#include "sendfile.h"

#undef SF_DEBUG

struct sendfile {
	zmqio_t *z;
	EVP_CIPHER_CTX *ctx;
#ifdef	SF_DEBUG
	EVP_MD_CTX *mdctx;
#endif
	size_t pos;
	uint64_t cookie;
};

static int send_data(sendfile_t *h, uint32_t sz, void *buf)
{
	char rep[256];
	size_t repsz = sizeof(rep);
	int rc;
	zmqio_t *z = h->z;

#ifdef	SF_DEBUG
	if(1 != EVP_DigestUpdate(h->mdctx, buf, sz))	{
		return -1;
	}
#endif

	z_send_many(z, 5, 
		DATA_WRITE_PREFIX, PREFIX_LEN, 
		&h->cookie, sizeof(uint64_t),
		&sz, sizeof(uint32_t),
		&h->pos, sizeof(uint64_t),
		buf, sz);
	rc = z_recv(z, rep, &repsz);
	if (rc) {
		return rc;
	}
	if (memcmp(rep, OK_PREFIX, 4)) {
		rep[repsz] = 0;
		eprintf("Error from zmq: %s\n", &rep[4]);
		return -1;
	}
	h->pos += sz;
	return 0;
}

int sendfile_start(zmqio_t *z, unsigned char key[16], uint64_t cookie, sendfile_t **handle)
{
	unsigned char iv[16];
	sendfile_t *ret;
	int retry;
	unsigned char ok;
	int i;
	
	if (!z || !key || !handle) {
		return -1;
	}
	ret = (struct sendfile *)malloc(sizeof(struct sendfile));
	if (!ret) {
		return -1;
	}
	
	if(!(ret->ctx = EVP_CIPHER_CTX_new())) {
		goto end;
	}

	for (i = 0; i < 16; i += 4) {
		retry = 10; ok = 0;
		while ( !ok && retry-- ) ok = _rdrand64_step((uint64_t *)&iv[i]);
		if ( !ok ) {
			eprintf("iv: RDRAND underflow\n");
		}
	}

	if(1 != EVP_EncryptInit_ex(ret->ctx, EVP_aes_128_ctr(), NULL, key, iv)) {
		goto end;
	}

#ifdef	SF_DEBUG
	if((ret->mdctx = EVP_MD_CTX_create()) == NULL) {
		goto end;
	}

	if(1 != EVP_DigestInit_ex(ret->mdctx, EVP_sha256(), NULL)) {
		goto end;
	}
#endif

	ret->z = z;
	ret->cookie = cookie;
	ret->pos = 0;
	if (send_data(ret, 16, iv)) {
		goto end;
	}

	*handle = ret;
	ret = NULL;
end:
	if (ret) {
		free(ret);
		return -1;
	} else {
		return 0;
	}
}

int sendfile_update(sendfile_t *h, void *buffer, size_t buffer_len)
{
	int len = 0;
	unsigned char *ciphertext = malloc(buffer_len);
	
	if (!ciphertext) {
		return -1;
	}

	if(1 != EVP_EncryptUpdate(h->ctx, ciphertext, &len, buffer, buffer_len)) {
		free(ciphertext);
		return -1;
    	}

	if (send_data(h, len, ciphertext)) {
		free(ciphertext);
		return -1;
	}

	free(ciphertext);
	return 0;
}

int sendfile_end(sendfile_t *h)
{
	unsigned char ciphertext[64];
	int len = 0;
	
	if(1 != EVP_EncryptFinal_ex(h->ctx, ciphertext, &len)) {
		return -1;
	}

	if (len > 0 && send_data(h, len, ciphertext)) {
		return -1;
	}

#ifdef	SF_DEBUG
	if(1 != EVP_DigestFinal_ex(h->mdctx, ciphertext, &len))	{
		return -1;
	}
	eprintf("Encrypted data hash %d:\n", len);
	eprint_hexstring(ciphertext, len);
	eprintf("\n");
	EVP_MD_CTX_destroy(h->mdctx);
#endif

	EVP_CIPHER_CTX_free(h->ctx);
	free(h);
}
