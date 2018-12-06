//
// Created by jcaros on 29/11/18.
//
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
enum{
    SEM_FUM1,
    SEM_FUM2,
    SEM_FUM3,
    SEM_AGEN
};
int main(int argc, char** argv){
    int idShMem; //handle a la memoria compartida
    int idSem; //handle al grupo de semáforos
    char* buf; //dirección del buffer de memoria compartida
    short vals[4]; //valores para inicializar los semáforos
    int fum1; 
    int fum2; 
    int fum3;
    int agente;
    int tipo=0;
    puts("Problema de Fumadores\n");
    if(argc<2) {
        //no nos pasaron ningún id, así que lo creamos
        idShMem = ReservarMemoriaComp(BUFSIZ);
        buf = (char*) MapearMemoriaComp(idShMem);
        //Queremos que los usuarios sean bloqueados cuando traten de
        //hacer lock a la memoria compartida. Cuando el segundo usuario
     //inicia el programa,desbloquearoan al primero para que pueda tipear.
        //Para eso son los ceros.
        vals[SEM_FUM1]=0;
        vals[SEM_FUM2]=0;
        vals[SEM_FUM3]=0;
        vals[SEM_AGEN]=0;
        idSem=CrearSemaforos(4,vals);
        //guardamos el id del semáforo en la memoria compartida para que
        //el otro usuario pueda obtenerlo
        *((int*) buf) =idSem;
        fum1 = SEM_FUM1;
        fum2 = SEM_FUM2;
        fum3 = SEM_FUM3;
        agente = SEM_AGEN;
        //escribimos el id del segmento de memoria compartida para que el otro
        //usuario sepa y pueda chatear con nosotros.
        printf("Eres el Agente. El id de la memoria compartida es: %d\n", idShMem);
        printf("Esperando a los fumadores...\n");
    }
    else {
        //tenemos un valor! quiere decir que somos el usuario dos.
        idShMem = atoi(argv[1]);
        tipo = atoi(argv[2]);
        buf = (char*) MapearMemoriaComp(idShMem);
        //obtenemos el id del grupo de semaforos desde la memoria compartida
        idSem = *((int*) buf);
        //grabamos a qué semáforos tenemos que esperar
        //recordemos que aquí los roles cambian
        fum1=SEM_FUM1;
        fum2=SEM_FUM2;
        fum3=SEM_FUM3;
        agente=SEM_AGEN;
        //Ponemos una cadena vacía en la memoria compartida.
        sprintf(buf, "");
        if(tipo==1){
            puts("Eres un fumador con TABACO. Esperando otros 2 fumadores...");
        }
        if(tipo==2){
            puts("Eres un fumador con PAPEL. Esperando ultimo fumador...");
        }
		  //desbloqueamos al agente
        if(tipo==3){
            puts("Eres un fumador con CERILLOS. Avisando al agente...");
            DesbloquearSemaforo(idSem, agente);
        }
    }
    int controlador=20;
    while(controlador){
        if(tipo==0){
            BloquearSemaforo(idSem,agente);
            int num=1+rand()%(4-1);
            printf("Nadie fumando...\n");
            if(num==1){
                printf("Se libero PAPEL y CERILLOS\n");
                DesbloquearSemaforo(idSem,fum1);
            }
            else if(num==2){
                printf("Se libero TABACO y CERILLOS\n");
                DesbloquearSemaforo(idSem,fum2);
            }
            else if(num==3){
                printf("Se libero TABACO y PAPEL\n");
                DesbloquearSemaforo(idSem,fum3);
            }

        }
        else if(tipo==1){
            BloquearSemaforo(idSem,fum1);
            printf("Fumador con TABACO, recoge de la mesa PAPEL y CERILLOS\n");
            printf("Fuma...\n");
            sleep(4);
            DesbloquearSemaforo(idSem,agente);

        }
        else if(tipo ==2){
            BloquearSemaforo(idSem,fum2);
            printf("Fumador con PAPEL, recoge de la mesa TABACO y CERILLOS\n");
            printf("Fuma...\n");
            sleep(4);
            DesbloquearSemaforo(idSem,agente);
        }
        else{
            BloquearSemaforo(idSem,fum3);
            printf("Fumador con CERILLOS, recoge de la mesa TABACO y PAPEL\n");
            printf("Fuma...\n");
            sleep(4);
            DesbloquearSemaforo(idSem,agente);
        }
    }
    //el agente libera los semaforos
    if(tipo ==0)
      BorrarSemaforos(idSem);

}
