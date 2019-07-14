#ifndef _MSGIO_H
#include "MSGIO.h"
#endif

#ifndef _ZMQNew_H
extern "C" {
	#include "ZMQNew.h"
}
#endif

using namespace std;

#include "unistd.h"
#include <unordered_map>
#include <iostream>
#include <typeinfo>

typedef struct STUDNET{
    int age;
    int score;
    char port[5];
    char name[5];
    char SID[5];
}STD;

typedef struct TEACHER{
    char name[5];
    char SID[8];
}TEA;

int main(int argc, char** argv){
    const char *addr[] = {"tcp://127.0.0.1:8888", "tcp://127.0.0.1:7777"};
    char *port[] = {"8888", "7777"};
    zmqio *z;
    long num = 2;
    z = z_new_server(addr, num);
    unordered_map <char*, MSGIO*> table;
    for (int i = 0; i < num; i ++){
        table.insert(std::pair<char*, MSGIO*>(port[i], new MSGIO(z, port[i])));
    }
    cout << "Waiting for client to connect......\n";
    static char incomming[1024*1024];
    for (;;){
        // Receive MSG from Client at port 8888 and 7777
        size_t incoming_sz = sizeof(incomming);
        int rc = z_recv(z, incomming, &incoming_sz);
        if (rc){
            eprintf("Error reading from zmq: %d\n", rc);
            break;
        }
        STD recv_std;
        if (!memcmp(incomming, CLIENT_KEY_PREFIX, PREFIX_LEN)){
            long step = PREFIX_LEN;
            memcpy(&recv_std.age, &incomming[step], sizeof(recv_std.age));
            step = step + sizeof(recv_std.age);
            memcpy(&recv_std.score, &incomming[step], sizeof(recv_std.score));
            step = step + sizeof(recv_std.score);
            memcpy(&recv_std.port, &incomming[step], sizeof(recv_std.port));
            step = step + sizeof(recv_std.port);
            memcpy(&recv_std.name, &incomming[step], sizeof(recv_std.name));
            step = step + sizeof(recv_std.name);
            memcpy(&recv_std.SID, &incomming[step], sizeof(recv_std.SID));
            printf("Age: %d\n", recv_std.age);
            printf("Score: %d\n", recv_std.score);
            printf("Port: %s\n", recv_std.port);
            printf("Name: %s\n", recv_std.name);
            printf("SID: %s\n", recv_std.SID);
            sleep(5); //time for processing the data
            //Prepare the response
            TEA response = {"LIAN", "2014229"};
            z_send_many(z, 2, CLIENT_KEY_PREFIX, PREFIX_LEN, &response, sizeof(response));
        }
    }
    zmq_close(z);
    return 0;
}

