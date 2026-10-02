#include "MicroRos.h"
#include "usart.h"
//#include "CLI.h"
#include "cmsis_os.h"
#include "ModbusRTU_Master.h"
#include "ModbusRTU_app.h"

#include <stdbool.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <uxr/client/transport.h>
#include <rmw_microxrcedds_c/config.h>
#include <rmw_microros/rmw_microros.h>

#include <std_msgs/msg/int16.h>
#include <std_msgs/msg/int16_multi_array.h>
#include <std_msgs/msg/int32.h>
#include <std_msgs/msg/int32_multi_array.h>
#include <std_msgs/msg/u_int16.h>
#include <std_msgs/msg/string.h>
#include <std_msgs/msg/float32.h>
#include <std_msgs/msg/float64.h>
#include <example_interfaces/srv/add_two_ints.h>
#include <std_srvs/srv/set_bool.h>
#include <rosidl_runtime_c/string_functions.h>

#include <example_interfaces/srv/add_two_ints.h>

bool cubemx_transport_open(struct uxrCustomTransport * transport);
bool cubemx_transport_close(struct uxrCustomTransport * transport);
size_t cubemx_transport_write(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * err);
size_t cubemx_transport_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* err);

void * microros_allocate(size_t size, void * state);
void microros_deallocate(void * pointer, void * state);
void * microros_reallocate(void * pointer, size_t size, void * state);
void * microros_zero_allocate(size_t number_of_elements, size_t size_of_element, void * state);

extern osMessageQueueId_t IMUAngleHandle;
//extern osMessageQueueId_t Wheel0Handle;
//extern osMessageQueueId_t Wheel1Handle;
//extern osMessageQueueId_t FB_dataHandle;
//extern osMessageQueueId_t LR_dataHandle;

extern int16_t FB_ros_data, LR_ros_data;
extern int16_t LR_Last_data, FB_Last_data;
extern int16_t Wheel_data[2];
extern bool motor_disable;

uint8_t mode = 0;

//extern uint16_t ModbusRegister[NUMBER_OF_REGISTER];

rcl_allocator_t freeRTOS_allocator;
rcl_allocator_t allocator;
rcl_init_options_t init_options;
rmw_init_options_t* rmw_options;
rcl_ret_t ret;
rclc_support_t support;
rcl_node_t node;
rcl_publisher_t imu_pub;
rcl_publisher_t encoder_pub;
//std_msgs__msg__Int32 send_msg;
std_msgs__msg__Float64 send_msg;
std_msgs__msg__Int32MultiArray encoder_data;

rclc_executor_t executor;
rclc_executor_t executor1;

rcl_subscription_t FB_sub;
rcl_subscription_t LR_sub;
rcl_subscription_t Wheel_rpm_sub;

//std_msgs__msg__Int32 received_msg;
std_msgs__msg__Int16 FB_msg;
std_msgs__msg__Int16 LR_msg;
std_msgs__msg__Int16MultiArray rpm_msg;


rcl_service_t service;
std_srvs__srv__SetBool_Request req;
std_srvs__srv__SetBool_Request res;

#define Forward_data					30
#define Backward_data					-30

void Micro_ros_Init(void)
{
	rmw_uros_set_custom_transport(
		true,
		(void *) &huart6,
		cubemx_transport_open,
		cubemx_transport_close,
		cubemx_transport_write,
		cubemx_transport_read);

	freeRTOS_allocator = rcutils_get_zero_initialized_allocator();
	freeRTOS_allocator.allocate = microros_allocate;
	freeRTOS_allocator.deallocate = microros_deallocate;
	freeRTOS_allocator.reallocate = microros_reallocate;
	freeRTOS_allocator.zero_allocate =  microros_zero_allocate;

	if (!rcutils_set_default_allocator(&freeRTOS_allocator)) {
		printf("Error on default allocators (line %d)\n", __LINE__);
	}


	rmw_ret_t ping_result;
	while(ping_result != RMW_RET_OK)
	{
		ping_result = rmw_uros_ping_agent(1000, 60);
	}

	allocator = rcl_get_default_allocator();
	init_options = rcl_get_zero_initialized_init_options();

	ret = rcl_init_options_init(&init_options, allocator);

	if(ret != RCL_RET_OK) printf("Error options_init (line %d)\n", __LINE__);
	rmw_options = rcl_init_options_get_rmw_init_options(&init_options);
	rcl_init_options_set_domain_id(&init_options, 30);
	rmw_uros_options_set_client_key(0xCAABBBAD, rmw_options);

	ret = rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator);
	ret = rclc_node_init_default(&node, "KC_Balancing", "", &support);


}

