#include<stdio.h>
#include<stdlib.h>
#define N 10

//PERSONA
struct persona {
	char genero;
	int tiempo;
};
typedef struct persona Persona;

void printPersona(Persona p){
	printf("Genero: %c - Tiempo: %d", p.genero, p.tiempo);
}
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

int main(){
	
	Cola col;
	col.size = 0;
	col.inic = 0;
	col.fin = -1;
	
	Persona nuevos[10];
	for(int i = 0;i<10;i++){
		nuevos[i].genero = 'h';
		nuevos[i].tiempo = i+1;
	}
	
	for(int i = 0;i < 10; i++)
		encolar(&col,nuevos[i]);	
	
	print(col);
	
	Persona aux = desencolar(&col);
	print(col);
	printPersona(aux);
	
}

