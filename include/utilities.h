#ifndef PRINT_H
#define PRINT_H

#include "dns_parser.h"

int detect_nxdomain(DNS_ANSWER dns_answer);

int detect_refused(DNS_ANSWER dns_answer);

int detect_null_ip(DNS_ANSWER dns_answer, int index);

int detect_divergent_ip(DNS_ANSWER dns_answer, int index);

void print_dns(DNS_ANSWER dns_answer, int qname_size, unsigned char *dns_servers, double elapsed_time);

#endif