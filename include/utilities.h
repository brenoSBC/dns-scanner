#ifndef PRINT_H
#define PRINT_H

#include "dns_parser.h"
#include "dns_builder.h"
#include "dns_client.h"

#include <stdio.h>
#include <float.h>
#include <time.h>
#include <unistd.h>

int detect_nxdomain(DNS_ANSWER dns_answer);

int detect_refused(DNS_ANSWER dns_answer);

int detect_null_ip(DNS_ANSWER dns_answer, int index);

int detect_divergent_ip(DNS_ANSWER dns_answer, int index);

void print_dns(DNS_ANSWER dns_answer, int qname_size, unsigned char *dns_servers, double elapsed_time);

void benchmark_dns_servers(int servers_count, char *dns_servers[], unsigned char *domain);
#endif