#include<stdio.h>
#include<stdlib.h>
#define N 5
struct cola {
	int size;
	int inic;
	int fin;
	int array[N];
};
typedef struct cola Cola;

int estaVacio(Cola *col){
	return col -> size == 0;
}
int estaLleno(Cola *col){
	return N == col -> size;
}
void encolar(Cola *col, int data){
	int i = col -> size;
	int *array = col -> array;

	if ( i == 0) {
		col -> inic = 0;
		col -> fin = 0;
		array[ col->fin ] = data;
		col -> size++;
	}
	else if (i < N){
		col -> fin = (col-> fin+1)%N; 
		array[col->fin] = data;
		col -> size++;
	}
}

int desencolar(Cola *col){
	
	int data;
	int i = col -> size;
	int *array = col -> array;
	
	if(i>0){
		data = col -> fin;
		if( i == 1){
			col -> inic = 0;
			col -> fin = 0;
		}		
		else {
			col -> fin = (col->fin-1+N) % N;
		}
	
			
		col -> size--;
		
	} 


}

void print(Cola col){
	for(int i = 0; i < col.size; i++){
		int a = col.array[(col.inic + i)%N];
		printf("%d ",a);
	}
	printf("\n%d ", estaVacio(&col));
	printf("%d ", estaLleno(&col));
	printf("%d \n", col.size);
	printf("\n");
		
}

int main(){
	
	Cola col;
	col.size = 0;
	col.inic = -1;
	col.fin = -1;
	
}

