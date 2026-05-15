#ifndef PRINT_H
#define PRINT_H

#include "../include/dns_parser.h"

static inline void print_dns(DNS_ANSWER dns_answer, int qname_size, unsigned char *dns_servers, double elapsed_time) {

    printf("\n============ DNS RESPONSE ============\n");

    printf("\nServer: %s", dns_servers);
    printf("\nTime: %f", elapsed_time);

    printf("\n\n[HEADER]");
    printf("\nID       : %d", dns_answer.id);
    printf("\nQR       : %d", dns_answer.qr);
    printf("\nRCODE    : %d", dns_answer.rcode);
    printf("\nQDCOUNT  : %d", dns_answer.qdcount);
    printf("\nANCOUNT  : %d", dns_answer.ancount);

    printf("\n\n[QUESTION]");
    printf("\nQNAME    : ");
    for(int i = 0; i < qname_size; i++) printf("%u ", dns_answer.qname[i]);
    printf("\nQTYPE    : %d", dns_answer.qtype);

    if(dns_answer.rcode == 0 && dns_answer.ancount > 0) {

        printf("\n\n[ANSWER]");

        if(dns_answer.type == 1 && dns_answer.dns_class == 1 && dns_answer.rdlength == 4) {
            printf("\nIPs:");

            for(int i = 0; i < dns_answer.ip_count; i++) {
                printf("\n%d: %d.%d.%d.%d",
                    i,
                    dns_answer.ips[i][0],
                    dns_answer.ips[i][1],
                    dns_answer.ips[i][2],
                    dns_answer.ips[i][3]);
            }
        }
    }

    printf("\n\n======================================\n");

}

#endif