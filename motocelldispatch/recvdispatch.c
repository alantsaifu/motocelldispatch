#include "recvdispatch.h"
#include "domotocell_inf.h"

mqd_t mqID;
struct mq_attr mqAttr;

void recvdispatch_init(void) {
    mqID = mq_open(MLDCL_MQ_NAME, O_RDONLY);
    if (mqID < 0){
        printf("open message queue %s error[%s]\n", MLDCL_MQ_NAME, strerror(errno));
    }  
}

void ptrecvdispatch_main(void) {
    unsigned int exec_period_usecs;
    int cnt=0;
    unsigned int iprio;
    unsigned int n;
    char buff[8192];

    pthread_detach(pthread_self());

    exec_period_usecs = 2000000; /*in micro-seconds*/

    while(1) {
#if 0        
        mq_getattr(mqID, &mqAttr);
        n = mq_receive(mqID, buff, mqAttr.mq_msgsize, &iprio);
        printf("read from mqs msg = %s\n", buff);
#endif
        switch (cnt) {
            case 0:
                motocell_init();
                break;
            case 1:
                //motor_chk_sta();
                break;
            case 2:
                //loadcell_chk_sta();
                break;
            case 3:
                motor_rising_exec(0, 1, 1, 1000, 1000, 1000);
                motor_rising_exec(0, 1, 1, -1000, 1000, 1000);
                break;
            case 4:
                //loadcell_exec();
                break;
            case 5:
                motor_rotation_exec(1, 1, 1, 500, 500, 500);
                motor_rotation_exec(1, 1, 1, -500, 500, 500);
                break;
            case 6:
                break;
            default:
                printf("Invalid counter value in sending thread\n");
                break;
        }
        
        cnt++;
        if(cnt > 6)
            cnt = 1;
        usleep(exec_period_usecs);
    }
}

