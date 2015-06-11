/* Copyright © 2015, Pittsburgh Supercomputing Center.  All Rights Reserved. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "libinflux.h"

CURL *curl;
CURLcode res;
char *host_url;     // URL hosting influxDB. Eg. "http://www.example.net/"
char *service_url;  // URL for the specific type of request. 
                    // Eg. "db/<database-name>/series?u=<username>&p=<password>"
int debug = 0;

/* Set-up and tear-down functions */

//Prepares libinflux & cURL - call before any other libinflux functions
void rest_init(char *host, char *service)
{
    curl = curl_easy_init();

    host_url = (char *)malloc(strlen(host));
    host_url = strcpy(host_url, host);

    service_url = (char *)malloc(strlen(service));
    service_url = strcpy(service_url, service);

}

//Cleans up memory used by the library
void rest_cleanup()
{
    free(host_url);
    free(service_url);
    curl_easy_cleanup(curl);
}

void set_host_url(char * url){
    host_url = (char *)realloc(host_url, strlen(url));
    host_url = strcpy(host_url, url);
}

void set_service_url(char * url){
    service_url = (char *)realloc(service_url, strlen(url));
    service_url = strcpy(service_url, url);
}

/* InfluxDB functions - Query & Write */

/* Sends the query string, *query, to the database represented by *service_url.
 * *query must be a properly formatted InfluxDB query.
 * Returns a CURLcode that is globally stored as res (until the next call)
 */
CURLcode influxQuery(char *query){
    char q[] = "&q=";   //parameter appended to url for queries
    int size = strlen(host_url) + strlen(service_url) + strlen(q);
    char *url = (char *)malloc(size+1);
    strcat(url, host_url);
    strcat(url, service_url);
    strcat(url, q);
    
    if(debug){printf("[q: %s]\n", url);}
    
    return sendGet(url, query);
}

/* Writes the JSON object, *data, to the database represented by *service_url.
 * *data must be a properly formatted InfluxDB JSON object.
 * Returns a CURLcode that is globally stored as res (until the next call)
 */
CURLcode influxWrite(char *data){
    int size = strlen(host_url) + strlen(service_url);
    char *url = (char *)malloc(size+1);
    strcat(url, host_url);
    strcat(url, service_url);

    if(debug){printf("[w: %s]\n", url);}

    return sendPost(url, data); 
}

/* Basic CURL functions - GET & POST */

/* Send a POST request to write *data to *url 
 * Returns CURLcode res
 */
CURLcode sendPost(char *url, char *data){
    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(data));
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        res = curl_easy_perform(curl);
    }
    return res;
}

/* Send a GET request to write *data (after url encoding) to *url 
 * Returns CURLcode res
 */
CURLcode sendGet(char *url, char *data){
    if(curl){
        if(data){ //urlencode data
            char *encoded_data = curl_easy_escape(curl, data, strlen(data));
            int size = strlen(url)+strlen(encoded_data)+1;
            char *tmp = malloc(size);
            if(tmp){
                strcat(tmp,url);
                strcat(tmp,encoded_data);
                url = tmp;
                curl_free(encoded_data);
            }
        }
        curl_easy_setopt(curl, CURLOPT_URL, url);
        res = curl_easy_perform(curl);
    }
    return res;
}

