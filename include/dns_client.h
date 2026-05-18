#ifndef DNS_CLIENT_H
#define DNS_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>

/* UDP Port & Buffer Size */

#define PORT 53
#define BUFSIZE 1024


/* Function Prototypes */

int create_dns_socket();

void send_dns_message(int sockfd, const unsigned char *message, int message_size, const char *server_ip);

int recv_dns_message(int sockfd, unsigned char *response, char *dns_servers);

#endif