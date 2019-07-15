/*
    I am Server
*/
#ifndef _MSGIO_H
#include "MSGIO.h"
#endif

#ifndef _ZMQNew_H
extern "C" {
	#include "ZMQNew.h"
}
#endif

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
#include "hexutil.h"
#include "crypto.h"
#include "byteorder.h"
#include "msgio.h"
#include "protocol.h"
#include "base64.h"
#include "iasrequest.h"
#include "sendfile.h"
#include "readdata.h"
#include "readconfig.h"
#include <map>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <typeinfo>

static sgx_ec256_public_t client_pub_key;
static sgx_ec256_signature_t client_sig;

template <class _Tp>  
struct my_equal_to : public binary_function<_Tp, _Tp, bool>{
    bool operator()(const _Tp& __x, const _Tp& __y) const  
    { return strcmp( __x, __y ) == 0; }  
};

struct Hash_Func{
    //BKDR hash algorithm
    int operator()(char * str)const
    {
        int seed = 131;
        int hash = 0;
        while(*str)
        {
            hash = (hash * seed) + (*str);
            str ++;
        }
        return hash & (0x7FFFFFFF);
    }
};

typedef unordered_map<char*, MSGIO*, Hash_Func,  my_equal_to<char*> > my_unordered_map;

int main(int argc, char** argv){
    const char *addr[] = {"tcp://127.0.0.1:8888", "tcp://127.0.0.1:7777"};
    char *port[] = {"8888", "7777"};
    zmqio *z;
    long num = 2;
    z = z_new_server(addr, num);
    my_unordered_map table;
    for (int i = 0; i < num; i ++){
        table.insert(my_unordered_map::value_type(port[i], new MSGIO(z, port[i])));
    }
    cout << "Waiting for client to connect......\n";
    static char incomming[1024*1024];
    for (;;){
        size_t incoming_sz = sizeof(incomming);
        int rc = z_recv(z, incomming, &incoming_sz);
        if (rc){
            eprintf("Error reading from zmq: %d\n", rc);
            break;
        }
        char local_port[5];
        memcpy(local_port, &incomming[PREFIX_LEN], sizeof(local_port));
        printf("%s\n", local_port);
        MSGIO* msgio = table[local_port];
        msgio->do_attestation_tranfer(incomming);
    }
    zmq_close(z);
    return 0;
}