#include<stdio.h>
#include<pthread.h>

pthread_mutex_t s1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t s2 = PTHREAD_MUTEX_INITIALIZER;

void *f1(void *args){
	while(1){
		pthread_mutex_lock(&s1);
		printf("x");
		fflush(stdout);

		pthread_mutex_unlock(&s2);
	}
}
void *f2(void *args){

	while(1){
		pthread_mutex_lock(&s2);
		printf("o");
		fflush(stdout);
		
		pthread_mutex_unlock(&s1);

	}

}

int main(){
	
	pthread_t h1;
	pthread_t h2;
		
	pthread_create(&h1, NULL, f1, NULL);
	pthread_create(&h2, NULL, f2, NULL);
	pthread_join(h1, NULL);
	pthread_join(h2, NULL);
	
	
}
