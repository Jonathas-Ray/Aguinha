#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int dia, mes, ano;
}Data;

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

int main (){
	return 0;
}