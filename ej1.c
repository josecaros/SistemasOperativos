#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_mutex_t s1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t s2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t s3 = PTHREAD_MUTEX_INITIALIZER;

int count = 1;

void *funcion01(void *arg) {
	int i;

	while(count>0) {
		pthread_mutex_lock(&s3);
		 for (i=0; i<count; i++) {
			printf("+");
			fflush(stdout);
			sleep(1);
		 }
		count--;
			printf("\n");
			fflush(stdout);
		pthread_mutex_unlock(&s1);
	}

	 	return NULL;
}
void *funcion02(void *arg) {
	int i;
	
	while(count>0) {
		pthread_mutex_lock(&s2);
		 for (i=0; i<count; i++) {
			printf("o");
			fflush(stdout);
			sleep(1);
		 }
			printf("\n");
			fflush(stdout);
		count--;
		pthread_mutex_unlock(&s3);

	}
	return NULL;
}
int main() {
	
	pthread_mutex_lock(&s2);
	pthread_mutex_lock(&s3);


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
	while(count>0){
		pthread_mutex_lock(&s1);
		 for (i=0; i<count; i++) {
			printf("x");
			fflush(stdout);
			sleep(1);
		 }
			printf("\n");
			fflush(stdout);
		count--;
		pthread_mutex_unlock(&s2);
		
	}	


	pthread_join(mihilo01,NULL);
	pthread_join(mihilo02,NULL);
	
	printf("\nFIN\n");
}	
