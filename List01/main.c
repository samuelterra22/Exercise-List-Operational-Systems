#include <stdio.h>
#include <stdlib.h>

double **allocate_matrix(int m, int n) {
    // ponteiro para a matriz
    double **v;

    // Verifica os parâmetros recebidos
    if (m < 1 || n < 1) {
        printf("** Erro: Parametro invalido **\n");
        return (NULL);
    }

    /* ** Alocação de linhas da matriz ** */

    // Um vetor de m ponteiros para double
    v = (double **) calloc(m, sizeof(double *));

    // Verifica se foi alocado memória
    if (v == NULL) {
        printf("** Erro: Memoria Insuficiente **");
        return (NULL);
    }

    /* ** aloca as colunas da matriz ** */
    for (int i = 0; i < m; i++) {
        // m vetores de n doubles
        v[i] = (double *) calloc(n, sizeof(double));
        if (v[i] == NULL) {
            printf("** Erro: Memoria Insuficiente **");
            return (NULL);
        }
    }
    return (v); /* retorna o ponteiro para a matriz */
}

double **deallocate_matrix(int m, int n, double **v) {
    if (v == NULL) return (NULL);

    // verifica parametros recebidos
    if (m < 1 || n < 1) {
        printf("** Erro: Parâmetro inválido **\n");
        return (v);
    }

    // libera as linhas da matriz
    for (int i = 0; i < m; i++) {
        free(v[i]);
    }

    // libera a matriz (vetor de ponteiros)
    free(v);

    // retorna um ponteiro nulo
    return (NULL);
}

void print_matrix(int m, int n, double **matrix) {
    printf("[\n");
    for (int i = 0; i < m; i++) {
        printf("[");
        for (int j = 0; j < n; j++) {
            printf("%lf ", matrix[i][j]);
        }
        printf("]\n");
    }
    printf("]\n");
}

int is_diagonal_matrix(int m, int n, double **matrix) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                if (matrix[i][j] != 0) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int main() {
    double **mat;
    int l, c;
    int i, j;

    l = 4;
    c = 4;

    mat = allocate_matrix(l, c);

    for (i = 0; i < l; i++) {
        for (j = 0; j < c; j++) {
            mat[i][j] = 1;
        }
    }

    print_matrix(l, c, mat);

    printf("%d\n", is_diagonal_matrix(l, c, mat));

    mat = deallocate_matrix(l, c, mat);

    return 0;
}