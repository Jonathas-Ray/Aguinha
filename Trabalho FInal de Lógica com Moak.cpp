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

typedef struct{
	int dia, mes, ano;
}Data;

void pauseClean();
void changeCodePageToUTF8();
int getInt();
int getFloat();
void getString(char entry[], int size);
int maxDiasPorMes(Data data);
Data corrigirData (Data data, int mudancaEmDias);
Data getData();
float gerarNumeroAleatorio();
void novosRegistros(Caixas *caixa, int i);
void gerarRelatorio(Caixas *caixa);
int verifyPosition(Caixas *caixa, char ID[]);
void cadastrarCaixa(Caixas *caixa, int i);
void apagarReservatorio(Caixas *caixa);
void editarReservatorio(Caixas *caixa);

int nCaixas = 0;
int execucoesCadastro = 0;
Data data;

void menuPrincipal(){
	printf("-----------------MENU-----------------\n");
	printf("- 1 - Cadastrar Novo Reservatório\n");
	printf("- 2 - Listar Caixas D'água\n");
	printf("- 3 - Relatório de Evasão [Últimos dez dias]\n");
	printf("- 4 - Alterar/Excluir Caixa\n");
	printf("- 5 - Sair\n");
	printf("\n\nData de hoje: %d/%d/%d\n", data.dia, data.mes, data.ano);
}

int main(){
	//setlocale(LC_ALL, "Portuguese");
	
	Caixas *caixa;
	int escolha = 0;
	changeCodePageToUTF8();
	do{
		data = getData();
		if (data.mes > 12 || data.mes <= 0){
			printf("Data inválida\n");
			pauseClean();
		} else if (data.dia <= 0 || data.dia > maxDiasPorMes(data)){
			printf("Data inválida\n");
			pauseClean();
		} else {
			break;
		}
	} while (1);

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
					int inicioCadastro = execucoesCadastro;     //
					for (int i = inicioCadastro; i < escolha; i++){
						cadastrarCaixa(caixa, i);
						execucoesCadastro ++;
					}
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
				pauseClean();
				break;
				 
			case 3: //Supõe-se que a cada vez escolhido, foi feito em dias diferentes;
				if (execucoesCadastro == 0){
				printf("Nenhuma Caixa cadastrada.\n");
				pauseClean();
				break;
						 			 								 
				}	else {
					for (int i = 0; i < execucoesCadastro; i++){ 
						gerarRelatorio(caixa);
					}
					for (int i = 0; i < execucoesCadastro; i++){ //Simula nova entrada para o próximo relatório
						novosRegistros(caixa, i); 						 			
					}									
				}
			 	
				data = corrigirData(data, 1);	//Passa o dia
				pauseClean();				 
				break;
				 
			case 4:
				if (execucoesCadastro == 0){
					printf("Nenhuma Caixa cadastrada.\n");
					pauseClean();
					break;
				} else {
					printf("Digite 1 para alterar a Caixa (ID, Capacidade Máxima), 2 para apagá-la ou outro número para desistir\n");
					switch(getInt()){
						case 1:
						editarReservatorio(caixa);
						break;
						
						case 2:
						apagarReservatorio(caixa);
						break;													
						
						default:
						break;								
					}
					pauseClean();
					break; 	
				}
			 						 			 
			case 5:
				return 0;
			 	 
			default:
				printf("Opção não reconhecida.\n");
				pauseClean();
		}
	}
	
	} while (1);

	free(caixa);
}

int verifyPosition(Caixas *caixa, char ID[]){ 
	int position = -1;
				 
	if (execucoesCadastro == 0){								
		return -1;
	} else {
		for (int i = 0; i < execucoesCadastro; i++){ //itera sobre os cadastrados
			if (strcmp(caixa[i].IDCaixa, ID) == 0){ //encontra o igual
				return i;
			}
		}
	}
	return -1;
}

