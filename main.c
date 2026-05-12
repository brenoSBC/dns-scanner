#include <stdio.h>

#include "dns_message.h"
#include "dns_send_message.h"

#define SERVER_IP "8.8.8.8"
#define PORT 53
#define BUFSIZE 1024

int main(void) {
    
    unsigned char header[12]; 
    unsigned char question[64];
    unsigned char message[512];    
    int question_size = 0;
    int message_pos = 0;

    build_dns_header(header);
    build_dns_question(question, &question_size);
    build_dns_message(message, header, question, &message_pos, question_size);

    // send_dns_message(message, message_pos);

    
    /* Cria socket UDP */
    // int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    // if(sockfd < 0) {
    //     perror("socket");
    //     exit(EXIT_FAILURE);
    // }

    // /* Configura endereço do servidor */
    // struct sockaddr_in server_addr;
    // socklen_t server_len;
    // ssize_t n;
    // server_len = sizeof(server_addr);

    // memset(&server_addr, 0, sizeof(server_addr));
    // server_addr.sin_family = AF_INET;
    // server_addr.sin_port   = htons(PORT);
    // inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // /* Envia ao servidor */
    // sendto(sockfd, message, message_pos, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // /* Resposta */
    // char buffer[BUFSIZE];
    // server_len = sizeof(server_addr);
    // n = recvfrom(sockfd, buffer, BUFSIZE - 1, 0, (struct sockaddr *)&server_addr, &server_len);

    // if (n < 0) {
    //     perror("recvfrom");
    //     close(sockfd);
    //     exit(EXIT_FAILURE);
    // }

    // buffer[n] = '\0';
    // printf("Resposta: %s", buffer);

    // close(sockfd);
  
    return 0;
}