void Micro_ros_PUB_Init(void)
{
	rclc_publisher_init_default(&imu_pub, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float64), "kc_imu_data");
	rclc_publisher_init_default(&encoder_pub, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32MultiArray), "encoder_wheel");
//	send_msg.data = 0;
}

extern double input;
extern int32_t aaaa[2];
int32_t data[2] = {0,};

void Micro_ros_Proc(void)
{


//	if(osMessageQueueGet(IMUAngleHandle, &send_msg, NULL, 0) == osOK)
//	{
//		ret = rcl_publish(&imu_pub, &send_msg, NULL);
//	}
	osMessageQueueGet(IMUAngleHandle, &send_msg, NULL, 0);
	ret = rcl_publish(&imu_pub, &send_msg, NULL);

//	Modbus_ReadHoldingRegister(0x01, 0x20A7, 4);
//	data[0] = ((int32_t)(ModbusRegister[0] << 16) | ModbusRegister[1]);
//	data[1] = ((int32_t)(ModbusRegister[2] << 16) | ModbusRegister[3]);

//	if((data[0] != aaaa[0]) || (data[1] != aaaa[1]))
	{
	//	encoder_data.data.data = data;
	//	encoder_data.data.size = sizeof(data);
		encoder_data.data.data = aaaa;
		encoder_data.data.size = sizeof(aaaa);
		ret = rcl_publish(&encoder_pub, &encoder_data, NULL);
		data[0] = aaaa[0];
		data[1] = aaaa[1];
	}
}

//void subscriber_callback(const void * msgin)
//{
//	const std_msgs__msg__Int32 * msg = (const std_msgs__msg__Int32 *)msgin;
//#if 0
//	Stepper_motor(0, msg->data);
//	test = msg->data;
//#else
//	printf("\r\n");
//	printf("\r\n");
//    printf("KC Hand Index: '%ld'\n", msg->data);
//    printf("Operation!!!!");
//    printf("\r\n");
//    printf("\r\n");
//#endif
//}

void FB_con_Callbak(const void * msgin)
{
//	const std_msgs__msg__Int16 * msg = (const std_msgs__msg__Int16 *)msgin;
	const std_msgs__msg__Float64 * msg = (const std_msgs__msg__Float64 *)msgin;
//	if(msg->data > Forward_data) data = Forward_data;
//	else if(msg->data < Backward_data) data = Backward_data;
	FB_ros_data = trunc(msg->data);

	if(FB_ros_data > 6) FB_ros_data = 6;
	else if(FB_ros_data < -6) FB_ros_data = -6;
//	osMessageQueuePut(FB_dataHandle, &data, 0, 0);
	mode = 1;
}

void LR_con_Callbak(const void * msgin)
{
//	const std_msgs__msg__Int16 * msg = (const std_msgs__msg__Int16 *)msgin;
	const std_msgs__msg__Float64 * msg = (const std_msgs__msg__Float64 *)msgin;
////	int16_t data;
//	double data;
////	if(msg->data != LR_Last_data)
//	{
//		if(FB_ros_data == 0)
//		{
//			if(msg->data > 20)
//			{
//				data = -20;
//			}
//			else if(msg->data < -20)
//			{
//				data = 20;
//			}
//			else
//			{
//				data = msg->data * -1;
//			}
//		}
//		else
//		{
//			if(msg->data > 20) data = 30;
//			else if(msg->data < -20) data = -30;
//		}
//	}
//	LR_ros_data = data;
////	osMessageQueuePut(LR_dataHandle, &LR_ros_data, 0, 0);

	LR_ros_data = trunc(msg->data);
	if(LR_ros_data != LR_Last_data)
	{
		if(LR_ros_data > 6) LR_ros_data = 6;
		else if(LR_ros_data < -6) LR_ros_data = -6;
	}
	mode = 1;
}

