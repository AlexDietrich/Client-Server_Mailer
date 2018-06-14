//
// Created by alexander on 12.10.17.
//

#ifndef V_SYS_TWMAILER_LISTMESSAGE_H
#define V_SYS_TWMAILER_LISTMESSAGE_H


#include "ServerOperation.h"

class ListMessage : public ServerOperation{
public:
    //Constructor
    ListMessage(const char * directory);

    virtual bool fillMe(string);

    //Searches for messages in specified Folder and lists them:
    //  <Number of Messages for specified User ?(0 if none were found)> \n
    //  <subject 1> \n
    //  <subject 2> \n
    //  ...
    //  <subject N> \n
    virtual string execute();

private:
    //Parses the input parameters
    string User;
};


#endif //V_SYS_TWMAILER_LISTMESSAGE_H
