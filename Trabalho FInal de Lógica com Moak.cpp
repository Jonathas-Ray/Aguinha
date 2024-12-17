#include <stdio.h>
#include <stdlib.h>
//#include <locale.h>
#include <string.h>
#include <time.h>

typedef struct{
	char IDCaixa[50];
	float capMaxima;
	float volume[10];
} Caixas;

int nCaixas;	/*Número de caixas declarado*/

void pauseClean();
void changeCodePageToUTF8();
int getInt();
int getFloat();
void getString(char entry[], int size);
float gerarNumeroAleatorio();
void novosRegistros(Caixas *caixa, int i);
void gerarRelatorio(Caixas *caixa, int i);

void menuPrincipal(){
	printf("-----------------MENU-----------------\n");
	printf("- 1 - Cadastrar Novo Reservatório\n");
	printf("- 2 - Listar Caixas D'água\n");
	printf("- 3 - Relatório de Evasão [Últimos dez dias]\n");
	printf("- 4 - Sair\n");
}

void cadastrarCaixa(Caixas *caixa, int i);

int main(){
	//setlocale(LC_ALL, "Portuguese");
	
	Caixas *caixa /*Ponteiro para o vetor de Caixas*/;
	int execucoesCadastro = 0/*Caixas de fato cadastradas*/, escolha = 0;
	changeCodePageToUTF8();

	do{
		
		menuPrincipal();
		
		switch(getInt()){
							 	
			case 1:	
				if (execucoesCadastro == 0){
					printf("Quantas caixas d'água você possui?\n");
					nCaixas = getInt();
					
					caixa = (Caixas *)malloc(nCaixas * sizeof(Caixas));
					if (caixa == NULL){
						printf("Erro ao alocar memória");
						pauseClean();
						return 0;				
					}
				}
					
				printf("Você possui %d caixas, %d sem cadastrar, quantas deseja cadastrar?\n", nCaixas, nCaixas - execucoesCadastro);
				escolha = getInt();	 					 				 
				
				if (escolha > 0 && (escolha + execucoesCadastro) <= nCaixas){
					escolha += execucoesCadastro;				
					for (int i = execucoesCadastro; i < escolha; i++){
						cadastrarCaixa(caixa, i);		 	
					}
					execucoesCadastro = escolha;				 					
				}	else {
					printf("Quantidade não disponível.\n");
					pauseClean();				 		 								 				
				}
				break;
						 
			case 2:	
				system("cls");
				if (execucoesCadastro == 0){
					printf("Nenhuma Caixa cadastrada.\n");
					pauseClean();
					break;			 			 								 
				}	else {
					for (int i = 0; i < execucoesCadastro; i++){
						printf("Caixa: %s\n", caixa[i].IDCaixa);	 
						printf("Volume máximo da caixa: %.2f\n", caixa[i].capMaxima);
						printf("\\\\----------------------------------------------------------------\n\n//");	 					 				
					}
					printf("Para verificar o volume atual, basta gerar relatório.\n\n");
				break;
				 
			case 3: //Supõe-se que a cada vez escolhido, foi feito em dias diferentes (2 para ser mais preciso);
				if (execucoesCadastro == 0){
				printf("Nenhuma Caixa cadastrada.\n");
				pauseClean();
				break;			 			 								 
			}	else {
				for (int i = 0; i < execucoesCadastro; i++){
					novosRegistros(caixa, i);
				}
		 		
				for (int i = 0; i < execucoesCadastro; i++){
					gerarRelatorio(caixa, i);	
				}	
			}	
				pauseClean();				 
				break;
				 
			case 4:
				return 0;
			 	 
			default:
				printf("Opção não reconhecida.\n");
				pauseClean();
		}
	}
	
	} while (1);

	free(caixa);
}

void gerarRelatorio(Caixas *caixa, int i){
	printf("Caixa de ID: %s\n", caixa[i].IDCaixa);
	
	for (int j = 0; j < 10; j++){
		printf("Volume à %d dias atrás: %.2f/10L \n", 9 - j, caixa[i].volume[j]);
	}
	printf("[]------------------------------------------[]\n\n");
	
}

void cadastrarCaixa(Caixas *caixa, int i){ //Por alguma razão eu tô dando dois pause
	printf("Digite o ID para associarmos à caixa nº %d: ", i + 1);
	getString(caixa[i].IDCaixa, 50);
	printf("Digite o volume máximo que a caixa comporta: \n");
	caixa[i].capMaxima = getFloat();
	for (int j = 0; j < 10; j++){
		caixa[i].volume[j] = 0.0;
	}
	pauseClean();
}

float gerarNumeroAleatorio() {
  static int inicializado = 0;
  if (!inicializado) {
      srand(time(NULL));
      inicializado = 1;
  }
  
  float aleatorio = ((float)rand() / RAND_MAX) * (20 - (-20)) + (-20);
  return aleatorio;
}

void novosRegistros(Caixas *caixa, int i){
	float novoRegistro1 = gerarNumeroAleatorio();
	float novoRegistro2 = gerarNumeroAleatorio();
	
	for (int j = 0; j < 8; j++){
		caixa[i].volume[j] = caixa[i].volume[j + 2];
	}
	
	if (caixa[i].volume[7] + novoRegistro1 < 0){
		caixa[i].volume[8] = 0;
	} else if (caixa[i].volume[7] + novoRegistro1 > caixa[i].capMaxima){
		caixa[i].volume[8] = caixa[i].capMaxima;
	} else {
		caixa[i].volume[8] = caixa[i].volume[7] + novoRegistro1;
	}
	
	if (caixa[i].volume[8] + novoRegistro1 < 0){
		caixa[i].volume[9] = 0;
	} else if (caixa[i].volume[8] + novoRegistro1 > caixa[i].capMaxima){
		caixa[i].volume[9] = caixa[i].capMaxima;
	} else {
		caixa[i].volume[9] = caixa[i].volume[8] + novoRegistro1;
	}
	
}

void pauseClean(){
	printf("\n\n");
	system("pause");
	system("cls");
}

void changeCodePageToUTF8(){
	system("chcp 65001");
	system("cls");
}

void getString(char entry[], int size){
	printf("\nResposta: ");
	fgets(entry, size, stdin);
	entry[strcspn(entry, "\n")] = 0;
	
	printf("\n\nCompreendido: %s", entry);
	pauseClean();
}

int getInt(){
	int entry;
	printf("Escolha: ");
	scanf("%d", &entry);
	while(getchar() != '\n');
	pauseClean();
	
	return entry;
}

int getFloat(){
	float entry;
	printf("Escolha: ");
	scanf("%f", &entry);
	while(getchar() != '\n');
	pauseClean();
	
	return entry;
}