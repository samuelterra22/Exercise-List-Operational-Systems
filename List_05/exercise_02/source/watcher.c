#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <mqueue.h>

#include "shared.h"

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

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
    int inotify_fd, wd;
    char buf[BUF_LEN] __attribute__ ((aligned(8)));
    ssize_t num_read;
    char *p;
    struct inotify_event *event;

    mqd_t listener;
    struct Message m;
    int item;

    struct mq_attr attr;
    attr.mq_flags = 0;                          /* Flags: 0 or O_NONBLOCK */
    attr.mq_maxmsg = 10;                        /* Max. # of messages on queue */
    attr.mq_msgsize = sizeof(struct Message);   /* Max. Message size (bytes) */
    attr.mq_curmsgs = 0;                        /* # of messages currently in queue */

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        printf("%s pathname...\n", argv[0]);

    inotify_fd = inotify_init();                 /* Create inotify instance */
    if (inotify_fd == -1)
        printf("inotify_init");

    /* For each command-line argument, add a watch for all events */

    for (int j = 1; j < argc; j++) {
        wd = inotify_add_watch(inotify_fd, argv[j], IN_ALL_EVENTS);
        if (wd == -1)
            printf("inotify_add_watch");

        printf("Watching %s using wd %d\n", argv[j], wd);
    }


    listener = mq_open(MQ_NAME_WATCHER, O_RDWR | O_CREAT, 0644, &attr);

    if (listener == -1) {
        perror("mq_open listener failed\n");
        exit(EXIT_FAILURE);
    }

    while (TRUE) {                                  /* Read events forever */
        num_read = read(inotify_fd, buf, BUF_LEN);
        if (num_read == 0)
            printf("read() from inotify fd returned 0!");

        if (num_read == -1)
            printf("read");

        printf("Read %ld bytes from inotify fd\n", (long) num_read);

        /* Process all of the events in buffer returned by read() */

        for (p = buf; p < buf + num_read;) {
            event = (struct inotify_event *) p;

            printf("enviando msg");

//            display_inotify_event(event);
            build_message(&m, event);
            send(listener, &m);


            p += sizeof(struct inotify_event) + event->len;
        }
    }

//    while (TRUE) {
//        item = produce_item();
//        build_message(&m, item);
//        send(listener, &m);
//
//        sleep(1);
//    }

    mq_close(listener);
    return EXIT_SUCCESS;

    exit(EXIT_SUCCESS);
}

#pragma clang diagnostic pop