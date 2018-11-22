// Ejercicio2
// -Ccahui Huaman Kristian 	

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>


struct parametros {
	int *array; 
	int tiempo; 
	int length;
};


int p = 0; //Posicion a escribir en el array
pthread_mutex_t s1 = PTHREAD_MUTEX_INITIALIZER;


void *fhilo(void *args){
	
	struct parametros *param= (struct parametros*)args;
	int i = param -> tiempo;	
	int n = param -> length;
	int *array  = param -> array;

	while(1){
		
		pthread_mutex_lock(&s1);
	
		//Verifico que la posicion a escribir es VALIDA
		 if(p < n){
			array[p] = i;
			printf("Escribi un: %d en la posicion: %d\n", i,p );
			fflush(stdout);
			p++;
		 }
		 else	{ 
			pthread_mutex_unlock(&s1);
			break;
			}
		pthread_mutex_unlock(&s1);
		sleep(i);
	}
	
}


int main(){

	int n;
	scanf("%d",&n);
	int array [n];


	//Incializando parametros para los hilos , con su tiempo
	struct parametros param[4];
	for (int i = 0;i < 4; i++){
		param[i].array = array;
		param[i].tiempo = i+1;
		param[i].length = n;
	}
	
	//Inicializamos los HILOS y sus Parametros para cada HILO
	pthread_t hilos[4];
	for(int i = 0; i < 4; i++){
		pthread_create(&hilos[i], NULL, fhilo,(void*)&param[i]);
	}
	
	//Espera a que finalicen los hilos creados
	for(int i = 0; i < 4; i++){
		pthread_join(hilos[i], NULL);
	}
	

}
