#include <stdio.h>
#include <time.h>

#include "include/dns_builder.h"
#include "include/dns_client.h"
#include "include/dns_parser.h"
#include "include/utilities.h"

int servers_count = 16;

char *dns_servers[] = {

    // Sem filtragem
    "8.8.8.8",        // Google
    "8.8.4.4",        // Google Secondary

    "1.1.1.1",        // Cloudflare
    "1.0.0.1",        // Cloudflare Secondary

    "9.9.9.10",       // Quad9 Unfiltered
    "64.6.64.6",      // Verisign

    // Segurança
    "9.9.9.9",        // Quad9 Security
    "208.67.222.222", // OpenDNS
    "185.228.168.9",  // CleanBrowsing Security
    "94.140.14.14",   // AdGuard DNS

    // Familiar
    "1.1.1.3",        // Cloudflare Family
    "208.67.222.123", // OpenDNS Family
    "185.228.168.168",// CleanBrowsing Family
    "94.140.14.15",   // AdGuard Family

    // Extras
    "76.76.2.0",      // Control D
    "189.38.95.96"    // GigaDNS
};

int main(int argc, char *argv[]) {
    
    unsigned char *domain = (unsigned char *)argv[1];   

    for(int i = 0; i < servers_count; i++) {
        
        unsigned char header[12]; 
        unsigned char question[512];
        unsigned char message[524];
        unsigned char response[BUFSIZE];
        int question_size = 0;
        int qname_size = 0;
        int message_pos = 0;
        DNS_ANSWER dns_answer = {0};
        struct timespec start, end;

        build_dns_header(header);
        build_dns_question(question, &question_size, &qname_size, domain);
        build_dns_message(message, header, question, &message_pos, question_size);

        int sockfd = create_dns_socket();

        clock_gettime(CLOCK_MONOTONIC, &start);

        send_dns_message(sockfd, message, message_pos, dns_servers[i]);
        int received = recv_dns_message(sockfd, response);

        clock_gettime(CLOCK_MONOTONIC, &end);

        if(received < 0) {
            close(sockfd); 
            continue;
        }
        dns_answer = parser_dns_response(response, qname_size);

        double elapsed_time = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0; //segundos convertidos para ms + nanosegundos convertidos para ms
        print_dns(dns_answer, qname_size, dns_servers[i], elapsed_time);

        close(sockfd);
    }

    benchmark_dns_servers(servers_count, dns_servers, domain);

    return 0;
}


/*
        dns_answer.ips[1][0] = 0;
        dns_answer.ips[1][1] = 0;
        dns_answer.ips[1][2] = 0;
        dns_answer.ips[1][3] = 0;
        
*/