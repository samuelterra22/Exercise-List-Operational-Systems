#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <limits.h>
#include <mqueue.h>

#include "shared.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

/******************************************************************************
 * Criar um programa que monitore um diretório apontado pelo usuário e mande
 * modificações para um outro processo que manterá um espelho dos arquivos
 * contidos no diretório monitorado. Para isso, a API ​ Inotify deverá ser
 * utilizada. Para comunicação, Message Queues do POSIX serão utilizadas entre
 * o processo que faz o monitoramento e o servidor que mantém um espelho dos
 * arquivos.
 *****************************************************************************/

int main(int argc, char *argv[]) {
    mqd_t watcher;
    struct mq_attr mq_attr;
    struct Message m;
    struct inotify_event *item;

    struct mq_attr attr;
    attr.mq_flags = 0;                          /* Flags: 0 or O_NONBLOCK */
    attr.mq_maxmsg = 10;                        /* Max. # of messages on queue */
    attr.mq_msgsize = sizeof(struct Message);   /* Max. Message size (bytes) */
    attr.mq_curmsgs = 0;                        /* # of messages currently in queue */

    watcher = mq_open(MQ_NAME_WATCHER, O_RDWR | O_CREAT, 0644, &attr);

    if (watcher == -1) {
        perror("can not create watcher msg queue\n");
        exit(1);
    }

    mq_getattr(watcher, &mq_attr);
    printf("mq maximum msgsize = %d\n", (int) mq_attr.mq_msgsize);

    while (TRUE) {

        receive(watcher, &m);

        item = extract_item(&m);
        printf("    wd ");
//        printf("    wd =%2d; ", item->wd);

        // display_inotify_event(item);
    }

    mq_close(watcher);
    return EXIT_SUCCESS;
}

#pragma clang diagnostic pop