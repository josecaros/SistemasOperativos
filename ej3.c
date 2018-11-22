#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

#define N 100;

/*
typestruct Cola {
	int size;
	int array[N];
};

void encolar(struct Cola)

*/




//Mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
//Variables de Condicion
pthread_cond_t vacio = PTHREAD_COND_INITIALIZER;
pthread_cond_t lleno = PTHREAD_COND_INITIALIZER;


void lee() {
	// p
	// if(cola.estaVacio)
	//	vacio.wait
	// int a =  cola.desencolar
	// printf(a);
	// if(cola.size == N-1)
	// 	lleno.signal
	// v
	
}
void escribe(int n) {
	// p
	// if(cola.estaLleno)
	//	lleno.wait
	// cola.encolar(n);
	// if(cola.size == 1);
	// 	vacio.signal;
	// v
}

void *productor() {
	while(1){
		int n = rand();
		escribe(n);
	}
}

void *consumidor() {
	while(1){
		lee();
	}
}
int main() {
	

}
