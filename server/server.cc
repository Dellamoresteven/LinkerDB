#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <iostream>

#include "../include/LinkerDB.h"

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
int main(int argc, char *argv[])
{
  /* To check if the user wants a different port then 8080 */
  int portNum = PORT;
  if(argc == 2){
    portNum = atoi(argv[1]);
  }

  initDB();
  read();
  /* The servers Socket */
  int server_fd;
  /* For incoming connections */
  int new_socket;
  /* The strcut needed for accepting sockets */
  struct sockaddr_in address;
  /* need this for binding new sockets */
  int addrlen = sizeof(address);
  /* Thread vector */
  std::vector<std::thread> workers;

  /**
   *  Creating my socket
   *  AF_INET = IP (4)
   *  SOCK_STREAM = Virtual Circuit Service
   *  IPPROTO_TCP = TCP protocal (the only that can be used with sock_stream)
   */
  server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if(debug) printf("Server Created on port %d", portNum);

  /* fulling out the things needed in the sockaddr_in struct */
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( portNum );

  memset(address.sin_zero, '\0', sizeof address.sin_zero);

  /* Bind my server to the struct */
  bind(server_fd, (struct sockaddr *)&address, sizeof(address));

  /* Starting listening */
  listen(server_fd, 5);

  for(;;){
    if(debug) printf("\n================= Waiting ==================\n\n");

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
      perror("Error accept");
      exit(EXIT_FAILURE);
    }

    /* I am going to create a new process to process the request sent by the user.
       The reason I am doing it this way is for scaleabilty, I want to be able to
       get a lot of requests and be able to process them quickly and safely. As
       learned in CS252 pool of proccess is the way to go */
    /* Fork the new request */
    // pid_t ret = fork();
    // std::thread t1(processCommand, new_socket);
    // if (ret == 0) { /* Child */
    //   close(server_fd);
    //   processCommand(new_socket);
    //   exit(0);
    // }
    /* I had to switch off using processes because I need to keep my global datastructures
       in tact. So instead I am going use threads since threads can share global datastrcutres
       assumming atomic actions */
    workers.push_back(std::thread(processCommand, new_socket));
  }
  return 0;
}

/* @link LINE: 12 */
void processCommand(int fd){

  std::string resp = "MY RESPONSE";

  char buffer[30000] = {0};
  long valread = read(fd , buffer, 30000);
  std::string input = buffer;
  std::string output = databaseHandler(input);
  write(fd , output.c_str() , output.length());
  if(debug) printf("------------------Incomming Message-------------------\n");
  if(debug) printf("%s\n", input.c_str());
  if(debug) printf("------------------------Sending-----------------------\n");
  if(debug) printf("%s\n", output.c_str());
  if(debug) printf("--------------------------Done------------------------\n");
  close(fd);
}
