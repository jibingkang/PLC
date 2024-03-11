/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: APP Router frame data handle.
 */

#ifndef APP_ROUTER_H
#define APP_ROUTER_H

#include <soc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_LEN 512-6

#define START_OFFSET 	0
#define TYPE_OFFSET 	1
#define LENGTH_OFFSET 	2
#define MAC_OFFSET		4
#define DATA_OFFSET		10
#define RF_DATA_OFFSET	4
#define DEFAULT_RSSI    -1
#undef DEBUG_LOG
#define PRODUCT_DATA    "2023-10-22"
#define SW_VERSION		"V4.12"
#define ROUTER_WHITE_LIST_ENABLE

#define FRAME_HEAD					(0x02)
#define SELF_CHECK_HEAD				(0x01)
#define BINDCODE_HEAD				(0x20)
#define UARTMS_HEAD					(0xa2)

#define CMD_MAC						(0x01)

#define CMD_DATA					(0x02)
#define CMD_CTRL_DIGITALTUBE		(0x03)
#define CMD_DATA_LEN				(0x06)
#define CMD_DATA_PROGRESS			(0x70)

//router send to label
#define CMD_PART_REFRESH_WAKEUP		(0x07)
#define CMD_PART_REFRESH_INFO 		(0x08)		

#define CMD_DATA_RESULT				(0x08)
//#define CMD_WAKEUP				(0x09)
#define CMD_ASK_LABELINFO			(0x09)
#define CMD_SET_LABEL_PARAMERS  	(0x0a)

#define CMD_WAKEUP_UPDATE			(0x0b)

#define CMD_SELF_CHECK				(0x0b)
#define CMD_CHANGE_FREQ				(0x0c)

#define CMD_TN_DATA					(0x0d)
	
#define CMD_RGB_CTRL_DATA			(0x0e)	
#define CMD_RGB_CTRL_FINISHED		(0x0f)

#define CMD_ROUTER_BROADCAST		(0x10)
#define CMD_LEDDISPLAY				(0x10)
#define CMD_LEDDISPLAY_ACk			(0x90)
#define CMD_PICKING_ORDERS			(0x12)
#define CMD_PICKING_ORDERS_ACK		(0x92)
#define CMD_PICKING_COMMOND			(0x12)
#define CMD_PICKING_COMMOND_ACK		(0x92)

#define CMD_MAC_ACK 				(0x81)
#define CMD_DATA_ACK				(0x82)
#define CMD_CTRL_DIGITALTUBE_ACK 	(0x83)
#define CMD_POWER_ACK				(0x84)
//#define CMD_ONOFF_ACK				(0x85)
#define CMD_TAG_INSPECT_ACK			(0x85)
#define CMD_WAKEUP_ACK				(0x86)

#define CMD_PART_REFRESH_ACK		(0x87)
#define CMD_PART_REFRESH_INFO_ACK	(0x88)

//#define CMD_WAKEUP_SERVER_ACK	(0x89)
#define CMD_TAG_LOGIN_INFO			(0x89)

#define CMD_SET_LABEL_PARAMERS_ACK  (0x8a)
#define CMD_WAKEUP_UPDATE_ACK		(0x8b)

#define CMD_TN_DATA_ACK				(0x8d)
#define CMD_RGB_CTRL_DATA_ACK		(0x8e)
#define CMD_RGB_CTRL_FINISHED_ACK		(0x8f)

//pick system
#define CMD_TAG_REQUEST_DATA	(0xc4)
#define CMD_REPORT_GOOD_INFO	(0xc5)
#define CMD_REPORT_ENTER		(0xd5)
#define CMD_TAG_REQUEST_BMP		(0xc6)

#define CMD_ADD_TIMER		0xF1
#define CMD_DEL_TIMER		0xF2

#define CMD_SET_SLEEP_TIME 	0xc1
#define CMD_SET_WAKEUP_TIME 0xc2

//CC2640
#define CMD_TAG_REQUEST_DATA	(0xc4)

#define FRAME_HEAD_DATA                	0x02
#define FRAME_HEAD_CMD                 	0xa2
#define CMD_SETCONFIG                  	0x01
#define CMD_CONFIGACK                  	0x61
#define CMD_SENDWAKEUPCODE             	0x02
#define CMD_WAKEUPCODEACK              	0x62
#define CMD_SENDPARTWAKEUPCODE		    0x03
#define CMD_PARTWAKEUPCODEACK			0x63
#define CMD_TAGLOGINACK					0x04
#define CMD_TAGLOGININFOACK				0x05
#define CMD_SETFREQUENCY               	0x06
#define CMD_SETRFBAUD					0x07
#define CMD_COMMONWAKEUP				0x08
#define CMD_CHANGEWAKEPERIOD			0x09
#define CMD_LED_DISPLAYDATA				0x0a
#define CMD_TAGUPDATA_WAKEUPCODE		0x0b
#define CMD_TAGUPDATA_WAKEUPCODEACK		0x8b
#define CMD_QUERYTAGLIST				0x0b
#define CMD_QUERYTAG_ACK				0x9b
#define CMD_TAG_INSPECT					0x0c
#define CMD_SET_LABELID					0x0d
#define CMD_TAG_INSPECT_OK				0x0e
#define CMD_CTRL_DIGITALTUBE_DATA  		0x0a
#define CMD_CTRL_DIGITALTUBE_DATA_ACK  	0x94

