#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <zconf.h>

/******************************************************************************
 * No presente exercício, Message Queues do POSIX devem ser utilizadas
 * (​mq_open(), mq_send(), mq_receive() ​ , etc). O problema do produtor
 * consumidor deve ser implementado com essas funções. Produtores e
 * consumidores serão processos separados, iniciados pelo usuário e utilizarão
 * troca de mensagens para sua operação.
 *****************************************************************************/
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

#include "shared.h"

#define N 10

int main() {
	mqd_t producer;
	struct mq_attr mq_attr;
	struct Message m;
	int item;

	struct mq_attr attr;
	attr.mq_flags = 0;                            /* Flags: 0 or O_NONBLOCK */
	attr.mq_maxmsg = 10;                        /* Max. # of messages on queue */
	attr.mq_msgsize = sizeof(struct Message);    /* Max. Message size (bytes) */
	attr.mq_curmsgs = 0;                        /* # of messages currently in queue */

	producer = mq_open(MQ_NAME, O_RDWR | O_CREAT, 0644, &attr);

	if (producer == -1) {
		perror("can not create msg queue\n");
		exit(1);
	}

	mq_getattr(producer, &mq_attr);
	printf("mq maximum msgsize = %d\n", (int) mq_attr.mq_msgsize);

	for (int j = 0; j < N; j++) send(producer, &m); /* envia N mensagens vazias */

	while (TRUE) {
		printf("vai receber\n");
		receive(producer, &m);

		printf("vai extrair\n");
		item = extract_item(&m);

		printf("vai enviar\n");
		send(producer, &m);

		printf("vai consumir\n");
		consume_item(item);

		printf("vai dormir 1s\n");
		sleep(1);
	}

	mq_close(producer);
	return EXIT_SUCCESS;
}

#pragma clang diagnostic pop