#include "include/dns_parser.h"

DNS_ANSWER parser_dns_response(unsigned char *response, int qname_size) {

    DNS_ANSWER answer;
    int pos = 0;

    // usar int agora pois tem 32 bits, entao todos os campos cabem sem precisar separar
    // int id recebe primeira posição de response que é os 8 primeiros bits do ID
    // e shifta 8 posições para a esquerda para entrar mais os 8 depois
    // pois os dados estao vindo como response[0]=00000000, response[1]=00000001



    ///* Parsing Header *///
    answer.id      = (response[pos++] << 8) | response[pos++];

    answer.flags   = (response[pos++] << 8) | response[pos++];
    answer.qr      = (answer.flags >> 15) & 0b1;    //o bit qr é o primeiro entao ta no 15
    answer.opcode  = (answer.flags >> 11) & 0b1111;
    answer.aa      = (answer.flags >> 10) & 0b1;
    answer.tc      = (answer.flags >> 9) & 0b1;
    answer.rd      = (answer.flags >> 8) & 0b1;
    answer.ra      = (answer.flags >> 7) & 0b1;
    answer.z       = (answer.flags >> 4) & 0b111;
    answer.rcode   = answer.flags & 0b1111;   //rcode sao os ultimos 4 bits entao so fazer a mascara 0b1111
    
    answer.qdcount = (response[pos++] << 8) | response[pos++];
    answer.ancount = (response[pos++] << 8) | response[pos++];
    answer.nscount = (response[pos++] << 8) | response[pos++];
    answer.arcount = (response[pos++] << 8) | response[pos++];


    ///* Parsing Question *///
    for(int i = 0; i < qname_size; i++) answer.qname[i] = response[pos++];
        answer.qtype   = (response[pos++] << 8) | response[pos++];
        answer.qclass  = (response[pos++] << 8) | response[pos++];


        ///* Parsing Answer *///
        if(answer.rcode != 0 || answer.ancount == 0) return answer;

        answer.ip_count = 0;

        for(int i = 0; i < answer.ancount; i++) {
        //NAME so consome esses 2 primeiros bytes pq eles sao uma referencia do name da question (ponteiro)
        answer.name1 = response[pos++];
        answer.name2 = response[pos++];

        answer.type = (response[pos++] << 8) | response[pos++];     // 16 bits
        answer.class = (response[pos++] << 8) | response[pos++];    // 16 bits

        // ttl 4 bytes 32 bits
        answer.ttl = (response[pos++] << 24) | 
                    (response[pos++] << 16) | 
                    (response[pos++] << 8)  | 
                    response[pos++];

        // rdlength 16 bits
        answer.rdlength = (response[pos++] << 8) | response[pos++];

        // Se a Answer não for TYPE = 1, CLASS = 1, RDLENGTH = 4, esses 4 bytes não são IPv4.
        if(answer.type == 1 && answer.class == 1 && answer.rdlength == 4) {

        
            answer.ips[answer.ip_count][0] = response[pos++];
            answer.ips[answer.ip_count][1] = response[pos++];
            answer.ips[answer.ip_count][2] = response[pos++];
            answer.ips[answer.ip_count][3] = response[pos++];

            answer.ip_count++;
        } else {
        // se não for IPv4, pula o RDATA
        pos += answer.rdlength;
        }
    }



    printf("\n================ DNS RESPONSE ================\n");

    printf("\n[HEADER]");
    printf("\nID       : %d", answer.id);
    printf("\nFLAGS    : %d", answer.flags);
    printf("\nQR       : %d", answer.qr);
    printf("\nRCODE    : %d", answer.rcode);
    printf("\nQDCOUNT  : %d", answer.qdcount);
    printf("\nANCOUNT  : %d", answer.ancount);
    printf("\nNSCOUNT  : %d", answer.nscount);
    printf("\nARCOUNT  : %d", answer.arcount);

    printf("\n\n[QUESTION]");
    printf("\nQNAME    : ");

    for(int i = 0; i < qname_size; i++) {
        printf("%u ", answer.qname[i]);
    }

    printf("\nQTYPE    : %d", answer.qtype);
    printf("\nQCLASS   : %d", answer.qclass);

    if(answer.rcode == 0 && answer.ancount > 0) {

        printf("\n\n[ANSWER]");
        printf("\nNAME PTR : %u %u", answer.name1, answer.name2);
        printf("\nTYPE     : %d", answer.type);
        printf("\nCLASS    : %d", answer.class);
        printf("\nTTL      : %d", answer.ttl);
        printf("\nRDLENGTH : %d", answer.rdlength);

        if(answer.type == 1 && answer.class == 1 && answer.rdlength == 4) {
            printf("\nIPs:");

            for(int i = 0; i < answer.ip_count; i++) {
                printf("\n- %d.%d.%d.%d",
                    answer.ips[i][0],
                    answer.ips[i][1],
                    answer.ips[i][2],
                    answer.ips[i][3]);
            }
        }
    }

    printf("\n\n==============================================\n");

    return answer;
}