void cadastrarCaixa(Caixas *caixa, int i){ 
	int verifyDuplicate;
	
	do{
		printf("Digite o ID para associarmos à caixa nº %d: ", i + 1);
		getString(caixa[i].IDCaixa, 50);
		verifyDuplicate = verifyPosition(caixa, caixa[i].IDCaixa);
								
		if (verifyDuplicate == -1){
 			break;
		} else {
			printf("ID já utilizado, escolha outro\n");
			pauseClean();		 		
		}
	} while (1);
	
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
	
	for (int j = 0; j < 9; j++){
		caixa[i].volume[j] = caixa[i].volume[j + 1];
	}
	
	if (caixa[i].volume[8] + novoRegistro1 < 0){
		caixa[i].volume[9] = 0;
	} else if (caixa[i].volume[8] + novoRegistro1 > caixa[i].capMaxima){
		caixa[i].volume[9] = caixa[i].capMaxima;
	} else {
		caixa[i].volume[9] = caixa[i].volume[9] + novoRegistro1;
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

int maxDiasPorMes(Data data){
	int anoComum[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int anoBissexto[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	
	if(data.ano % 4 != 0){
		return anoComum[data.mes - 1];
	} else if (data.ano % 100 != 0){
		return anoBissexto[data.mes - 1];
	} else if (data.ano % 400 == 0){
		return anoBissexto[data.mes - 1];
	} else {
		return anoComum[data.mes - 1];
	}
}

Data corrigirData (Data data, int mudancaEmDias){
	data.dia += mudancaEmDias;
	
	while (data.dia > maxDiasPorMes(data)){
		data.dia -= maxDiasPorMes(data);
		data.mes ++;
		if (data.mes == 13){
		 	data.mes = 1;
		 	data.ano ++;
		}
	}
	
	while (data.dia <= 0){
		data.mes --;
		if (data.mes == 0){
 			data.mes = 12;
 			data.ano --;	 	 
		}
		data.dia += maxDiasPorMes(data);
	}
	
	return data;
}

Data getData(){
	Data data;
	printf("Data(dd/mm/aaaa): ");
	scanf("%d/%d/%d", &data.dia, &data.mes, &data.ano);
	pauseClean();
	return data;
}

void apagarReservatorio(Caixas *caixa){
	char ID[50];
	getString(ID, 50);
	int position = verifyPosition(caixa, ID);
	
	if (position == -1){
		printf("Nenhuma caixa cadastrada\n");
		pauseClean();						
	} else {
		
		if (position != -1){
			// Encontrou
			if (position == (execucoesCadastro - 1)){ //ULTIMA POSIÇÃO
	 	 		caixa[position].IDCaixa[0] = '\0';
			 	caixa[position].capMaxima = 0.0;
			 	for (int i = 0; i < 10; i++){
			 		caixa[position].volume[i] = 0.0;	 	
			 	}
			} else { //MEIO
			
				for (int i = position; i < execucoesCadastro - 2; i++){ 
					strcpy(caixa[i].IDCaixa, caixa[i+1].IDCaixa);
					caixa[i].capMaxima = caixa[i+1].capMaxima;
					for(int j = 0; j < 10; j++){
						caixa[i].volume[j] = caixa[i+1].volume[j];
					}
				} 
				caixa[execucoesCadastro - 1].IDCaixa[0] = '\0';
			 	caixa[execucoesCadastro - 1].capMaxima = 0.0;
			 	for (int i = 0; i < 10; i++){
			 		caixa[execucoesCadastro - 1].volume[i] = 0.0;	 	
			 	}
				pauseClean();
			}
			 	
		} else {
			printf("%s não encontrado\n", ID);
			pauseClean();		 		 
		}
	}
}

void editarReservatorio(Caixas *caixa){
	char ID[50], newID[50];
	printf("Digite o ID para buscarmos: ");
	getString(ID, 50);
	int position = verifyPosition(caixa, ID);
	
	if (position == -1){
		printf("Nenhuma caixa cadastrada\n");
		pauseClean();						
	} else {
		
		if (position != -1){
			printf("Digite 1 para alterar o ID, 2 para a Capacidade Máxima ou outro número para desistir\n");
				switch(getInt()){
					case 1:
					do {
						printf("Digite o novo ID, não pode ser igual ao anterior nem igual a outro já registrado\n");
						getString(newID, 50);
						int verifyDuplicate = verifyPosition(caixa, newID);
									
						if (verifyDuplicate != -1){
				 			strcpy(caixa[position].IDCaixa, newID);
							pauseClean();											 			  				 		 			 
							break;
						} else {
							printf("ID já utilizado, escolha outro\n");
							pauseClean();		 		
						}
					} while (1);							
					break;
					
					case 2:
					printf("Digite a nova capacidade Máxima: ");
					caixa[position].capMaxima = getFloat();
					break;
					
					default:
					break;								
				}
			 	
		} else {
			printf("%s não encontrado\n", ID);
			pauseClean();		 		 
		}
	}
}

void gerarRelatorio(Caixas *caixa){
	char ID[50];
	printf("Digite o ID para buscarmos: ");
	getString(ID, 50);
	int position = verifyPosition(caixa, ID);
	Data ultimasDatas = data;

	if (position == -1) {
		printf("Nenhuma caixa cadastrada\n");
		pauseClean();
	} else {
		
		printf("|****************************************|\n");
		printf("|	             <RELATÓRIO>	    		     |\n");
		printf("|****************************************|\n");
		printf("| ID: %s				 		         \n", caixa[position].IDCaixa);
		printf("| Capacidade Máxima: %.2f		             \n", caixa[position].capMaxima);
		printf("| Volume atual: %.2f \n", caixa[position].volume[9]);
		printf("| Data de hoje: %d/%d/%d \n", data.dia, data.mes, data.ano);
		printf("|****************************************| \n");
		for (int j = 8; j >= 0; j--){
			corrigirData (ultimasDatas, -1);
			printf("|Volume: %.2f / 10L \n| Data: %d/%d/%d\n", caixa[position].volume[j], ultimasDatas.dia, ultimasDatas.mes, ultimasDatas.ano);
			printf("|---------------------------------------\n");
		}
		pauseClean();
	}
}