#include <stdio.h>

#include "include/dns_builder.h"
#include "include/dns_client.h"
#include "include/dns_parser.h"
#include "utilities/print_dns.h"

int servers_count = 3;

char *dns_servers[] = {
    "8.8.8.8",
    "9.9.9.9",
    "1.1.1.1"
};

int main(int argc, char *argv[]) {
    
    unsigned char *domain = (unsigned char *)argv[1];   

    for(int i = 0; i < servers_count; i++) {

        unsigned char header[12]; 
        unsigned char question[512];
        unsigned char message[524];

        int question_size = 0;
        int qname_size = 0;
        int message_pos = 0;

        DNS_ANSWER dns_answer;

        build_dns_header(header);
        build_dns_question(question, &question_size, &qname_size, domain);
        build_dns_message(message, header, question, &message_pos, question_size);

        int sockfd = create_dns_socket();
        send_dns_message(sockfd, message, message_pos, dns_servers[i]);

        unsigned char response[BUFSIZE];
        int received = recv_dns_message(sockfd, response);
        if(received < 0) {
            close(sockfd); 
            continue;
        }

        dns_answer = parser_dns_response(response, qname_size);

        print_dns(dns_answer, qname_size);

        close(sockfd);
    }

    return 0;
}