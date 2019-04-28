# Tutorat PSR

Étudiants : Hind MALTI, Pierre FRISON

## Choix de la trame

XXXYYYYY YYYYYYYY (2 bytes)

XXX : commandes
* 000 getStatus : demande à l'interface son status (les Y sont ignorés)
* 001 retStatus : l'interface retourne son status (Y = ZZZZZ ZZZZZZZ1 : mode éveillé | Y = ZZZZZ ZZZZZZZ0 : mode sommeil & Z = identifiant de l'interface, numéro du groupe)
* 010 setStatus : demande à l'interface de passer dans le mode sommeil ou éveillé (Y = 00000 00000001 : mode éveillé | Y = 00000 00000000 : mode sommeil)
* 011 getCommande : demande à l'interface la commande en mémoire
* 100 retCommande : l'interface retourne sa commande en mémoire (les Y représentent un pourcentage : Y = 11111 11111111 : 100%, Y = 00000 00000000 : 0%)
* 101 setCommande : envoie à l'interface la commande à executer (les Y représentent un pourcentage : Y = 11111 11111111 : 100%, Y = 00000 00000000 : 0%)

Ce choix nous permet de nous affranchir de la provenance de la trame (UDP et TCP). Cela équivaut à une encapsulation de niveau 7 (niveau applicatif) dans le modèle OSI.

## Communication entre les interfaces tangibles et la sonde

La sonde envoie des requêtes UDP et l'interface, des requêtes TCP. Néanmoins, comme tout le monde n'était d'accord sur ce point, nous avons mis en place un serveur UDP et un serveur TCP sur les deux appareils. Nous pouvons donc traiter toutes les requêtes, qu'elles viennent d'UDP ou de TCP.

## Interface tangible

Pour afficher les commandes envoyées à l'interface tangible, nous avons utilisé un afficheur 7 ségments pour afficher la commande en hexadécimal.

Nous avons programmé deux mode pour les interfaces :
* un mode éveillé : l'interface écoute et interprète toutes les requêtes reçues.
* un mode sommeil : l'interface n'écoute et n'interprète que les requêtes *getStatus* et *setStatus*. Les requêtes concernant les commandes sont ignorées.

## Serveur HTTP

Le serveur affiche un tableau contenant l'états des interfaces tangibles de tous les groupes (NR = l'interface ne répond pas, SL = en mode veille, OK = en mode actif), leurs addresses IP et la commande en mémoire.

Lorsqu'une page web est demandée au serveur HTTP, *getPage* est appelée et analyse la requête HTTP (composé d'une méthode HTTP (GET, POST, etc.) et d'un chemin). Si la page n'est pas reconnue un code d'erreur 404 est renvoyée. 

La seule page disponible est */* ou */index.html*. Lorsqu'elle est demandée plusieurs requêtes sont effectuées aux interfaces tangibles pour créer le tableau :
* un broadcast de *getStatus* pour récupérer leurs status et leurs addresses ip.
* un broadcast de *getCommande* pour récupérer leurs commandes en mémoire.

L'envoi et la réception n'étant pas instantanée (et les trames envoyées en broadcast UDP), un délai (1 seconde) est donc nécessaire pour avoir le temps de récupérer tous les messages des interfaces.

## Sonde

La sonde analyse le réseau pour détecter les packets ARP. Toutes les 5 secondes la fonction passée en paramètre de *sniff* et donne le nombre de packets détecté durant ce lapse de temps.

*sniffer_result* est alors appelé. Dans notre cas, on envoie directement le nombre de packet ARP reçu en argument sans transformation.