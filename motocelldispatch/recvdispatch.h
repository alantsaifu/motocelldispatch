#ifndef THREAD1_H
#define THREAD1_H

#include <pthread.h>
#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MLDCL_MQ_NAME "/mqldcl"

#define SUCCESS   (0)
#define WRONG_PAR (-1)
#define WRONG_DEV (-2)

void ptrecvdispatch_main(void);

#endif
