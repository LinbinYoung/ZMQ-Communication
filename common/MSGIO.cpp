#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#ifndef _ZMQNew_H
extern "C" {
	#include "ZMQNew.h"
}
#endif

#ifndef _MSGIO_H
#include "MSGIO.h"
#endif

using namespace std;

MSGIO::MSGIO(zmqio_t *z, const char* port_msg){
	zmq = z;
	memcpy(port, port_msg, strlen(port_msg));
	RS = NOT_STARTED;
}

MSGIO::~MSGIO(){
}

void do_attestation(){
	
}