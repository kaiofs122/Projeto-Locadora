#include "filme.h"

// Procura a posicao vazia a ser inserido o filme, senão será inserido no fim do arquivo //
int posicao_inserir_filme(FILE *arquivo_filme){
  int posicao = 0;
  Filmes filmes;
  rewind(arquivo_filme);
  while(1){
    fread(&filmes, sizeof(Filmes), 1, arquivo_filme);
    if(feof(arquivo_filme))
      break;
    if(filmes.codigo_filme == 0)
      break;
    posicao++;
  }
  return(posicao);
}

// Atualiza a quantidade de filmes no arquivo //
void atualizar_registro_filmes(FILE *arquivo_filme, int *registro_filmes){
  Filmes filmes;
  rewind(arquivo_filme);
  while(1){
    fread(&filmes, sizeof(Filmes), 1, arquivo_filme);
    if(feof(arquivo_filme))
      break;
    if(filmes.codigo_filme != 0)
      (*registro_filmes)++;
  }
}

// Ler os dados do filme //
void ler_dados(Filmes *filmes){
  printf("\nInsira dos dados: \n");
  printf("\tInforme o titulo do filme: ");
  scanf(" %[^\n]", filmes->titulo);
  printf("\tCategoria do filme: ");
  scanf(" %[^\n]", filmes->categoria);
  printf("\tAno de lancamento: ");
  scanf("%d", &filmes->ano);
  printf("\tInforme o valor do filme: ");
  scanf("%f", &filmes->valor);
}

// Adiciona um novo filme ao arquivo //
void adicionar_filme(FILE *arquivo_filme, int *registro_filmes){
  Filmes filmes;
  int posicao = posicao_inserir_filme(arquivo_filme);
  filmes.codigo_filme = posicao + 1;
  filmes.disponivel = 1;
  ler_dados(&filmes);
  rewind(arquivo_filme);
  fseek(arquivo_filme, posicao * sizeof(Filmes), SEEK_SET);
  if(fwrite(&filmes, sizeof(Filmes), 1, arquivo_filme))
    printf("\nFilme inserido com sucesso.\n");
  else
    printf("\nERRO: falha ao inserir filme.\n");
  (*registro_filmes)++;
  rewind(arquivo_filme);
}

// Edita por partes dados da struct filmes //
void editar_por_partes(Filmes *filmes){
  int editar;
  do{
    printf("Editar: Sair(0) | titulo(1) | categoria(2) | Ano(3) | Valor(4) | Disponivel (5): ");
    scanf("%d", &editar);
    switch(editar){
      case 0:
        printf("\nEdicao finalizada.\n");
      break;
      case 1:
        printf("\nInforme o titulo do filme: ");
        scanf(" %[^\n]", filmes->titulo);
      break;
      case 2:
          printf("\nCategoria do filme: ");
          scanf(" %[^\n]", filmes->categoria);
      break;
      case 3:
        printf("\nAno de lancamento: ");
        scanf("%d", &filmes->ano);
      break;
      case 4:
        printf("\nValor: ");
        scanf("%f", &filmes->valor);
      break;
      case 5:
        printf("\nFilme disponivel: 1 - disponivel ou 0 - indisponivel: ");
        scanf("%d", &filmes->disponivel);
      break;
      default:
        printf("\nCodigo invalido.\n");
    }
  }while(editar != 0);
}

// Edita um filme já inserido no arquivo //
void editar_filme(FILE *arquivo_filme){
  Filmes filmes;
  int codigo_editar;
  char editar;
  printf("Digite o codigo do filme a ser editado: ");
  scanf("%d", &codigo_editar);
  
  fseek(arquivo_filme, (codigo_editar - 1) * sizeof(Filmes), SEEK_SET);
  fread(&filmes, sizeof(Filmes), 1, arquivo_filme);
  
  printf("\n\tTitulo do filme: %s\n", filmes.titulo);
  printf("\tCategoria do filme: %s\n", filmes.categoria);
  printf("\tAno de lancamento: %d\n", filmes.ano);
  printf("\tValor R$ %.2f\n", filmes.valor);
  if(filmes.disponivel == 1)
    printf("\tFilme disponivel.\n");
  else
    printf("\tFilme indisponivel.\n");

  printf("\nDeseja editar o filme [S/N]: ");
  scanf(" %c", &editar);
  if(toupper(editar) == 'S'){
    editar_por_partes(&filmes);
    rewind(arquivo_filme);
    fseek(arquivo_filme, (codigo_editar - 1) * sizeof(Filmes), SEEK_SET);
  }else{
    printf("\nEdicao finalizada.\n");
    return;
  }
  
  if(fwrite(&filmes, sizeof(Filmes), 1, arquivo_filme))
    printf("\nFilme editado com sucesso.\n");
  else
    printf("\nERRO: falha ao editar filme.\n");
  
  rewind(arquivo_filme);
}

