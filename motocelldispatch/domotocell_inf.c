#include "domotocell_inf.h"
#include "domotocell.h"

void motocell_init(void) {
    int status;

    motocell_mq_msg_t send_msg;
    send_msg.type = MOTOCELL_INIT;
    status = mq_send(motocell_mq, (const char*)&send_msg, sizeof(send_msg), 1);
}

void motor_exec(void){
    int status;
    motocell_mq_msg_t send_msg;

    send_msg.type = MOTOR_EXEC;
    send_msg.data.mtcl_spd.speed = 0; 

    status = mq_send(motocell_mq, (const char*)&send_msg, sizeof(send_msg), 1);
}

void loadcell_exec(void){
    int status;
    motocell_mq_msg_t send_msg;

    send_msg.type = LOADCELL_EXEC;

    status = mq_send(motocell_mq, (const char*)&send_msg, sizeof(send_msg), 1);
}

void loadcell_chk_sta(void) {
    int status;
    motocell_mq_msg_t send_msg;

    send_msg.type = LOADCELL_CHK_STA;

    status = mq_send(motocell_mq, (const char*)&send_msg, sizeof(send_msg), 1);
}

void motor_chk_sta(void) {
    int status;
    motocell_mq_msg_t send_msg;

    send_msg.type = MOTOR_CHK_STA;

    status = mq_send(motocell_mq, (const char*)&send_msg, sizeof(send_msg), 1);
}




