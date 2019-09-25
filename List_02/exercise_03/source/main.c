#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

/******************************************************************************
 * Um processo irá criar um processo filho. O processo filho deverá calcular
 * o pi utilizando o método de monte carlo. O processo filho deve programar
 * o tempo de 2 segundos para o cálculo utilizando setitimer. Esse processo
 * implementa o tratador do sinal SIGALRM. Quando o tempo estiver espirado, ele
 * deve comunicar ao pai por pipe o pi gerado até aquele momento eterminar. O
 * pai, quando receber o número, imprime na tela o resultado, seu pid e
 * termina.
 *****************************************************************************/

#define READ 0
#define WRITE 1
#define TRUE 1
#define FALSE 0

int STOP_MONTE_CARLO = FALSE;

void monte_carlo_pi(int FD[2]) {
    double x, y, val;
    unsigned long sampled = 0, hit = 0;
    srand48(time(NULL));

    printf("Starting PI calculation by Monte Carlo\n");

    while (!STOP_MONTE_CARLO) {
        x = drand48();
        y = drand48();

        if (x * x + y * y < 1) hit++;

        val = 4 * (((double) hit) / (double) sampled);

        sampled++;
    }

    close(FD[READ]);

    write(FD[WRITE], &val, sizeof(double));

    close(FD[WRITE]);
}

void timer_handler() {
    STOP_MONTE_CARLO = TRUE;
    printf("Monte carlo stopped after 2 sec. by SIGALRM\n");
}

int main(int argc, const char *argv[]) {

    // variable to store process pid
    pid_t pid;

    // file descriptor
    int FD[2];

    if (pipe(FD) == -1) {
        printf("FD pipe failed\n");
        return EXIT_FAILURE;
    }

    // make process fork
    if ((pid = fork()) < 0) {
        printf("Fork failed\n");
        return EXIT_FAILURE;
    }

    // parent process
    if (pid != 0) {
        // wait all children
        while (wait(NULL) > 0);

        close(FD[WRITE]);
        double buffer;

        read(FD[READ], &buffer, sizeof(double));
        fprintf(stderr, "Value of pi: %.20lf.", buffer);

        close(FD[READ]);

    }
        // children process
    else {
        struct itimerval timer;

        // set the handler alarm
        signal(SIGALRM, timer_handler);

        // set the alarm
        timer.it_value.tv_sec = 2;
        timer.it_value.tv_usec = 0;
        timer.it_interval.tv_sec = 0;
        timer.it_interval.tv_usec = 0;

        if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
            printf("setitimer error");
            exit(EXIT_FAILURE);
        }

        monte_carlo_pi(FD);

        _exit(EXIT_SUCCESS);
    }

    return EXIT_SUCCESS;
}