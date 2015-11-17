/* Copyright © 2015, Pittsburgh Supercomputing Center.  All Rights Reserved. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libinflux.h"

void usage(){
    printf("Usage:\n");
    printf(" -s <0 or 1> (disable or enable ssl peer verification. Must precede all other arguments.)\n");
    printf(" -q <query>\n");
    printf(" -w <data>\n");
    exit(8);
}

int main(int argc, char *argv[]){

    //prepare curl
    char url[]="https://hotel.psc.edu:8086/";
    int ssl_verify = 1;
    rest_init();
    influxConn *hostA = create_conn(url, "test", "dbuser", "TcitoPsb", ssl_verify);
    CURLcode res;

    //parse arguments
    printf("%s:\n", argv[0]);
    while( (argc > 1) && (argv[1][0] == '-') ){
        switch (argv[1][1])
        {
            case 's': // set ssl peer verification. On: 1, Off: 0
                if(argv[1][3] == '0')
                    ssl_verify = 0;
                else if(argv[1][3] == '1')
                    ssl_verify = 1;
                break;

            case 'q': // query
                hostA->ssl = ssl_verify;
                res = influxQuery(hostA, &argv[1][3]);
                if( res != CURLE_OK)
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                break;

            case 'w': // write
                hostA->ssl = ssl_verify;
                res = influxWrite(hostA, &argv[1][3]);
                if( res != CURLE_OK)
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                break;

            default:
                printf("Invalid argument: %s\n", argv[1]);
                usage();
        }

        //arguments were consumed
        argv += 2;
        argc -= 2;
    }


    rest_cleanup(hostA);
    free(hostA);
    return 0;
}
