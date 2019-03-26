#include <stdio.h>
#include <string.h>
#include <libnet.h>
#include "pagehtml.h"

#define NBR_INTERFACES 13
#define UDP_REQUETE_LENGTH 2

static char *code200(char *contentType){
    if(strcmp(contentType, "html") == 0)
        return "HTTP/1.0 200 OK\n \
            Server: Sioux/0.9.0\n \
            Content-type: text/HTML\n\n";
    if(strcmp(contentType, "css") == 0)
        return "HTTP/1.0 200 OK\n \
            Server: Sioux/0.9.0\n \
            Content-type: text/CSS\n\n";
    return "HTTP/1.0 200 OK\n \
        Server: Sioux/0.9.0\n\n";
}

static char *code404(){
    return "HTTP/1.0 404 Not Found\n \
        Server: Sioux/0.9.0\n \
        Content-type: text/HTML\n\n";
}

static char *pageAccueilTitle(){
    return "<!DOCTYPE html>\n \
        <html>\n \
            <head>\n \
                 <meta charset=\"UTF-8\"> \n \
                <title>Sonde du groupe 8</title>\n \
                <link rel=\"stylesheet\" href=\"styles.css\"/>\n \
            </head>\n \
            <body>\n \
                <h1>Sonde du groupe 8</h1>\n";
}

static void pageAccueilTable(char *page){
    strcat(page, "<h2>Status des interfaces tangibles :</h2>\n \
        <table class=\"table\">\n \
            <tr>\n \
                <th>Interface n°</th>\n \
                <td>1</td>\n \
                <td>2</td>\n \
                <td>3</td>\n \
                <td>4</td>\n \
                <td>5</td>\n \
                <td>6</td>\n \
                <td>7</td>\n \
                <td>8</td>\n \
                <td>9</td>\n \
                <td>10</td>\n \
                <td>11</td>\n \
                <td>12</td>\n \
                <td>13</td>\n \
            </tr>\n \
            <tr>\n \
                <th>Status</th>\n");
    
    // broadcast getStatus
    char message[UDP_REQUETE_LENGTH];
    message[0] = 0x00;
    message[1] = 0x00;
    //char *message = "0123456";
    sendUDPBroadcast(message, UDP_REQUETE_LENGTH, 2020);
    
    for(int i=0; i<NBR_INTERFACES; i++){
        strcat(page, "<td>LOL</td>\n");
    }
    
    strcat(page, "</tr>\n \
        </table>\n");
}

static char *pageAccueilEnd(){
    return "</body>\n \
        </html>";
}

static void pageAccueil(char *page){
    strcat(page, code200("html"));
    strcat(page, pageAccueilTitle());
    pageAccueilTable(page);
    strcat(page, pageAccueilEnd());
}

static char *pageAccueilCSS(){
    return ".table{\n \
            border: 1px solid black;\n \
            margin-left: 20px; \n \
        }\n \
        th, td{\n \
            border: solid 1px black;\n \
            padding: 5px;\n \
            text-align: left;\n \
        }";
}

void getPage(char *request, char *path, char *page){
    if(strcmp(request, "GET") == 0) {
        if(strcmp(path, "/") == 0
                || strcmp(path, "/index.html") == 0) {
            pageAccueil(page);
            return;
        }
        if(strcmp(path, "/styles.css") == 0) {
            strcat(page, code200("css"));
            strcat(page, pageAccueilCSS());
            return;
        }
    }
    strcat(page, code404());
    strcat(page, "<h1>Error 404 : Page not found !</h1>");
}
