#include "include/dns_parser.h"

DNS_Response parser_response(unsigned char *response) {

    DNS_Response result;
    int pos = 0;

    // usar int agora pois tem 32 bits, entao todos os campos cabem sem precisar separar
    // int id recebe primeira posição de response que é os 8 primeiros bits do ID
    // e shifta 8 posições para a esquerda para entrar mais os 8 depois
    // pois os dados estao vindo como response[0]=00000000, response[1]=00000001

    result.id      = (response[pos++] << 8) | response[pos++];
    result.flags   = (response[pos++] << 8) | response[pos++];
    result.qdcount = (response[pos++] << 8) | response[pos++];
    result.ancount = (response[pos++] << 8) | response[pos++];
    result.nscount = (response[pos++] << 8) | response[pos++];
    result.arcount = (response[pos++] << 8) | response[pos++];
    result.qtype   = (response[pos++] << 8) | response[pos++];
    result.qclass  = (response[pos++] << 8) | response[pos++];

    result.qr = (result.flags >> 15) & 1;     //o bit qr é o primeiro entao ta no 15
    result.rcode = result.flags & 0b1111;   //rcode sao os ultimos 4 bits entao so fazer a mascara 0b1111

    printf("\nID: %d", result.id);
    printf("\nFLAGS: %d", result.flags);
    printf("\nQDCOUNT: %d", result.qdcount);
    printf("\nANCOUNT: %d", result.ancount);
    printf("\nNSCOUNT: %d", result.nscount);
    printf("\nARCOUNT: %d", result.arcount);
    printf("\nQTYPE: %d", result.qtype);
    printf("\nQCLASS: %d", result.qclass);


    printf("\nQR: %d", result.qr);
    printf("\nRCODE: %d", result.rcode);

    return result;
}