#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

struct zmqio {
    void *ctx;
    void *socket;
    int is_server;
};

#ifndef _ZMQNew_H
#include "ZMQNew.h"
#endif

zmqio_t *z_new_server(const char **addr, long num){
	int rc;
	zmqio_t *ret = (zmqio_t *)malloc(sizeof(zmqio_t));
	if (!ret)
		return NULL;
	ret->ctx = zmq_ctx_new();
	ret->socket = zmq_socket(ret->ctx, ZMQ_REP);
	for (int i = 0; i < num; i ++){
		printf ("%s\n", addr[i]);
		rc = zmq_bind(ret->socket, addr[i]);
		if (rc) {
			z_close(ret);
			printf ("Fail to create the ZMQ Server!!!!!!");
			return NULL;
		}
	}
	ret->is_server = 1;
	return ret;
}

zmqio_t *z_new_client(const char *addr){
	zmqio_t *ret = (zmqio_t *)malloc(sizeof(zmqio_t));
	if (!ret)
		return NULL;
	ret->ctx = zmq_ctx_new();
	ret->socket = zmq_socket(ret->ctx, ZMQ_REQ);
	zmq_connect(ret->socket, addr);
	ret->is_server = 0;
	return ret;
}

int z_recv(zmqio_t *ctx, void *p, size_t *sz){
	printf("z_recv Entering\n");
	int rc, more, trunc = 0;
	//size_t more_size = sizeof(more);
	size_t maxsz = *sz;
	*sz = 0;
	do {
		zmq_msg_t part;
		size_t msz;
		rc = zmq_msg_init(&part);
		if (rc != 0) {
			rc = -1;
			break;
		}
		rc = zmq_msg_recv(&part, ctx->socket, 0);
		if (rc < 0) {
			break;
		}
		msz = zmq_msg_size(&part);
		if (msz > maxsz - *sz) {
			msz = maxsz - *sz;
			trunc = 1;
		}
		memcpy(p+*sz, zmq_msg_data(&part), msz);
		*sz += msz;
		more = zmq_msg_more(&part);
		zmq_msg_close(&part);
	} while (more && *sz < maxsz);
	if (rc < 0) {
		return -1;
	} else if (more || trunc) {
		return 1;
	} else {
		return 0;
	}
}

int z_recv_many(zmqio_t *ctx, int ndata, ...){
	int rc, more, trunc = 0;
	//size_t more_size = sizeof(more);
	char *cur, *msp;
	unsigned int remain = 0, tocopy;
	zmq_msg_t part;
	size_t msz;
	/*
		It is used when the number of parameters of a function is not set
	*/
	va_list ap;
	va_start(ap, ndata);
	do {
		rc = zmq_msg_init(&part);
		if (rc != 0) {
			rc = -1;
			break;
		}
		rc = zmq_msg_recv(&part, ctx->socket, 0);
		if (rc < 0) {
			break;
		}
		msz = zmq_msg_size(&part);
		msp = (char *)zmq_msg_data(&part);
		while (msz) {
			if (remain == 0) {
				if (ndata > 0) {
					--ndata;
					cur = (char *)va_arg(ap, void *);
					remain = va_arg(ap, unsigned int);
				} else {
					trunc = 1;
					break;
				}
			}
			tocopy = msz > remain ? remain : msz;
			memcpy(cur, msp, tocopy);
			msp += tocopy;
			cur += tocopy;
			remain -= tocopy;
			msz -= tocopy;
		}
		more = zmq_msg_more(&part);
		zmq_msg_close(&part);
	} while (more);
	va_end(ap);
	if (rc < 0) {
		return -1;
	} else if (more || trunc) {
		return 1;	/* Data from ZMQ is larger than buffer */
	} else if (ndata || remain) {
		return 2;	/* Buffer is larger than data from ZMQ */
	} else {
		return 0;
	}
}

int z_send(zmqio_t *ctx, const void *p, size_t sz, int more){
	int flag = 0;
	if (more) {
		flag = ZMQ_SNDMORE;
	}
	return (0>zmq_send(ctx->socket, p, sz, flag));
}

int z_send_str(zmqio_t *ctx, const char *tag, size_t tag_len, const char *s){
	return z_send(ctx, tag, tag_len, 1) || z_send(ctx, s, strlen(s), 0);
}

void z_close(zmqio_t *ctx)
{
	if (ctx) {
		zmq_close(ctx->socket);
		zmq_ctx_destroy(ctx->ctx);
		free(ctx);
	}
}

int z_send_many(zmqio_t *ctx, int ndata, ...){
	void *p;
	int sz;
	va_list ap;

	va_start(ap, ndata);
	while (ndata) {
		p = va_arg(ap, void *);
		sz = va_arg(ap, unsigned int);
		if (z_send(ctx, p, sz, ndata != 1)) {
			return 1;
		}
		--ndata;
	}
	va_end(ap);
	
	return 0;
}