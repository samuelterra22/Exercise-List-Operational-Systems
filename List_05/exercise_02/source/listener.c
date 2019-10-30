#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <limits.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "message.h"

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

void handle_message(struct Message *message, char *watcher_path, char *listener_path) {
    // IN_CREATE
    // IN_DELETE
    // IN_MODIFY
    // IN_MOVED_FROM
    // IN_MOVED_TO

    char *path_tmp = malloc(sizeof(char) * strlen(listener_path));
    strcpy(path_tmp, listener_path);

    struct inotify_event *event = extract_item(message);

    display_inotify_event(event);

    if (event->mask & IN_CREATE){
        if (event->mask & IN_ISDIR){
            mkdir(strcat(path_tmp, event->name), 0644);
        } else {
            int fd = open(strcat(path_tmp, event->name), O_CREAT);
            close(fd);
        }
    }

    if (event->mask & IN_DELETE){
        if (event->mask & IN_ISDIR){
            rmdir(strcat(path_tmp, event->name));
        } else {
            remove(strcat(path_tmp, event->name));
        }
    }

    if (event->mask & IN_MODIFY){
        printf("path: %s", watcher_path);
        // remove(event->name);
    }
}

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);

    mqd_t watcher;
    struct mq_attr mq_attr;
    struct Message m;

    struct mq_attr attr;
    attr.mq_flags = 0;                          /* Flags: 0 or O_NONBLOCK */
    attr.mq_maxmsg = 10;                        /* Max. # of messages on queue */
    attr.mq_msgsize = sizeof(struct Message) + NAME_MAX + 1;   /* Max. Message size (bytes) */
    attr.mq_curmsgs = 0;                        /* # of messages currently in queue */

    if (argv[1] == NULL) {
        printf("Inform the watcher path!\n");
        printf("example to run: $ ./listener [watcher_path] [listener_path]\n");
        exit(EXIT_FAILURE);
    }

    if (argv[2] == NULL) {
        printf("Inform the listener path!\n");
        printf("example to run: $ ./listener [watcher_path] [listener_path]\n");
        exit(EXIT_FAILURE);
    }

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        printf("%s pathname...\n", argv[0]);

    watcher = mq_open(MQ_NAME_WATCHER, O_RDWR | O_CREAT, 0644, &attr);

    if (watcher == -1) {
        perror("can not create watcher msg queue\n");
        exit(1);
    }

    mq_getattr(watcher, &mq_attr);
    printf("mq maximum msgsize = %d\n", (int) mq_attr.mq_msgsize);

    while (TRUE) {
        receive(watcher, &m);

//        display_inotify_event(item);

        handle_message(&m, argv[1], argv[2]);
    }

    mq_close(watcher);
    return EXIT_SUCCESS;
}

#pragma clang diagnostic pop