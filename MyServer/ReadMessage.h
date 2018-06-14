//
// Created by alexander on 12.10.17.
//

#ifndef V_SYS_TWMAILER_READMESSAGE_H
#define V_SYS_TWMAILER_READMESSAGE_H


#include "ServerOperation.h"

class ReadMessage : public ServerOperation {
public:
    //Constructor
    ReadMessage(const char * directory);

    virtual bool fillMe(string);

    //Searches for message in specified Folder and reads it, returns:
    //  <whole content of specified message> \n
    //or empty string if nothing was found.
    virtual string execute();

private:
    //Parses the input parameters
    string User;
    int MessageNumber;
    int ParameterCount;
};


#endif //V_SYS_TWMAILER_READMESSAGE_H
