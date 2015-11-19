/* Copyright © 2015, Pittsburgh Supercomputing Center.  All Rights Reserved. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libinflux.h"

void usage(){
    printf("Usage:\n");
    printf(" -h \n\tDisplays this help text.\n\n");
    printf(" -s <0 or 1> \n\tDisable or enable ssl peer verification. Only affects succeding arguments.)\n\n");
    printf(" -q <query>\n\tSend query string to the database.\n\n");
    printf(" -w <data>\n\tSend write string to the database.\n\n");
    exit(8);
}

int dataCallback(char *response)
{
    if(response){
        printf("%s\n", response);
    }else{
        printf("empty response\n");
    }

    return 0;
}

int main(int argc, char *argv[]){

    //prepare curl
    char url[]="https://hotel.psc.edu:8086/";
    int ssl_verify = 1;
    libinflux_init();
    influxConn *hostA = create_conn(url, "test", "dbuser", "TcitoPsb", ssl_verify);
    CURLcode res;

//    set_callback(hostA, dataCallback);

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
                
                //arguments were consumed
                argv += 2;
                argc -= 2;
                break;

            case 'q': // query
                hostA->ssl = ssl_verify;
                res = influxQuery(hostA, &argv[1][3]);
                if( res != CURLE_OK){
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                }

                //arguments were consumed
                argv += 2;
                argc -= 2;
                break;

            case 'w': // write
                hostA->ssl = ssl_verify;
                res = influxWrite(hostA, &argv[1][3]);
                if( res != CURLE_OK){
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                }
                
                //arguments were consumed
                argv += 2;
                argc -= 2;
                break;


            case 't': // test/check connection
                hostA->ssl = ssl_verify;
                res = influxCheck(hostA);
                if( res != CURLE_OK)
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

                //arguments were consumed
                argv += 1;
                argc -= 1;
                break;


            case 'd': // enable debug
                set_debug(true);
                
                //arguments were consumed
                argv += 1;
                argc -= 1;
                break;

            default:
                printf("Invalid argument: %s\n", argv[1]);
                usage();
        }

    }

    free_conn(hostA);
    libinflux_cleanup();
    
    return 0;
}
