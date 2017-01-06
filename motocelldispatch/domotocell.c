#include "domotocell.h"
#include "recvdispatch.h"

struct mq_attr motocell_mq_attr;
mqd_t motocell_mq;

struct mq_attr motorising_mq_attr;
mqd_t motorising_mq;

struct mq_attr motorotation_mq_attr;
mqd_t motorotation_mq;

pthread_mutex_t mut_motocell;
pthread_mutex_t mut_motorisingmsg;
pthread_mutex_t mut_motorotationmsg;

motocell_msgcnt_t mclmsgcnt;

#define INIT_MLMQ(attr, mcell_mq, mcell_mq_name, mcell_mq_attr) do{\
        attr.mq_maxmsg = 1024;\
        attr.mq_msgsize = sizeof(motocell_mq_msg_t);\
        mcell_mq = mq_open(mcell_mq_name, O_RDWR | O_CREAT | O_EXCL, 0666, &attr);\
        if (mcell_mq < 0){\
                if (errno == EEXIST){\
                    mq_unlink(mcell_mq_name);\
                    mcell_mq = mq_open(mcell_mq_name, O_RDWR | O_CREAT, 0666, &attr);\
                    if(mcell_mq < 0){\
                        printf("open message queue %s error[%s]\n", mcell_mq_name, strerror(errno));\
                    }\
                }\
                else{\
                    printf("open message queue %s error[%s]\n", mcell_mq_name, strerror(errno));\
                }\
        }\
        if (mq_getattr(mcell_mq, &mcell_mq_attr) < 0){\
            printf("get the message queue attribute error\n");\
        }\
}while (0)

/* motor and loadcell state */
static motocell_state_t motocell_state;

void motocell_set_state(motocell_state_id_t new_cam_state) {
    motocell_state.state_id = new_cam_state;  
}

void domotocell_init(void) {

    struct mq_attr attr;

    INIT_MLMQ(attr, motocell_mq, MOTOCELL_MQ_NAME, motocell_mq_attr);
    INIT_MLMQ(attr, motorising_mq, MOTORISING_MQ_NAME, motorising_mq_attr);
    INIT_MLMQ(attr, motorotation_mq, MOTOROTATION_MQ_NAME, motorotation_mq_attr);

    motocell_state.state_id = MOTOCELL_STATE_NOT_READY;
}

