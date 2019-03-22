/*
 *
 *  roof_client.c: Writes cmd to be run in POSIX shared memory object
 *                 
 */
#include "roof_client.h"

// Setup shared memory and semaphores
void client_setup()
{   
    //  mutual exclusion semaphore, mutex_sem 
    if ((mutex_sem = sem_open (SEM_MUTEX_NAME, 0, 0, 0)) == SEM_FAILED)
        error ("sem_open");

    // Get shared memory 
    if ((fd_shm = shm_open (SHARED_MEM_NAME, O_RDWR, 0)) == -1)
        error ("shm_open");

    if ((shm_ptr = mmap (NULL, sizeof (struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED,
            fd_shm, 0)) == MAP_FAILED)
        error ("mmap");

    // counting semaphore, indicating the number of available cmd slots.
    if ((cmd_count_sem = sem_open (SEM_CMD_COUNT_NAME, 0, 0, 0)) == SEM_FAILED)
        error ("sem_open");

    // counting semaphore, indicating the number of cmds to be run.
    if ((spool_signal_sem = sem_open (SEM_SPOOL_SIGNAL_NAME, 0, 0, 0)) == SEM_FAILED)
        error ("sem_open");
}

//  Push the command to cmd spooler
void client_push_cmd(char ucmd)
{
    // get a cmd: P (cmd_count_sem);
    if (sem_wait (cmd_count_sem) == -1)
        error ("sem_wait: buffer_count_sem");

    /* There might be multiple clients. We must ensure that 
        only one client uses cmd_buf_idx at a time.  */
    
    // P (mutex_sem);
    if (sem_wait (mutex_sem) == -1)
        error ("sem_wait: mutex_sem");
    
    shm_ptr->cmd_buf[shm_ptr->cmd_buf_idx] = ucmd;
    (shm_ptr->cmd_buf_idx)++;
    
    if (shm_ptr->cmd_buf_idx == MAX_CMDS)
        shm_ptr->cmd_buf_idx = 0;
    
    // Release mutex sem: V (mutex_sem)
    if (sem_post (mutex_sem) == -1)
        error ("sem_post: mutex_sem");
    
    // Tell cmd spooler that there is a cmd to run: V (spool_signal_sem);
    if (sem_post (spool_signal_sem) == -1)
        error ("sem_post: (spool_signal_sem");
}

// Print system error and exit
void error (char *msg)
{
    perror (msg);
    exit (1);
}