#include <stdio.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/sem.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

int shmid, shmidgettone,semid,semid2,shmidpid,pid1, pid2;
int (*p)[2];
char (*g)[5], pedina1, pedina2;
//FUNZIONE PER ELIMINARE SEMAFORI E MEMORIE
void elimina(int pid1, int pid2) {
kill(pid1,SIGQUIT);
kill(pid2, SIGQUIT);
_exit(-1);	
}

//FUNZIONE PER CHIUDERE TUTTO
void chiudi (int n)
{	
	elimina(pid1, pid2);
}

void tavolino(int n){
kill(pid2,SIGUSR1);
kill(pid1,SIGUSR2);
exit(0);
}

void tavolino2(int n){
kill(pid1,SIGUSR1);
kill(pid2,SIGUSR2);

}
//FUNZIONE PER CAPTARE IL CTRL+C
void primapressione(int n){
printf("\n");
printf("Hai premuto Ctrl+C, schiaccialo nuovamente per chiudere il programma\n");
fflush(stdout);
signal(SIGINT, chiudi);
}

//FUNZIONE CHE SPEGNE PERCHÈ SCADUTO IL TEMPO
void spegni(int n){
kill(pid1, SIGALRM);
kill(pid2,SIGALRM);
}


//MAIN

int main(int argc, char *argv[]){
int a;
int shmid, shmidgettone,semid,semid2;
char gettone1[1];
if (argc<2){
	printf("Non hai inserito i simboli da far usare ai giocatori.\nRiesegui il programma inserendoli separati da spazio --> ./mulinoServer X O\n");
	_exit(0);
}
//creazione memoria condivisa
//TABELLONE
 shmid= shmget(1611,sizeof(char)*49,IPC_CREAT|IPC_EXCL|0644);
 char (*c)[7] = shmat(shmid,NULL,0);
//PEDINE
 shmidgettone=shmget(1612, sizeof(char)*5, IPC_CREAT|IPC_EXCL|0644);
 char (*g)[5]= shmat(shmidgettone,NULL,0);
//PID
shmidpid=shmget(1613, sizeof(int)*3,IPC_CREAT|IPC_EXCL|0644);
 int (*p)[3]=shmat(shmidpid,NULL,0);
 *p[0]=' ';
 *p[1]=' ';
 *p[2]=getpid();

   if ( shmidgettone < 0 ) {
    perror("Errore creazione memoria condivisa: ");
    _exit(-1);
  }

  *g[0]=*argv[1];
  *g[1]=*argv[2];
pedina1=*argv[1];
pedina2=*argv[2];
 //controllo crezione memoria tabellone
 	  if ( shmid < 0 ) {
    perror("Errore creazione memoria condivisa: ");
    _exit(-1);
  }
 //controllo creazione memoria gettone
 	  if ( shmidgettone< 0 ) {
    perror("Errore creazione memoria condivisa: ");
    _exit(-1);
  }
  //controllo memoria pid
 
     if ( shmidpid < 0 ) {
    perror("Errore creazione memoria condivisa: ");
    _exit(-1);
  }
 //inizializzo il campo da gioco	
 int i, j, somma;
 	
 for (i=0; i<7;i++)
 {
 	for (j=0; j<7; j++)
 	{
 		
 		*c[i*7+j]=' ';
 		*c[i*7+3]='-';
 		*c[3*7+j]='-';
 		*c[3*7+3]= ' ';
 		somma= i+j;
 		if ( i == j)
    		{
    			*c[i*7+j]='-';
    		}
    		if (somma == 6)
    		{
    			*c[i*7+j]='-';
    		}
    			
 	}
 }
signal(SIGALRM, spegni);


//CREAZIONE SEMAFORI
semid	= semget(1234, 1, IPC_CREAT|0777);
semid2 = semget(12345,1, IPC_CREAT|0777);
//inizializzazione semafori
unsigned short initarray[1];
initarray[0]=1;
semctl(semid, 1, SETALL,initarray);
semctl(semid2,1,SETALL,initarray);
struct sembuf op,op1; 
char ch;
int l=0;

//CATCH SEGNALE
signal(SIGINT, primapressione);
signal(SIGUSR1,tavolino);
signal(SIGUSR2, tavolino2);
signal(SIGTERM, tavolino);
signal(SIGQUIT, tavolino2);

while(*p[0]==' ' || *p[1]==' '){
 	 initarray[0]=0;
  	semctl(semid2,1,SETALL,initarray);
  }
 
  
  	op1.sem_num=0;
	op1.sem_op=+1;
	op1.sem_flg=SEM_UNDO;

	semop(semid2,&op1,1);
  
while (l<=24){
alarm(20);
op.sem_num=0;
op.sem_op=-1;
op.sem_flg=0;
semop(semid, &op, 1);
alarm(0);
int contatorex, contatoreo, riga, col;
riga=0;
//controllo di vincita sulle righe
   	
//alarm(20);
   	while (riga<7){
   		for (col=0;col<7;col++){
   			//controllo X
   			if(*c[riga*7+col]==*argv[1]){
   			contatorex ++;
   			if(riga==3 && col==2 && contatorex!=3){
   				contatorex=0;
   				contatoreo=0;
   			}else if( riga == 3 && contatorex==3){
   			
   				printf("VITTORIA per giocatore con simbolo %s\n", argv[1]);
   				
   				kill(pid1, SIGUSR1);
   				kill(pid2, SIGUSR2);
   				_exit(0);
   			}
   			
   			}
   			//controllo O
   			else if(*c[riga*7+col]==*argv[2]){
   			 contatoreo ++;
   			 if(riga==3 && col==2 && contatoreo!=3){
   				contatorex=0;
   				contatoreo=0;
   			}else if( riga == 3 && contatoreo==3){
   			
   			printf("VITTORIA per giocatore con simbolo %s\n", argv[2]);
   				
   				kill(pid2, SIGUSR1);
   				kill(pid1, SIGUSR2);
   				_exit(0);
   			}
   			}
   			
   			if (riga!=3 && contatorex == 3){
   				printf("VITTORIA per giocatore con simbolo %s\n", argv[1]);
   			
   				kill(pid2, SIGUSR2);
   				kill(pid1, SIGUSR1);
   				//elimina(shmid, semid,semid2,shmidgettone,shmidpid, pid1, pid2);
   				_exit(0);
   			} 
   			else if(riga!=3 && contatoreo==3){
   				printf("VITTORIA per giocatore con simbolo %s\n", argv[2]);
   				
   				kill(pid1, SIGUSR2);
   				kill(pid2, SIGUSR1);
   				//elimina(shmid, semid,semid2,shmidgettone,shmidpid, pid1, pid2);
   				_exit(0);
   			}
   			
   		}
   		contatorex=0;
   		contatoreo=0;
   		riga++;
   	}
   int colonna=0;
 // controllo vincita sulle colonne
 	while (colonna<7){
   		for (riga=0;riga<7;riga++){
   		//controllo X
   			if(*c[riga*7+colonna]==*argv[1]){
   			contatorex ++;
   			}
   			 if(colonna==3 && riga== 2 && contatorex!=3){
   				contatorex=0;
   				contatoreo=0;
   			}else if(colonna==3 && contatorex==3){
   			
   			printf("VITTORIA per giocatore con simbolo %s\n", argv[1]);
   				//*g[4]=*argv[1];
   				kill(pid2, SIGUSR2);
   				kill(pid1,SIGUSR1);
   				_exit(0);
   			}
   			//controllo O
   			else if(*c[riga*7+colonna]==*argv[2]){
   			 contatoreo ++;
   			} if(colonna==3 && riga==2 &&  contatoreo!=3){
   				contatorex=0;
   				contatoreo=0;
   			}else if( colonna==3 && contatoreo==3){
   			
   			printf("VITTORIA per giocatore con simbolo %s\n", argv[2]);
   				
   				kill(pid1, SIGUSR2);
   				kill(pid2,SIGUSR1);
   				_exit(0);
   			}
   			if (colonna!=3 && contatorex ==3){
   				printf("VITTORIA per giocatore con simbolo %s\n", argv[1]);
   				
   				kill(pid1,SIGUSR1);
   				kill(pid2,SIGUSR2);
   				//elimina(shmid, semid,semid2,shmidgettone,shmidpid, pid1, pid2);
   				_exit(0);
   			} 
   			else if(colonna!=3 && contatoreo==3){
   				printf("VITTORIA per giocatore con simbolo %s\n", argv[2]);
   				
   				kill(pid1,SIGUSR2);
   				kill(pid2, SIGUSR1);
   				//elimina(shmid, semid,semid2,shmidgettone,shmidpid, pid1,pid2);
   				_exit(0);
   			}
   		}
   		
   		contatorex=0;
   		contatoreo=0;
   		colonna++;
   	}
   	pid1= *p[0];
   	pid2=*p[1];  
   	if (l==24){
   	
   	
		kill(pid1, SIGTERM);
		kill(pid2, SIGTERM);
		//printf("PAREGGIO\n");
   		fflush(stdout);
   		
	
   	}
   	//ch=getchar();
//op1.sem_num=0;
//op1.sem_op=+1;
//op1.sem_flg=SEM_UNDO;
//semop(semid2, &op1, 1);

l++;

}

elimina(pid1, pid2);
 return 0;
 }
 
 /********************
 * VR473361
 * Sara Goldoni
 * 23/01/22
 * mulinoServer.c
 *********************/


 
 
 
 
 
 

 
 
