#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define ENTRADAS_MAX 1024
#define TAMANHO_MAX 80
#define TAMANHO_NOME 8
#define DEBUG 0

int parent[ENTRADAS_MAX];
char nome[ENTRADAS_MAX ][TAMANHO_NOME];
char data[ENTRADAS_MAX][10];
char hora[ENTRADAS_MAX][10];

int posicao_atual = 0;
int codigo = 0;

char datalocal[10];
char horalocal[10];

void data_hora() {

    time_t atual = time(NULL);
    struct tm  data_atual = *localtime(&atual);

    sprintf(horalocal, "%d:%d:%d", data_atual.tm_hour, data_atual.tm_min, data_atual.tm_sec);
    sprintf(datalocal, "%d/%d/%d", data_atual.tm_mday, data_atual.tm_mon + 1, data_atual.tm_year + 1900);
}

void mkdir(char *nomedir) {

    int i = 0;
    int existe = 0;

    if (nomedir == NULL) {

        printf(" Coloque um nome.\n");
        return;
    }

    if (strlen(nomedir) > 8) {

        printf(" Maximo de 8 letras no nome.\n");
    }

    for (i = 0; i < ENTRADAS_MAX; i++) {

        if (parent[i] == posicao_atual) {

            if (strcmp(nomedir, nome[i]) == 0) {

                printf(" Arquivo ja existe.\n");
                existe = 1;
                return;
            }
        }
    }

    if (existe == 0) {

        codigo++;
        data_hora();

        parent[codigo] = posicao_atual;

        strcpy(nome[codigo], nomedir);
        strcpy(data[codigo], datalocal);
        strcpy(hora[codigo], horalocal);
    }
}

void rmdir(char *nomedir) {

    if(nomedir == NULL) {

        printf(" Insira o nome do arquivo a ser excluido.\n");
    }

    int positivo = 0;
    int i = 0;
    int j = 0;

    for (i = 0; i < ENTRADAS_MAX; i++) {

        if (parent[i] == posicao_atual) {

            if (strcmp(nomedir, nome[i]) == 0) {

                positivo = 1;
                break;
            }
        }
    }

    if (positivo == 0) {

        printf(" O diretorio nao existe.\n");
        return;
    }

    for (j = 0; j < ENTRADAS_MAX; j++) {

    if (parent[j] == posicao_atual) {

        if (strcmp(nomedir, nome[i]) == 0) {

            printf(" Nao apagado, existe diretorio's' dentro dele.\n");
            return;
        }
    }

        if (positivo == 1) {

            parent[i] = 0;
            strcpy(nome[i], "");
            strcpy(data[i], "");
            strcpy(hora[i], "");
        }
    }
}

void ren(char *antigo, char *novo) {

    if(antigo == NULL || novo == NULL) {

        printf(" Insira o nome do arquivo e o novo nome.\n");
    }

    if (strlen(novo) > 8) {

        printf(" Maximo de 8 letras no nome.\n");
    }

    int positivo = 0;
    int i = 0;
    int j = 0;
    int local = 0;

    for (i = 0; i < ENTRADAS_MAX; i++) {

        if (parent[i] == posicao_atual) {

            if (strcmp(antigo, nome[i]) == 0) {

                positivo = 1;
                local = i;
            }

            if (strcmp(novo, nome[i]) == 0) {

                printf(" Ja existe um diretorio neste endereco com o mesmo nome.\n");
                return;
            }
        }
    }

    if (positivo == 0) {

        printf(" O diretorio nao existe.\n");
        return;
    }

    if (positivo == 1) {

        strcpy(nome[local], novo);
    }
}

void pwd() {

    int posicao = posicao_atual;
    int vetor[1024];
    int i = 0;

    if (posicao == 0){
        printf("%s\n", nome[0]);
        return;
    }

    while (posicao > 0) {

        vetor[i] = posicao;

        posicao = parent[posicao];

        i++;
    }

    i--;

    for (; i >= 0; i--) {

        printf("/%s", nome[vetor[i]]);

    }
    printf("\n");

    return;
}

 void cd(char *parametro) {

    int i = 0;

    if (parametro == NULL) {

        printf(" Digite um caminho valido. ou ' -- ' para voltar ou '..' para ir a pasta raiz.\n");
        return;
    }

    if (strcmp(parametro, "--") == 0) {

        if (posicao_atual == 0) {

            printf("/");
            printf("\n");
        }
        else {

            posicao_atual = parent[posicao_atual];
        }
        return;
    }

    int positivo = 0;

    for (i = 0; i < ENTRADAS_MAX; i++) {

        if (parent[i] == posicao_atual) {

            if (strcmp(nome[i], parametro) == 0) {

                positivo = 1;
                posicao_atual = i;
                break;
            }
        }
    }

    if (positivo == 0) {

            printf(" Diretorio nao encontrado.\n");
    }
    return;
 }

