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
	col.inic = 0;
	col.fin = -1;
	
	encolar(&col,1);	
	encolar(&col,2);	
	encolar(&col,3);	
	encolar(&col,4);	
	encolar(&col,5);	
	
	print(col);
	
	printf("a %d \n",desencolar(&col));	
	printf("a %d \n",desencolar(&col));	
	printf("a %d \n",desencolar(&col));	
	printf("a %d \n",desencolar(&col));	
	printf("a %d \n",desencolar(&col));	
	printf("a %d \n",desencolar(&col));	
	printf("a %d \n",desencolar(&col));	
	
}

