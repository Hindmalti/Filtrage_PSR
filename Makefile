#
# Variables d'environnement
#
export CC = gcc
export LD = gcc
export CLIB = ar cq
export CFLAGS = -Wall -Wextra -pedantic

#
# Constantes liees au projet
#

DIRS = Threads Network Sioux Tangible

#
# La cible generale
#

all: $(patsubst %, _dir_%, $(DIRS))

$(patsubst %,_dir_%,$(DIRS)):
	cd $(patsubst _dir_%,%,$@) && make

debug: CFLAGS += -g -DDEBUG
debug: all

#
# La cible de nettoyage
#

clean: $(patsubst %, _clean_%, $(DIRS))

$(patsubst %,_clean_%,$(DIRS)):
	cd $(patsubst _clean_%,%,$@) && make clean