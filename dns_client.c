#include "include/dns_client.h"

int create_dns_socket() {

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

void send_dns_message(int sockfd, const unsigned char *message, int message_size, const char *server_ip) {

    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    int send_bytes = sendto(sockfd, message, message_size, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    
    if(send_bytes < 0) {
        perror("sendto");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

int recv_dns_message(int sockfd, unsigned char *response) {

    struct sockaddr_in server_addr;
    socklen_t server_len = sizeof(server_addr);

    int recv_bytes = recvfrom(sockfd, response, BUFSIZE, 0, (struct sockaddr *)&server_addr, &server_len);

    if(recv_bytes < 0) {
        perror("recvfrom");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    return recv_bytes;
}