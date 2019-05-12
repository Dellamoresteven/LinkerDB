#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <iostream>

#define PORT 8080

/**
 *  This function will accept a proccess and send the command to database.cc and
 *  write this back to the socket as a response.
 */
void processCommand(int);

#define debug 1
/* This is going to be where everything starts up, I will add calls to database.cc
   so when a request comes in I will summon database.cc and get the response and
   send that back to the requester */
int main(int argc, char const *argv[])
{
  int server_fd;
  int new_socket;
  struct sockaddr_in address;
  /* need this for binding new sockets */
  int addrlen = sizeof(address);

  /**
   *  Creating my socket
   *  AF_INET = IP (4)
   *  SOCK_STREAM = Virtual Circuit Service
   *  0 = protocal (0 because there is only 1 type for SOCKS_STREAM)
   */
  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if(debug) printf("Server Created on port %d", PORT);

  /* fulling out the things needed in the sockaddr_in struct */
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( PORT );

  memset(address.sin_zero, '\0', sizeof address.sin_zero);

  /* Bind my server to the struct */
  bind(server_fd, (struct sockaddr *)&address, sizeof(address));

  /* Starting listening */
  listen(server_fd, 30);

  for(;;){
    printf("\n=========== Waiting ===========\n\n");

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
      perror("Error accept");
      exit(EXIT_FAILURE);
    }
    /* I am going to create a new process to process the request sent by the user.
       The reason I am doing it this way is for scaleabilty, I want to be able to
       get a lot of requests and be able to process them quickly and safely. As
       learned in CS252 pool of proccess is the way to go */

    /* Fork the new request */
    pid_t ret = fork();

    if (ret == 0) { /* Child */
      processCommand(new_socket);
      close(new_socket);
      exit(1);
    }
    printf("\n\nEFASDF\n\n");
    close(new_socket);
    printf("\n\nEFASDF2\n\n");
  }

  return 0;
}

/* @link LINE: 12 */
void processCommand(int fd){
  return;
  // std::string resp = "MY RESPONSE";
  // char buffer[30000] = {0};
  // long valread = read( fd , buffer, 30000);
  //
  // write(fd , resp.c_str() , resp.length());
  // printf("------------------Incomming Message-------------------\n");
  // printf("%s\n", buffer);
  // printf("-------------------------Done-------------------------\n");
}
