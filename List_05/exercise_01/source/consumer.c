#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * No presente exercício, Message Queues do POSIX devem ser utilizadas
 * (​mq_open(), mq_send(), mq_receive() ​ , etc). O problema do produtor
 * consumidor deve ser implementado com essas funções. Produtores e
 * consumidores serão processos separados, iniciados pelo usuário e utilizarão
 * troca de mensagens para sua operação.
 *****************************************************************************/

#include <stdlib.h>
#include <mqueue.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "shared.h"

int main() {
    mqd_t mq;
    struct mq_attr mq_attr;
    struct item *itemptr;
    int n, buflen;
    char *bufptr;

    mq = mq_open(MQNAME, O_RDWR | O_CREAT, 0666, NULL);

    if (mq == -1) {
        perror("can not create msg queue\n");
        exit(1);
    }

    mq_getattr(mq, &mq_attr);
    printf("mq maximum msgsize = %d\n", (int) mq_attr.mq_msgsize);

    /* allocate large enough space for the buffer */
    buflen = mq_attr.mq_msgsize;
    bufptr = (char *) malloc(buflen);

    while (TRUE) {
        n = mq_receive(mq, (char *) bufptr, buflen, NULL);
        if (n == -1) {
            perror("mq_receive failed\n");
            exit(1);
        }
        printf("mq_receive success, message size=%d\n", n);
        itemptr = (struct item *) bufptr;
        printf("item->id = %d\n", itemptr->id);
        printf("item->astr = %s\n", itemptr->astr);
        printf("\n");
    }

    free(bufptr);
    mq_close(mq);
    return EXIT_SUCCESS;
}