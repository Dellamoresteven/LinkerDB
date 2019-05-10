// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <iostream>

#define PORT 8080

#define debug 1
/* This is going to be where everything starts up, I will add calls to database.cc
   so when a request comes in I will summon database.cc and get the response and
   send that back to the requester */
int main(int argc, char const *argv[])
{
  int server_fd;
  int new_socket;
  long valread;
  struct sockaddr_in address;
  /* need this for binding new sockets */
  int addrlen = sizeof(address);

  std::string resp = "MY RESPONSE";

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
    /* gonna wanna send back the database.cc response here */
    char buffer[30000] = {0};
    valread = read( new_socket , buffer, 30000);
    printf("%s\n",buffer );
    write(new_socket , resp.c_str() , resp.length());
    printf("------------------Incomming Message-------------------\n");
    close(new_socket);
  }
  return 0;
}
