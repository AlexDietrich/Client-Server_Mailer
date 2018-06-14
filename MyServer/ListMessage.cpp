//
// Created by alexander on 12.10.17.
//

#include <regex>
#include "ListMessage.h"
#include <dirent.h>
#include <fstream>



ListMessage::ListMessage(const char * directory) :ServerOperation(directory) {
    statusMessage = "User:";
}

//returns false either way, because first line after LIST command has to be the username
bool ListMessage::fillMe(string line) {

    unsigned long end = line.find('\n');

    if(end == string::npos || end == 0 || end > 9){
        //no \n found, or no user given (\n is first character), or username to long
        statusMessage = "Username \"" + line +  "\" not valid\n";
        return false; //eventually return true if we want to give the sender another chance of not being a total dick and sending bullshit.
    }
    else{
        //ignore \n if found:
        User = line.substr(0, end);
        statusMessage = EXECUTEPENDING;
        return false;
    }
}



string ListMessage::execute() {
    string result = "", subjects = "";
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

    //while directory isn't empty or didn't reach end keep looking:
    while((userDirEntry = readdir(userDir)) != nullptr){
        //only read regular files
        if(userDirEntry->d_type == DT_REG){
            //ToDo: look if filename ends with .msg or .txt
            messageFile.open(dir + "/" + userDirEntry->d_name);
            if(messageFile.is_open()){
                //reading if open
                getline(messageFile, line); //get First line->should be sender... ignore this part for now, maybe add to list later
                getline(messageFile, line); //get second line->should be subject
                //close file again
                messageFile.close();
                //update results
                count ++;
                subjects.append(to_string(count) + "...");
                subjects.append(line);
                subjects.append("\n");
            }
        }
    }
    result.append(to_string(count));
    result.append("\n");
    result.append(subjects);

    statusMessage = SUCCESS;

    closedir(userDir);
    return result;
}