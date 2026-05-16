#include "include/utilities.h"

#include "include/dns_client.h"

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


void benchmark_dns_servers(int servers_count, char *dns_servers[], unsigned char *domain) {

    printf("\n==============================\n");
    printf("DNS BENCHMARK (%d QUERIES)\n", 10);
    printf("==============================\n\n");

    for(int i = 0; i < servers_count; i++) {

        double total_time = 0.0;
        double min_time = DBL_MAX;
        double max_time = 0.0;

        int success = 0;
        int failed = 0;

        for(int j = 0; j < 10; j++) {

            unsigned char header[12];
            unsigned char question[512];
            unsigned char message[524];
            unsigned char response[BUFSIZE];

            int question_size = 0;
            int qname_size = 0;
            int message_pos = 0;

            struct timespec start, end;

            build_dns_header(header);
            build_dns_question(question, &question_size, &qname_size, domain);
            build_dns_message(message, header, question, &message_pos, question_size);

            int sockfd = create_dns_socket();

            clock_gettime(CLOCK_MONOTONIC, &start);

            send_dns_message(sockfd, message, message_pos, dns_servers[i]);

            int received = recv_dns_message(sockfd, response);

            clock_gettime(CLOCK_MONOTONIC, &end);

            close(sockfd);

            if(received < 0) {
                failed++;
                continue;
            }

            double elapsed_time =
                (end.tv_sec - start.tv_sec) * 1000.0 +
                (end.tv_nsec - start.tv_nsec) / 1000000.0;

            total_time += elapsed_time;

            if(elapsed_time < min_time) min_time = elapsed_time;

            if(elapsed_time > max_time) max_time = elapsed_time;

            success++;
        }

        double avg_time = 0.0;

        if(success > 0) avg_time = total_time / success;

        double loss = ((double)failed / 10) * 100.0;

        printf("Server: %s\n", dns_servers[i]);

        printf("AVG TIME : %.2f ms\n", avg_time);
        printf("MIN TIME : %.2f ms\n", min_time);
        printf("MAX TIME : %.2f ms\n", max_time);

        printf("LOSS: %.1f%%\n", loss);

        printf("------------------------------\n");
    }
}