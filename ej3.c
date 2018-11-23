#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

#define N 100

//COLA 
struct cola {
	int size;
	int inic;
	int fin;
	int array[N];
};
typedef struct cola Cola;


//Funciones para la COLA
int estaVacio(Cola *cola);
int estaLleno(Cola *cola);
void encolar(Cola *cola, int n);
int desencolar(Cola *cola);


//Mutex y Variables de Condicion
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t vacio = PTHREAD_COND_INITIALIZER;
pthread_cond_t lleno = PTHREAD_COND_INITIALIZER;

//Esctructura COLA a utilizar
Cola col;


//Metodo para el CONSUMIDOR
void lee() {	
	pthread_mutex_lock(&mutex);	

	if(estaVacio(&col))
		pthread_cond_wait(&vacio, &mutex);

	int a = desencolar(&col); 
	printf("Lee: %d\n", a);
	fflush(stdout);

	if(col.size+1 == N)
		pthread_cond_signal(&lleno);

	pthread_mutex_unlock(&mutex);
}

//Metodo para el PRODUCTOR
void escribe(int n) {
	pthread_mutex_lock(&mutex);	
	if(estaLleno(&col))
		pthread_cond_wait(&lleno, &mutex);
	encolar(&col,n);
	printf("Escribe: %d\n", n);
	fflush(stdout);
	
	if(col.size == 1);
		pthread_cond_signal(&vacio);
	
	pthread_mutex_unlock(&mutex);
}

//Funcion Hilo PRODUCTOR
void *productor() {
	while(1){
		sleep(1);
		int n = rand();
		escribe(n);
	}
}

//Funcion Hilo CONSUMIDOR
void *consumidor() {
	while(1){
		sleep(1);
		lee();
	}
}

int main() {
	//Inicializo la Cola
	col.size = 0;
	col.inic = 0;
	col.fin = -1;
		

	pthread_t h1;
	//Hilo Productor 
	pthread_create(&h1,NULL, productor,NULL);
	//Hilo Consumidor
	consumidor();	

}



//IMPLEMETACION DE LAS METODOS DE COLA

int estaVacio(Cola *col){
	return col -> size == 0;
}
int estaLleno(Cola *col){
	return N == col -> size;
}
void encolar(Cola *col, int data){
	int i = col -> size;
	int *array = col -> array;

	if (i < N){
		col -> fin = (col-> fin+1)%N; 
		array[col->fin] = data;
		col -> size++;
	}
}

int desencolar(Cola *col){
	
	int data ;
	int i = col -> size;
	int *array = col -> array;
	
	if( i>0 ){
		data = array[col -> inic];
		col -> inic = (col->inic+1) % N;
		col -> size--;
		
		return data;	
	} 
	
	return -1;

}
