//
// Created by adietrich on 12.10.17.
//

#ifndef V_SYS_TWMAILER_SERVEROPERATION_H
#define V_SYS_TWMAILER_SERVEROPERATION_H

#include <string.h>
#include <stdio.h>
#include <string>
#include <map>



using namespace std;


//Class ServerOperation is the base Class for all the Operations this server can execute.
class ServerOperation {

protected:
    string statusMessage;
    //ToDo: Put this in Config file
    const char * MESSAGEDIR;
    const char USERDIR[6] = "users";
    const char SUCCESS[4] = "OK\n";
    const char FAILURE[5] = "ERR\n";
    const char EXECUTEPENDING[4] = "EP\n";


public:
    //Constructor
    ServerOperation(const char * directory);

    //fillMe fills internal arguments of operation with a string.
    //returns true id next line needs to be read and filled and false if Operation got all needed arguments or failed.
    virtual bool fillMe(string) = 0;

    string getStatus();

    //Executes the operation returns the server response as a string
    virtual string execute() = 0;
};


#endif //V_SYS_TWMAILER_SERVEROPERATION_H
