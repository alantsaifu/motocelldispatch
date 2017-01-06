#ifndef CAM_H
#define CAM_H

#include <pthread.h>
#include <mqueue.h>
#include <fcntl.h>
#include "domotocell_inf.h"
#include <stdio.h>

#define MOTOCELL_MQ_NAME "/motocell_mq"
#define MOTORISING_MQ_NAME "/motorising_mq"
#define MOTOROTATION_MQ_NAME "/motorotation_mq"

typedef struct motocell_msgcnt_s {
    unsigned int gmotocellmsg;
    unsigned int gmotorisingmsg;
    unsigned int gmotorotationmsg;
} motocell_msgcnt_t;

extern struct mq_attr motocell_mq_attr;
extern mqd_t motocell_mq;

extern struct mq_attr motorising_mq_attr;
extern mqd_t motorising_mq;

extern struct mq_attr motorotation_mq_attr;
extern mqd_t motorotation_mq;

extern pthread_mutex_t mut_motocell;
extern pthread_mutex_t mut_motorisingmsg;
extern pthread_mutex_t mut_motorotationmsg;

extern motocell_msgcnt_t mclmsgcnt;

typedef enum motocell_state_id_s {
    MOTOCELL_STATE_NOT_READY=0,
    MOTOCELL_STATE_READY,
    LOADCELL_STATE_CHK_STA,
    MOTOR_STATE_CHK_STA,
    MOTOR_STATE_EXEC_ACT
} motocell_state_id_t;

typedef struct motocell_state_s {
    motocell_state_id_t state_id;
    //struct timeval timeout_ts;
} motocell_state_t;

void ptmotoldcell_main(void);
void motocell_int_msg_handler(void);
void loadcell_chk_sta_msg_handler(void);
void motor_chk_sta_msg_handler(void);
void motor_exec_act_msg_handler(void);

#endif
