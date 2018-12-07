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
#include "cola.h"
#include <sys/wait.h>

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

enum {
    sem1,
    sem2,
    sem3
};



int main (){
    int numUser;
    int numBanios;
    int numBanios1=numBanios;
    puts("Ingrese la cantidad de Usuarios");
    scanf("%i",&numUser);
    puts("Ingrese la cantidad de baños");
    scanf("%i",&numBanios);
    int idShMem;
    Cola* buf;

    pid_t pid;

    int idSem;
    short vals[3];

    idSem=CrearSemaforos(3,vals);

    idShMem= ReservarMemoriaComp(sizeof(struct cola));
    buf=(Cola*) MapearMemoriaComp(idShMem);
    buf->inic=0;
    buf->size=0;
    buf->fin=-1;
    while(numUser--){
        int carac=rand()%2;
        char sexo;
        if(carac==0)
            sexo='M';
        else
            sexo='H';
        Persona data={sexo,rand () % (10-5+1) + 5};
        encolar(buf,data);
    }
    while(numBanios--){
        pid=fork();
        if(pid==0){
            //
            break;
        }
    }

    while(numBanios1--)
        wait();
    BorrarSemaforos(idSem);
}
