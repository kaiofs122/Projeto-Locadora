#include "locacao.h"
#include "filme.h"

// Procura a posicao vazia a ser inserido a locacao, senão será inserido no fim do arquivo //
int posicao_inserir_locacao(FILE *arquivo_locacao){
  int posicao = 0;
  Locacao locacao;
  rewind(arquivo_locacao);
  while(1){
    fread(&locacao, sizeof(Locacao), 1, arquivo_locacao);
    if(feof(arquivo_locacao))
      break;
    if(locacao.codigo_locacao == 0)
      break;
    posicao++;
  }
  return(posicao);
}

// Atualiza a quantidade de locacoes no arquivo //
void atualizar_registro_locacao(FILE *arquivo_locacao, int *registro_locacao){
  Locacao locacao;
  rewind(arquivo_locacao);
  while(1){
    fread(&locacao, sizeof(Locacao), 1, arquivo_locacao);
    if(feof(arquivo_locacao))
      break;
    if(locacao.codigo_locacao != 0)
      (*registro_locacao)++;
  }
}

// Faz a contagem de filmes disponiveis no arquivo //
int filmes_disponiveis(FILE *arquivo_filme){
  Filmes filmes;
  int disponiveis = 0;
  rewind(arquivo_filme);
  while(1){
    fread(&filmes, sizeof(Filmes), 1, arquivo_filme);
    if(feof(arquivo_filme))
      break;
    if(filmes.codigo_filme != 0 && filmes.disponivel != 0)
      disponiveis++;
  }
  return disponiveis;
}

// Adiciona uma nova locacao ao arquivo //
void adicionar_locacao(FILE *arquivo_locacao, FILE *arquivo_filme, int *registro_locacao){
  Locacao locacao = {0};
  Filmes filmes = {0};
  int opcao, qtd_filmes = 0, confirmar = 0;
  
  int posicao = posicao_inserir_locacao(arquivo_locacao);
  locacao.codigo_locacao = posicao + 1;
  
  int disponiveis = filmes_disponiveis(arquivo_filme);
  if(disponiveis > 0){
    ler_dados_usuario(&locacao);
    rewind(arquivo_filme);
    printf("\nFilmes disponiveis para locacao: \n");
    while(1){
      fread(&filmes, sizeof(Filmes), 1, arquivo_filme);
      if(feof(arquivo_filme))
        break;
      if(filmes.codigo_filme != 0 && filmes.disponivel != 0){
        printf("\n\tCodigo: %d", filmes.codigo_filme);
        printf("\n\tTitulo do filme: %s\n", filmes.titulo);
        printf("\tCategoria do filme: %s\n", filmes.categoria);
        printf("\tAno de lancamento: %d\n", filmes.ano);
        printf("\tValor R$ %.2f\n", filmes.valor);
      }
      if(filmes.disponivel != 0 && filmes.codigo_filme != 0){
        printf("\nDigite 1 para escolher o filme ou \"0\" para ver o proximo: ");
        scanf("%d", &confirmar);
      }
      
      if(confirmar != 0 && filmes.codigo_filme != 0){
        filmes.disponivel = 0;
        locacao.filmes_locados[qtd_filmes] = filmes.codigo_filme;
        locacao.valor += filmes.valor;
        qtd_filmes++;
        
        fseek(arquivo_filme, (filmes.codigo_filme - 1) * sizeof(Filmes), SEEK_SET);
        if(fwrite(&filmes, sizeof(Filmes), 1, arquivo_filme))
          printf("\nFilme atualizado com sucesso.\n");
        else
          printf("\nERRO: falha ao atualizar filme.\n");
      
      }
    }
    printf("Data do aluguel (dd/mm/aaaa): ");
    scanf(" %[^\n]", locacao.data_aluguel);
    printf("Data da entrega (dd/mm/aaaa): ");
    scanf(" %[^\n]", locacao.data_entrega);
    locacao.qtd_locados = qtd_filmes;
  
    rewind(arquivo_locacao);
    fseek(arquivo_locacao, posicao * sizeof(Locacao), SEEK_SET);
    
    if(fwrite(&locacao, sizeof(Locacao), 1, arquivo_locacao))
      printf("\nLocação realizada com sucesso.\n");
    else
      printf("\nERRO: Ao realizar locacao.\n");
    rewind(arquivo_locacao);
    (*registro_locacao)++;
  }else{
    printf("\nNao ha filmes disponiveis para a locacao, volte mais tarde...!\n");
  }
}

