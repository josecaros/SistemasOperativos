//
// Created by jcaros on 06/12/18.
//

#ifndef LABORATORIO9Y10_COLA_H
#define LABORATORIO9Y10_COLA_H
//tu codigo aqui

#include <stdio.h>
#include <stdlib.h>
#define N 100

//PERSONA
struct persona {
	char genero;
	int tiempo;
};
typedef struct persona Persona;

void printPersona(Persona p){
	printf("Genero: %c - Tiempo: %d\n", p.genero, p.tiempo);
}

//COLA
struct cola {
	int size;
	int inic;
	int fin;
	Persona array[N];
};
typedef struct cola Cola;

int estaVacio(Cola *col){
	return col -> size == 0;
}
int estaLleno(Cola *col){
	return N == col -> size;
}
void encolar(Cola *col, Persona data ){
	int i = col -> size;
	Persona *array = col -> array;

	if (i < N){
		col -> fin = (col-> fin+1)%N; 
		array[col->fin] = data;
		col -> size++;
	}
}

Persona desencolar(Cola *col){
	
	Persona data ;
	int i = col -> size;
	Persona *array = col -> array;

	if( i>0 ){
		data = array[col -> inic];
		col -> inic = (col->inic+1) % N;
		col -> size--;
		
		return data;	
	} 

	return;


}

void print(Cola col){
	for(int i = 0; i < col.size; i++){
		Persona persona = col.array[(col.inic + i)%N];
		printPersona(persona);
	}
	printf("\nVacio %d ", estaVacio(&col));
	printf("Lleno%d ", estaLleno(&col));
	printf("Length%d \n", col.size);
	printf("\n");
		
}





#endif //LABORATORIO9Y10_COLA_H
