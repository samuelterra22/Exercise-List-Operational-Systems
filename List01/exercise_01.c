#include <stdio.h>
#include <stdlib.h>

double **allocate_matrix(int l, int r) {
    // ponteiro para a matriz
    double **v;

    // Verifica os parâmetros recebidos
    if (l < 1 || r < 1) {
        printf("** Error: Parâmetros inválido **\n");
        return (NULL);
    }

    if (l != r) {
        printf("** Error: matriz não é quadrada **\n");
        return (NULL);
    }

    /* ** Alocação de linhas da matriz ** */

    // Um vetor de m ponteiros para double
    v = (double **) calloc(l, sizeof(double *));

    // Verifica se foi alocado memória
    if (v == NULL) {
        printf("** Error: Memória Insuficiente **");
        return (NULL);
    }

    /* ** aloca as colunas da matriz ** */
    for (int i = 0; i < l; i++) {
        // l vetores de r doubles
        v[i] = (double *) calloc(r, sizeof(double));
        if (v[i] == NULL) {
            printf("** Error: Memória Insuficiente **");
            return (NULL);
        }
    }
    return (v); // retorna o ponteiro para a matriz
}

double **deallocate_matrix(int l, int r, double **v) {
    if (v == NULL) return (NULL);

    // verifica parâmetros recebidos
    if (l < 1 || r < 1) {
        printf("** Error: Parâmetros inválidos **\n");
        return (v);
    }

    if (l != r) {
        printf("** Error: matriz não é quadrada **\n");
        return (v);
    }

    // libera as linhas da matriz
    for (int i = 0; i < l; i++) {
        free(v[i]);
    }

    // libera a matriz (vetor de ponteiros)
    free(v);

    // retorna um ponteiro nulo
    return (NULL);
}

void print_matrix(int l, int r, double **matrix) {
    printf("[\n");
    for (int i = 0; i < l; i++) {
        printf("[");
        for (int j = 0; j < r; j++) {
            printf("%lf ", matrix[i][j]);
        }
        printf("]\n");
    }
    printf("]\n");
}

int is_diagonal_matrix(int l, int r, double **matrix) {
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < r; j++) {
            if (i == j) {
                if (matrix[i][j] != 0) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int is_bottom_diagonal(int m, int n, double **matrix) {
    /*
     *  a11  0   0
     *  a21 a22  0
     *  a31 a32 a33
     * */
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (i < j) {
                if (matrix[i][j] != 0) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int is_upper_diagonal(int m, int n, double **matrix) {
    /*
     *  a11 a12 a13
     *  0   a22 a23
     *  0   0   a33
     * */
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (i > j) {
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

    printf("is_diagonal_matrix: %d\n", is_diagonal_matrix(l, c, mat));
    printf("is_bottom_diagonal: %d\n", is_bottom_diagonal(l, c, mat));
    printf("is_bottom_diagonal: %d\n", is_upper_diagonal(l, c, mat));

    mat = deallocate_matrix(l, c, mat);

    return 0;
}