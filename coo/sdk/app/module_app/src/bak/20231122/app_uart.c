/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: APP UART handle.
 */

#include "app_uart.h"
#include "app_main.h"
#include "app_proto.h"
#include "app_common.h"
#include "soc_mdm_types.h"
#include "soc_mdm_sem.h"
#include "soc_stdlib.h"
#include "soc_mdm_uart.h"
#include "stdio.h"
#include "soc_mdm_task.h"
#include "soc_mdm_timecnt.h"
#include "app_cmd_msg.h"

#ifdef __cplusplus
extern "C" {
#endif


EXT_PRV ext_uart_port g_app_uart_port = EXT_UART_0;
EXT_PRV ext_uart_init_type g_app_uart_cfg = { 230400, 8, 1, 0 }; //test for uart---plc


td_u32 app_uart_init(td_void)
{
    td_u32 ret;

    uapi_uart_close(g_app_uart_port);
    ret = uapi_uart_open(g_app_uart_port);
    ret |= uapi_uart_ioctl(g_app_uart_port, UART_CFG_SET_ATTR, (uintptr_t)&g_app_uart_cfg);
    if (ret != EXT_ERR_SUCCESS) {
        app_printf("open uart %d failed.\n", g_app_uart_port);
        return EXT_ERR_FAILURE;
    }
    
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
//    app_timer_start(APP_TIMER_ID_UART, 5000, EXT_TIMER_TYPE_PERIOD);  
#endif    

    return EXT_ERR_SUCCESS;
}

//test for uart---plc

EXT_PRV td_void app_uart_rx(app_data_buf *buf_head, td_u8 ch)
{
//#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//    uapi_sleep(APP_UART_SLEEP_DURTION_MS); return;
//#endif    
    uapi_sem_wait(buf_head->sem_id,EXT_SYS_WAIT_FOREVER);

    if (buf_head->rx_length < APP_DATA_BUF_SIZE) {
        buf_head->buffer[buf_head->rx_length] = ch;
        buf_head->rx_length++;		
    }	
    
    if(buf_head->rx_length == APP_DATA_BUF_SIZE) {
    
 	ext_sys_queue_msg msg;
		
	memset_s(&msg, sizeof(msg), 0, sizeof(msg));
	msg.msg_id = ID_APP_MSG_UART_RX;		
	app_send_message(&msg); 
	app_timer_stop(APP_TIMER_ID_UART);
    
    }
    else {
        app_timer_start(APP_TIMER_ID_UART, 20, EXT_TIMER_TYPE_ONCE);   
    }
    
    uapi_sem_signal(buf_head->sem_id);   
}

td_void app_uart_task_body(td_u32 param)
{
    app_printf("app_uart_task_body\n");
    app_data_buf *buf_head = (app_data_buf *)(uintptr_t)param;
    td_s32 len;
    td_u8 ch = 0;

    for (;;) {
        len = uapi_uart_read(g_app_uart_port, &ch, 1);
        if (len == 1) {
            app_uart_rx(buf_head, ch);
            continue;
        }
        uapi_sleep(10/*APP_UART_SLEEP_DURTION_MS*/);
    }
}

//test for uart---plc

td_void app_uart_on_timer(td_u32 param)
{
/*
{
    app_data_buf *buf_head = (app_data_buf *)(uintptr_t)param;
    td_u8  testbuf[] ={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
     	       
    for(td_u32 i=0;i< 10;i++) {

	for(td_u32 j=0;j< sizeof(testbuf);j++) {
	       
	    if (buf_head->rx_length < APP_DATA_BUF_SIZE) {
	 	buf_head->buffer[buf_head->rx_length] = testbuf[j];
		buf_head->rx_length++;		
	    } 
	}
    }
}    
*/
{
    ext_sys_queue_msg msg;
    
    if(!param){;}
	
	
    memset_s(&msg, sizeof(msg), 0, sizeof(msg));
    msg.msg_id = ID_APP_MSG_UART_RX;		
    app_send_message(&msg);
} 
}


td_u32 app_uart_tx(td_pbyte data, td_u16 data_length)
{
    if (data == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    if (uapi_uart_write(g_app_uart_port, data, data_length) != (td_s32)data_length) {
        app_printf("app_uart_tx(): send failed.\n");
        return EXT_ERR_FAILURE;
    }

    return EXT_ERR_SUCCESS;
}

#ifdef __cplusplus
}
#endif

