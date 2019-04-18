#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define ENTRADAS_MAX 1024
#define TAMANHO_MAX 80
#define TAMANHO_NOME 80
#define DEBUG 0

int parent[ENTRADAS_MAX];
char nome[ENTRADAS_MAX ][TAMANHO_NOME];
char data[ENTRADAS_MAX][10];
char hora[ENTRADAS_MAX][10];

int posicao_atual = 0;
int codigo = 0;

char datalocal[15];
char horalocal[15];

void data_hora() {

    time_t atual = time(NULL);
    struct tm  data_atual = *localtime(&atual);

    sprintf(horalocal, "%d:%d:%d", data_atual.tm_hour, data_atual.tm_min, data_atual.tm_sec);
    sprintf(datalocal, "%d/%d/%d", data_atual.tm_mday, data_atual.tm_mon + 1, data_atual.tm_year + 1900);
}

int setting() {

    int i = 0;

    for (i = 0; i < 1024; i++) {

        parent[i] = -1;

        strcpy(nome[i], "");
        strcpy(data[i], "");
        strcpy(hora[i], "");
    }

    data_hora();

    parent[0] = 0;
    strcpy(nome[0], "/");
    strcpy(data[0], datalocal);
    strcpy(hora[0], horalocal);

    main();

    return 0;
}

void mkdir(char *nomedir) {

    int i = 0;
    int existe = 0;

    if (nomedir == NULL) {

        printf(" Coloque um nome.\n");
        return;
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

void pwd() {

    char caminho[TAMANHO_MAX];

    strcat(caminho, "/");

    int posicao = 0;

    posicao = posicao_atual;

    while (posicao > 0) {

        strcat(caminho, nome[posicao]);
        strcat(caminho, "/");

        posicao = parent[posicao];
    }

    printf(" %s ", caminho);

    printf("\n");
}

 void cd(char *parametro) {

    int i = 0;

    if (parametro == NULL) {

        printf(" Digite um caminho valido. ou ' -- ' para voltar a raiz.\n");
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

void ls(char *parametro, char *parametro2) {

    int i = 0;

    if (parametro != NULL && strcmp(parametro2, "-l") != 0) {

        printf(" Parametro invalido.\n");
        return;
    }

    printf(" Arquivos...\n");

    for (i = 1; i < ENTRADAS_MAX; i++) {

        if (parent[i] == posicao_atual) {

            if (parametro == NULL) {

                printf("- %s\n", nome[i]);
            }
            else if (strcmp(parametro, "-l") == 0) {

                printf("-> %s %s '%s'\n", datalocal[i], horalocal[i], nome[i]);
                return;
            }
        }
    }
}

debug() {

    int i = 0;

    for (i = 0; i < 10; i++) {

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

            printf(" Trocar o nome do diretorio de %s para %s .\n", parametro1, parametro2);
            executei_comando = 1;
        }

        if (strcmp(comando,"ls") == 0) {

            ls(parametro1, parametro2);
            executei_comando =1;
        }

        if(strcmp(comando, "cp") == 0) {

            printf(" Copiar arquivo de %s para %s.\n", parametro1,parametro2);
            executei_comando=1;

        }

        if (strcmp(comando, "debug" ) == 0) {

            debug();
            executei_comando=1;
        }

        if (strcmp(comando, "clear" ) == 0) {

            printf(" Limpando a tela.\n");
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
            printf("\n cp - Copia um arquivo de um diretório para outro.");
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