// Exclui um filme cadastrado no arquivo//
void excluir_filme(FILE *arquivo_filme, int *registro_filmes){
  Filmes filmes, excluir_filme = {0};
  int codigo_excluir;
  char excluir;
  
  printf("\nDigite o codigo do filme a ser excluido: ");
  scanf("%d", &codigo_excluir);
  fseek(arquivo_filme, (codigo_excluir - 1) * sizeof(Filmes), SEEK_SET);
  fread(&filmes, sizeof(Filmes), 1, arquivo_filme);
  
  printf("\n\tTitulo do filme: %s\n", filmes.titulo);
  printf("\tCategoria do filme: %s\n", filmes.categoria);
  printf("\tAno de lancamento: %d\n", filmes.ano);
  printf("\tValor R$ %.2f\n", filmes.valor);
  if(filmes.disponivel == 1)
    printf("\tFilme disponivel.\n");
  else
    printf("\tFilme indisponivel.\n");
  
  printf("\nDeseja excluir o filme [S/N]: ");
  scanf(" %c", &excluir);
  if(toupper(excluir) == 'S'){
    rewind(arquivo_filme);
    fseek(arquivo_filme, (codigo_excluir - 1) * sizeof(Filmes), SEEK_SET);
  }else{
    printf("\nEdicao finalizada.\n");
    return;
  }
  
  if(fwrite(&excluir_filme, sizeof(Filmes), 1, arquivo_filme))
    printf("\nFilme excluido com sucesso.\n");
  else
    printf("\nERRO: falha ao excluir filme.\n");
  
  (*registro_filmes)--;
}

// Imprimi filmes no arquivo //
void imprimir_filmes(FILE *arquivo_filme){
  printf("Filmes cadastrados: \n");
  Filmes filmes;
  rewind(arquivo_filme);
  while(1){
    fread(&filmes, sizeof(Filmes), 1, arquivo_filme);
    if(feof(arquivo_filme))
      break;
    if(filmes.codigo_filme != 0){
      printf("\n\tCodigo do filme: %d\n", filmes.codigo_filme);
      printf("\tTitulo do filme: %s\n", filmes.titulo);
      printf("\tCategoria do filme: %s\n", filmes.categoria);
      printf("\tAno de lancamento: %d\n", filmes.ano);
      printf("\tValor R$ %.2f\n", filmes.valor);
      if(filmes.disponivel == 1)
        printf("\tFilme disponivel.\n");
      else
        printf("\tFilme indisponivel.\n");
    }
  }
}

// Menu com as operações para gerenciar os filmes // 
void menu_filmes(FILE *arquivo_filme, int *registro_filmes){
  int opcao;
  do{
    printf("\nEscolha a opção:\n");
    printf("\nInserir Filme(1) | Imprimir Filmes(2) \n");
    printf("Editar Filme(3) | Excluir Filme(4) | Sair(0): ");
    scanf("%d", &opcao);
    
    switch(opcao){
      case 0:
        printf("\nSaindo do menu.....!\n");
      break;
      case 1:
        system("clear");
        adicionar_filme(arquivo_filme, registro_filmes);
      break;
      case 2:
        system("clear");
        imprimir_filmes(arquivo_filme);
      break;
      case 3:
        system("clear");
        editar_filme(arquivo_filme);
      break;
      case 4:
        system("clear");
        excluir_filme(arquivo_filme, registro_filmes);
      break;
      default:
        printf("\nCodigo invalido.\n");
    }
  }while(opcao != 0);
}