// Mostra os dados da locacao lidos do arquivo //
void imprimir_dados_locacao(Locacao locacao, FILE *arquivo_filme){
  Filmes filmes;
  rewind(arquivo_filme);
  printf("\nDados da locacao: ");
  printf("\n\tCodigo da locacao: %d\n", locacao.codigo_locacao);
  printf("\tNome do usuario: %s\n", locacao.usuario.nome);
  printf("\tCPF do usuario: %s\n", locacao.usuario.cpf);
  printf("\tData do aluguel: %s\n", locacao.data_aluguel);
  printf("\tData da entrega: %s\n", locacao.data_entrega);
  printf("\n\tFilmes locados: \n");
  while(1){
    fread(&filmes, sizeof(Filmes), 1, arquivo_filme);
    if(feof(arquivo_filme))
      break;
    if(filmes.codigo_filme != 0){
      for(int i = 0; i < locacao.qtd_locados; i++){
        if(filmes.codigo_filme == locacao.filmes_locados[i]){
          printf("\t\tCodigo do filme: %d\n", locacao.filmes_locados[i]);
          printf("\t\tTitulo do filme: %s\n", filmes.titulo);
          printf("\t\tCategoria do filme: %s\n", filmes.categoria);
          printf("\t\tAno: %d", filmes.ano);
          printf("\tValor R$ %.2f\n", filmes.valor);
          printf("\n\t\t------------------------\n");
        }
      }
    }
  }
  printf("\tValor da locacao: %.2f\n", locacao.valor); 
}

// Lista todas as locacoes cadastrados //
void listar_locacao(FILE *arquivo_locacao, FILE *arquivo_filme){
  Locacao locacao;
  rewind(arquivo_locacao);
  while(1){
    fread(&locacao, sizeof(Locacao), 1, arquivo_locacao);
    if(feof(arquivo_locacao))
      break;
    if(locacao.codigo_locacao != 0)
      imprimir_dados_locacao(locacao, arquivo_filme);   
  } 
}

// Exclui uma locacao cadastrada no arquivo //
void excluir_locacao(FILE *arquivo_locacao, int *registro_locacao){
  Locacao locacao, excluir_locacao = {0};
  int codigo_excluir;
  char excluir; 
  printf("Digite o codigo da locacao a ser excluida: ");
  scanf("%d", &codigo_excluir);
  fseek(arquivo_locacao, (codigo_excluir - 1) * sizeof(Locacao), SEEK_SET);
  fread(&locacao, sizeof(Locacao), 1, arquivo_locacao);
  
  printf("\n\tCodigo da locacao: %d\n", locacao.codigo_locacao);
  printf("\tNome do usuario: %s\n", locacao.usuario.nome);
  printf("\tCPF do usuario: %s\n", locacao.usuario.cpf);
  printf("\tData do aluguel: %s\n", locacao.data_aluguel);
  printf("\tData da entrega: %s\n", locacao.data_entrega);
  printf("\tValor da locacao: R$ %.2f\n", locacao.valor);  

  printf("\nDeseja excluir a locacao [S/N]: ");
  scanf(" %c", &excluir);
  if(toupper(excluir) == 'S'){
    rewind(arquivo_locacao);
    fseek(arquivo_locacao, (codigo_excluir - 1) * sizeof(Locacao), SEEK_SET);
  }else{
    printf("\nExclusao finalizada.\n");
    return;
  }
  if(fwrite(&excluir_locacao, sizeof(Locacao), 1, arquivo_locacao))
    printf("\nLocacao excluida com sucesso.\n");
  else
    printf("\nERRO: falha ao excluir locacao.\n");
  (*registro_locacao)--;
}

// Consulta locação pelo seu código //
void consulta_codigo(FILE *arquivo_locacao, FILE *arquivo_filme){
  Locacao locacao;
  int codigo, tinha_locacao = 0;
  rewind(arquivo_locacao);

  printf("Entre com o código da locacao: ");
  scanf("%d", &codigo);
  
  while(1){
    fread(&locacao, sizeof(Locacao), 1, arquivo_locacao);
    if(feof(arquivo_locacao))
      break;
    if(locacao.codigo_locacao == codigo){
      imprimir_dados_locacao(locacao, arquivo_filme);
      tinha_locacao = 1;
      break;
    }
  }
  if(tinha_locacao == 0)
    printf("\nCódigo do filme não encontrado!\n");
}

