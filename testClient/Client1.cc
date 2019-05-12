/**
 * DISCLAIMER: THIS CODE IS COPIED FROM https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa
 * PLEASE GIVE CREDIT FOR HIS AMAZING TUTORAL! I WILL REWRITE THIS WHEN I GO THROUGH TESTING
 * tl dr: not my code
 *
 * Some of this code has been motified to let me send multiple messages TCP like. (Connection like)
 */

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>

#define PORT 8080

int main(int argc, char const *argv[])
{
  for(;;){
    int sock = 0; long valread;
    struct sockaddr_in serv_addr;
    std::string req;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed\n");
        return -1;
    }

    printf("WUT0\n");
    getline(std::cin, req);
    printf("WUT1\n");

    send(sock , req.c_str(), req.length() , 0);
    printf("WUT2\n");
    valread = read(sock , buffer, 1024);
    printf("%s\n", buffer);
    printf("WUT3\n");
  }
    return 0;
}
