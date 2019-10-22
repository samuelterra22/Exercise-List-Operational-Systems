#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/******************************************************************************
 * O problema 1 desta lista será implementado com threads e mutexes (POSIX).
 *****************************************************************************/

#define MAX 1000000000
#define BUF_SIZE 10

pthread_mutex_t mutex; // = PTHREAD_MUTEX_INITIALIZER;          /* quantos numeros produzir */
pthread_cond_t empty; // = PTHREAD_COND_INITIALIZER;            /* usado para sinalizacao */
pthread_cond_t full; // = PTHREAD_COND_INITIALIZER;             /* usado para sinalizacao */

int buffer = -1;                             /* buffer usado entre produtor e consumidor */

void *producer(void *ptr) {                 /* dados do produtor */
    for (int i = 1; i <= MAX; i++) {
        pthread_mutex_lock(&mutex);     /* obtem acesso exclusivo ao buffer */
        while (buffer == BUF_SIZE) pthread_cond_wait(&full, &mutex);

        // if buf_index == buf_size, wait full, mutex

        buffer++;                         /*coloca item no buffer */
        printf("producer: %d\n", buffer);

        pthread_cond_signal(&empty);        /* acorda consumidor */
        pthread_mutex_unlock(&mutex);   /* libera acesso ao buffer */

        usleep(500 * 1000);
    }
    pthread_exit(0);
}

void *consumer(void *ptr) { /* dados do consumidor */
    for (int i = 1; i <= MAX; i++) {
        pthread_mutex_lock(&mutex);     /* obtem acesso exclusivo ao buffer */

        // buf_index == -1, wait empty, mutex
        while (buffer == -1) pthread_cond_wait(&empty, &mutex);

        buffer--;                         /* retira o item do buffer */

        printf("consumer: %d\n", buffer);

        pthread_cond_signal(&full);        /* acorda o produtor */
        pthread_mutex_unlock(&mutex);   /* libera acesso ao buffer */

        usleep(500 * 1000);
    }
    pthread_exit(0);
}

int main(int argc, char **argv) {
    pthread_t thread_producer, thread_consumer;

    pthread_mutex_init(&mutex, 0);
    pthread_cond_init(&empty, 0);
    pthread_cond_init(&full, 0);

    pthread_create(&thread_consumer, NULL, consumer, NULL);
    pthread_create(&thread_producer, NULL, producer, NULL);

    pthread_join(thread_producer, NULL);
    pthread_join(thread_consumer, NULL);

    pthread_cond_destroy(&empty);
    pthread_cond_destroy(&full);
    pthread_mutex_destroy(&mutex);
}


















