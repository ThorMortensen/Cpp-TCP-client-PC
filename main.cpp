/* 
 * File:   main.cpp
 * Author: Thor
 *
 * Created on 24. marts 2015, 16:00
 */

#include <stdint.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstdlib>

//For network 
#include <unistd.h>//For recognising the 'close' systemcall in cygwin 
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

#include "ProgArg_s.h"

#define PATH_ARG 1
#define EXPECTED_NO_OF_ARGS 4+PATH_ARG
#define NO_FLAGS 0
#define CMD_AMOUNT 10
#define RX_BUFFER_SIZE 1400




using namespace std;

//Can't return character for some strange reson?! Use this instead of std::endl;
static const string ENDL = ("\n\r");

//Print the usage for this program
static const string USAGE = ("Usage: -ip xxx.xxx.xxx.xxx -port xxxx");

static const string CLOSE_CONNECTION = ("KILL_C");

//Argument expected for this program 
ProgArg_s ipAddresForThisServer(1, "-ip", STRING, 12);
ProgArg_s portNrForThisServer(2, "-port", NUMBER);
std::vector<ProgArg_s*> args_v(2);

int main(int argc, char** argv) {

    int32_t errorCode = 0;
    int32_t sockedId = 0;

    char rxBuffer [RX_BUFFER_SIZE];

    bool keepAlive = true;
    string userInput;
    stringstream intConveter;
    int found = 0;


    args_v[0] = &ipAddresForThisServer;
    args_v[1] = &portNrForThisServer;

    struct addrinfo hostInfo;
    struct addrinfo* hostInfoList;

    memset(&hostInfo, 0, sizeof (hostInfo));

    //================== Argument Checks ===================
    if (argc < EXPECTED_NO_OF_ARGS || argc > EXPECTED_NO_OF_ARGS) {
        cout << "Wrong number of arguments" << ENDL;
        cout << USAGE << ENDL;
        return false;
    }

    // O(n^2)   :-)
    for (uint8_t i = 0; i < args_v.size(); i++) {//Get arguments  
        uint8_t j;
        for (j = 1; j < argc; j++) {//0.arg is always path. Don't check that 
            if (args_v[i]->equals(argv[j])) {
                if (!args_v[i]->isValid(argv[j + 1])) {
                    args_v[i]->printError();
                    cout << USAGE << ENDL;
                    return false;
                }
                break;
            }
        }
        if (!args_v[i]->hasValue) {
            args_v[i]->printError();
            cout << USAGE << ENDL;
        }
    }
    //Address info : address = Address field unspecifed (both IPv4 & 6)
    hostInfo.ai_addr = AF_UNSPEC;

    // Address info : socket type. (Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.)
    hostInfo.ai_socktype = SOCK_STREAM;


    errorCode = getaddrinfo(ipAddresForThisServer.getParamVal().c_str(), portNrForThisServer.getParamVal().c_str(), &hostInfo, &hostInfoList);
    if (errorCode != 0) std::cout << "getaddrinfo error" << gai_strerror(errorCode);

    sockedId = socket(hostInfoList->ai_family, hostInfoList->ai_socktype, hostInfoList->ai_protocol);
    if (sockedId == -1) std::cout << "Socket error" << strerror(errno);

    //Could have used a bind call before connect to specify what local port to use 

    //Connect to the address specefied. Comes from the getaddrinfo --> into to the hostInfoList
    errorCode = connect(sockedId, hostInfoList->ai_addr, hostInfoList->ai_addrlen);
    if (errorCode == -1) std::cout << "Connect error" << strerror(errno);

    freeaddrinfo(hostInfoList); //Not needed anymore.. buhu  :-(

    while (keepAlive) {

        getline(cin, userInput);

        if (userInput == CLOSE_CONNECTION) {
            keepAlive = false;
        }

        ssize_t bytesSent = send(sockedId, userInput.c_str(), userInput.length(), NO_FLAGS);
        if (bytesSent != userInput.length())std::cout << "Send error. Bytes lost";

        ssize_t bytesRx = recv(sockedId, rxBuffer, RX_BUFFER_SIZE, NO_FLAGS);
        if (bytesRx == 0) {
            std::cout << "host connection shut down." << ENDL;
            keepAlive = false;
        }
        if (bytesRx == -1)std::cout << "Rx error!" << strerror(errno) << ENDL;

        cout << bytesRx << " From Zybo: " << ENDL;
        rxBuffer[bytesRx] = 0;
        cout << rxBuffer << ENDL;
    }

    close(sockedId);

    return 0;
}
