#ifndef PRINT_H
#define PRINT_H

#include "../include/dns_parser.h"

static inline void print_dns(DNS_ANSWER dns_answer, int qname_size) {

    printf("\n================ DNS RESPONSE ================\n");

    printf("\n[HEADER]");
    printf("\nID       : %d", dns_answer.id);
    printf("\nQR       : %d", dns_answer.qr);
    printf("\nRCODE    : %d", dns_answer.rcode);
    printf("\nQDCOUNT (Quantidade de Questions): %d", dns_answer.qdcount);
    printf("\nANCOUNT (Quantidade de dns_answers)  : %d", dns_answer.ancount);

    printf("\n\n[QUESTION]");
    printf("\nQNAME    : ");

    for(int i = 0; i < qname_size; i++) printf("%u ", dns_answer.qname[i]);
    printf("\nQTYPE(A)    : %d", dns_answer.qtype);

    if(dns_answer.rcode == 0 && dns_answer.ancount > 0) {

        printf("\n\n[ANSWER]");

        if(dns_answer.type == 1 && dns_answer.dns_class == 1 && dns_answer.rdlength == 4) {
            printf("\nIPs:");

            for(int i = 0; i < dns_answer.ip_count; i++) {
                printf("\n- %d.%d.%d.%d",
                    dns_answer.ips[i][0],
                    dns_answer.ips[i][1],
                    dns_answer.ips[i][2],
                    dns_answer.ips[i][3]);
            }
        }
    }

    printf("\n\n==============================================\n");

}

#endif