#include <stdio.h>
#include <string.h>

static char *pageAccueil(){
    return "Bienvenou !";
}

char *getPage(char *request, char *path){
    if(strcmp(request, "GET") == 0) {
        if(strcmp(path, "/") == 0
                || strcmp(path, "/index.html") == 0) {
            return pageAccueil();
        }
    }
    
    return "Error 404 : Page not found !";
}
