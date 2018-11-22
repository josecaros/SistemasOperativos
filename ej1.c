#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_mutex_t s1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t s2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t s3 = PTHREAD_MUTEX_INITIALIZER;

int count = 20;

struct parametro {

	char letra;
	pthread_mutex_t *lock;
	pthread_mutex_t *unlock;
};

void *funcionUnica(void *arg) {
	
	struct parametro *param = (struct parametro*)arg;
	pthread_mutex_t *m1 = param -> lock;
	pthread_mutex_t *m2 = param -> unlock;
	char letra = param -> letra;

	int i = 0;

	while(count > 0) {
		pthread_mutex_lock(m1);
		 for (i=0; i<count; i++) {
			printf("%c",letra);
			fflush(stdout);
		//	sleep(1);
		 }
		count--;
			printf("\n");
			fflush(stdout);
		pthread_mutex_unlock(m2);
	}

	 	return NULL;
}
int main() {
	
	pthread_mutex_lock(&s2);
	pthread_mutex_lock(&s3);

	struct parametro params[3];
	params[0].letra = 'x';
	params[0].lock = &s1;
	params[0].unlock = &s2;
		
	params[1].letra = 'o';
	params[1].lock = &s2;
	params[1].unlock = &s3;

	params[2].letra = '+';
	params[2].lock = &s3;
	params[2].unlock = &s1;

	pthread_t hilos[2];

	
	for(int i=0; i < 2; i++){
		pthread_create(&hilos[i],NULL,&funcionUnica, (void*)&params[i+1]);
	}	
	
	funcionUnica((void*)&params[0]);
	
	for(int i=0;i<2;i++)
		pthread_join(hilos[i],NULL);

	printf("FIN\n");
}	
