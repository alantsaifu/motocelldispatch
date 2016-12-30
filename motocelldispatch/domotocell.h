#ifndef CAM_H
#define CAM_H

#include <pthread.h>
#include <mqueue.h>
#include <fcntl.h>
#include "domotocell_inf.h"
#include <stdio.h>

#define MOTOCELL_MQ_NAME "/motocell_mq"

extern struct mq_attr motocell_mq_attr;
extern mqd_t motocell_mq;

typedef enum motocell_state_id_s {
    MOTOCELL_STATE_NOT_READY=0,
    MOTOCELL_STATE_READY,
    LOADCELL_STATE_CHK_STA,
    MOTOR_STATE_CHK_STA
} motocell_state_id_t;

typedef struct motocell_state_s {
    motocell_state_id_t state_id;
    //struct timeval timeout_ts;
} motocell_state_t;

void ptmotoldcell_main(void);
void motocell_int_msg_handler(void);
void loadcell_chk_sta_msg_handler(void);
void motor_chk_sta_msg_handler(void);

#endif
