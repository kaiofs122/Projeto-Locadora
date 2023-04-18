#include "locacao.h"

// Ler dados do usuario //
void ler_dados_usuario(Locacao *locacao){
  system("clear");
  printf("\nInsira dos dados do cliente: \n");
  printf("\tInsira seu nome: ");
  scanf(" %[^\n]", locacao->usuario.nome);
  printf("\tInsira seu numero de telefone: ");
  scanf(" %[^\n]", locacao->usuario.telefone);
  printf("\tInsira o nome da sua rua: ");
  scanf(" %[^\n]", locacao->usuario.endereco.rua);
  printf("\tInsira o numero da sua casa: ");
  scanf("%d", &locacao->usuario.endereco.numero_casa);
  printf("\tInsira o nome do seu bairro: ");
  scanf(" %[^\n]", locacao->usuario.endereco.bairro);
  printf("\tInsira o nome da sua cidade: ");
  scanf(" %[^\n]", locacao->usuario.endereco.cidade);
  printf("\tInsira o nome do seu estado: ");
  scanf(" %[^\n]", locacao->usuario.endereco.estado);
}

// Edita dados do usuario no arquivo //
void editar_usuario(Locacao *locacao){
  int opcao;
  char editar;
  printf("\n\tCodigo: %d\n", locacao->usuario.codigo);
  printf("\tNome: %s\n", locacao->usuario.nome);
  printf("\tTelefone: %s\n", locacao->usuario.telefone);
  printf("\tRua: %s\n", locacao->usuario.endereco.rua);
  printf("\tNumero da casa: %d\n", locacao->usuario.endereco.numero_casa);
  printf("\tBairro: %s\n", locacao->usuario.endereco.bairro);
  printf("\tCidade: %s\n", locacao->usuario.endereco.cidade);
  printf("\tEstado: %s\n", locacao->usuario.endereco.estado);

  printf("\nDeseja editar esse cliente? [S/N]: ");
  scanf(" %c", &editar);
  if(toupper(editar) == 'S'){
    do{
      printf("\nInforme o codigo para edicao:\n");
      printf("\n\tNome(1) | Telefone(2) | Rua(3) | Numero da casa(4) | ");
      printf("\n\tBairro(5) | Cidade(6) | Estado(7) | Sair(0): ");
      scanf("%d", &opcao);
    
      switch(opcao){
        case 0:
          printf("\nSaindo do menu...!\n");
        break;
        case 1:
          printf("Entre com o novo nome: ");
          scanf(" %[^\n]", locacao->usuario.nome);
        break;
        case 2:
          printf("Entre com o novo telefone: ");
          scanf(" %[^\n]", locacao->usuario.telefone);
          break;
        case 3:
          printf("Entre com a nova tua: ");
          scanf(" %[^\n]", locacao->usuario.endereco.rua);
          break;
        case 4:
          printf("Entre com o novo numero da casa: ");
          scanf("%d", &locacao->usuario.endereco.numero_casa);
          break;
        case 5:
          printf("Entre com o novo bairro: ");
          scanf(" %[^\n]", locacao->usuario.endereco.bairro);
          break;
        case 6:
          printf("Entre com a nova cidade: ");
          scanf(" %[^\n]", locacao->usuario.endereco.cidade);
          break;
        case 7:
          printf("Entre com o novo estado: ");
          scanf(" %[^\n]", locacao->usuario.endereco.estado);
          break;
        default:
          printf("\nCodigo invalido, tente novamente.\n");
      }   
    }while(opcao != 0);
  }
}