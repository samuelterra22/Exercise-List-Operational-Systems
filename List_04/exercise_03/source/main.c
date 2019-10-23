#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>

/******************************************************************************
 * O problema 1 desta lista será implementado com threads e mutexes (POSIX).
 *****************************************************************************/

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

#define N 10
#define TRUE 1
#define FALSE 0

int *buffer;            /* buffer usado entre produtor e consumidor */
int *global_item;       /* item para contator global */
int buffer_pos = -1;    /* posicao do buffer */

pthread_mutex_t mutex;
pthread_cond_t empty;       /* usado para sinalizacao */
pthread_cond_t full;        /* usado para sinalizacao */


void lock(pthread_mutex_t *m) {
	pthread_mutex_lock(m);
}

void unlock(pthread_mutex_t *m) {
	pthread_mutex_unlock(m);
}

void signal(pthread_cond_t *c) {
	pthread_cond_signal(c);
}

void wait(pthread_cond_t *c, pthread_mutex_t *m) {
	pthread_cond_wait(c, m);
}

void produce_item(int producer_id) {
	buffer_pos++;                           /*coloca item no buffer */
	*global_item = *global_item + 1;
	buffer[buffer_pos] = *global_item;
	printf("Produtor [%d] produzindo item: %d\n", producer_id, *global_item);
}

void consume_item(int consumer_id) {
	printf("Consumidor [%d] consumindo item: %d \n", consumer_id, buffer[buffer_pos]);
	buffer_pos--;                           /* retira o item do buffer */
}

void *producer(void *args) {                /* dados do produtor */
	int *producer_id = (int *) args;
	while (TRUE) {
		lock(&mutex);                       /* obtem acesso exclusivo ao buffer */

		while (buffer_pos == N) wait(&full, &mutex);    /* chega se o buffer está cheio */

		produce_item(*producer_id);          /* produz item */

		signal(&empty);                      /* acorda consumidor */
		unlock(&mutex);                      /* libera acesso ao buffer */

		usleep(500 * 1000);
	}
}

void *consumer(void *args) {                 /* dados do consumidor */
	int *consumer_id = (int *) args;
	while (TRUE) {
		lock(&mutex);                        /* obtem acesso exclusivo ao buffer */

		while (buffer_pos == -1) wait(&empty, &mutex);    /* checa se o buffer está vazio */

		consume_item(*consumer_id);          /* consome item */

		signal(&full);                       /* acorda o produtor */
		unlock(&mutex);                      /* libera acesso ao buffer */

		usleep(550 * 1000);
	}
}

int main(int argc, char **argv) {
	pthread_t *thread_producer, *thread_consumer;
	int *producer_ids, *consumer_ids, m, n;

	buffer = (int *) mmap(NULL, sizeof(int) * N, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
	global_item = (int *) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);

	pthread_mutex_init(&mutex, 0);
	pthread_cond_init(&empty, 0);
	pthread_cond_init(&full, 0);

	printf("Informe o valor de m consumidores: ");
	scanf("%d", &m);

	printf("Informe o valor de n produtores: ");
	scanf("%d", &n);

	thread_producer = malloc(n * sizeof(pthread_t));
	thread_consumer = malloc(m * sizeof(pthread_t));

	producer_ids = malloc(n * sizeof(int));
	consumer_ids = malloc(m * sizeof(int));

	for (int i = 0; i < 2; i++) {
		producer_ids[i] = consumer_ids[i] = i;
		if (pthread_create(&thread_producer[i], NULL, producer, (void *) &producer_ids[i]) != 0) {
			printf("Thread not created.\n");
			exit(0);
		}
		if (pthread_create(&thread_consumer[i], NULL, consumer, (void *) &consumer_ids[i]) != 0) {
			printf("Thread not created.\n");
			exit(0);
		}
	}

	for (int i = 0; i < 2; i++) {
		pthread_join(thread_producer[i], NULL);
		pthread_join(thread_consumer[i], NULL);
	}

	pthread_cond_destroy(&empty);
	pthread_cond_destroy(&full);
	pthread_mutex_destroy(&mutex);
}


#pragma clang diagnostic pop