// Consulta locacao pelo nome do filme //
void consulta_nome_filme(FILE *arquivo_locacao, FILE *arquivo_filme){
  Filmes filmes;
  Locacao locacao;
  char nome_filme[30];
  int tinha_filme = 0, tinha_locacao = 0;

  rewind(arquivo_filme);
  printf("Informe o nome do filme para buscar a locacao: ");
  scanf(" %[^\n]", nome_filme);
  while(1){
    fread(&filmes, sizeof(Filmes), 1, arquivo_filme);
    if(feof(arquivo_filme))
      break;
    if(filmes.codigo_filme != 0){
      if(strstr(filmes.titulo, nome_filme)){
        tinha_filme = 1;
        break;
      }
    }
  }
  rewind(arquivo_locacao);
  if(tinha_filme == 1){
    while(1){
      fread(&locacao, sizeof(Locacao), 1, arquivo_locacao);
      if(feof(arquivo_locacao))
        break;     
      if(locacao.codigo_locacao != 0){
        for(int i = 0; i < locacao.qtd_locados; i++){
          if(filmes.codigo_filme == locacao.filmes_locados[i]){
            imprimir_dados_locacao(locacao, arquivo_filme);
            tinha_locacao = 1;
            break;
          }
        }
      }
    }
  }
  if(tinha_locacao == 0)
    printf("\nFilme nao encontrado");
  
  rewind(arquivo_locacao);
  rewind(arquivo_filme);
}

// Consulta locacao por nome de usuario //
void consulta_nome_usuario(FILE *arquivo_locacao, FILE *arquivo_filme){
  Locacao locacao;
  char nome_usuario[30];
  int tinha_usuario = 0;
  rewind(arquivo_locacao);

  printf("Entre com o nome do Usuário: ");
  scanf(" %[^\n]", nome_usuario);

  while(1){
    fread(&locacao, sizeof(Locacao), 1, arquivo_locacao);
    if(feof(arquivo_locacao))
      break;
    if(strstr(locacao.usuario.nome, nome_usuario)){
      tinha_usuario = 1;
      break;
    }
  }
  if(tinha_usuario == 1)
    imprimir_dados_locacao(locacao, arquivo_filme);
  else
    printf("\nNome de usuario nao encontrado.\n");
  rewind(arquivo_locacao);
}

// Menu para a consulta de dados no arquivo locacao //
void menu_consulta_locacao(FILE *arquivo_locacao, FILE *arquivo_filme){
  int opcao;
  do{
    printf("\nEscolha a opção para consulta locacao: \n");
    printf("\tNome do filme(1) | Código de locação(2) | \n");
    printf("\tNome do usuário(3) | Sair(0): ");
    scanf("%d", &opcao);
    
    switch(opcao){
      case 0:
        printf("\nSaindo do menu...!\n");
      break;
      case 1:
        system("clear");
        consulta_nome_filme(arquivo_locacao, arquivo_filme);
      break;
      case 2:
        system("clear");
        consulta_codigo(arquivo_locacao, arquivo_filme);
      break;
      case 3:
        system("clear");
        consulta_nome_usuario(arquivo_locacao, arquivo_filme);
      break;
      default:
        printf("Opcao invalida, tente novamente.\n");
    }
  }while(opcao != 0);
}

// Edita filme locado pelo usuario //
void editar_filme_locacao(Locacao *locacao, FILE *arquivo_filme){
  int codigo_editar, tinha_codigo = 0, editar_filme;
  imprimir_dados_locacao((*locacao), arquivo_filme);
  printf("\nInforme o codigo do filme a ser editado: ");
  scanf("%d", &codigo_editar);
  for(int i = 0; i < locacao->qtd_locados; i++){
    if(codigo_editar == locacao->filmes_locados[i]){
      tinha_codigo = 1;
      printf("\n\n");
      imprimir_filmes(arquivo_filme);
      printf("Informe o codigo do novo filme ou \"0\" para sair: ");
      scanf("%d", &editar_filme);
      if(editar_filme != 0)
        locacao->filmes_locados[i] = editar_filme;
      else
        break;
    }
  }
  if(tinha_codigo == 0)
    printf("\nCodigo inexistente.\n");
}

