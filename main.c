#include "filme.h"
#include "locacao.h"

int main() {
  
  int opcao;
  int registro_locacao = 0, registro_filme = 0;
  FILE *arquivo_filme, *arquivo_locacao;

  arquivo_locacao = fopen("arquivo_locacao.bin", "r+b");
  if(arquivo_locacao == NULL){
    arquivo_locacao = fopen("arquivo_locacao.bin", "w+b");
    if(arquivo_locacao == NULL){
      printf("ERRO: Ao criar arquivo de locacao.\n");
      exit(1);
    }
  }
  arquivo_filme = fopen("arquivo_filme.bin", "r+b");
  if(arquivo_filme == NULL){
    arquivo_filme = fopen("arquivo_filme.bin", "w+b");
    if(arquivo_filme == NULL){
      printf("ERRO: Ao criar arquivo de filme.\n");
      exit(1);
    }
  }else{
    
    atualizar_registro_locacao(arquivo_locacao, &registro_locacao);
    atualizar_registro_filmes(arquivo_filme, &registro_filme);
    
    printf("Locacoes cadastradas: %d\n", registro_locacao);
    printf("Filmes cadastrados: %d\n", registro_filme);
    
    do{
      printf("\nEscolha uma opção: \n");
      printf("\tLocar Filme(1) | Excluir Locacao(2) | Editar Dados da Locacao(3) | \n");
      printf("\tListar Locacoes(4) | Consultar Locacoes(5) | Menu Filme (6) |\n");
      printf("\tUsuario Com Mais Filmes Locados (7) | Filme de maior valor (8) | Sair(0): ");
      scanf("%d", &opcao);
      
      switch(opcao){
        case 0:
          printf("\nFinalizando Programa...!\n");
        break;
        case 1:
          system("clear");
          adicionar_locacao(arquivo_locacao, arquivo_filme, &registro_locacao);
        break;
        case 2:
          system("clear");
          excluir_locacao(arquivo_locacao, &registro_locacao);
        break;
        case 3:
          system("clear");
          editar_locacao(arquivo_locacao, arquivo_filme);
        break;
        case 4:
          system("clear");
          listar_locacao(arquivo_locacao, arquivo_filme);
        break;
        case 5:
          system("clear");
          menu_consulta_locacao(arquivo_locacao, arquivo_filme);
        break;
        case 6:
          system("clear");
          menu_filmes(arquivo_filme, &registro_filme);
        break;
        case 7:
          system("clear");
          usuario_com_mais_filmes(arquivo_locacao, arquivo_filme);
        break;
        case 8:
          system("clear");
          filme_maior_valor(arquivo_filme);
        break;
        default:
          printf("\nCodigo invalido, tente novamente.\n");
      }
    }while(opcao != 0);
  } 
  
  fclose(arquivo_locacao);
  fclose(arquivo_filme);
  
  return 0;
}