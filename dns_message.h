#ifndef DNS_MESSAGE_H
#define DNS_MESSAGE_H

#include <stdio.h>

/* DNS HEADER */

// 16 bits
#define ID_HIGH 0b00000000
#define ID_LOW  0b00000001

// flags
#define QR      0b0                  // 1 bit
#define OPCODE  0b0000               // 4 bits
#define AA      0b0                  // 1 bit
#define TC      0b0                  // 1 bit
#define RD      0b1                  // 1 bit
#define RA      0b0                  // 1 bit
#define Z       0b000                // 3 bits
#define RCODE   0b0000               // 4 bits

#define FLAGS_HIGH (( QR << 7 ) | (OPCODE << 3 ) | ( AA << 2 ) | ( TC <<1 ) | ( RD ))
#define FLAGS_LOW  (( RA << 7 ) | ( Z << 4 ) | (RCODE))

// 16 bits
#define QDCOUNT_HIGH 0b00000000      
#define QDCOUNT_LOW  0b00000001

// 16 bits
#define ANCOUNT_HIGH 0b00000000
#define ANCOUNT_LOW  0b00000000

// 16 bits
#define NSCOUNT_HIGH 0b00000000
#define NSCOUNT_LOW  0b00000000

// 16 bits
#define ARCOUNT_HIGH 0b00000000 
#define ARCOUNT_LOW  0b00000000


/* DNS QUESTION */

#define DOMAIN "www.pucrs.br"

#define QTYPE_HIGH 0b00000000
#define QTYPE_LOW  0b00000001

#define QCLASS_HIGH 0b00000000
#define QCLASS_LOW  0b00000001


void build_dns_header(unsigned char *header);

void build_dns_question(unsigned char *qname, int *question_size);

void build_dns_message(unsigned char *message, const unsigned char *header, const unsigned char *question, int *message_pos, int question_size);

#endif