#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>
#include "shared.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

/******************************************************************************
 * No presente exercício, Message Queues do POSIX devem ser utilizadas
 * (​mq_open(), mq_send(), mq_receive() ​ , etc). O problema do produtor
 * consumidor deve ser implementado com essas funções. Produtores e
 * consumidores serão processos separados, iniciados pelo usuário e utilizarão
 * troca de mensagens para sua operação.
 *****************************************************************************/

int main() {
	mqd_t consumer;
	struct Message m;
	int item;

	struct mq_attr attr;
	attr.mq_flags = 0;                            /* Flags: 0 or O_NONBLOCK */
	attr.mq_maxmsg = 10;                        /* Max. # of messages on queue */
	attr.mq_msgsize = sizeof(struct Message);    /* Max. Message size (bytes) */
	attr.mq_curmsgs = 0;                        /* # of messages currently in queue */

	consumer = mq_open(MQ_NAME, O_RDWR | O_CREAT, 0644, &attr);

	if (consumer == -1) {
		perror("mq_open failed\n");
		exit(EXIT_FAILURE);
	}

	while (TRUE) {

		printf("vai extrair\n");
		item = produce_item();

		printf("vai receber\n");
		receive(consumer, &m);

		printf("vai montar mensagem\n");
		build_message(&m, item);

		printf("vai enviar\n");
		send(consumer, &m);

		printf("vai dormir 1s\n");
		sleep(1);
	}

	mq_close(consumer);
	return EXIT_SUCCESS;
}

#pragma clang diagnostic pop