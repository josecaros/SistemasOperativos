#include<stdio.h>
#include<pthread.h>

#define TAM_ARRAY 100

int array[TAM_ARRAY];
pthread_mutex_t s1;//Declaro Semaforo

void *verificar(void *args);
int main(){

	pthread_mutex_init(&s1,NULL); //Inicio Semaforo


	pthread_t hilo2;
	pthread_create(&hilo2,NULL,verificar,NULL);	

	int contador = 0;
	while(1){
		pthread_mutex_lock(&s1); // P(s);
		for(int i=0;i<TAM_ARRAY;i++)
			array[i] = contador;
		contador++;
		pthread_mutex_unlock(&s1);	//V(s);
	}

}

void *verificar(void *args){
	while(1){


		int estado = 0;
				
		pthread_mutex_lock(&s1);
		for(int i=0;i<TAM_ARRAY;i++)
			if(array[0]!=array[i]){
				estado = 1;		
				break;
			}
		if(estado == 1)
			printf("Hijo:Incorrecto\n");	
		else 
			printf("Hijo:Correcto\n");
		pthread_mutex_unlock(&s1);
	}	
}


