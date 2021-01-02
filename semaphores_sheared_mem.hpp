#ifndef SEMAPHORE_SHARED_MEM
#define SEMAPHORE_SHARED_MEM
#include <sys/shm.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <fstream>

#define sh_mem_p1_key_file "SH_MEM_PM1"
#define sh_mem_p1_size_file sizeof(stats*)

#define sh_mem_p2_key_file "SH_MEM_PM2"
#define sh_mem_p2_size_file sizeof(stats*)

#define sh_mem_counter_key_file "SH_MEM_COUNTERS"
#define sh_mem_counter_size_file sizeof(int)

#define p1_shem_key_file "p1_sem"
#define p2_shem_key_file "p2_sem"

void die(char er[1000]);


int generate_memory_segment(key_t mem_key, size_t mem_sz, char* file_name){
    int mem_seg_id,mem_fd;
    /* Allocate a shared memory id. */
    mem_seg_id = shmget (mem_key, mem_sz, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    if(mem_seg_id < 0)die("can not allocate memory segment");
    #if DEBUG>= 2
        else cout<<"! alocated memory with id "<<mem_seg_id<<endl;
        cout<<"! making shared memory file and writing key\n";
    #endif
    /*creating file for memory key*/
    mem_fd = open(file_name, O_WRONLY | O_TRUNC | O_EXCL | O_CREAT, 0644);
    if (mem_fd < 0)die("Could not open mem.id");
    /*writing memory id*/
    if (write(mem_fd, &mem_key, sizeof(key_t)) < 0) die("Could not write key to file");
    close(mem_fd);
    return mem_seg_id;
}

int get_memory_id_from_file(char* file_name, int mem_sz){
    int mem_fd,mem_seg_id;
    key_t mem_key;
    /*Recover mem_key from file*/
    mem_fd = open(file_name, O_RDONLY);
    if (mem_fd < 0) die("Could not open memory file for reading");
    if (read(mem_fd, &mem_key, sizeof(int)) != sizeof(int)) die("Error reading the memory key");
    close(mem_fd);
    /*optain mem id*/
    mem_seg_id=shmget(mem_key, mem_sz, 0666);
    #if DEBUG>= 2
        cout<<"- opened file and got memory id "<<mem_seg_id<<endl;
    #endif
    return mem_seg_id;
}

void clear_mem(char* mem_file, int mem_sz){
    /* Deallocate the shared memory segment. */
    if(shmctl (get_memory_id_from_file(mem_file,mem_sz), IPC_RMID, 0) < 0)die("can not dealocate memory");
    #if DEBUG >= 2
        cout<<"!! deleted memory !!\n";
    #endif
    //removing memory file
    if (unlink(mem_file) < 0) die("Could not unlink memory file ");
    #if DEBUG >= 2
        else printf("!! unlinked memory file !!\n");
    #endif
}


////semaphore////
int generate_semaphore(key_t sem_key, char* file_name, unsigned int num=0){
    char final_file_name[50]="\0";
    if(num!=0)
        sprintf(final_file_name,"%u",num);
    strcat(final_file_name,file_name);

    int sem_fd,sem_id;
    /*make file for semaphore key*/
    #if DEBUG >= 2
        cout<<"!! making shemaphore file and writing key!!\n";
    #endif
    sem_fd = open(final_file_name, O_WRONLY | O_TRUNC | O_EXCL | O_CREAT, 0644);
    if (sem_fd < 0)die("Could not open sem.key");
    /*write the semaphore key*/
    if (write(sem_fd, &sem_key, sizeof(key_t)) < 0) die("Could not write key to file");
    close(sem_fd);
    /*creating shemapgore*/
    sem_id = semget(sem_key, 1, IPC_CREAT | IPC_EXCL | 0600);
    if (sem_id < 0) die("Could not create sem");
    #if DEBUG >= 2
        else  cout<<"!! created semaphore!! "<<sem_id << endl;
    #endif

    return sem_id;
}

int get_semaphore_id_from_file(char* file_name, unsigned int num=0){
    char final_file_name[50]="\0";
    if(num!=0)
        sprintf(final_file_name,"%u",num);
    strcat(final_file_name,file_name);

    int sem_fd, sem_id;
    key_t sem_key;
    /*Recover sem_key from file*/
    sem_fd = open(final_file_name, O_RDONLY);
    if (sem_fd < 0) die("Could not open sem key for reading");
    if (read(sem_fd, &sem_key, sizeof(key_t)) != sizeof(key_t)) die("Error reading the semaphore key");
    close(sem_fd);
    // Now obtain the (hopefully) existing sem
    sem_id = semget(sem_key, 0, 0);
    if (sem_id < 0) die("Could not obtain semaphore");
    #if DEBUG >= 2
    cout<<"- opened file and got semaphore "<<sem_id<<endl;
    #endif
    return sem_id;
}

void initialise_semaphore(int sem_id){
    /*seting shemaphore to 0*/
    if(semctl(sem_id, 0 , SETVAL, 0) < 0) die("Could not set value of semaphore");
    #if DEBUG >= 1
        else cout<<" and set to 1\n";
    #endif
}

static int semaphore_wait(int sem_id){
    #if DEBUG  >= 1
        cout<<"!! semaphore "<<sem_id<<"waiting!!\n";
    #endif
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1; /* P() */
    sem_b.sem_flg = 0;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_p failed\n");
        return(0);
    }
    return(1);
}

