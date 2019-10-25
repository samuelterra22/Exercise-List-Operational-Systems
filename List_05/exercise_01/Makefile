##################### Exercise 1 Makefile #######################
# AUTHOR:   Samuel Terra Vieira                                 #
# ADDRESS:  Universidade Federal de Lavras                      #
# DATE:     09/2019                                             #
#################################################################

all: producer consumer

consumer: source/consumer.c
	gcc -o consumer source/consumer.c source/shared.c -lrt

producer: source/producer.c
	gcc -o producer source/producer.c source/shared.c -lrt

clean:
	rm -fr *~ producer consumer