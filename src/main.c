/* Copyright © 2015, Pittsburgh Supercomputing Center.  All Rights Reserved. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libinflux.h"

void usage(){
    printf("Usage:\n");
    printf(" -q <query>\n");
    printf(" -w <data>\n");
    exit(8);
}

int main(int argc, char *argv[]){

    //prepare curl
    char url[]="http://indigo.psc.edu:8086/";
    rest_init();
//    influxConn *hostA = create_conn(url, "testdb", "dbuser", "<your password>");
    influxConn *hostA = create_conn(url, "xsight", "dbuser", "TcitoPsb");
    CURLcode res;

    //parse arguments
    printf("%s:\n", argv[0]);
    while( (argc > 1) && (argv[1][0] == '-') ){
        switch (argv[1][1])
        {
            case 'q':
                //printf("query: %s", &argv[1][3]);
                res = influxQuery(hostA, &argv[1][3]);
                if( res != CURLE_OK)
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                break;
            case 'w':
                //printf("write:  %s", &argv[1][3]);
                res = influxWrite(hostA, &argv[1][3]);
                if( res != CURLE_OK)
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                break;
            default:
                printf("Invalid argument: %s\n", argv[1]);
                usage();
        }
        argv += 2;
        argc -= 2;
    }


    rest_cleanup(hostA);
    free(hostA);
    return 0;
}
