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

void MSGIO::do_attestation_tranfer(char *MSG){
	/*
		This Part should belong to service provider.
		The Third Party Service should locate at Client.
		There are three kinds of header:
		1. RA__: REMOTE_ATT
		2. DATA: PARAMETER
		3. CAL_: CALCULATION
	*/
	STD recv_std;
	if (!memcmp(MSG, REMOTE_ATT, PREFIX_LEN)){
		//do_attestation, RS record difference stage of Remote Attestation
		long step = PREFIX_LEN;
		TEA response = {"LIAN", "2014229"};
		switch (RS){
		case NOT_STARTED:
			/* code */
			memcpy(&recv_std, &MSG[step], sizeof(recv_std));
        	printf("Port: %s\n", recv_std.port);
        	sleep(5); //time for processing the data
        	//Prepare the response
        	z_send_many(zmq, 2, REMOTE_ATT, PREFIX_LEN, &response, sizeof(response));
			RS = NOT_STARTED;
			break;
		default:
			break;
		}//end of swith
	}else if (!memcpy(MSG, PARAMETER, PREFIX_LEN)){
		// Finish Remote Attestation and Start Tranfering Parameter
	}else if (!memcpy(MSG, CALCULATION, PREFIX_LEN)){
		// Finish Tranfering Data and Start Running Model
	}
	return;
}