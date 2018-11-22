#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_mutex_t s1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t s2 = PTHREAD_MUTEX_INITIALIZER;


void *funcion01(void *arg) {
	int i;
	for (i=0; i<20; i++) {
		printf(“+”);
		fflush(stdout);
		sleep(1);
	}
	return NULL;
}
void *funcion02(void *arg) {
	int i;
	for (i=0; i<20; i++) {
		printf(“o”);
		fflush(stdout);
		sleep(1);
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
		sleep(3);
	}
	exit(0);
}
