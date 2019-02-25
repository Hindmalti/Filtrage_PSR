#
# Makefile pour generer la bibliotheque de fonctions de communication
#

all: libnet.a

#
# La cible de nettoyage
#

clean: 
	rm -f core *.o libnet.a

#
# Les cibles pour la bibliotheque
#

libnet.o: libnet.c libnet.h
 
libcom.a: libnet.o
	rm -f $@
	$(CLIB) $@ $+
