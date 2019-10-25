#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "shared.h"

void receive(mqd_t mq, struct Message *message) {
	int n = mq_receive(mq, (char *) message, sizeof(struct Message), NULL);
	if (n == -1) {
		perror("mq_receive failed\n");
		exit(EXIT_FAILURE);
	}
}

int extract_item(struct Message *message) {
	return message->id;
}

void build_message(struct Message *message, int item) {
	message->id = item;
}

void send(mqd_t mq, struct Message *message) {
	int n = mq_send(mq, (char *) message, sizeof(struct Message), 0);

	if (n == -1) {
		perror("mq_send failed\n");
		exit(EXIT_FAILURE);
	}
}