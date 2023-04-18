#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct{
  int codigo_filme, ano, disponivel;
  char titulo[30], categoria[30];
  float valor;
}Filmes;

void menu_filmes(FILE *arquivo, int *registro_filmes);
void imprimir_filmes(FILE *arquivo_filmes);
void atualizar_registro_filmes(FILE *arquivo, int *registro_filmes);