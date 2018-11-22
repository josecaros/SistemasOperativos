#include<stdio.h>
#include<pthread.h>
#define TAM_ARRAY 100

int array[TAM_ARRAY];

void *verificar(void *args);
int main(){

	int contador = 0;
	pthread_t hilo2;
	
	pthread_create(&hilo2,NULL,verificar,NULL);	


	while(1){
		for(int i=0;i<TAM_ARRAY;i++)
			array[i] = contador;
		contador++;
	}

}

void *verificar(void *args){
	while(1){

		int estado = 0;
		for(int i=0;i<TAM_ARRAY;i++)
			if(array[0]!=array[i]){
				estado = 1;		
				break;
			}
		if(estado == 1)
			printf("Hijo:Incorrecto\n");	
		else 
			printf("Hijo:Correcto\n");
	}	
}


