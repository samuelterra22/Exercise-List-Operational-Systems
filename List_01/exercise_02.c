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
    struct tProcessList *next;
} ProcessList;

typedef struct tUserList {
    int uid;
    char *name;
    struct tUserList *next;
    struct tProcessList *processList;
} UserList;

void insert_user_list(UserList **p, int uid, char *name) {
    UserList *current, *new, *previous;

    new = (UserList *) malloc(sizeof(UserList));

    current = *p;
    previous = NULL;

    new->uid = uid;
    new->name = malloc(strlen(name) + 1);
    strcpy(new->name, name);
    new->processList = NULL;

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

void print_user_list(UserList *p) {
    while (p != NULL) {
        printf("%s %d\n", p->name, p->uid);
        ProcessList *processList = p->processList;
        while (processList != NULL) {
            printf("%s\n", processList->name);
            processList = processList->next;
        }
        p = p->next;
    }
}

void *destroy_user_list(UserList *l) {
    UserList *p = l;
    while (p != NULL) {
        UserList *t = p->next;
        free(p->name);
        free(p);
        p = t;
    }
    return NULL;
}

UserList *search_user_in_list(UserList *userList, int uid) {
    UserList *p;
    for (p = userList; p != NULL; p = p->next) {
        if (p->uid == uid) {
            return p;
        }
    }
    return NULL;
}

UserList *remove_user_in_list(UserList *userList, int uid) {
    // ponteiro para o elemento anterior
    UserList *preview = NULL;

    // ponteiro para percorrer a lista
    UserList *p = userList;

    // procura o elemento na lista, guardando o anterior
    while (p != NULL && p->uid != uid) {
        preview = p;
        p = p->next;
    }

    // verifica se achou o anterior
    if (p == NULL) {
        // se nao achou, retorna a lista original
        return userList;
    }

    // retira o elemento
    if (preview == NULL) {
        // retira elemento do inicio
        userList = p->next;
    } else {
        // retira elemento do meio da lista
        preview->next = p->next;
    }

    // libera memório do ponteiro que foi utilizado para percorrer a lista
    free(p);

    return userList;
}

/****************************************************
 * Função resposável por verificar se a lista de
 * usuários está vazia.
 *
 * @param userList  Ponteiro para lista de usuários.
 *
 * @return Retorna 1 quando a lista for vazia ou 0
 * caso contrário.
 ***************************************************/
int is_user_list_empty(UserList *userList) {
    return userList == NULL;
}

/****************************************************
 * Função resposável por inicializar uma lista de
 * usuários.
 *
 * @param void
 *
 * @return Retorna NULL, "inicializando" a lista.
 ***************************************************/
UserList *init_user_list(void) {
    return NULL;
}

/****************************************************
 * Função resposável por inicializar uma lista de
 * processos.
 *
 * @param void
 *
 * @return Retorna NULL, "inicializando" a lista.
 ***************************************************/
ProcessList *init_process_list(void) {
    return NULL;
}

void insert_process_for_user_list(UserList *userList, int userId, char *processName) {
    // Busca o usuário
    UserList *user = search_user_in_list(userList, userId);

    // Aloca memória para o novo processo
    ProcessList *newProcess = (ProcessList *) malloc(sizeof(ProcessList));

    // Adiciona informações do processo
    newProcess->userId = userId;
    newProcess->name = malloc(strlen(processName) + 1);
    strcpy(newProcess->name, processName);
    newProcess->next = NULL;

    // Verifica se a lista de processos do usuário está vazia
    if (user->processList == NULL) {
        user->processList = newProcess;
        user->processList->next = NULL;
    } else {
        // Se não tiver vazia, o próximo da lista é o processo atual
        ProcessList *first = user->processList;
        while (first->next != NULL) {
            first = first->next;
        }
        first->next = newProcess;
    }
}

int main(void) {
    UserList *userList = init_user_list();
    int quantity = 0;
    int uid;
    char *name;

    scanf("%d", &quantity);

    for (int i = 0; i < quantity; i++) {
        scanf("%s %d", name, &uid);
        insert_user_list(&userList, uid, name);
    }

    scanf("%d", &quantity);

    for (int i = 0; i < quantity; i++) {
        scanf("%s %d", name, &uid);

        insert_process_for_user_list(userList, uid, name);
    }

    print_user_list(userList);
    userList = destroy_user_list(userList);

    exit(EXIT_SUCCESS);
}