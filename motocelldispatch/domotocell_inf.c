#include "domotocell_inf.h"
#include "domotocell.h"

void motocell_init(void) {
    int status;
    motocell_mq_msg_t send_msg;
    
    send_msg.type = MOTOCELL_INIT;
    pthread_mutex_lock(&mut_motocell);
        mclmsgcnt.gmotocellmsg++;
        status = mq_send(motocell_mq, (const char*)&send_msg, sizeof(send_msg), 1);
        printf("[Debug] mq_send status = %d\n", status);
        printf("[Debug] mq_send gmotocellmsg = %d\n", mclmsgcnt.gmotocellmsg);
    pthread_mutex_unlock(&mut_motocell);
}

uint8_t moto;
    uint8_t action;
    uint8_t mode;
    int32_t position;
    uint32_t speed;
    uint32_t acc;

void motor_rising_exec(uint8_t moto, uint8_t action, uint8_t mode, int32_t position, uint32_t speed, uint32_t acc){
    int status;
    motocell_mq_msg_t send_msg;

    send_msg.type = MOTOR_EXEC;
    send_msg.data.mtcl_spd.moto = moto;
    send_msg.data.mtcl_spd.action = action;
    send_msg.data.mtcl_spd.mode = mode;
    send_msg.data.mtcl_spd.position = position;
    send_msg.data.mtcl_spd.speed = speed;
    send_msg.data.mtcl_spd.acc = acc;

    pthread_mutex_lock(&mut_motorisingmsg);
        mclmsgcnt.gmotorisingmsg++;
        status = mq_send(motorising_mq, (const char*)&send_msg, sizeof(send_msg), 1);
    pthread_mutex_unlock(&mut_motorisingmsg);
}

void motor_rotation_exec(uint8_t moto, uint8_t action, uint8_t mode, int32_t position, uint32_t speed, uint32_t acc){
    int status;
    motocell_mq_msg_t send_msg;

    send_msg.type = MOTOR_EXEC;
    send_msg.data.mtcl_spd.moto = moto;
    send_msg.data.mtcl_spd.action = action;
    send_msg.data.mtcl_spd.mode = mode;
    send_msg.data.mtcl_spd.position = position;
    send_msg.data.mtcl_spd.speed = speed;
    send_msg.data.mtcl_spd.acc = acc;

    pthread_mutex_lock(&mut_motorotationmsg);
        mclmsgcnt.gmotorotationmsg++;
        status = mq_send(motorotation_mq, (const char*)&send_msg, sizeof(send_msg), 1);
    pthread_mutex_unlock(&mut_motorotationmsg);
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




