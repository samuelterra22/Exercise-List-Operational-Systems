#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (FUNC)(int, double **);

double **allocate_matrix(int dimension) {
    // ponteiro para a matriz
    double **v;

    // Verifica os parâmetros recebidos
    if (dimension < 1) {
        printf("** Error: Parâmetros inválido **\n");
        return (NULL);
    }

    /* ** Alocação de linhas da matriz ** */

    // Um vetor de m ponteiros para double
    v = (double **) calloc(dimension, sizeof(double *));

    // Verifica se foi alocado memória
    if (v == NULL) {
        printf("** Error: Memória Insuficiente **");
        return (NULL);
    }

    /* ** aloca as colunas da matriz ** */
    for (int i = 0; i < dimension; i++) {
        // l vetores de r doubles
        v[i] = (double *) calloc(dimension, sizeof(double));
        if (v[i] == NULL) {
            printf("** Error: Memória Insuficiente **");
            return (NULL);
        }
    }
    return (v); // retorna o ponteiro para a matriz
}

double **deallocate_matrix(int dimension, double **v) {
    if (v == NULL) return (NULL);

    // verifica parâmetros recebidos
    if (dimension < 1) {
        printf("** Error: Parâmetros inválidos **\n");
        return (v);
    }

    // libera as linhas da matriz
    for (int i = 0; i < dimension; i++) {
        free(v[i]);
    }

    // libera a matriz (vetor de ponteiros)
    free(v);

    // retorna um ponteiro nulo
    return (NULL);
}

void print_matrix(int dimension, double **matrix) {
    printf("[\n");
    for (int i = 0; i < dimension; i++) {
        printf("[");
        for (int j = 0; j < dimension; j++) {
            printf("%lf ", matrix[i][j]);
        }
        printf("]\n");
    }
    printf("]\n");
}

int is_diagonal_matrix(int dimension, double **matrix) {
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if (i == j) {
                if (matrix[i][j] != 0) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int is_bottom_diagonal(int dimension, double **matrix) {
    /*
     *  a11  0   0
     *  a21 a22  0
     *  a31 a32 a33
     * */
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if (i < j) {
                if (matrix[i][j] != 0) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int is_upper_diagonal(int dimension, double **matrix) {
    /*
     *  a11 a12 a13
     *  0   a22 a23
     *  0   0   a33
     * */
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if (i > j) {
                if (matrix[i][j] != 0) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int execute(FUNC func, int dimension, double **mat) {
    return (*func)(dimension, mat);
}

void fill_matrix(int dimension, double **mat) {
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            printf("Informe o dado Matriz[%d][%d]: ", i, j);
            scanf("%lf", &mat[i][j]);
        }
    }
}

int main(void) {
    double **mat;
    int dimension, proc;

    printf("Informe a dimensão da matriz (ex:3): ");
    scanf("%d", &dimension);

    mat = allocate_matrix(dimension);

    FUNC *functions[3];
    functions[0] = &is_diagonal_matrix;
    functions[1] = &is_bottom_diagonal;
    functions[2] = &is_upper_diagonal;

    fill_matrix(dimension, mat);

    printf("Qual procedimento deve ser executado?\n");
    printf("[1] - Diagonal;\n");
    printf("[2] - Triangular inferior;\n");
    printf("[3] - Triangular superior;\n");
    scanf("%d", &proc);

    if (proc < 1 || proc > 3) {
        printf("Método informado é inválido");
        exit(EXIT_FAILURE);
    }

    print_matrix(dimension, mat);

    int exec_result = execute(functions[proc - 1], dimension, mat);

    if (exec_result) {
        printf("A matriz é do tipo selecionado");
    } else {
        printf("A matriz não é do tipo selecionado");
    }

    mat = deallocate_matrix(dimension, mat);

    exit(EXIT_SUCCESS);
}