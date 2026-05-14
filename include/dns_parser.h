#ifndef DNS_PARSER_H
#define DNS_PARSER_H

#include <stdio.h>

#define MAX_IPS 16

typedef struct {
    /* Header */
    int id;

    int flags;
    int qr;
    int opcode;
    int aa;
    int tc;
    int rd;
    int ra;
    int z;
    int rcode;

    int qdcount;
    int ancount;
    int nscount;
    int arcount;

    /* Question */
    char qname[512];
    int qtype;
    int qclass;

    /* Answer */
    int name1;
    int name2;
    int type;
    int class;
    int ttl;
    int rdlength;
    
    int ip_count;
    unsigned char ips[MAX_IPS][4];

} DNS_ANSWER;

DNS_ANSWER parser_dns_response(unsigned char *response, int qname_size);

#endif