#include <stdio.h>

#include "include/dns_builder.h"
#include "include/dns_client.h"
#include "include/dns_parser.h"

int main(int argc, char *argv[]) {
    
    unsigned char *domain = (unsigned char *)argv[1];   
    unsigned char header[12]; 
    unsigned char question[512];
    unsigned char message[524];

    int question_size = 0;
    int message_pos = 0;

    build_dns_header(header);
    build_dns_question(question, &question_size, domain);
    build_dns_message(message, header, question, &message_pos, question_size);

    int sockfd = create_dns_socket();
    send_dns_message(sockfd, message, message_pos, "8.8.8.8");

    unsigned char response[BUFSIZE];
    int received = recv_dns_message(sockfd, response);

    printf("Recebi %d bytes\n", received);

    parser_response(response);

    // close(sockfd);
  
    return 0;
}