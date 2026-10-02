#ifndef INC_MICROROS_H_
#define INC_MICROROS_H_

#include "main.h"

void Micro_ros_Init(void);
void Micro_ros_PUB_Init(void);
void Micro_ros_SUB_Init(void);
void subscriber_callback(const void * msgin);
void Micro_ros_SUB_Init(void);
void Micro_ros_Proc(void);

void encoder_publish(void);

void FB_con_Callbak(const void * msgin);
void LR_con_Callbak(const void * msgin);
void Wheel_rpm_Callback(const void * msgin);

void Micro_ros_spin(uint32_t time);

void service_callback(const void * req, void * res);
void Micro_ros_service_Init(void);


#endif /* INC_MICROROS_H_ */
