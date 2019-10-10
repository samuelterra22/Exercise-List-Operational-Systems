#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>

/******************************************************************************
 * Problema dos Filósofos Glutões. Nesse exercício, o usuário deverá indicar o
 * número de filósofos que será utilizado no problema. Um programa deverá ser
 * feito que permita os filósofos adquirirem os recursos (garfos) e comerem.
 * Cada filósofo será implementado por uma thread da biblioteca pthreads. Para
 * fazer a sincronização, semáforos POSIX serão utilizados.
 *****************************************************************************/

#define FALSE       0
#define TRUE        1

#define N           5           /* Número de processos */
#define LEFT        (i+N-1)%N   /* numero do vizinho a esquerda de i */
#define RIGHT       (i+1)%N     /* numero do vizinho a direita de i */
#define THINKING    0           /* o filosofo esta pensando */
#define HUNGRY      1           /* o filosofo esta tentando pegar garfos */
#define EATING      2           /* o filosofo esta comendo */

int state[N];
sem_t mutex;
sem_t s[N];

void eat(int i) {
	printf("Filoso %d comendo...\n", i);
	usleep(200 * 1000);
	printf("Filoso %d liberou o garfo\n", i);
}

void think(int i) {
	printf("Filoso %d pensando...\n", i);
	usleep(200 * 1000);
}

void down(sem_t *sem) {
	sem_wait(sem);
}

void up(sem_t *sem) {
	sem_post(sem);
}

/* i: o n umero do filosofo, de 0 a N-1 */
void test(int i) {
	if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
		state[i] = EATING;
		up(&s[i]);
	}
}

/* i: o número do filosofo, de 0 a N-1 */
void take_forks(int i) {
	down(&mutex);
	state[i] = HUNGRY;
	test(i);
	up(&mutex);
	down(&s[i]);
}

/* i: o número do filosofo, de 0 a N-1 */
void put_forks(int i) {
	down(&mutex);
	state[i] = THINKING;
	test(LEFT);
	test(RIGHT);
	up(&mutex);
}

/* i: o numero do filosofo, de 0 a N-1 */
void *philosopher(void *args) {
	int *i = (int *) args;
	while (TRUE) {
		think(*i);
		take_forks(*i);
		eat(*i);
		put_forks(*i);
	}
}

int main(int argc, const char *argv[]) {
	pthread_t thread_id[N];
	int ids[N];

	sem_init(&mutex, 0, 1);

	for (int i = 0; i < N; i++) {
		sem_init(&s[i], 0, 0);
	}

	for (int i = 0; i < N; i++) {
		ids[i] = i;
		if (pthread_create(&thread_id[i], NULL, philosopher, (void *) &ids[i]) != 0) {
			printf("Thread not created.\n");
			exit(0);
		}
	}

	for (int i = 0; i < N; i++) {
		pthread_join(thread_id[i], NULL);
	}

	sem_destroy(&mutex);
	sem_unlink("/mutex_sem");
	sem_unlink("/s_sem");

	return EXIT_SUCCESS;
}

#pragma clang diagnostic pop