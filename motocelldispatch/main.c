#include "recvdispatch.h"
#include "domotocell.h"

pthread_t ptrecvdispatch;
pthread_t ptdomotocell;

void thread_handler_close(void) {
    pthread_cancel(ptrecvdispatch);
    pthread_cancel(ptdomotocell);

    mq_close(motocell_mq);
    mq_unlink(MOTOCELL_MQ_NAME);

    mq_close(motorising_mq);
    mq_unlink(MOTORISING_MQ_NAME);

    mq_close(motorotation_mq);
    mq_unlink(MOTOROTATION_MQ_NAME);
}

int main(void) {    
    //pthread_attr_t attr;
    int status; 

    recvdispatch_init();
    domotocell_init();

    mclmsgcnt.gmotocellmsg = 0;
    mclmsgcnt.gmotorisingmsg = 0;
    mclmsgcnt.gmotorotationmsg = 0;

    pthread_mutex_init(&mut_motocell,NULL);
    pthread_mutex_init(&mut_motorisingmsg,NULL);
    pthread_mutex_init(&mut_motorotationmsg,NULL);

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

    //thread_handler_close();
#if 0
    pthread_join(ptrecvdispatch, NULL);
    pthread_join(ptdomotocell, NULL);
#endif
    while(1);
    return 0;
}

