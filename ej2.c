#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

struct parametros {
	int idHilo;
	int tiempo;
};

#define TAM 20
int p = 0;
int array[TAM];
pthread_mutex_t s1 = PTHREAD_MUTEX_INITIALIZER;



void *fhilo(void *args){
	
	struct parametros *param= (struct parametros*)args;
	int i = param -> tiempo;	

	while(1){
			
		fflush(stdout);
		sleep(i);
		pthread_mutex_lock(&s1);

		 if(p < TAM){
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
	}
	
}
void print(){

	for(int i = 0; i<TAM; i++)
		printf("%d ",array[i]);
	printf("\n");
}

int main(){

	pthread_t hilos[4];
	struct parametros param[4];
	for(int i = 0; i < 4; i++){
		param[i].idHilo = i;
		param[i].tiempo = rand()%5+1;
		pthread_create(&hilos[i], NULL, fhilo,(void*)&param[i]);
	}
	for(int i = 0; i < 4; i++)
		pthread_join(hilos[i], NULL);
	
	printf("\n");	
	print();
		
	

}
