#include "recvdispatch.h"
#include "domotocell.h"

pthread_t ptrecvdispatch;
pthread_t ptdomotocell;

void thread_handler_close(void) {
    pthread_cancel(ptrecvdispatch);
    pthread_cancel(ptdomotocell);

    mq_close(motocell_mq);
    mq_unlink(MOTOCELL_MQ_NAME);
}

int main(void) {
    //pthread_attr_t attr;
    int status; 

    recvdispatch_init();
    domotocell_init();

    //pthread_attr_init(&attr);
    //pthread_attr_setstacksize(&attr, 1024*1024);
    
    printf("creating the recv command and dispatched pthread\n");
    status = pthread_create(&ptrecvdispatch, NULL/*&attr*/, (void*)&ptrecvdispatch_main, NULL);
    if (status != 0) {
        printf("Failed to create ptrecvdispatch with status = %d\n", status);
    }    

    printf("creating do motocell action thread\n");
    status = pthread_create(&ptdomotocell, NULL/*&attr*/, (void*)&ptmotoldcell_main, NULL);
    if (status != 0) {
        printf("Failed to create ptdomotocell thread with status = %d\n", status);
    }

    pthread_join(ptrecvdispatch, NULL);
    pthread_join(ptdomotocell, NULL);

    return 0;
}

