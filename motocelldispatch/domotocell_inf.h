#ifndef CAM_INF_H
#define CAM_INF_H

#include <pthread.h>
#include <mqueue.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>

typedef enum cam_msg_id_s {
    MOTOCELL_INIT = 0,
    MOTOR_CHK_STA,
    LOADCELL_CHK_STA,
    MOTOR_EXEC,
    LOADCELL_EXEC
} motocell_msg_id_t;

#if 0
typedef union motocell_speed_data_s {
    uint8_t moto;
    uint8_t action;
    uint8_t mode;
    int32_t position;
    uint32_t speed;
    uint32_t acc;
} motocell_speed_data_t;

typedef union motocell_data_s {
    motocell_speed_data_t mtcl_spd;
} motocell_data_t;
#endif

typedef struct motocell_speed_data_s {
    uint8_t moto;
    uint8_t action;
    uint8_t mode;
    int32_t position;
    uint32_t speed;
    uint32_t acc;
} motocell_speed_data_t;

typedef struct motocell_data_s {
    motocell_speed_data_t mtcl_spd;
} motocell_data_t;

typedef struct motocell_mq_msg_s {
    motocell_msg_id_t type;
    motocell_data_t data;
} motocell_mq_msg_t;

void motocell_init(void);
void loadcell_chk_sta(void);
void motor_chk_sta(void);
void motor_exec(uint8_t moto, uint8_t action, uint8_t mode, int32_t position, uint32_t speed, uint32_t acc);
void loadcell_exec(void);

#endif
