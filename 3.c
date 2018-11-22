#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

void *funcion01(void *arg) {
	int i;
	for (i=0; i<20; i++) {
		printf("+");
		fflush(stdout);
	}
	return NULL;
}
void *funcion02(void *arg) {
	int i;
	for (i=0; i<20; i++) {
		printf("o");
		fflush(stdout);
	}
	return NULL;
}
int main() {
	pthread_t mihilo01, mihilo02;
	int i;
	if (pthread_create(&mihilo01,NULL,funcion01,NULL)) {
		printf("Error creando el hilo.");
		abort();
	}
	if (pthread_create(&mihilo02,NULL,funcion02,NULL)) {
		printf("Error creando el hilo.");
		abort();
	}
	for (i=0; i<20; i++) {
		printf("x");
		fflush(stdout);
	}
//	pthread_join(mihilo01,NULL);
//	pthread_join(mihilo02,NULL);
	printf("\nFIN\n");
}	