// Edita dados especificos da struct locacao //
void editar_locacao_partes(Locacao *locacao, FILE *arquivo_filme){
  int opcao;
  do{
    printf("\nEscolha a opcao: \n");
    printf("\tEditar Usuario (1) | Editar Filme (2) | \n");
    printf("\tEditar Locacao (3) | Sair (0): ");
    scanf("%d", &opcao);
    switch(opcao){
      case 0:
        printf("\nSaindo do menu...!\n");
      break;
      case 1:
        system("clear");
        editar_usuario(locacao);
      break;
      case 2:
        system("clear");
        editar_filme_locacao(locacao, arquivo_filme);
      break;
      case 3:
        printf("\tEditar dados da locacao:\n");
        printf("\tData do aluguel (dd/mm/aaaa): ");
        scanf(" %[^\n]", locacao->data_aluguel);
        printf("\tData da entrega (dd/mm/aaaa): ");
        scanf(" %[^\n]", locacao->data_entrega);
        printf("\tValor da locacao: ");
        scanf("%f", &locacao->valor);
      break;
      default:
        printf("\nOpcao invalida, tente novamente.\n");
    }
  }while(opcao != 0);
}

// Editar dados cadastrados no arquivo de locacao //
void editar_locacao(FILE *arquivo_locacao, FILE *arquivo_filme){
  Locacao locacao;
  int codigo_editar;
  char editar;
  printf("Digite o codigo da locacao a ser editada: ");
  scanf("%d", &codigo_editar);
  
  fseek(arquivo_locacao, (codigo_editar - 1) * sizeof(Locacao), SEEK_SET);
  fread(&locacao, sizeof(Locacao), 1, arquivo_locacao);
  
  printf("\n\tCodigo da locacao: %d\n", locacao.codigo_locacao);
imprimir_dados_locacao(locacao, arquivo_filme);
  
  printf("\nDeseja editar dados da locacao[S/N]: ");
  scanf(" %c", &editar);
  if(toupper(editar) == 'S'){
    editar_locacao_partes(&locacao, arquivo_filme);
    rewind(arquivo_locacao);
    fseek(arquivo_locacao, (codigo_editar - 1) * sizeof(Locacao), SEEK_SET);
  }else{
    printf("\nEdicao finalizada.\n");
    return;
  }
  
  if(fwrite(&locacao, sizeof(Locacao), 1, arquivo_locacao))
    printf("\nLocacao editada com sucesso.\n");
  else
    printf("\nERRO: falha ao editar locacao.\n");
  
  rewind(arquivo_locacao);
}

// Busca o usuario com maior numero de filmes locados //
void usuario_com_mais_filmes(FILE *arquivo_locacao, FILE *arquivo_filme){
  rewind(arquivo_locacao);
  Locacao locacao, usuario_filmes = {0};
  while(1){
    fread(&locacao, sizeof(Locacao), 1, arquivo_locacao);
    if(locacao.qtd_locados > usuario_filmes.qtd_locados){
      usuario_filmes = locacao;
    }
    if(feof(arquivo_locacao))
      break;
  }
  printf("\nUSUARIO COM MAIOR QUANTIDADE DE FILMES LOCADOS: \n");
  printf("\nNome do usuario: %s\n", usuario_filmes.usuario.nome);
  imprimir_dados_locacao(usuario_filmes, arquivo_filme);
}

// Busca o filme de maior valor //
void filme_maior_valor(FILE *arquivo_filme){
  Filmes filmes, filme_maior_valor = {0};
  rewind(arquivo_filme);
  while(1){
    fread(&filmes, sizeof(Filmes), 1, arquivo_filme);
    if(feof(arquivo_filme))
      break;
    if(filmes.codigo_filme != 0){
      if(filmes.valor > filme_maior_valor.valor){
        filme_maior_valor = filmes;
      }
    }
  }
  printf("\nO FILME DE MAIOR VALOR:\n");
  printf("\n\tTitulo do filme: %s\n", filme_maior_valor.titulo);
  printf("\tCategoria do filme: %s\n", filme_maior_valor.categoria);
  printf("\tAno de lancamento: %d\n", filme_maior_valor.ano);
  printf("\tValor: %.2f\n", filme_maior_valor.valor);
  printf("\tValor R$ %.2f\n", filme_maior_valor.valor);
} 