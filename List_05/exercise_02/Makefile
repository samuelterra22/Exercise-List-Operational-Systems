##################### Exercise 2 Makefile #######################
# AUTHOR:   Samuel Terra Vieira                                 #
# ADDRESS:  Universidade Federal de Lavras                      #
# DATE:     09/2019                                             #
#################################################################

all: watcher listener

listener: source/listener.c
	gcc -o listener source/listener.c source/shared.c -lrt

watcher: source/watcher.c
	gcc -o watcher source/watcher.c source/shared.c -lrt

clean:
	rm -fr *~ watcher listener