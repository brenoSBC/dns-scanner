#include "include/dns_builder.h"

void build_dns_header(unsigned char *header) {

    header[0] = ID_HIGH;
    header[1] = ID_LOW;

    header[2] = FLAGS_HIGH;
    header[3] = FLAGS_LOW;

    header[4] = QDCOUNT_HIGH;
    header[5] = QDCOUNT_LOW;

    header[6] = ANCOUNT_HIGH;
    header[7] = ANCOUNT_LOW;

    header[8] = NSCOUNT_HIGH;
    header[9] = NSCOUNT_LOW;

    header[10] = ARCOUNT_HIGH;
    header[11] = ARCOUNT_LOW;
}

void build_dns_question(unsigned char *question, int *question_size, int *qname_size, unsigned char *domain) {

    int pos = 0;
    int len = 0;
    int len_pos = pos++;

    for(int i = 0; domain[i] != '\0'; i++) {
        if(domain[i] == '.') {
            question[len_pos] = len;
            len = 0;
            len_pos = pos++;
        } else {
            question[pos++] = domain[i];
            len++;
        }
    }
    question[len_pos] = len;
    question[pos++] = 0;

    *qname_size = pos;

    question[pos++] = QTYPE_HIGH;
    question[pos++] = QTYPE_LOW;
    
    question[pos++] = QCLASS_HIGH;
    question[pos++] = QCLASS_LOW;

    *question_size = pos;
}

void build_dns_message(unsigned char *message, const unsigned char *header, const unsigned char *question, int *message_pos, int question_size) {
    
    for(int i = 0; i < 12; i++) {
        message[(*message_pos)++] = header[i];
    }
    
    for(int i = 0; i < question_size; i++) {
        message[(*message_pos)++] = question[i];
    }

    /* DEBUG */
    // printf("question size: %d\n\n", question_size);

    // for(int i = 0; i < *message_pos; i++) {
    //     for(int bit = 7; bit >= 0; bit--) {
    //         printf("%d", (message[i] >> bit) & 1);
    //         }
    //         printf(" ");
    //     }
    // printf("\n");
}