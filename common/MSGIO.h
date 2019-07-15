#ifndef _MSGIO_H
#define _MSGIO_H

#include <sys/types.h>
#include <stdio.h>
#include <string>

#ifndef _ZMQNew_H
extern "C" {
	#include "ZMQNew.h"
}
#endif

#ifndef _COMMON_H
#include "common.h"
#endif

using namespace std;
/* A 1MB buffer should be sufficient for demo purposes */
#define MSGIO_BUFFER_SZ	1024*1024

#define DEFAULT_PORT "7777"		// A C string for getaddrinfo()

class MSGIO{
private:
	zmqio_t *zmq;
	char buffer[MSGIO_BUFFER_SZ];
	char port[10];
	RA_state RS;
public:
	MSGIO(zmqio_t *z, const char* port);
	~MSGIO();
	void do_attestation_tranfer(char *MSG);
};

#endif
