#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "shared.h"

int i = 0;

int produce_item() {
	printf("Produziu %d\n", i++);
	return i;
}

void consume_item(int item) {
	printf("Consumiu %d\n", item);
}

void receive(mqd_t mq, struct Message *message) {
	int n = mq_receive(mq, (char *) message, sizeof(struct Message), NULL);
	if (n == -1) {
		perror("mq_receive failed\n");
		exit(EXIT_FAILURE);
	}

//	printf("asdasd: %d\n", message->id);

//	printf("mq_receive success, message size=%d\n", n);
}

int extract_item(struct Message *message) {
	return message->id;
}

void build_message(struct Message *message, int item) {
	message->id = item;
	strcpy(message->astr, "os is good\n");
}

void send(mqd_t mq, struct Message *message) {
	int n = mq_send(mq, (char *) message, sizeof(struct Message), 0);

	if (n == -1) {
		perror("mq_send failed\n");
		exit(EXIT_FAILURE);
	}
}