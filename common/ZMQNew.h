#ifndef _ZMQNew_H
#define _ZMQNew_H

#include <stdarg.h>
#include <stdio.h>
#include <zmq.h>
#include <string.h>

typedef struct zmqio zmqio_t;

zmqio_t *z_new_server(const char **addr, long num);
zmqio_t *z_new_client(const char *addr);
int z_recv(zmqio_t *ctx, void *p, size_t *sz);
int z_recv_many(zmqio_t *ctx, int ndata, ...);
int z_send(zmqio_t *ctx, const void *p, size_t sz, int more);
int z_send_str(zmqio_t *ctx, const char *tag, size_t tag_len, const char *s);
int z_send_many(zmqio_t *ctx, int ndata, ...);
void z_close(zmqio_t *ctx);

#endif