#define CMD_PLC_WHITE_LIST_SWITCH       0x31
#define CCO_WHITE_LIST_DISABLE 0
#define CCO_WHITE_LIST_ENABLE 1
#define CCO_REBOOT 


#define PTL_MAC_ADDR_LEN = 4
#define COMMON_SYNCWORD	0x9351de0b

#pragma pack (1)

struct header {
	td_u8 magic;
	td_u8 cmd;
	td_u16 length;
};

typedef struct {
	td_u8 magic;    //frame head
	td_u8 cmd;      //fram cmd type
	td_u16 length;  //frame length
} app_router_frame_head;

struct cmd_tag {
	struct header header;
	td_u32 mac;
	td_u16 width;
	td_u16 height;
	td_u8 power;
	td_u8 screentype;
	td_s8 rssi;
};

struct cmd_tag_register_info {
	struct header header;
	td_u32 mac;
	td_u8 acProductionBatch[3];
	td_u16 acSerialNumber;
	td_u8 cHardwareVersion;
	td_u8 cSoftwareVersion;
	td_u8 cManufacture;
};

typedef struct {
    struct header header;
    td_u8 rx_buffer[MAX_LEN];
	td_u16 crc16;
} app_rx_router_frame_data;

typedef struct {
    struct header header;
    td_u32 mac;
	td_u16 router_id;
    td_u8 index;
    td_u8 result;
} app_rx_router_frame_data_ack;

struct cmd_wakeupcode {
	struct header header;
	td_u16 sendTimeout; //单位：ms
	td_u16 bmpLength;
	td_u16 routerid;
	td_u32 syncword;
	td_u16 freq;
	/*新版版增加协议*/
//if defined(TAG_SLEEP_2)
	td_u16 shortPeriod;
	td_u16 shortPeriodOuttime;
	td_u8 opflag;	
	td_u16 routerFreq;
	td_u16 taskId;
	td_u8 moudleId;
//endif
};

struct cmd_wakeupcodeack {
	struct header header;
	td_u32 mac;
	td_u16 router_id;
	td_u8 power;
	td_u8 state;
	td_s8 rssi;
};

//图片信息
typedef struct {
	struct header header;
	td_u8 index;
	td_u16 x;
	td_u16 y;
	td_u16 width;
	td_u16 height;
	td_u32 size;
	td_u8 screentype;
	td_u16 outtime;
	td_u16 ledstate;
	td_u32 ledrgb;
	td_u16 crc;
} app_router_part_refresh_info;

typedef struct {
	struct header header;
	td_u32 mac;
	td_u16 router_id; 
	td_u8 index;
	td_u8 state;
} app_router_part_refresh_info_ack;

typedef struct {
	td_u32 taskid;		    //任务id
	td_u32 opflag;      	//0xa0单个操作，0xa1批量操作
	td_u32 lperiod;	 	    //长休眠ms
	td_u32 speriod;     	//短休眠ms
	td_u32 speriodkeeptime; //短休眠持续时间
} Tag_REFRESH_MSG;

typedef struct {
	struct header header;//0x0e
	td_u16 timeout;
	td_u32 rgb;
	td_u8 data[20];
	td_u8 buzzerflag;
	td_u16 rgbcycle;
	td_u16 duration;
#if defined(TAG_SLEEP_2)
	Tag_REFRESH_MSG TagRefreshMsg;
#endif
} cmd_rgb_ctrl_data;

typedef struct {
	cmd_rgb_ctrl_data RgbCtrlData_s;
	td_u32 mac;
	td_u32 freq;
	td_u32 num;
	td_u32 cmdtokenlen;
	td_char cmdtoken[0];
} cmd_tx_rgb_ctrl_data;

typedef struct {
	struct header header;
	td_u32 mac;
	td_u16 router_id;
	td_u8 result; 
} cmd_rgb_ctrl_data_ack;

typedef struct {
    struct header header;
	td_u32 syncWord;            
	td_u16 freq;              
	td_u32 mac;         /* mac of label */
} app_router_set_labelid;

//仓储二代enter按键反馈
struct cmd_report_goodinfo {
	struct header header;
	td_u32 mac;
	td_u16 router_id;
	td_s16 picknums;
};

//仓储二代enter按键反馈
struct cmd_report_enter{
	struct header header;
	td_u32 mac;
	td_u16 router_id;
	td_s16 number;		//数量qty
	td_s16 number1;	//不合格数badQty
	td_s16 number2;	//报工单号id
};

typedef struct {
	struct header header;
	td_u16 sendTimeout;
	//td_u16 routerid;
	td_u16 reporttime;
	td_u32 syncword;
	td_u16 freq;
} Cmd_Tag_Inspect;

typedef struct {
	struct header header;
	td_u32 mac;
	td_u16 router_id;
	td_u8 power;
	td_u8 state;
	td_s8 rssi;
	td_s8 index;
	td_s16 nodenum;
} Cmd_Tag_Inspect_Ack;

typedef struct {
	struct header header;
	td_u32 mac;
	td_u16 freq;
} Cmd_Tag_Inspect_OK;

typedef struct {
	struct header header;
	td_u8 cfg;  //0x01:rfReceive ,0x02:uartReceive
	td_u16 freq;
	td_u32 ipAddr;
} Cmd_Config;

struct cmd_plc_white_list_config {
	struct header header;
	td_u8 state;
};

td_s32 app_router_frame_handle_rx(EXT_CONST td_u8 *data, td_u16 data_len);

#ifdef __cplusplus
}
#endif
#endif
