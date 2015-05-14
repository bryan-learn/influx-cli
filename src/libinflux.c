/* Copyright © 2015, Pittsburgh Supercomputing Center.  All Rights Reserved. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

CURL *curl;
CURLcode res;
char *host_url; // URL to send requests to. Eg. http://www.example.net/

void rest_init(char *url)
{
    curl = curl_easy_init();
    host_url = (char *)malloc(strlen(url));
    host_url = strcpy(host_url, url);
}

void rest_cleanup()
{
    free(host_url);
    curl_easy_cleanup(curl);
}

void set_host_url(char * url){
    host_url = (char *)realloc(host_url, strlen(url));
    host_url = strcpy(host_url, url);
}

/* InfluxDB functions - Query & Write */

CURLcode influxQuery(char *query){
    char service_url[] = "db/xsight_dev/series?u=d-admin&p=TcitoPsb&q=";
    int size = strlen(host_url) + strlen(service_url);
    char *url = (char *)malloc(size+1);
    strcat(url, host_url);
    strcat(url, service_url);
    
    return sendGet(url, query);
}

CURLcode influxWrite(char *data){
    char service_url[] = "db/xsight_dev/series?u=d-admin&p=TcitoPsb";
    int size = strlen(host_url) + strlen(service_url);
    char *url = (char *)malloc(size+1);
    strcat(url, host_url);
    strcat(url, service_url);

   return sendPost(url, data); 
}

/* Basic CURL functions - GET & POST */

CURLcode sendPost(char *url, char *data){
    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(data));
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        res = curl_easy_perform(curl);
    }
    return res;
}

CURLcode sendGet(char *url, char *data){
    if(curl){
        if(data != NULL){ //urlencode data
            char *encoded_data = curl_easy_escape(curl, data, strlen(data));
            int size = strlen(url)+strlen(encoded_data)+1;
            char *tmp = malloc(size);
            if(tmp != NULL){
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