static int semaphore_signal(int sem_id){
    #if DEBUG >= 1
        cout<<"!! semaphore "<<sem_id<<"releasing !! \n";
    #endif
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1; /* V() */
    sem_b.sem_flg = 0;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_v failed\n");
        return(0);
    }
    return(1);
}

void clear_sem(char* sem_file, unsigned int num=0){
    char final_file_name[50]="\0";
    if(num!=0)
        sprintf(final_file_name,"%u",num);
    strcat(final_file_name,sem_file);

    /*deleting semaphore*/
    if (semctl(get_semaphore_id_from_file(final_file_name), 0, IPC_RMID) < 0)die("Could not delete semaphore");
    #if DEBUG >= 1
        else cout<<"!! deleted semaphore!!\n";
    #endif
    //removing semaphore file
    if (unlink(final_file_name) < 0) die("Could not unlink key file");
    #if DEBUG >= 1
        else printf("!! unlinked semaphore file!!\n");
    #endif
}


void initialized_all_shared_memmory_semaphores(){

    //~~~~~~~~~~~~~~~~Generate_keys~~~~~~~~~~~~~~~~~~~~~~//
        key_t sh_mem_p1_key =ftok("main",1);
        key_t sh_mem_p1_size =ftok("main",2);
        key_t sh_mem_p2_key =ftok("main",3);
        key_t sh_mem_p2_size =ftok("main",4);
        key_t sh_mem_counter_key =ftok("main",5);
        key_t sh_mem_counter_size =ftok("main",6);
        key_t p1_shem_key=ftok("main",7);
        key_t p2_shem_key =ftok("main",8);



    //~~~~~~~~~~~~~~~~Shared_memory~~~~~~~~~~~~~~~~~~~~~~//
        //Shared_memory for P1
        generate_memory_segment(sh_mem_p1_key,sh_mem_p1_size,sh_mem_p1_key_file);
        int P_mem_id=get_memory_id_from_file(sh_mem_p1_key_file,sh_mem_p1_size);
        char* mem_P = (char*) shmat(P_mem_id, NULL, 0);
        if(mem_P==(void*)-1)die("shared memory main");
        #if DEBUG >= 2
            printf ("!! shared mem P1 attached at main and initialized !!\n");
        #endif

        //Shared_memory for P2
        generate_memory_segment(sh_mem_p2_key,sh_mem_p2_size,sh_mem_p2_key_file);
        int P_ENC_mem_id=get_memory_id_from_file(sh_mem_p2_key_file,sh_mem_p2_size);
        char* mem_P_ENC = (char*) shmat(P_ENC_mem_id, NULL, 0);
        if(mem_P_ENC==(void*)-1)die("shared memory main");
        #if DEBUG >= 2
            printf ("!! shared mem P2 attached at main and initialized !!\n");
        #endif

        //Shared_memory for Stats
        generate_memory_segment(sh_mem_counter_key,sh_mem_counter_size,sh_mem_counter_key_file);
        int ENC_CHAN_mem_id=get_memory_id_from_file(sh_mem_counter_key_file,sh_mem_counter_size);
        char* mem_ENC_CHAN = (char*) shmat(ENC_CHAN_mem_id, NULL, 0);
        if(mem_ENC_CHAN==(void*)-1)die("shared memory main");
        #if DEBUG >= 2
            printf ("!! shared mem stats attached at main and initialized !!\n");
        #endif


    //~~~~~~~~~~~~~~~~Semaphore~~~~~~~~~~~~~~~~~~~~~~~~~//
        int sem_p1=generate_semaphore(p1_shem_key,p1_shem_key_file);
        initialise_semaphore(sem_p1);
        int sem_p2=generate_semaphore(p2_shem_key,p2_shem_key_file);
        initialise_semaphore(sem_p2);


}


void die(char er[1000]){//for handling errors
    int fd = open("error.txt", O_WRONLY | O_TRUNC | O_EXCL | O_CREAT, 0644);
    write(fd, er, strlen(er));
    close(fd);
    perror(er);
    exit(1);
}



#endif