void Wheel_rpm_Callback(const void * msgin)
{
	const std_msgs__msg__Int16MultiArray * msg = (const std_msgs__msg__Int16MultiArray *)msgin;
	int16_t data[2];
#if 0
	if(msg->data.data[0] >= Forward_data) data[0] = Forward_data;
	else if(msg->data.data[0] <= Backward_data) data[0] = Backward_data;
	else data[0] = msg->data.data[0];

	if((msg->data.data[1] * -1) >= Forward_data) data[1] = Forward_data;
	else if((msg->data.data[1] * -1) <= Backward_data) data[1] = Backward_data;
	else data[1] = msg->data.data[1] * -1;
#else
	data[0] = msg->data.data[0];
	data[1] = msg->data.data[1] * -1;
#endif
	Wheel_data[0] = data[0];				//left
	Wheel_data[1] = data[1];				//right
//	osMessageQueuePut(Wheel0Handle, &Wheel_data[0], 0, 0);
//	osMessageQueuePut(Wheel1Handle, &Wheel_data[1], 0, 0);
	mode = 2;
}

void Micro_ros_SUB_Init(void)
{
//	rclc_subscription_init_default(&subscriber, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), "kchand_run");
//
//	rclc_executor_init(&executor, &support.context, 1, &allocator);
//	rclc_executor_add_subscription(&executor, &subscriber, &received_msg, &subscriber_callback, ON_NEW_DATA);

	rclc_subscription_init_default(&FB_sub, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float64), "amr_fb");
	rclc_subscription_init_default(&LR_sub, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float64), "amr_lr");
	rclc_subscription_init_default(&Wheel_rpm_sub, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int16MultiArray), "cmd_wheel_rpm");

	rpm_msg.data.capacity = 10;
	rpm_msg.data.size = 0;
	rpm_msg.data.data = (int16_t*) malloc(rpm_msg.data.capacity * sizeof(int16_t));

	rpm_msg.layout.dim.capacity = 10;
	rpm_msg.layout.dim.size = 0;
	rpm_msg.layout.dim.data = (std_msgs__msg__MultiArrayDimension*) malloc(rpm_msg.layout.dim.capacity * sizeof(std_msgs__msg__MultiArrayDimension));

	  for(size_t i = 0; i < rpm_msg.layout.dim.capacity; i++){
		  rpm_msg.layout.dim.data[i].label.capacity = 10;
		  rpm_msg.layout.dim.data[i].label.size = 0;
		  rpm_msg.layout.dim.data[i].label.data = (char*) malloc(rpm_msg.layout.dim.data[i].label.capacity * sizeof(char));
	  }

	rclc_executor_init(&executor, &support.context, 3, &allocator);
	rclc_executor_add_subscription(&executor, &FB_sub, &FB_msg, &FB_con_Callbak, ON_NEW_DATA);
	rclc_executor_add_subscription(&executor, &LR_sub, &LR_msg, &LR_con_Callbak, ON_NEW_DATA);
	rclc_executor_add_subscription(&executor, &Wheel_rpm_sub, &rpm_msg, &Wheel_rpm_Callback, ON_NEW_DATA);
}

void Micro_ros_spin(uint32_t time)
{
	rclc_executor_spin_some(&executor, RCL_MS_TO_NS(time));
	rclc_executor_spin_some(&executor1, RCL_MS_TO_NS(time));
}

void service_callback(const void * request, void * response)
{
    const example_interfaces__srv__AddTwoInts_Request * req = (const example_interfaces__srv__AddTwoInts_Request *)request;
    example_interfaces__srv__AddTwoInts_Response * res = (example_interfaces__srv__AddTwoInts_Response *)response;

    res->sum = req->a + req->b; // 요청 받은 두 수의 합 계산
//    printf("%d\n", res->sum);
}

void service_Tooque_EnDis_callback(const void * req, void * res)
{
    const std_srvs__srv__SetBool_Request *request = (const std_srvs__srv__SetBool_Request *) req;
    std_srvs__srv__SetBool_Response *response = (std_srvs__srv__SetBool_Response *) res;

    if (request->data)
    {
    	motor_disable = true;
        response->success = true;
    }
}

void Micro_ros_service_Init(void)
{
	rclc_service_init_default(
		&service,
		&node,
		ROSIDL_GET_SRV_TYPE_SUPPORT(std_srvs, srv, SetBool),
		"Tooque_EnDis");

	rclc_executor_init(&executor1, &support.context, 1, &allocator);
	rclc_executor_add_service(&executor1, &service, &req, &res, service_Tooque_EnDis_callback);
}




