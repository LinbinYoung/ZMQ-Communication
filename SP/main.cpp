/*
    I am Client
*/
#ifndef _MSGIO_H
#include "MSGIO.h"
#endif

#ifndef _ZMQNew_H
extern "C" {
	#include "ZMQNew.h"
}
#endif

using namespace std;

#include <unordered_map>
#include <iostream>
#include <typeinfo>

int main(int argc, char** argv){
    printf("%s\n", "Connecting to the server........");
    const char *addr = "tcp://127.0.0.1:7777";
    zmqio *z;
    z = z_new_client(addr);
    // Send MSG to Server at 127.0.0.1:8888
    STD std_info = {"7777", 21, 90, "SHU", "2015"};
    static char incomming[1024*1024];
    size_t incomming_sz = sizeof(incomming);
    TEA recv_tea;
    z_send_many(z, 2, REMOTE_ATT, PREFIX_LEN, &std_info, sizeof(STD));
    int rc = z_recv(z, incomming, &incomming_sz);
    if (rc){
        eprintf("Error reading from zmq: %d\n", rc);
        return 0;
    }
    if (!memcmp(incomming, REMOTE_ATT, PREFIX_LEN)){
        long step = PREFIX_LEN;
        memcpy(&recv_tea.name, &incomming[step], sizeof(recv_tea.name));
        step = step + sizeof(recv_tea.name);
        memcpy(&recv_tea.SID, &incomming[step], sizeof(recv_tea.SID));
        printf("Name: %s\n", recv_tea.name);
        printf("SID: %s\n", recv_tea.SID);
    }
    return 0;
}