void ls(char *parametro) {

    if (parametro != NULL && strcmp(parametro, "-l") != 0) {

        printf("Digite 'ls' ou 'ls -l' para mostrar os arquivos deste diretorio.\n", parametro);
        return;
    }

    printf("Arquivos...\n");

    int i = 0;

    for (i = 1; i < ENTRADAS_MAX; i++) {

        if (parent[i] == posicao_atual) {

            if (parametro == NULL){

                printf("- %s\n", nome[i]);
            }

            else if (strcmp(parametro, "-l") == 0) {

                printf("- %s %s %s\n", nome[i], data[i], hora[i]);
            }
        }
    }
}

void debug() {

    int i = 0;

    for (i = 0; i < 20; i++) {

        printf(" [%i]: %i - [%i]: '%s' - [%i]: %s - [%i]: %s\n", i, parent[i], i, nome[i], i, data[i], i, hora[i]);
    }
}

int main() {

    int executei_comando = 0;
    char linha_de_comando[TAMANHO_MAX];
    char *comando = "_", *parametro1, *parametro2;

    while (strcmp(comando,"poweroff") != 0) {

        printf("user~# ");
        gets(linha_de_comando);

        comando = strtok(linha_de_comando," ");
        parametro1 = strtok(NULL," ");
        parametro2 = strtok(NULL,"\0");

        data_hora();

        parent[0] = 0;
        strcpy(nome[0], "root/");
        strcpy(data[0], datalocal);
        strcpy(hora[0], horalocal);

        if (strcmp(comando,"mkdir") == 0) {

            mkdir(parametro1);
            executei_comando = 1;
        }

        if (strcmp(comando,"cd") == 0) {

            cd(parametro1);
            executei_comando= 1;
        }

        if (strcmp(comando,"pwd") == 0) {

            pwd();
            executei_comando = 1;
        }

        if ( strcmp(comando,"ren") == 0 ) {

            ren(parametro1, parametro2);
            executei_comando = 1;
        }

        if ( strcmp(comando,"rmdir") == 0 ) {

            rmdir(parametro1);
            executei_comando = 1;
        }

        if (strcmp(comando,"ls") == 0) {

            ls(parametro1);
            executei_comando =1;
        }

        if(strcmp(comando, "time") == 0) {

            data_hora();

            printf(" %s\n %s\n", datalocal, horalocal);
            executei_comando=1;

        }

        if (strcmp(comando, "debug" ) == 0) {

            debug();
            executei_comando=1;
        }

        if (strcmp(comando, "clear" ) == 0) {

            system("cls");
            executei_comando=1;
        }

        if (strcmp(comando, "help") == 0) {

            printf(" pwd - Exibe a posicao na estrutura de diretorios.");
            printf("\n mkdir - Cria um diretorio.");
            printf("\n cd - Chama o diretorio.");
            printf("\n ren - Renomeia um diretorio.");
            printf("\n copyright - Exibe o nome dos autores do programa.");
            printf("\n poweroff - Sai do terminal.");
            printf("\n time - Exbe data e horas atuais.");
            printf("\n clear - Limpa a tela.\n");
            executei_comando = 1;
        }

        if (strcmp(comando,"copyright") == 0) {

            printf("\n -Isaque Torres\n -Joao Aguirra\n -Mateus Santos\n -Rodrigo Ventura\n\n Copyright 2019-2019 - Sistemas operacionais I\n\n ");
            executei_comando = 1;

        }

        if (strcmp(comando,"poweroff") == 0) {

            printf("Encerrando a aplicacao.\n");
            executei_comando = 1;
        }

        if (executei_comando != 0) {

            executei_comando = 0;
        }
        else
        {
            printf("Comando ou nome de arquivo invalido.\n");
        }
    }
}

int setting() {

    int i = 0;

    for (i = 0; i < 1024; i++) {

        parent[i] = -1;

        strcpy(nome[i], "");
        strcpy(data[i], "");
        strcpy(hora[i], "");
    }

    system("clear");
    main();

    return 0;
}
