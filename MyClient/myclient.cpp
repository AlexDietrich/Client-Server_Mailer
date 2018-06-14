/* myclient.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#define BUF 1024

void receive(int fd, char *buf);

int main (int argc, char **argv) {
    //ToDo: Put this in Config file
    const char SUCCESS[4] = "OK\n";
    const char FAILURE[5] = "ERR\n";
    //const char EXECUTEPENDING[4] = "EP\n";
    u_int16_t PORT;
    char* SERVERADRESS;
  int create_socket;
  char buffer[BUF];
  struct sockaddr_in address;
  //long size;

  if( argc < 3 ){
     printf("Usage: %s <Port-number> <ServerAdresse>\n", argv[0]);
     exit(EXIT_FAILURE);
  }
    else{
      PORT = (uint16_t)atoi(argv[1]);
      SERVERADRESS = argv[2];
  }

  if ((create_socket = socket (AF_INET, SOCK_STREAM, 0)) == -1)
  {
     perror("Socket error");
     return EXIT_FAILURE;
  }
  
  memset(&address,0,sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons (PORT);
  inet_aton (SERVERADRESS, &address.sin_addr);

  if (connect ( create_socket, (struct sockaddr *) &address, sizeof (address)) == 0)
  {
      printf ("Connection with server (%s) established\n", inet_ntoa (address.sin_addr));
      //expect a response;
      receive(create_socket, buffer);
      printf ("%s ", buffer);
  }
  else
  {
     perror("Connect error - no server available");
     return EXIT_FAILURE;
  }
    //connected, do something till user wishes to quit:
  do {
      //send command
      fgets (buffer, BUF, stdin);
      send(create_socket, buffer, strlen (buffer), 0);

      //receive response
      receive(create_socket, buffer);

      //start parameter sending
      while (strcmp (buffer, SUCCESS) != 0 && strcmp (buffer, FAILURE) != 0){ //As long as the end wasn't reached (natural or error)
          //print received instructions (status message)
          printf ("%s ", buffer);
          //get user input and send
          fgets (buffer, BUF, stdin);
          send(create_socket, buffer, strlen (buffer), 0);
          //receive response
          receive(create_socket, buffer);
      }

      //override received status message with result:
      send(create_socket, SUCCESS, strlen(SUCCESS), 0); //send confirmation
      strcpy(buffer, "");
      receive(create_socket, buffer);

      //quit if command is quit
      if(strcmp (buffer, "quit\n") == 0){
          break;
      }

      //Print only result
      printf ("%s ", buffer);
      //request next command
      printf ("Please enter your command:\n");

  } while (true);
  close (create_socket);
  return EXIT_SUCCESS;
}

void receive(int fd, char *buf){
    long size=recv(fd,buf, BUF-1, 0);
    if (size>0)
    {
        buf[size]= '\0';
    }
}