void ptmotoldcell_main(void) {
    unsigned int exec_period_usecs;
    int status = 0;
    motocell_mq_msg_t recv_msg;
    unsigned int iprio;

    pthread_detach(pthread_self());
    exec_period_usecs = 1000000;

    while(1) {
        if(mclmsgcnt.gmotocellmsg){
            pthread_mutex_lock(&mut_motocell);
            mclmsgcnt.gmotocellmsg--;
            mq_getattr(motocell_mq, &motocell_mq_attr);
            status = mq_receive(motocell_mq, (char*)&recv_msg, \
                            sizeof(recv_msg), &iprio);
            printf("[Debug] sizeof(recv_msg) = %ld\n", sizeof(recv_msg));
            printf("[Debug] motocell_mq_attr.mq_msgsize = %ld\n", motocell_mq_attr.mq_msgsize);
            printf("[Debug] motocell_mq_attr.mq_maxmsg = %ld\n", motocell_mq_attr.mq_maxmsg);

            printf("[Debug] get bytes = %d\n", status);
            //printf("[Debug] sizeof(recv_msg) = %ld\n", sizeof(recv_msg));
            printf("[Debug] mq_receive gmotocellmsg = %d\n", mclmsgcnt.gmotocellmsg);
            pthread_mutex_unlock(&mut_motocell);
        }
        if(mclmsgcnt.gmotorisingmsg){
            pthread_mutex_lock(&mut_motorisingmsg);
            mclmsgcnt.gmotorisingmsg--;
            mq_getattr(motorising_mq, &motorising_mq_attr);
            status = mq_receive(motorising_mq, (char*)&recv_msg, \
                            sizeof(recv_msg), &iprio);
            printf("[Debug] mq_receive gmotorisingmsg = %d\n", mclmsgcnt.gmotorisingmsg);

            pthread_mutex_unlock(&mut_motorisingmsg);
        }
        if(mclmsgcnt.gmotorotationmsg){
            pthread_mutex_lock(&mut_motorotationmsg);
            mclmsgcnt.gmotorotationmsg--;
            mq_getattr(motorotation_mq, &motorotation_mq_attr);
            status = mq_receive(motorotation_mq, (char*)&recv_msg, \
                            sizeof(recv_msg), &iprio);
            printf("[Debug] mq_receive gmotorotationmsg = %d\n", mclmsgcnt.gmotorotationmsg);
#if 0
            printf("[Debug] recv_msg.type = %d\n", recv_msg.type);
            printf("[Debug] recv_msg.data.mtcl_spd.acc = %d\n", recv_msg.data.mtcl_spd.acc);
            printf("[Debug] recv_msg.data.mtcl_spd.action = %d\n", recv_msg.data.mtcl_spd.action);
            printf("[Debug] recv_msg.data.mtcl_spd.mode = %d\n", recv_msg.data.mtcl_spd.mode);
            printf("[Debug] recv_msg.data.mtcl_spd.moto = %d\n", recv_msg.data.mtcl_spd.moto);
            printf("[Debug] recv_msg.data.mtcl_spd.position = %d\n", recv_msg.data.mtcl_spd.position);
            printf("[Debug] recv_msg.data.mtcl_spd.speed = %d\n", recv_msg.data.mtcl_spd.speed);
#endif
            pthread_mutex_unlock(&mut_motorotationmsg);
        }

        if (status == sizeof(recv_msg)) {
            status = 0xFF;
            //while(1){            
                switch(recv_msg.type) {
                    case MOTOCELL_INIT:
                        printf("[Debug] recv_msg.type %s\n", "MOTOCELL_INIT");
                        motocell_int_msg_handler(); 
                        break;         
                    case LOADCELL_CHK_STA:
                        printf("[Debug] recv_msg.type %s\n", "LOADCELL_CHK_STA");
                        loadcell_chk_sta_msg_handler();
                        break;         
                    case MOTOR_CHK_STA:
                        printf("[Debug] recv_msg.type %s\n", "MOTOR_CHK_STA");
                        motor_chk_sta_msg_handler();
                        break;
                    case MOTOR_EXEC:
                        printf("[Debug] recv_msg.type %s\n", "MOTOR_EXEC");
                        motor_exec_act_msg_handler();
                        break;       
                    default:
                        printf("%s\n", "motocell Received invalid msg");
                        break;         
                }
            //}     
            //printf("[Debug] motocell_state.state_id %d\n", motocell_state.state_id);
        }
        
        switch(motocell_state.state_id) {
            case MOTOCELL_STATE_NOT_READY:
                printf("[Debug] %s\n", "=============== MOTOCELL_STATE_NOT_READY ==============="); 
                break;
            case MOTOCELL_STATE_READY:
                printf("[Debug] %s\n", "=============== MOTOCELL_STATE_READY ==============="); 
                motocell_set_state(MOTOR_STATE_CHK_STA);
                break;         
            case LOADCELL_STATE_CHK_STA:
                printf("[Debug] %s\n", "=============== LOADCELL_STATE_CHK_STA ===============");
                motocell_set_state(MOTOR_STATE_CHK_STA);
                break;         
            case MOTOR_STATE_CHK_STA:
                printf("[Debug] %s\n", "=============== MOTOR_STATE_CHK_STA ===============");
                motocell_set_state(LOADCELL_STATE_CHK_STA);
                break;
            case MOTOR_STATE_EXEC_ACT:
                printf("[Debug] %s\n", "=============== MOTOR_STATE_EXEC_ACT ===============");
                motocell_set_state(MOTOR_STATE_CHK_STA);
                //[Todo] see the motor action status : run ? stop
                break;    
            default:
                printf("[Debug] %s\n", "=============== motocell invalid status ===============");
                break;         
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
    if (motocell_state.state_id == MOTOCELL_STATE_NOT_READY) {
        return;
    } 
    motocell_set_state(LOADCELL_STATE_CHK_STA);
}

void motor_exec_act_msg_handler(void){
    if (motocell_state.state_id == MOTOCELL_STATE_NOT_READY) {
        return;
    }
    motocell_set_state(MOTOR_STATE_EXEC_ACT);
}

void motor_chk_sta_msg_handler(void) {
    if (motocell_state.state_id == MOTOCELL_STATE_NOT_READY) {
        return;
    }
    motocell_set_state(MOTOR_STATE_CHK_STA);
}
