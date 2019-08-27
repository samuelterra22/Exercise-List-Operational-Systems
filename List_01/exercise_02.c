#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 Fazer um programa que receba uma lista de nomes de usuário de um SO juntamente com seu
identificador uid. Uma lista encadeada ordernada por uid deve ser criada para guardar essas
informações. Após isso, uma lista de processos juntamente com o id do usuário que o criou é entrada
pelo usuário. Dentro de cada nó da lista de nomes de usuário, deve ser armazenado o ponteiro de início
de uma outra lista, a lista de processos do respectivo usuário. No final deve ser apresentado cada
usuário, seu id e a lista de processos que estão executando com seu uid.
*/

typedef struct tProcessList {
    int userId;
    char *name;
} ProcessList;

typedef struct tUserList {
    int uid;
    char *name;
    struct tUserList *next;
    ProcessList *processList;
} UserList;

void insert(UserList **p, int uid, char *name) {
    UserList *current, *new, *previous;

    new = (UserList *) malloc(sizeof(UserList));

    current = *p;
    previous = NULL;

    new->uid = uid;
    new->name = malloc(strlen(name));
    strcpy(new->name, name);

    // ProcessList *pl = (ProcessList *) malloc(sizeof(ProcessList));
    // new->processList = pl;

    if (current == NULL) {
        new->next = NULL;
        *p = new;
    } else {
        while (current != NULL && current->uid < uid) {
            previous = current;
            current = current->next;
        }

        new->next = current;

        if (previous == NULL) {
            *p = new;
        } else {
            previous->next = new;
        }
    }
}

void print_list(UserList *p) {
    while (p != NULL) {
        printf("-------------------------------\n");
        printf("uid: %d\n", p->uid);
        printf("name: %s\n", p->name);
        printf("---------- Processos ----------\n");
        while (p->processList != NULL) {
            printf("%s\n", p->processList->name);
        }
        printf("-------------------------------\n");
        p = p->next;
    }
}

void *destroy_list(UserList *l) {
    UserList *p = l;
    while (p != NULL) {
        UserList *t = p->next;
        free(p->name);
        free(p);
        p = t;
    }
    return NULL;
}


int main(void) {
    UserList *list = NULL;
    int quantity = 0;
    int uid;
    char *name;

    printf("Insira o número de entradas: ");
    scanf("%d", &quantity);

    printf("-------------------------------\n");
    for (int i = 0; i < quantity; i++) {
        printf("Insira o nome do usuário[%d]: ", i);
        scanf("%s", name);

        printf("Insira o uid do usuário[%d]: ", i);
        scanf("%d", &uid);

        insert(&list, uid, name);
        printf("-------------------------------\n");
    }

    print_list(list);
    list = destroy_list(list);
    print_list(list);

    exit(EXIT_SUCCESS);
}