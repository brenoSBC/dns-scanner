#include "include/dns_parser.h"

// usar int agora pois tem 32 bits, entao todos os campos cabem sem precisar separar
// int id recebe primeira posição de response que é os 8 primeiros bits do ID
// e shifta 8 posições para a esquerda para entrar mais os 8 depois
// pois os dados estao vindo como response[0]=00000000, response[1]=00000001

DNS_ANSWER parser_dns_response(unsigned char *response, int qname_size) {

    DNS_ANSWER answer;
    int pos = 0;

    ///* Parsing Header *///
    answer.id      = (response[pos++] << 8) | response[pos++];

    answer.flags   = (response[pos++] << 8) | response[pos++];
    answer.qr      = (answer.flags >> 15) & 0b1;                  
    answer.opcode  = (answer.flags >> 11) & 0b1111;
    answer.aa      = (answer.flags >> 10) & 0b1;
    answer.tc      = (answer.flags >> 9) & 0b1;
    answer.rd      = (answer.flags >> 8) & 0b1;
    answer.ra      = (answer.flags >> 7) & 0b1;
    answer.z       = (answer.flags >> 4) & 0b111;
    answer.rcode   = answer.flags & 0b1111;                       
    
    answer.qdcount = (response[pos++] << 8) | response[pos++];
    answer.ancount = (response[pos++] << 8) | response[pos++];
    answer.nscount = (response[pos++] << 8) | response[pos++];
    answer.arcount = (response[pos++] << 8) | response[pos++];


    ///* Parsing Question *///
    for(int i = 0; i < qname_size; i++) answer.qname[i] = response[pos++];
    answer.qtype   = (response[pos++] << 8) | response[pos++];
    answer.qclass  = (response[pos++] << 8) | response[pos++];


    ///* Parsing Answer *///
    answer.ip_count = 0;
    if(answer.rcode != 0 || answer.ancount == 0) return answer;

    for(int i = 0; i < answer.ancount; i++) {
        
        answer.name1 = response[pos++];                             
        answer.name2 = response[pos++];

        answer.type = (response[pos++] << 8) | response[pos++];     
        answer.dns_class = (response[pos++] << 8) | response[pos++];    

        answer.ttl = (response[pos++] << 24) |                      
                     (response[pos++] << 16) | 
                     (response[pos++] << 8)  | 
                      response[pos++];
        
        answer.rdlength = (response[pos++] << 8) | response[pos++];  

        if(answer.type == 1 && answer.dns_class == 1 && answer.rdlength == 4) {
            answer.ips[answer.ip_count][0] = response[pos++];
            answer.ips[answer.ip_count][1] = response[pos++];
            answer.ips[answer.ip_count][2] = response[pos++];
            answer.ips[answer.ip_count][3] = response[pos++];

            answer.ip_count++;
        } else {
            pos += answer.rdlength;
        }
    }
    return answer;
}