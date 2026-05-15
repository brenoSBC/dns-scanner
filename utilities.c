#include "include/utilities.h"

int detect_null_ip(DNS_ANSWER dns_answer, int index) {

    if(
        (dns_answer.ips[index][0] == 0 &&
         dns_answer.ips[index][1] == 0 &&
         dns_answer.ips[index][2] == 0 &&
         dns_answer.ips[index][3] == 0)
        ||
        (dns_answer.ips[index][0] == 127 &&
         dns_answer.ips[index][1] == 0   &&
         dns_answer.ips[index][2] == 0   &&
         dns_answer.ips[index][3] == 1)
        ) 
    {
        return -1;
    }
    return 0;
}

int detect_nxdomain(DNS_ANSWER dns_answer) {

    if(dns_answer.rcode == 3) return -1;
    return 0;
}

int detect_refused(DNS_ANSWER dns_answer) {

    if(dns_answer.rcode == 5) return -1;
    return 0;
}


void print_dns(DNS_ANSWER dns_answer, int qname_size, unsigned char *dns_servers, double elapsed_time) {

    printf("\n============ DNS RESPONSE ============\n");

    printf("\nServer: %s", dns_servers);
    printf("\nTime: %f", elapsed_time);

    printf("\n\n[HEADER]");
    printf("\nID       : %d", dns_answer.id);
    printf("\nQR       : %d", dns_answer.qr);

    int nxdomain_flag = detect_nxdomain(dns_answer);
    int refused_flag  = detect_refused(dns_answer);
    if(nxdomain_flag < 0) printf("\nRCODE    : %d NXDOMAIN", dns_answer.rcode);
    if(refused_flag < 0)  printf("\nRCODE    : %d REFUSED", dns_answer.rcode);
    if(dns_answer.rcode == 0) printf("\nRCODE    : %d SUCESS", dns_answer.rcode);
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

                int null_ip_flag = detect_null_ip(dns_answer, i);

                if(null_ip_flag < 0) {
                    printf("\n[WARNING] Null IP detected:");
                    printf(" %d.%d.%d.%d",
                    dns_answer.ips[i][0],
                    dns_answer.ips[i][1],
                    dns_answer.ips[i][2],
                    dns_answer.ips[i][3]);
                    continue;
                }

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