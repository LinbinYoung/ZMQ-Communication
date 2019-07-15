#if !defined(SEND_FILE_H)
#define SEND_FILE_H

#include "zeromqio.h"
typedef struct sendfile sendfile_t;


#ifdef __cplusplus
extern "C" {
#endif

int sendfile_start(zmqio_t *z, unsigned char key[16], uint64_t cookie, sendfile_t **handle);
int sendfile_update(sendfile_t *handle, void *buffer, size_t buffer_len);
int sendfile_end(sendfile_t *handle);


#ifdef __cplusplus
}
#endif

#endif
