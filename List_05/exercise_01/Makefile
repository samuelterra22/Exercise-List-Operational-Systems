##################### Exercise 1 Makefile #######################
# AUTHOR:   Samuel Terra Vieira                                 #
# ADDRESS:  Universidade Federal de Lavras                      #
# DATE:     09/2019                                             #
#################################################################

all: producer consumer

consumer: source/consumer.c
	gcc -Wall -o consumer source/consumer.c -lrt

producer: source/producer.c
	gcc -Wall -o producer source/producer.c -lrt

clean:
	rm -fr *~ producer consumer