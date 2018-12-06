//
// Created by jcaros on 06/12/18.
//
#include "cola.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *_buf;
};

//declaraciones de las funciones wrappers para simplificarnos la vida
int ReservarMemoriaComp(int n) {
    return shmget(IPC_PRIVATE, n, IPC_CREAT | SHM_R | SHM_W);
}
void* MapearMemoriaComp(int id) {
    void* addr;
    addr = shmat(id, NULL, 0); //adjuntamos el segmento
    shmctl(id, IPC_RMID, NULL); //y lo marcamos para que se destruya
    return addr;
}
int CrearSemaforos(int n, short* vals) {
    union semun arg;
    int id;
    id=semget(IPC_PRIVATE, n, SHM_R | SHM_W);
    arg.array=vals;
    semctl(id, 0, SETALL, arg);
    return id;
}
void BorrarSemaforos(int id) {
    if(semctl(id, 0, IPC_RMID, NULL)==-1) {
        perror("Error liberando semáforo!");
        exit(EXIT_FAILURE);
    }
}
void BloquearSemaforo(int id, int i) {
    struct sembuf sb;
    sb.sem_num = i;
    sb.sem_op=-1;
    sb.sem_flg=SEM_UNDO;
    semop(id,&sb,1);
}
void DesbloquearSemaforo(int id, int i) {
    struct sembuf sb;
    sb.sem_num = i;
    sb.sem_op = 1;
    sb.sem_flg = SEM_UNDO;
    semop(id, &sb, 1);
}

void entraMujer(){
	printf("Mujer Entrando\n");	
}
void saleMujer(){
	printf("Sale Mujer \n");
}
void entraHombre() {
	printf("Entra Hombre");
}
void saleHombre(){
	printf("Sale Hombre");
}

void serviciosHigenicos(Cola col, int i){
	
	
	while(!estaVacio(&col)){
		Persona a = desencolar(&col);
		printPersona(a);					
	}
	printf("Proceso %d\n",i);
						
	
}

int main () {
	//INICIANDO LA COLA	
	Cola col;
	col.inic = 0;
	col.fin = -1;
	col.size = 0;

	key_t clave = ftok("/bin/ls",33);
	int idMemoria = shmget(clave,100,IPC_CREAT | 0777);	

	Cola* colCompartida = (Cola*)shmat(idMemoria,0,0);

	Persona per[10];
	for(int i = 0;i<10;i++){
		per[i].genero = 'h';
		per[i].tiempo = i+1;
		encolar(&col, per[i]);
	}

	int L = 3;
	for(int i = 0; i < L; i++){
	
		pid_t pid = fork();
		//HIJO
		if(pid == 0){
			serviciosHigenicos(col,i+1);
		}	
	}




	sleep(4);




}
