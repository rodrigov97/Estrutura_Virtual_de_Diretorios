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
char data[ENTRADAS_MAX][20];
char hora[ENTRADAS_MAX][20];

int posicao_atual = 0;
int codigo = 0;

char datalocal[20];
char horalocal[20];

void data_hora() {

    time_t atualizar_hora;

    atualizar_hora = time(NULL);

    struct tm horario = *localtime(&atualizar_hora);

    sprintf(horalocal, "%d:%d:%d", horario.tm_hour, horario.tm_min, horario.tm_sec);
    sprintf(datalocal, "%d/%d/%d", horario.tm_mday, horario.tm_mon + 1, horario.tm_year + 1900);
}

int content() {

    int executei_comando = 0;
    char linha_de_comando[TAMANHO_MAX];
    char *comando = "_", *parametro1, *parametro2;

    while (strcmp(comando,"poweroff") != 0) {

        printf("user~# ");
        gets(linha_de_comando);

        comando = strtok(linha_de_comando," ");
        parametro1 = strtok(NULL," ");
        parametro2 = strtok(NULL,"\0");

        //data_hora();

        if (strcmp(comando,"mkdir") == 0) {

            executei_comando = 1;

            //Cria um diretório

            int i = 0;
            int existe = 0;

            if (parametro1 == NULL) {

                printf(" Coloque um nome.\n");
            }

            if (strlen(parametro1) > 8) {

                printf(" Maximo de 8 letras no nome.\n");
            }

            for (i = 1; i < ENTRADAS_MAX; i++) {

                if (parent[i] == posicao_atual) {

                    if (strcmp(parametro1, nome[i]) == 0) {

                        printf(" Arquivo ja existe.\n");
                        existe = 1;
                    }
                }
            }

            if (existe == 0) {

                codigo++;
                data_hora();

                parent[codigo] = posicao_atual;

                strcpy(nome[codigo], parametro1);
                strcpy(data[codigo], datalocal);
                strcpy(hora[codigo], horalocal);
            }
        }

        if (strcmp(comando,"cd") == 0) {

            executei_comando = 1;

            //Se move pelos diretorios

            int i = 0;
            int existe = 0;

            if (parametro1 == NULL) {

                printf(" Digite o nome do arquivo ou '--' para subir um nível");
            }

            if (strcmp(parametro1, "--") == 0) {

                if (posicao_atual == 0) {

                    printf("/");
                    printf("\n");
                    existe = 1;
                }
                else {

                    posicao_atual = parent[posicao_atual];
                    existe = 1;
                }
            }

            for (i = 0; i < ENTRADAS_MAX; i++) {

                if (parent[i] == posicao_atual) {

                    if (strcmp(parametro1, nome[i]) == 0) {

                        existe = 1;
                        posicao_atual = i;
                        break;
                    }
                }
            }

            if (existe == 0) {

                printf(" Endereco inexistente.\n");
            }
        }

        if (strcmp(comando,"pwd") == 0) {

            executei_comando = 1;

            //Escrever o caminho

            int posicao = posicao_atual;
            int i = 0;
            int vetor[1024];

            if (posicao == 0) {

                printf("%s/", nome[0]);
            }

            while (posicao > 0) {

                vetor[i] = posicao;
                posicao = parent[posicao];
                i++;
            }

            i--;
            for(; i >= 0; i--) {

                printf("%s/", nome[vetor[i]]);
            }
            printf("\n");
        }

        if ( strcmp(comando,"ren") == 0 ) {

            executei_comando = 1;

            //Renomeia um diretório

            if (parametro1 == NULL || parametro2 == NULL) {

                printf(" Digite: 'Nome a ser trocado' e 'Novo nome'.\n");
            }

            if ((strlen(parametro1) > 8) || (strlen(parametro2) > 8)) {

                printf(" Maximo de 8 caracteres no nome.\n");
            }

            int positivo = 0;
            int i = 0;
            int j = 0;
            int local = 0;

            for (i = 0; i < ENTRADAS_MAX; i++) {

                if (parent[i] == posicao_atual) {

                    if (strcmp(parametro1, nome[i]) == 0) {

                        positivo = 1;
                        local = i;
                    }

                    if (strcmp(parametro2, nome[i]) == 0) {

                        printf(" Ja existe um diretorio neste endereco com o mesmo nome.\n");
                    }
                }
            }

            if (positivo == 0) {

                printf(" O diretorio nao existe.\n");
            }

            if (positivo == 1) {

                strcpy(nome[local], parametro2);
            }
        }

        if ( strcmp(comando,"rmdir") == 0 ) {

            executei_comando = 1;

            //Remove um diretorio

            int i = 0;
            int m = 0;
            int existe = 0;

            if (parametro1 == NULL) {

                printf(" Digite 'rmdir' para remover o diretorio desejado.\n");
            }

            for (i = 1; i < ENTRADAS_MAX; i++) {

                if (parent[i] == posicao_atual) {

                    if (strcmp(nome[i], parametro1) == 0) {

                        existe = 1;
                        break;
                    }
                }
            }

            if (existe == 0) {

                printf(" Arquivo não encontrado.\n");
            }

            for (m = 1; m < ENTRADAS_MAX; m++) {

                if (parent[m] == i) {

                    printf(" Impossivel apagar, existem diretorios dentro dele.\n");
                    existe = 0;
                }
            }

            if (existe == 1) {

                parent[i] = 0;
                strcpy(nome[i], "");
                strcpy(data[i], "");
                strcpy(hora[i], "");
            }
        }

        if (strcmp(comando,"ls") == 0) {

            executei_comando = 1;

            //Lista os arquivos do diretorio atual

            int i = 0;

            if (parametro1 != NULL && strcmp(parametro1, "-l") != 0) {

                printf(" Comando invalido digite 'ls' ou 'ls -l' para listar os arquivos.\n");
            }

            for (i = 1; i < ENTRADAS_MAX; i++) {

                if (parent[i] == posicao_atual) {

                    if (parametro1 == NULL) {

                        printf("-- %s\n", nome[i]);
                    }

                    else if (strcmp(parametro1, "-l") == 0) {

                        printf("-- %s | %s - %s\n", nome[i], data[i], hora[i]);
                    }
                }
            }
        }

        if(strcmp(comando, "time") == 0) {
            
            executei_comando = 1;

            //Mostra a hora atual de acordo com a do sistema

            data_hora();

            printf(" %s\n %s\n", datalocal, horalocal);
        }

        if (strcmp(comando, "debug" ) == 0) {

            executei_comando=1;

            data_hora();

            printf("Posicao: %i | Codigo: %i\n", posicao_atual, codigo);

            int i;

            for (i = 0; i < 20; i++) {

                printf("Parent[%i]: %i - Nome[%i]: %s - Data[%i]: %s - Hora[%i]: %s\n", i, parent[i], i, nome[i], i, data[i], i, hora[i]);
            }
        }

        if (strcmp(comando, "clear" ) == 0) {

            executei_comando=1;

            if (strcmp(parametro1, "win") == 0) {

                system("cls");
            }
            else if (strcmp(parametro1, "lin") == 0) {

                system("clear");
            }
        }

        if (strcmp(comando, "help") == 0) {

            printf(" pwd - Exibe a posicao na estrutura de diretorios.");
            printf("\n mkdir - Cria um diretorio.");
            printf("\n rmdir - Remove um diretorio.");
            printf("\n cd - Chama o diretorio.");
            printf("\n ren - Renomeia um diretorio.");
            printf("\n ls - Lista os arquivos presentes do diretório atual.");
            printf("\n copyright - Exibe o nome dos autores do programa.");
            printf("\n poweroff - Sai do terminal.");
            printf("\n time - Exbe data e horas atuais.");
            printf("\n clear - Limpa a tela.\n");
            executei_comando = 1;
        }

        if (strcmp(comando,"copyright") == 0) {
            
            executei_comando = 1;

            //Printa o nome dos integrantes e do projeto

            printf("\n Projeto0 - Estrutura Virtual de Diretorios.");
            printf("\n -Isaque Torres\n -Joao Aguirra\n -Mateus Santos\n -Rodrigo Ventura\n\n Copyright 2019-2019 - Sistemas operacionais I\n\n ");
        }

        if (strcmp(comando,"poweroff") == 0) {

            executei_comando = 1;

            //Desliga o programa
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

int main() {

    int i = 0;

    for (i = 0; i < 1024; i++) {

        parent[i] = -1;

        strcpy(nome[i], "");
        strcpy(data[i], "");
        strcpy(hora[i], "");
    }

    data_hora();

    parent[0] = 0;

    strcpy(nome[0], "root");
    strcpy(data[0], datalocal);
    strcpy(hora[0], horalocal);
    
    content();

    return;
}
