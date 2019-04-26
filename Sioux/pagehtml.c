#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "comInterface.h"

static void code200(char *contentType, char *page){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    strcat(page, "HTTP/1.0 200 OK\n \
            Server: Sioux/0.9.0\n");
    
    strcat(page, "Date: ");
    char date[80];
    strftime(date, 80, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    strcat(page, date);
    strcat(page, "\n");

    if(strcmp(contentType, "html") == 0)
        strcat(page, "Content-type: text/HTML\n\n");
    if(strcmp(contentType, "css") == 0)
        strcat(page, "Content-type: text/CSS\n\n");
}

static char *code404(){
    return "HTTP/1.0 404 Not Found\n \
        Server: Sioux/0.9.0\n \
        Content-type: text/HTML\n\n";
}
/*<meta http-equiv=\"refresh\" content=\"5\"> \n \*/
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
        <table>\n \
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
    
    // reset etats tableau
    initTableauxInterface();
    // broadcast getStatus
    broadCastGetStatus();
    // attente du retour des status des interfaces
    usleep(500000); // 500 ms
    // broadcast getCommande
    broadCastGetCommande();
    // attente du retour des commandes des interfaces
    usleep(500000); // 500 ms
    
    for(int i=0; i<NBR_INTERFACES; i++){
        int etat = getEtatInterface(i);
        if(etat == -1)
            strcat(page, "<td class=\"not_responding\">NR</td>\n");
        else if(etat == 0)
            strcat(page, "<td class=\"sleep_mode\">SL</td>\n");
        else if(etat == 1)
            strcat(page, "<td class=\"listening_mode\">OK</td>\n");
        else
            strcat(page, "<td class=\"unknow_status\">S?</td>\n");
    }
    
    strcat(page, "</tr>\n \
        <tr>\n \
            <th>IP</th>");
    
    for(int i=0; i<NBR_INTERFACES; i++){
        char *ip = getIpInterface(i);
        if(ip[0] == '\0')
            strcat(page, "<td>-</td>\n");
        else {
            strcat(page, "<td class=\"ip\">");
            strcat(page, ip);
            strcat(page, "</td>\n");
        }
    }
    
    strcat(page, "</tr>\n \
        <tr>\n \
            <th>Commande</th>");
    
    for(int i=0; i<NBR_INTERFACES; i++){
        int commande = getCommandeInterface(i);
        if(commande == -1)
            strcat(page, "<td>-</td>\n");
        else {
            char hex[5];
            sprintf(hex, "%04x", commande);
            hex[4] = '\0';

            strcat(page, "<td>0x");
            strcat(page, hex);
            strcat(page, "</td>\n");
        }
    }
    
    strcat(page, "</tr>\n \
        </table>\n");
}

static char *pageAccueilEnd(){
    return "</body>\n \
        </html>";
}

static void pageAccueil(char *page){
    code200("html", page);
    strcat(page, pageAccueilTitle());
    pageAccueilTable(page);
    strcat(page, pageAccueilEnd());
}

static char *pageAccueilCSS(){
    return "table{\n \
            border: 1px solid black;\n \
            margin-left: 20px;\n \
        }\n \
        th, td{\n \
            border: solid 1px black;\n \
            padding: 5px;\n \
            text-align: center;\n \
        }\n \
        .not_responding{\n \
            color: red;\n \
        }\n \
        .sleep_mode{\n \
            color: orange;\n \
        }\n \
        .listening_mode{\n \
            color: green;\n \
        }\n \
        .unknow_status{\n \
            color: black;\n \
        }\n \
        .ip{\n \
            font-size: 10px;\n \
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
            code200("css", page);
            strcat(page, pageAccueilCSS());
            return;
        }
    }
    strcat(page, code404());
    strcat(page, "<h1>Error 404 : Page not found !</h1>");
}
