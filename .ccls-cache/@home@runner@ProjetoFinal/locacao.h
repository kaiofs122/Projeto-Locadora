#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TAM 20

typedef struct{
  char rua[30], bairro[30], cidade[30], estado[30];
  int numero_casa;
}Endereco;

typedef struct{
  char cpf[30];
  char nome[30], telefone[30];
  Endereco endereco;
}Usuario;

typedef struct{
  int codigo_locacao;
  char data_aluguel[15], data_entrega[15];
  float valor;
  int filmes_locados[TAM];
  int qtd_locados;
  Usuario usuario;
}Locacao;

void atualizar_registro_locacao(FILE *arquivo_locacao, int *registro_locacao);
void adicionar_locacao(FILE *arquivo_locacao, FILE *arquivo_filme, int *registro_locacao);
void listar_locacao(FILE *arquivo_locacao, FILE *arquivo_filme);
void excluir_locacao(FILE *arquivo_locacao, int *registro_locacao);
void editar_locacao(FILE *arquivo_locacao, FILE *arquivo_filme);
void menu_consulta_locacao(FILE *arquivo_locacao, FILE *arquivo_filme);
void usuario_com_mais_filmes(FILE *arquivo_locacao, FILE *arquivo_filme);
void filme_maior_valor(FILE *arquivo_filme);

// Funcões no usuario.c //
void ler_dados_usuario(Locacao *locacao);
void editar_usuario(Locacao *locacao);