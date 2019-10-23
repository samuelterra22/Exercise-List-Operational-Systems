#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>
#include "shared.h"

/******************************************************************************
 * No presente exercício, Message Queues do POSIX devem ser utilizadas
 * (​mq_open(), mq_send(), mq_receive() ​ , etc). O problema do produtor
 * consumidor deve ser implementado com essas funções. Produtores e
 * consumidores serão processos separados, iniciados pelo usuário e utilizarão
 * troca de mensagens para sua operação.
 *****************************************************************************/

int main() {
    mqd_t mq;
    struct item item;
    int n, i = 0;

    mq = mq_open(MQNAME, O_RDWR);

    if (mq == -1) {
        perror("mq_open failed\n");
        exit(1);
    }

    while (TRUE) {
        item.id = i;
        strcpy(item.astr, "os is good\n");

        n = mq_send(mq, (char *) &item, sizeof(struct item), 0);

        if (n == -1) {
            perror("mq_send failed\n");
            exit(1);
        }

        i++;
        sleep(1);
    }

    mq_close(mq);
    return EXIT_SUCCESS;
}
