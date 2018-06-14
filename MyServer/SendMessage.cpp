//
// Created by alexander on 12.10.17.
//

#include <netinet/in.h>
#include <queue>
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include "SendMessage.h"
#include <chrono>
#include <fstream>

SendMessage::SendMessage(const char *directory) : ServerOperation(directory) {
    index = 1;
    statusMessage = "Sender:";
}

string SendMessage::execute() {
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    );
    /// save receiver with \n on the end of the string
    string receiver_print = receiver;
    ///delete \n from the end of the string for routing in directories
    receiver.pop_back();
    ///build correct path into directory of the receiver
    string path_of_Directory = string(MESSAGEDIR) + '/' + receiver;
    ///build a unique path to the mail txt file
    string path_to_file = string(MESSAGEDIR) + '/' + receiver + '/' + std::to_string(ms.count()) + ".txt";

    ///convert all strings into char * for functions opendir, mkdir, fprintf
    const char * path = path_of_Directory.c_str();
    const char * path_to_File = path_to_file.c_str();
    const char * sender_char = sender.c_str();
    const char * subject_char = subject.c_str();
    const char * message_final_char = message_final.c_str();

    ///check if the directory is available, if not - create new
    DIR* test = opendir(path);
    if(test == nullptr){
        mkdir(path, 0777);
    }
    else{
        closedir(test);
    }

    ///open the file with the unique path
    fstream mail;
    mail.open(path_to_File, fstream::out);

    ///write all information into the mail-txt file.
    mail << "Sender: " << sender_char;
    mail << "Subject: " << subject_char;
    mail << message_final_char;

    ///close the file
    mail.close();

    statusMessage = SUCCESS;
    return "Message sent!\n";
}

bool SendMessage::fillMe(string message) {
    ///check if the input from client is just a . and \n, if yes - the input from user is finished
    if(message != ".\n"){
        switch(index){
            case 1:
                ///first information for the server is the username, which is the sender
                if(message.length() <= 9 && message.length() > 1){
                    sender = message;
                    index ++;
                    ///give the client the information about the next required input.
                    statusMessage = "Receiver:";
                    return true;
                }
                ///if the input is not valid - return false and give the client information about required form for the input.
                statusMessage = "Invalid Sender - MIN 1 Character and MAX 8 Characters!!!";
                return false;
            case 2:
                ///second information the server needs is the username from the receiver
                if(message.length() <= 9 && message.length() > 1){
                    receiver = message;
                    index ++;
                    statusMessage = "Subject:";
                    return true;
                }
                statusMessage = "Invalid Receiver - Min 1 Character andi Max 8 Characters";
                return false;
            case 3:
                ///check if the 3rd input is correct, has to be the subject and not longer as 80 characters
                if(message.length() <= 81){
                    subject = message;
                    index ++;
                    statusMessage = "Message-Line:";
                    return true;
                }
                 statusMessage = "Invalid Subject - Max 80 Characters!";
                return false;
            default:
                ///the last input is the message. At each new line the message gets added to the messages before.
                message_final += message;
                return true;
        }
    }/// check if the index is 4 and all steps are done succesfully - if yes set the server status to "ready of execute" and return true
    else if(index == 4){
        statusMessage = EXECUTEPENDING;
        return false;
    }else{
        ///if something went wront in the steps above - return false and give the client the information about invalid input.
        statusMessage = "Invalid Input - Operation cancelled!";
        return false;
    }
}