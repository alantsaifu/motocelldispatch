#ifndef CAM_INF_H
#define CAM_INF_H

#include <pthread.h>
#include <mqueue.h>
#include <fcntl.h>

typedef enum cam_msg_id_s {
    MOTOCELL_INIT = 0,
    MOTOR_CHK_STA,
    LOADCELL_CHK_STA,
    MOTOR_EXEC,
    LOADCELL_EXEC
} motocell_msg_id_t;

typedef union motocell_speed_data_s {
    unsigned int speed;
} motocell_speed_data_t;

typedef union motocell_data_s {
    motocell_speed_data_t mtcl_spd;
} motocell_data_t;

typedef struct motocell_mq_msg_s {
    motocell_msg_id_t type;
    motocell_data_t data;
} motocell_mq_msg_t;

void motocell_init(void);
void loadcell_chk_sta(void);
void motor_chk_sta(void);
void motor_exec(void);
void loadcell_exec(void);

#endif
