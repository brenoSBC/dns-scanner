#ifndef DNS_PARSER_H
#define DNS_PARSER_H

#include <stdio.h>

typedef struct {
    int id;
    int flags;
    int qr;
    int rcode;

    int qdcount;
    int ancount;
    int nscount;
    int arcount;

    int qtype;
    int qclass;
} DNS_Response;

DNS_Response parser_response(unsigned char *response);

#endif