# Tutorat PSR

## Choix de la trame

XXXYYYYY YYYYYYYY (2 bytes)

XXX : commandes
* 000 getStatus : demande à l'interface son status (les Y sont ignorés)
* 001 retStatus : l'interface retourne son status (Y = ZZZZZ ZZZZZZZ1 : mode éveillé | Y = ZZZZZ ZZZZZZZ0 : mode sommeil & Z = identifiant de l'interface, numéro du groupe)
* 010 setStatus : demande à l'interface de passer dans le mode sommeil ou éveillé (Y = 00000 00000001 : mode éveillé | Y = 00000 00000000 : mode sommeil)
* 011 getCommande : demande à l'interface la commande en mémoire
* 100 retCommande : l'interface retourne sa commande en mémoire (les Y représentent un pourcentage : Y = 11111 11111111 : 100%, Y = 00000 00000000 : 0%)
* 101 setCommande : envoie à l'interface la commande à executer (les Y représentent un pourcentage : Y = 11111 11111111 : 100%, Y = 00000 00000000 : 0%)

Le choix nous permet de nous affranchir de la provenance de la trame (UDP et TCP). Cela équivaut à une encapsulation de niveau 7 (niveau applicatif) dans le modèle OSI.
