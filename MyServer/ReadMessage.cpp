//
// Created by alexander on 12.10.17.
//

#include "ReadMessage.h"
#include <dirent.h>
#include <fstream>

ReadMessage::ReadMessage(const char * directory) : ServerOperation(directory) {
    ParameterCount = 0;
    MessageNumber = 0; //give invalid message number at beginning
    statusMessage = "User:";
}


bool ReadMessage::fillMe(string parameter) {
    //fill the needed parameter
    if(ParameterCount == 0){
        if(parameter.length() <= 9 && parameter.length() > 1){ //is the Username given 1-8 chars long?
            User = parameter;
            User.pop_back();
            ParameterCount ++;
            statusMessage = "Message-number:";
        }
        else{
            statusMessage = "Invalid User - Max 8 Characters!";
            return false;
        }
    }
    else if(ParameterCount == 1){
        if(!parameter.empty() && isdigit(parameter[0]) && parameter[0] != '0'){ //is the parameter even a number bigger than 0?
            MessageNumber = stoi(parameter);
            ParameterCount ++;
            statusMessage = EXECUTEPENDING;
        }
        else{
            statusMessage = "Invalid Message Number - Must be integer bigger than 0!";
            return false;
        }
    }

    //check if ready and return true if != 2 and false if all parameters were given
    return ParameterCount != 2;
}

string ReadMessage::execute() {
    string result = "";
    int count = 0;
    string dir = string(MESSAGEDIR) + "/" + User;

    DIR* userDir = opendir(dir.c_str()); //Open User Directory

    struct dirent * userDirEntry; //individual entries in the directory.
    ifstream messageFile;
    string line;

    //if no directory found return 0
    if(userDir == nullptr){
        statusMessage = FAILURE;
        closedir(userDir);
        return "No such User \"" + User + "\" found\n";
    }

    //while directory isn't empty or didn't reach right file keep looking:
    while((userDirEntry = readdir(userDir)) != nullptr && count <= MessageNumber){
        //only read regular files
        if(userDirEntry->d_type == DT_REG){
            //ToDo: look if filename ends with .msg or .txt
            messageFile.open(dir + "/" + userDirEntry->d_name);
            if(messageFile.is_open()){
                //count as countable file
                count ++;
                if(count == MessageNumber){
                    //reading if reached right file
                    while(!messageFile.eof()){
                        //read till the end and save lines to the result:
                        getline(messageFile, line);
                        result.append(line);
                        result.append("\n");
                    }
                }
                //close file again
                messageFile.close();
            }
        }
    }

    if(result == ""){
        statusMessage = FAILURE;
        result = "No matching file found.\n";
    }

    statusMessage = SUCCESS;
    closedir(userDir);
    return result;
}
