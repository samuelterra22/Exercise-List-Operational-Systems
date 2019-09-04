#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*************************************************************************
 Um processo irá criar um processo filho. O processo filho deverá calcular
 o pi utilizando o método de monte carlo. O processo filho deve programar
 o tempo de 2 segundos para o cálculo utilizando setitimer. Esse processo
 implementa o tratador do sinal SIGALRM. Quando o tempo estiver espirado,
 ele deve comunicar ao pai por pipe o pi gerado até aquele momento e
 terminar. O pai, quando receber o número, imprime na tela o resultado,
 seu pid e termina.
 ************************************************************************/

int main(int argc, const char *argv[]) {
    printf("Exercício 03.");

    exit(EXIT_SUCCESS);
}