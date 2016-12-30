#include "recvdispatch.h"
#include "domotocell_inf.h"

void recvdispatch_init(void) {
    //Nothing  
}

void ptrecvdispatch_main(void) {
    unsigned int exec_period_usecs;
    int cnt=0;

    exec_period_usecs = 1000; /*in micro-seconds*/

    while(1) {
        switch (cnt) {
            case 0:
                motocell_init();
                break;
            case 1:
                motor_chk_sta();
                break;
            case 2:
                loadcell_chk_sta();
                break;
            case 3:
                motor_exec();
                break;
            case 4:
                loadcell_exec();
                break;
            case 5:
                break;
            case 6:
                break;
            default:
                printf("Invalid counter value in sending thread\n");
                break;
        }
        
        cnt=1;
        usleep(exec_period_usecs);
    }
}

