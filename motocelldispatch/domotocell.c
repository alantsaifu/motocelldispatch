#include "domotocell.h"

struct mq_attr motocell_mq_attr;
mqd_t motocell_mq;

/* motor and loadcell state */
static motocell_state_t motocell_state;

void motocell_set_state(motocell_state_id_t new_cam_state) {
    motocell_state.state_id = new_cam_state;  
}

void domotocell_init(void) {

    motocell_mq_attr.mq_maxmsg = 10;
    motocell_mq_attr.mq_msgsize = sizeof(motocell_mq_msg_t);

    motocell_mq = mq_open(MOTOCELL_MQ_NAME, \
                    O_CREAT | O_RDWR | O_NONBLOCK, \
                    0666, \
                    &motocell_mq_attr);

    //ASSERT(cam_mq != -1);
    motocell_state.state_id = MOTOCELL_STATE_NOT_READY;
}

void ptmotoldcell_main(void) {
    unsigned int exec_period_usecs;
    int status;
    motocell_mq_msg_t recv_msg;

    exec_period_usecs = 1000;

    while(1) {
        status = mq_receive(motocell_mq, (char*)&recv_msg, \
                            sizeof(recv_msg), NULL);

        if (status == sizeof(recv_msg)) {

            switch(recv_msg.type) {
                //[Todo] Implement the action handle by state machine 
                case MOTOCELL_INIT:
                    motocell_int_msg_handler(); 
                    break;         
                case LOADCELL_STATE_CHK_STA:
                    loadcell_chk_sta_msg_handler();
                    break;         
                case MOTOR_STATE_CHK_STA:
                    motor_chk_sta_msg_handler();
                    break;         
                default:
                    printf("%s\n", "CAM Received invalid msg.");
                    break;         
            }
        }
 
        usleep(exec_period_usecs);
    }
}

void motocell_int_msg_handler(void) {
    if (motocell_state.state_id != MOTOCELL_STATE_NOT_READY) {
        return;
    } 
    motocell_set_state(MOTOCELL_STATE_READY);
}

void loadcell_chk_sta_msg_handler(void) {
    if (motocell_state.state_id != MOTOCELL_STATE_READY) {
        return;
    } 
    motocell_set_state(LOADCELL_STATE_CHK_STA);
}

void motor_chk_sta_msg_handler(void) {
    if (motocell_state.state_id != MOTOCELL_STATE_READY) {
        return;
    } 
    motocell_set_state(MOTOR_STATE_CHK_STA);
}
