#include <stdio.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
 #include <sys/types.h>
       #include <signal.h>
#include <time.h>
int shmid,semid,semid2, shmidgettone, shmidpid, pid1, pid2,pid3, pedina1, pedina2,k, pid;
int (*p)[3];
char (*g)[5];
void elimina (int shmid, int semid, int semid2, int shmidgettone, int shmidpid){


shmctl(shmid, IPC_RMID, 0);
shmctl(shmidgettone, IPC_RMID,0);
shmctl(shmidpid, IPC_RMID,0);
semctl(semid, 0, IPC_RMID);
semctl(semid2, 0, IPC_RMID);

exit(0);

}

void pareggio(int n){

printf("\n");
if(k==12){
printf("PARTITA TERMINATA IN PARITÀ\n");
fflush(stdout);
elimina(shmid, semid,semid2,shmidgettone, shmidpid);
}
}
void termesterna(int n){
printf("\n");
printf("TERMINAZIONE ESTERNA\n");
fflush(stdout);
elimina(shmid, semid,semid2,shmidgettone, shmidpid);
}
void chiudi (int n)
{	
	elimina(shmid, semid,semid2,shmidgettone, shmidpid);
	

exit(0);

}
void termina(int n){
int terminatore;
terminatore=getpid();

if (terminatore==pid1){
kill(pid3,SIGUSR1);
}
else if (terminatore==pid2){
	kill(pid3,SIGUSR2);
}
}

void temposcaduto (int n){
printf("\n");
pid=semctl(semid2, 0, GETPID);
//printf("%d\n",pid);
if(pid==pid1){
printf("tempo scaduto");
kill(pid3,SIGTERM);
}
else if(pid == pid2){
printf("tempo scaduto");
kill(pid3,SIGQUIT);
}
//elimina(shmid, semid,semid2,shmidgettone, shmidpid);

}
void vittoria (int n){
printf("\n");
printf("HAI VINTO!\n");
fflush(stdout);
elimina(shmid, semid,semid2,shmidgettone, shmidpid);

}

void sconfitta (int n){
printf("\n");
printf("HAI PERSO!\n");
fflush(stdout);
elimina(shmid, semid,semid2,shmidgettone, shmidpid);
}

//MAIN
int main(int argc, char *argv[]){
char pedina1, pedina2;
int pid;


//attacco al tabellone
shmid= shmget(1611,sizeof(char)*49,0644);
char (*c)[7] = shmat(shmid,NULL,0);

//attacco all'array con i gettoni
shmidgettone=shmget(1612, sizeof(char)*5,0644);
char (*g)[5]= shmat(shmidgettone,NULL,0);
*g[2]=' ';
//attacco pid
shmidpid=shmget(1613, sizeof(int)*3,0644);
int (*p)[3]=shmat(shmidpid,NULL,0);
//*p[0]=' ';
//*p[1]=' ';
if (*p[0]==' '){
*p[0]=getpid();
//printf("pid:%d ", *p[0]);
}
else{
*p[1]=getpid();
}
	pid1= *p[0];
   	pid2=*p[1];  
  
//recupero il pid del processo
//pid=getpid();
//printf("il mio pid è: %d\n", pid);
int i,j;
pedina1=*g[0];
pedina2=*g[1];
if (*g[0]!=' '){
printf("Giocatore 1: %s, gettone: %c\n", argv[1], *g[0]);
*g[0]=' ';
}
else{
printf("Giocatore 2: %s, gettone: %c\n", argv[1], *g[1]);
*g[1]=' ';
}

if(*g[2]==' '){
*g[2]='a';
}
pid3=*p[2];

//printf("pid2 %d\n", *p[1]);

//SEGNALI
signal(SIGINT, termina);
signal(SIGTERM, pareggio);
signal(SIGUSR1, vittoria);
signal(SIGUSR2, sconfitta);
signal(SIGQUIT,termesterna);
signal(SIGALRM, temposcaduto);
//semafori
semid	= semget(1234, 1, 0777);
semid2= semget(12345,1,0777);
struct sembuf op,op1;
char ch;
 k=0;





while(k!=13){

op1.sem_num=0;
op1.sem_op=-1;
op1.sem_flg=SEM_UNDO;

semop(semid2,&op1,1);
int x,y;
printf("mossa dell'avversario\n");
fflush(stdout);
for (i=0; i<7;i++)
 {
   for (j=0; j<7; j++)
   {
 	
		printf("%c\t", *c[i*7+j]);
		
  }
	printf("\n");
}

printf("\n");

printf("FAI LA TUA MOSSA, inserire le coordinate: ");
fflush(stdout);
if (argc>2){
x=rand()%7;
y=rand()%7;
srand(time(0));
}
else{
scanf("%d %d", &x ,&y);
}
printf("\n");
while(*c[x*7+y]!='-'){
if (argc>2 ){
	x=rand()%7;
	y=rand()%7;
	srand(time(0));
}
else{
	printf("le coordinate inserite non sono legali, riprova\n");
	printf("inserire le coordinate: ");
	scanf("%d %d", &x ,&y);
	printf("\n");
	}
	}

if (*g[2]=='a' && *c[x*7+y]=='-'){

	*c[x*7+y]=pedina1;
	*g[2]=' ';
}
else if (*g[2]==' ' && *c[x*7+y]=='-'){

	*c[x*7+y]=pedina2;
	*g[2]='a';
}


printf("ecco la tua mossa\n");
for (i=0; i<7;i++)
 {
   for (j=0; j<7; j++)
   {
 	
		printf("%c\t", *c[i*7+j]);
		
  }
	printf("\n");
}
printf("\n");

op.sem_num=0;
op.sem_op=+1;
op.sem_flg=SEM_UNDO;
semop(semid, &op, 1);

op1.sem_num=0;
op1.sem_op=+1;
op1.sem_flg=SEM_UNDO;
semop(semid2, &op1, 1);


//op.sem_num=0;
//op.sem_op=+1;
//op.sem_flg=SEM_UNDO;
//semop(semid, &op, 1);
//op1.sem_op=+1;

k++;
}



return 0;
}

 /********************
 * VR473361
 * Sara Goldoni
 * 23/01/22
 * mulinoClient.c
 *********************/














