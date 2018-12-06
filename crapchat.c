/*
 * crapchat_v2.c 
 * CrapChat original hecho por Keith Gaughan <kmgaughan@eircom.net> 
 * Adaptación y traducción al español por Gustavo Puma 
 * 
 * Software libre bajo la licencia DSL: 
 * 
http://www.dsl.org/co
pyleft/dsl.txt
 */
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
enum {
  SEM_USER_1, 
  //el turno de la primera persona 
  SEM_USER_2 
    //el turno de la 2da persona 
};
int main(int argc, char* argv[]) { 
  int idShMem; //handle a la memoria compartida 
  int idSem; //handle al grupo de semáforos 
  char* buf; //dirección del buffer de memoria compartida 
  short vals[2]; //valores para inicializar los semáforos 
  int miSem; //semáforo para nuestro usuario
  int tuSem; //semáforo para el otro usuario 
  puts("Bienvenido a CrapChat! Escribe '\\quit' para salir\n");
  //obtenemos el id del segmento de memoria de la línea de comandos 
  if(argc<2) {
    //no nos pasaron ningún id, así que lo creamos
    idShMem = ReservarMemoriaComp(BUFSIZ); 
    buf = (char*) MapearMemoriaComp(idShMem); 
    //Queremos que los usuarios sean bloqueados cuando traten de 
    //hacer lock a la memoria compartida. Cuando el segundo usuario 
    //inicia el programa, desbloquearoan al primero para que pueda tipear. 
    //Para eso son los ceros. 
    vals[SEM_USER_1]=0; 
    vals[SEM_USER_2]=0; 
    idSem=CrearSemaforos(2,vals); 
    //guardamos el id del semáforo en la memoria compartida para que 
    //el otro usuario pueda obtenerlo 
    *((int*) buf) =idSem; 
    miSem = SEM_USER_1; 
    tuSem = SEM_USER_2; 
    //escribimos el id del segmento de memoria compartida para que el otro
    //usuario sepa y pueda chatear con nosotros. 
    printf("Eres el usuario uno. El id de la memoria compartida es: %d\n", idShMem); 
    printf("Esperando al usuario dos...\n"); 
  }
  else { 
    //tenemos un valor! quiere decir que somos el usuario dos.
    idShMem = atoi(argv[1]); 
    buf = (char*) MapearMemoriaComp(idShMem); 
    //obtenemos el id del grupo de semaforos desde la memoria compartida
    idSem = *((int*) buf); 
    //grabamos a qué semáforos tenemos que esperar 
    //recordemos que aquí los roles cambian 
    miSem=SEM_USER_2; 
    tuSem=SEM_USER_1;
    //Ponemos una cadena vacía en la memoria compartida. 
    sprintf(buf, ""); 
    //desbloqueamos al otro usuario para indicar que pueden hablar 
    puts("Eres el usuario dos. Avisando al usuario uno..."); 
    DesbloquearSemaforo(idSem, tuSem); 
  }
  for(;;) {
    //Esperar hasta que sea mi turno de hablar 
    BloquearSemaforo(idSem, miSem); 
    //salió el otro usuario? 
    if(strcmp(buf, "\\quit\n")==0) 
      break; 
    //imprimir la respuesta, si hay alguna 
    if(strlen(buf)>0) 
      printf("Respuesta: %s", buf); 
    //obtener tu respuesta 
    printf("> "); 
    fgets(buf,BUFSIZ, stdin); 
    //Pasamos el control al otro usuario.
    DesbloquearSemaforo(idSem, tuSem); 
    //quieres salir? 
    if(strcmp(buf, "\\quit\n") == 0) 
      break;
  } 
  //el primer usuario tiene que liberar los semáforos. 
  if(miSem == SEM_USER_1) 
    BorrarSemaforos(idSem);
}  
