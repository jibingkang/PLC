/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: APP Router frame handle.
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
#include "app_router.h"
#include "soc_mdm_mem.h"
#include "soc_mdm_topo.h"
#include <soc_mdm_mac.h>
#include <soc_mdm_sys.h>
#include "app_nv.h"
#include "app_cco_whitelist.h"

#ifdef __cplusplus
extern "C" {
#endif

EXT_EXTERN td_u8 app_broadcast_mac[EXT_PLC_MAC_ADDR_LEN];
EXT_EXTERN td_u8 app_send_tag_mac[EXT_PLC_MAC_ADDR_LEN];
EXT_EXTERN td_u16 app_router_id;
td_u8 app_default_tag_mac[EXT_PLC_MAC_ADDR_LEN] = {0,0,0xc0,0xa8,0x1,0x65};
int whitelist_num = 0;
td_bool whitelistStatus = TD_FALSE;

td_s32 router_app_whitelist_set(td_void);


void mydebug(const char *tag, td_u8 *buffer, td_u32 length)
{	
	td_u32 i;	
	printf("===tag=%s buffer length=%d====\n", tag, length);	
	for (i=0; i<length; i++) {		
		if (i%16==0) printf("\n");		
		printf("%02x ", buffer[i]);		
	}	
	printf("\n\n=======%s end============\n", tag);
}
td_u32 myhtonl(td_u32 lHost)
{
	td_char* p = (td_char*)&lHost;
	td_char c = *p;
	*p = *(p + 3);
	*(p + 3) = c;
	c = *(p + 1);
	*(p + 1) = *(p + 2);
	*(p + 2) = c;
	return lHost;
}

td_u16 myhtons(td_u16 sHost)
{
	td_char* p = (td_char*)&sHost;
	td_char c = *p;
	*p = *(p + 1);
	*(p + 1) = c;
	return sHost;
}
/*
td_u32 app_set_dev_addr_to_nv(td_u8 addr[EXT_PLC_MAC_ADDR_LEN])
{
    td_u32 ret;
    app_dev_addr_inf nv;
    (td_void)memset_s(&nv,sizeof(nv), 0, sizeof(nv));
    ret = uapi_nv_read(ID_NV_APP_DEV_ADDR_INF, &nv, sizeof(nv));
    if (ret == EXT_ERR_SUCCESS) {
        if (memcmp(nv.dev_addr, addr, EXT_PLC_MAC_ADDR_LEN) != 0) {
            (td_void) memcpy_s(nv.dev_addr, sizeof(nv.dev_addr), addr, EXT_PLC_MAC_ADDR_LEN);
            ret = uapi_nv_write(ID_NV_APP_DEV_ADDR_INF, &nv, sizeof(nv));
        }
    }
    return ret;
}

td_u32 app_get_dev_addr_from_nv(td_u8 addr[EXT_PLC_MAC_ADDR_LEN])
{
    td_u32 ret;
    app_dev_addr_inf nv;
    (td_void)memset_s(&nv,sizeof(nv), 0, sizeof(nv));
    ret = uapi_nv_read(ID_NV_APP_DEV_ADDR_INF, &nv, sizeof(nv));
    if (ret == EXT_ERR_SUCCESS) {
        if (memcmp(nv.dev_addr, addr, EXT_PLC_MAC_ADDR_LEN) != 0) {
            (td_void) memcpy_s(addr, sizeof(nv.dev_addr), nv.dev_addr, sizeof(nv.dev_addr));
        }
    }
    return ret;
}
*/
/*
td_s32 enable_whitelist(td_void)
{
    ext_mac_valid_mac_list * pstMeterList = TD_NULL;
    ext_mac_white_list_change_reason enReason = EXT_MAC_WHITE_LIST_CHANGE_REASON_UNKNOWN;
    pstMeterList = (ext_mac_valid_mac_list *) uapi_malloc (EXT_MOD_ID_USR, sizeof(ext_mac_valid_mac_list));
    if(pstMeterList == TD_NULL) {
        return EXT_ERR_NO_MORE_MEMORY;
    }
    memset(pstMeterList, 0, sizeof(ext_mac_valid_mac_list));
    // 依据业务逻辑填写白名单列表及配置白名单原因enReason
    //……
    // 调用设置白名单接口
    uapi_set_valid_mac_list(pstMeterList, enReason);
    uapi_free(EXT_MOD_ID_USR,pstMeterList);
    uapi_set_white_list_switch(TD_TRUE, TD_FALSE, EXT_MAC_WHITE_LIST_CHANGE_REASON_APP_STOP_SEARCH);
    return EXT_ERR_SUCCESS;
}
*/
td_s32 enable_whitelist(td_void)
{
 #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#if 0
    /* Setup whitelist to MAC */
    td_u32 app_whitelist_setup(td_void);
    /* White List: clear */
    td_u32 app_whitelist_clear(td_void);
    /* White List: Get White List */
    app_whitelist_head *app_get_whitelist(td_void);
    /* White List: Add node */
    td_u32 app_whitelist_add(td_pbyte list, td_u16 count);
    /* White List: Del node */
    td_u32 app_whitelist_delete(td_pbyte list, td_u16 count);
    /* timeout proc */
    td_void app_whitelist_timeout(td_void);
    td_u16 app_get_whitelist_sta_num(td_void);
    td_void app_whitelist_timeout_notify_mac(td_void);
    */
#endif

    //uapi_set_white_list_switch(TD_TRUE, TD_FALSE, EXT_MAC_WHITE_LIST_CHANGE_REASON_APP_STOP_SEARCH);
    whitelist_num = app_get_whitelist_sta_num();
    uapi_get_white_list_switch(&whitelistStatus);
    app_printf("sta_num: %d, whitelistStatus: %d \n", whitelist_num, whitelistStatus);
    //保存白名单
    router_app_whitelist_set();
    uapi_set_white_list_switch(TD_TRUE, TD_TRUE, EXT_MAC_WHITE_LIST_CHANGE_REASON_APP_CON);
    //uapi_set_white_list_switch(TD_FALSE, TD_FALSE, EXT_MAC_WHITE_LIST_CHANGE_REASON_APP_CON);
    /*
    uapi_set_app_auth(TD_TRUE);
    uapi_set_white_list_assoc_switch(TD_TRUE);
    */
#endif
    return EXT_ERR_SUCCESS;
}
//app_whitelist_storage
td_s32 router_app_whitelist_set(td_void)
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    app_whitelist_clear();
    app_whitelist_head *white_list = app_get_whitelist();
    td_u16 i = 0;
    td_u16 node_num = 0;
    td_u16 max_tei = 0;
    node_num = uapi_get_topo_node_num();
    max_tei = uapi_topo_get_max_tei();
    app_printf("app_get_topo_info: node_num: %d, max_tei: %d\n", node_num, max_tei);
    ext_mdm_nm_topo_node_info *topo_head = TD_NULL;
    //EXT_ND_TYPE_CCO=2, EXT_ND_TYPE_STA=3;
    topo_head = (ext_mdm_nm_topo_node_info *)uapi_malloc(EXT_MOD_ID_APP_COMMON, node_num*sizeof(ext_mdm_nm_topo_node_info));
    if (topo_head == TD_NULL) {
        uapi_free(EXT_MOD_ID_APP_COMMON, topo_head);
        return EXT_ERR_MALLOC_FAILUE;
    }
    uapi_get_topo_info(&topo_head);    
    for(i = 0; i < node_num; i++) {
        if (topo_head[i].product_type == EXT_ND_TYPE_STA) {
            app_printf("whitelist_set topo[%d]: mac:%02x:%02x:%02x:%02x:%02x:%02x\n", i, topo_head[i].mac[0], topo_head[i].mac[1],
                topo_head[i].mac[2],topo_head[i].mac[3],topo_head[i].mac[4],topo_head[i].mac[5]);
            //app_printf("whitelist_set topo info: %d, %d, %d, %d, %d, %d, %d, %d, %d \n", topo_head[i].product_type, topo_head[i].sta_ver_index,
                //topo_head[i].phase, topo_head[i].heartbeat_count, topo_head[i].level, topo_head[i].proxy_tei,
                //topo_head[i].ability, topo_head[i].state, topo_head[i].end_sequence);
            memcpy_s(white_list->list + WHITE_LIST_NODE_SIZE * (i-1), WHITE_LIST_NODE_SIZE, 
                topo_head[i].mac, sizeof(topo_head[i].mac));
        }
    }
    //ext_mac_valid_mac_list * pstMeterList = TD_NULL;
    //uapi_set_valid_mac_lis
    white_list->total = i-1;
    whitelist_num = app_get_whitelist_sta_num();
    app_printf("white_list->total: %d whitelist_num: %d \n", white_list->total, whitelist_num);
    //app_print_buf("white_list->list", white_list->list, WHITE_LIST_DATA_SIZE);
    uapi_free(EXT_MOD_ID_APP_COMMON, topo_head);
#endif
    return EXT_ERR_SUCCESS;
}

td_s32 app_topo_is_ok(td_u8 mac_addr[EXT_PLC_MAC_ADDR_LEN]) 
{
    if (mac_addr == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    td_s32 ret = 0;
    ext_mdm_nm_topo_node_info *entry = TD_NULL;
    entry = (ext_mdm_nm_topo_node_info *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(ext_mdm_nm_topo_node_info));
    if (entry == TD_NULL) {
        uapi_free(EXT_MOD_ID_APP_COMMON, entry);
        return EXT_ERR_MALLOC_FAILUE;
    }
    
    ret = uapi_query_mac_attr(mac_addr, entry);
    app_printf("app_topo_is_ok: sta info[%02x:%02x:%02x:%02x:%02x:%02x]: %d, %d, %d, %d, %d, %d, %d, %d, %d ...\n", entry->product_type, 
            entry->mac[0], entry->mac[1],entry->mac[2],entry->mac[3],entry->mac[4],entry->mac[5], 
            entry->sta_ver_index, entry->phase, entry->heartbeat_count, entry->level, 
            entry->proxy_tei, entry->ability, entry->state, entry->end_sequence);
    app_printf("app_topo_is_ok: ret: %d, mac: %02d:%02d:%02d:%02d:%02d:%02d\n", ret, 
                mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    if (ret) {
        uapi_free(EXT_MOD_ID_APP_COMMON, entry);  
        return ret;
    } 
/*
    判断是否在线不可以判断心跳
    if(!entry->heartbeat_count) {
        app_printf("app_topo_is_ok: ret: %d, mac: %02d:%02d:%02d:%02d:%02d:%02d\n", EXT_TP_IN_USE_BUT_NO_HT, 
                mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
        uapi_free(EXT_MOD_ID_APP_COMMON, entry);
        return EXT_TP_IN_USE_BUT_NO_HT;
    }
*/
    uapi_free(EXT_MOD_ID_APP_COMMON, entry);
#endif
    return EXT_ERR_SUCCESS;
}

td_s32 part_refresh_info_handle(EXT_CONST td_u8 *data, td_u16 data_len)
{
    if ((data == TD_NULL) || (data_len==0)) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    td_s32 ret =0;
    //app_router_part_refresh_info *part_refresh_info = TD_NULL;
    app_router_part_refresh_info_ack *part_refresh_info_ack = TD_NULL;
     part_refresh_info_ack = (app_router_part_refresh_info_ack *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(app_router_part_refresh_info_ack));
    if (part_refresh_info_ack == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    part_refresh_info_ack->header.magic = FRAME_HEAD;
    part_refresh_info_ack->header.cmd = CMD_PART_REFRESH_INFO_ACK;
    part_refresh_info_ack->header.length = myhtons(sizeof(app_router_part_refresh_info_ack));
    if (memcpy_s(&part_refresh_info_ack->mac, EXT_PLC_MAC_ADDR_LEN-2, &app_send_tag_mac[2], EXT_PLC_MAC_ADDR_LEN-2) != EOK) {
        uapi_free(EXT_MOD_ID_APP_COMMON, part_refresh_info_ack);
        return EXT_ERR_MEMCPY_FAIL;
    }
    part_refresh_info_ack->router_id=myhtons(app_router_id);
    part_refresh_info_ack->state=TD_TRUE;
    //mydebug("wakeupcode", (td_void *)part_refresh_info_ack, sizeof(app_router_part_refresh_info_ack));
    ret = app_chl_tx_from_uart_to_plc((EXT_CONST td_u8 *)data, data_len, app_send_tag_mac);
    app_printf("ret: %d\n", ret);
    if (!ret) {
        app_chl_rx_to_uart((EXT_CONST td_u8 *)part_refresh_info_ack, sizeof(app_router_part_refresh_info_ack));
    }
    
    uapi_free(EXT_MOD_ID_APP_COMMON, part_refresh_info_ack);

    if (ret == EXT_ROM_ERR_MALLOC_FAILUE) {
        uapi_usr_reboot(EXT_SYS_REBOOT_CAUSE_USR0);
    }
    return EXT_ERR_SUCCESS;
}

td_s32 wakeup_code_handle(EXT_CONST td_u8 *data, td_u16 data_len)
{

    if ((data == TD_NULL) || (data_len==0)) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    struct cmd_wakeupcode *wakeupcode = TD_NULL;
    struct cmd_wakeupcodeack *wakeupcodeack = TD_NULL;
    wakeupcode = (struct cmd_wakeupcode *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(struct cmd_wakeupcode));
    if (wakeupcode == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    wakeupcodeack = (struct cmd_wakeupcodeack *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(struct cmd_wakeupcodeack));
    if (wakeupcodeack == TD_NULL) {
        uapi_free(EXT_MOD_ID_APP_COMMON, wakeupcode);
        return EXT_ERR_MALLOC_FAILUE;
    }
    if (memcpy_s(wakeupcode, sizeof(struct cmd_wakeupcode), data, data_len) != EOK) {
        uapi_free(EXT_MOD_ID_APP_COMMON, wakeupcode);
        uapi_free(EXT_MOD_ID_APP_COMMON, wakeupcodeack);
        return EXT_ERR_MEMCPY_FAIL;
    }
    app_print_buf("wakeupcode", data, data_len);
    //mydebug("wakeupcode", (td_void *)wakeupcode, sizeof(struct cmd_wakeupcode));
    //设置基站id
    app_router_id  = myhtons(wakeupcode->routerid);
    //设置发送标签mac
    (td_void) memcpy_s(&app_send_tag_mac[2], EXT_PLC_MAC_ADDR_LEN-2, &wakeupcode->syncword, EXT_PLC_MAC_ADDR_LEN-2);
    app_printf("wakeupcode->syncword: 0x%08x\n", wakeupcode->syncword);
    wakeupcodeack->header.magic=FRAME_HEAD;
    wakeupcodeack->header.cmd=CMD_WAKEUP_ACK;
    //header.length没有计算rssi
    wakeupcodeack->header.length=myhtons(sizeof(struct cmd_wakeupcodeack)-1);
    wakeupcodeack->mac = wakeupcode->syncword;
    wakeupcodeack->router_id = wakeupcode->routerid;
    wakeupcodeack->power = 100;
    wakeupcodeack->rssi = -30;
    wakeupcodeack->state = TD_TRUE;
    app_printf("wakeup_code_handle, routerid: %d, mac: %02x:%02x:%02x:%02x:%02x:%02x \n", 
                app_router_id, app_send_tag_mac[0], app_send_tag_mac[1], app_send_tag_mac[2],app_send_tag_mac[3],
                app_send_tag_mac[4],app_send_tag_mac[5]);
    app_printf("wakeupcodeack: mac:%08x, router_id:%d, power:%d, rssi:%d, state:%d (length:%04x) \n", myhtonl(wakeupcodeack->mac),
               myhtons(wakeupcodeack->router_id), wakeupcodeack->power,wakeupcodeack->rssi, wakeupcodeack->state, sizeof(struct cmd_wakeupcodeack));
    
    //mydebug("wakeupcodeack", (td_void *)wakeupcodeack, sizeof(struct cmd_wakeupcodeack));
    if(!app_topo_is_ok(app_send_tag_mac)) {
        app_chl_rx_to_uart((EXT_CONST td_u8 *)wakeupcodeack, sizeof(struct cmd_wakeupcodeack));
    }
    uapi_free(EXT_MOD_ID_APP_COMMON, wakeupcode);
    uapi_free(EXT_MOD_ID_APP_COMMON, wakeupcodeack);
    return EXT_ERR_SUCCESS;
}

td_s32 data_handle(EXT_CONST td_u8 *data, td_u16 data_len)
{

    if ((data == TD_NULL) || (data_len==0)) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    td_s32 ret = 0;
    app_rx_router_frame_data_ack *dataack = TD_NULL;
    app_rx_router_frame_data *rx_data = TD_NULL;

    dataack = (app_rx_router_frame_data_ack *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(app_rx_router_frame_data_ack));
    if (dataack == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    rx_data = (app_rx_router_frame_data *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(app_rx_router_frame_data));
    if (rx_data == TD_NULL) {
        uapi_free(EXT_MOD_ID_APP_COMMON, dataack);
        return EXT_ERR_MALLOC_FAILUE;
    }
/*
    if (memcpy_s(dataack, sizeof(app_rx_router_frame_data_ack), data, data_len) != EOK) {
        uapi_free(EXT_MOD_ID_APP_COMMON, dataack);
        return EXT_ERR_MEMCPY_FAIL;
    }
*/
    dataack->header.magic=FRAME_HEAD;
    dataack->header.cmd=CMD_DATA_ACK;
    dataack->header.length=myhtons(sizeof(app_rx_router_frame_data_ack));
    if (memcpy_s(&dataack->mac, EXT_PLC_MAC_ADDR_LEN-2, &app_send_tag_mac[2], EXT_PLC_MAC_ADDR_LEN-2) != EOK) {
        uapi_free(EXT_MOD_ID_APP_COMMON, dataack);
        uapi_free(EXT_MOD_ID_APP_COMMON, rx_data);
        return EXT_ERR_MEMCPY_FAIL;
    }
    dataack->router_id = myhtons(app_router_id);
    dataack->index = data[2];
    dataack->result = TD_TRUE;
    td_u32 sec,ms,us;
    uapi_time_counter_gettime(&us,&ms,&sec);
    app_printf("data_handle:[%d:%d:%d] datalen=%d(mac:%02x:%02x:%02x:%02x:%02x:%02x)\n",sec,ms,us,data_len,
                app_send_tag_mac[0], app_send_tag_mac[1], app_send_tag_mac[2],app_send_tag_mac[3],
                app_send_tag_mac[4],app_send_tag_mac[5]);
    if (memcpy_s(rx_data, sizeof(app_rx_router_frame_data), data, data_len) != EOK) {
        uapi_free(EXT_MOD_ID_APP_COMMON, dataack);
        uapi_free(EXT_MOD_ID_APP_COMMON, rx_data);
        return EXT_ERR_MEMCPY_FAIL;
    }
    rx_data->header.magic = FRAME_HEAD_CMD;
    //mydebug("rx_data", (void *)rx_data, sizeof(app_rx_router_frame_data));

    //if (!app_chl_tx_from_uart_to_plc((EXT_CONST td_u8 *)rx_data, data_len, app_send_tag_mac)) {
    ret = app_chl_tx_from_uart_to_plc((EXT_CONST td_u8 *)rx_data, data_len, app_send_tag_mac);
    app_printf("ret: %d\n", ret);
    if (!ret) {
        #if defined(UART_DATA_ACK)
        app_chl_rx_to_uart((EXT_CONST td_u8 *)dataack, sizeof(app_rx_router_frame_data_ack));
        #endif
    }
    uapi_free(EXT_MOD_ID_APP_COMMON, dataack);
    uapi_free(EXT_MOD_ID_APP_COMMON, rx_data);

    if (ret == EXT_ROM_ERR_MALLOC_FAILUE) {
        uapi_usr_reboot(EXT_SYS_REBOOT_CAUSE_USR0);
    }
    return EXT_ERR_SUCCESS;
}

td_s32 rgb_ctrl_data_handle(EXT_CONST td_u8 *data, td_u16 data_len)
{

    if ((data == TD_NULL) || (data_len==0)) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    td_s32 ret = 0;
    cmd_tx_rgb_ctrl_data *rgbctrldata = TD_NULL;
    cmd_rgb_ctrl_data_ack *rgbctrldataack = TD_NULL;

    rgbctrldata = (cmd_tx_rgb_ctrl_data *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(cmd_tx_rgb_ctrl_data));
    if (rgbctrldata == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    rgbctrldataack = (cmd_rgb_ctrl_data_ack *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(cmd_rgb_ctrl_data_ack));
    if (rgbctrldataack == TD_NULL) {
        uapi_free(EXT_MOD_ID_APP_COMMON, rgbctrldata);
        return EXT_ERR_MALLOC_FAILUE;
    }

    rgbctrldataack->header.magic=FRAME_HEAD;
    rgbctrldataack->header.cmd=CMD_RGB_CTRL_DATA_ACK;
    rgbctrldataack->header.length=myhtons(sizeof(cmd_rgb_ctrl_data_ack));
    if (memcpy_s(&rgbctrldataack->mac, EXT_PLC_MAC_ADDR_LEN-2, &app_send_tag_mac[2], EXT_PLC_MAC_ADDR_LEN-2) != EOK) {
        uapi_free(EXT_MOD_ID_APP_COMMON, rgbctrldataack);
        uapi_free(EXT_MOD_ID_APP_COMMON, rgbctrldata);
        return EXT_ERR_MEMCPY_FAIL;
    }
    rgbctrldataack->router_id = myhtons(app_router_id);
    rgbctrldataack->result = TD_TRUE;
    td_u32 sec,ms,us;
    uapi_time_counter_gettime(&us,&ms,&sec);
    app_printf("rgb_ctrl_data_handle:[%d:%d:%d] datalen=%d(mac:%02x:%02x:%02x:%02x:%02x:%02x)\n",sec,ms,us,data_len,
                app_send_tag_mac[0], app_send_tag_mac[1], app_send_tag_mac[2],app_send_tag_mac[3],
                app_send_tag_mac[4],app_send_tag_mac[5]);
    if (memcpy_s(rgbctrldata, sizeof(cmd_tx_rgb_ctrl_data), data, data_len) != EOK) {
        uapi_free(EXT_MOD_ID_APP_COMMON, rgbctrldataack);
        uapi_free(EXT_MOD_ID_APP_COMMON, rgbctrldata);
        return EXT_ERR_MEMCPY_FAIL;
    }
    rgbctrldata->RgbCtrlData_s.header.magic = FRAME_HEAD_CMD;
    //mydebug("rx_data", (void *)rgbctrldata, sizeof(cmd_tx_rgb_ctrl_data));
    ret = app_chl_tx_from_uart_to_plc((EXT_CONST td_u8 *)rgbctrldata, data_len, app_send_tag_mac);
    app_printf("ret: %d\n", ret);
    if (!ret) {
        #if defined(UART_DATA_ACK)
        app_chl_rx_to_uart((EXT_CONST td_u8 *)rgbctrldataack, sizeof(cmd_rgb_ctrl_data_ack));
        #endif
    }
    uapi_free(EXT_MOD_ID_APP_COMMON, rgbctrldataack);
    uapi_free(EXT_MOD_ID_APP_COMMON, rgbctrldata);

    if (ret == EXT_ROM_ERR_MALLOC_FAILUE) {
        uapi_usr_reboot(EXT_SYS_REBOOT_CAUSE_USR0);
    }
    return EXT_ERR_SUCCESS;
}

td_s32 app_get_topo_info(td_void) 
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    td_u16 sta_num = 0;
    td_u16 max_tei = 0;
    td_u16 i = 0;
    td_u8 ret = 0;
    td_u8 tmp_mac[EXT_PLC_MAC_ADDR_LEN] = {0};
    td_u8 tag_mac[EXT_PLC_MAC_ADDR_LEN] = {0xff,0xff,0x63,0x16,0x3a,0x0b};
    //td_u8 coo_mac[EXT_PLC_MAC_ADDR_LEN] = {0x0,0x0,0xc0,0xa8,0x1,0xcc};
    sta_num = uapi_get_topo_node_num();
    max_tei = uapi_topo_get_max_tei();
    app_printf("app_get_topo_info: sta_num:%d, max_tei:%d\n", sta_num, max_tei);
    uapi_get_my_mac(tmp_mac, EXT_PLC_MAC_ADDR_LEN);
    app_printf("uapi_get_my_mac1:%02x:%02x:%02x:%02x:%02x:%02x\n", tmp_mac[0], tmp_mac[1],
            tmp_mac[2], tmp_mac[3], tmp_mac[4], tmp_mac[5]);
    
    //uapi_set_cco_mac_addr(coo_mac);
    //uapi_get_my_mac(tmp_mac, EXT_PLC_MAC_ADDR_LEN);
    //app_printf("uapi_get_my_mac2:%02x:%02x:%02x:%02x:%02x:%02x\n", tmp_mac[0], tmp_mac[1],
    //       tmp_mac[2], tmp_mac[3], tmp_mac[4], tmp_mac[5]);

    //uapi_get_cco_mac_addr(tmp_mac);
    //app_printf("uapi_get_cco_mac_addr:%02x:%02x:%02x:%02x:%02x:%02x\n", tmp_mac[0], tmp_mac[1],
            //tmp_mac[2], tmp_mac[3], tmp_mac[4], tmp_mac[5]);
    //uapi_get_local_mac_addr(tmp_mac, sizeof(tmp_mac));
    //app_printf("uapi_get_local_mac_addr:%02x:%02x:%02x:%02x:%02x:%02x\n", tmp_mac[0], tmp_mac[1],
            //tmp_mac[2], tmp_mac[3], tmp_mac[4], tmp_mac[5]);
    ext_mdm_nm_topo_node_info *entry = TD_NULL;
    entry = (ext_mdm_nm_topo_node_info *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(ext_mdm_nm_topo_node_info));
    if (entry == TD_NULL) {
        uapi_free(EXT_MOD_ID_APP_COMMON, entry);
        return EXT_ERR_MALLOC_FAILUE;
    }
    app_printf("EXT_MDM_NM_TOPO_NODE_NUM_MAX: %d\n", EXT_MDM_NM_TOPO_NODE_NUM_MAX);
    ret = uapi_query_mac_attr(tag_mac, entry);
    if (!ret) {
        app_printf("sta[%d]: mac:%02x:%02x:%02x:%02x:%02x:%02x\n", i, entry->mac[0], entry->mac[1],
            entry->mac[2],entry->mac[3],entry->mac[4],entry->mac[5]);
        app_printf("sta info: %d, %d, %d, %d, %d, %d, %d, %d, %d ...\n", entry->product_type, 
            entry->sta_ver_index, entry->phase, entry->heartbeat_count, entry->level, 
            entry->proxy_tei, entry->ability, entry->state, entry->end_sequence);
    } else {
        app_printf("uapi_query_mac_attr: ret = %d \n", ret);
    }
    
    /*
    ext_mdm_nm_topo_node_info **topo_node_info = TD_NULL;
    topo_node_info = (ext_mdm_nm_topo_node_info **)uapi_malloc(EXT_MOD_ID_APP_COMMON, EXT_MDM_NM_TOPO_NODE_NUM_MAX*sizeof(ext_mdm_nm_topo_node_info));
    if (topo_node_info == TD_NULL) {
        uapi_free(EXT_MOD_ID_APP_COMMON, entry);
        uapi_free(EXT_MOD_ID_APP_COMMON, topo_node_info);
        return EXT_ERR_MALLOC_FAILUE;
    }
    */
    ext_mdm_nm_topo_node_info *topo_head = TD_NULL;
    uapi_get_topo_info(&topo_head);
    if (topo_head == TD_NULL) {
        app_printf("topo_head null");
        uapi_free(EXT_MOD_ID_APP_COMMON, entry);
    } else {
        for (i = 0; i < sta_num; i++) {
            app_printf("topo[%d]: mac:%02x:%02x:%02x:%02x:%02x:%02x\n", i, topo_head[i].mac[0], topo_head[i].mac[1],
                topo_head[i].mac[2],topo_head[i].mac[3],topo_head[i].mac[4],topo_head[i].mac[5]);
            app_printf("topo info: %d, %d, %d, %d, %d, %d, %d, %d, %d \n", topo_head[i].product_type, topo_head[i].sta_ver_index,
                topo_head[i].phase, topo_head[i].heartbeat_count, topo_head[i].level, topo_head[i].proxy_tei,
                topo_head[i].ability, topo_head[i].state, topo_head[i].end_sequence);
        }
    }
       
    app_printf("uapi_free topo_node_info\n");
    uapi_free(EXT_MOD_ID_APP_COMMON, entry);
#endif
    return EXT_ERR_SUCCESS;
}

td_s32 app_send_inspect_ack(ext_mdm_nm_topo_node_info *topo_node)
{
    Cmd_Tag_Inspect_Ack *taginspectack = TD_NULL;
    taginspectack = (Cmd_Tag_Inspect_Ack *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(Cmd_Tag_Inspect_Ack));
    if (taginspectack == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }
    taginspectack->header.magic = FRAME_HEAD;
    taginspectack->header.cmd = CMD_TAG_INSPECT_ACK;
    taginspectack->header.length = myhtons(sizeof(Cmd_Tag_Inspect_Ack)-1);
    memcpy_s(&taginspectack->mac, EXT_PLC_MAC_ADDR_LEN-2, &topo_node->mac[2], EXT_PLC_MAC_ADDR_LEN-2);
    taginspectack->power = 100;
    taginspectack->rssi = -10;
    //taginspectack->state = topo_node->state;
    taginspectack->state = topo_node->product_type;
    taginspectack->index = topo_node->end_sequence;
    #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    taginspectack->nodenum = uapi_get_topo_node_num();
    #endif
    app_chl_rx_to_uart((EXT_CONST td_u8 *)taginspectack, sizeof(Cmd_Tag_Inspect_Ack));
    app_printf("app_send_inspect_ack: mac: %02d.%02d.%02d.%02d, product_type: %d heartbeat: %d state: %d\n", 
        taginspectack->mac & 0xff, taginspectack->mac >> 8 & 0xff, taginspectack->mac >> 16 & 0xff, 
        taginspectack->mac >> 24 & 0xff, topo_node->product_type, topo_node->heartbeat_count, topo_node->state);
    uapi_free(EXT_MOD_ID_APP_COMMON, taginspectack);

    return EXT_ERR_SUCCESS;
}

td_s32 app_inspect_handle(EXT_CONST td_u8 *data, td_u16 data_len)
{
    if ((data == TD_NULL) || (data_len==0)) {
        return EXT_ERR_INVALID_PARAMETER;
    }
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    Cmd_Tag_Inspect *taginspect = TD_NULL;
    td_u16 node_num = 0;
    td_u16 max_tei = 0;
    td_u16 i = 0;
    td_u8 node_mac[EXT_PLC_MAC_ADDR_LEN] = {0xff,0xff,0x63,0x16,0x3a,0x0b};
    td_u32 tag_mac = 0;
    node_num = uapi_get_topo_node_num();
    max_tei = uapi_topo_get_max_tei();
    app_printf("app_get_topo_info: node_num: %d, max_tei: %d\n", node_num, max_tei);

    taginspect = (Cmd_Tag_Inspect *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(Cmd_Tag_Inspect));
    if (taginspect == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }
    
    if (memcpy_s(taginspect, sizeof(Cmd_Tag_Inspect), data, data_len) != EOK) {
        uapi_free(EXT_MOD_ID_APP_COMMON, taginspect);
        return EXT_ERR_MEMCPY_FAIL;
    }

    ext_mdm_nm_topo_node_info *topo_head = TD_NULL;
    ext_mdm_nm_topo_node_info *entry = TD_NULL;
    entry = (ext_mdm_nm_topo_node_info *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(ext_mdm_nm_topo_node_info));
    if (entry == TD_NULL) {
        uapi_free(EXT_MOD_ID_APP_COMMON, taginspect);
        return EXT_ERR_MALLOC_FAILUE;
    }
    //EXT_ND_TYPE_CCO=2, EXT_ND_TYPE_STA=3;
    topo_head = (ext_mdm_nm_topo_node_info *)uapi_malloc(EXT_MOD_ID_APP_COMMON, node_num*sizeof(ext_mdm_nm_topo_node_info));
    if (topo_head == TD_NULL) {
        uapi_free(EXT_MOD_ID_APP_COMMON, taginspect);
        uapi_free(EXT_MOD_ID_APP_COMMON, entry);
        return EXT_ERR_MALLOC_FAILUE;
    }
    uapi_get_topo_info(&topo_head);
    tag_mac = myhtonl(taginspect->syncword);    
    if (tag_mac == COMMON_SYNCWORD) {
        app_printf("inspect all tag : %08x\n", tag_mac);
        for(i = 0; i < node_num; i++) {
            app_printf("topo[%d]: mac:%02x:%02x:%02x:%02x:%02x:%02x\n", i, topo_head[i].mac[0], topo_head[i].mac[1],
                topo_head[i].mac[2],topo_head[i].mac[3],topo_head[i].mac[4],topo_head[i].mac[5]);
            app_printf("topo info: %d, %d, %d, %d, %d, %d, %d, %d, %d \n", topo_head[i].product_type, topo_head[i].sta_ver_index,
                topo_head[i].phase, topo_head[i].heartbeat_count, topo_head[i].level, topo_head[i].proxy_tei,
                topo_head[i].ability, topo_head[i].state, topo_head[i].end_sequence);
            if (topo_head[i].product_type == EXT_ND_TYPE_STA || EXT_ND_TYPE_CCO) {
                app_send_inspect_ack(&topo_head[i]);
                uapi_sleep(10);
                app_printf("uapi_sleep(10)\n");
            }
        }
    } else {
        app_printf("inspect tag : %08x\n", tag_mac);
        //memcpy_s(&node_mac[2], EXT_PLC_MAC_ADDR_LEN-2, &tag_mac, EXT_PLC_MAC_ADDR_LEN-2);
        if (!uapi_query_mac_attr(node_mac, entry)) {
            app_printf("sta[%d]: mac:%02x:%02x:%02x:%02x:%02x:%02x\n", i, entry->mac[0], entry->mac[1],
                entry->mac[2],entry->mac[3],entry->mac[4],entry->mac[5]);
            app_printf("sta info: %d, %d, %d, %d, %d, %d, %d, %d, %d ...\n", entry->product_type, 
                entry->sta_ver_index, entry->phase, entry->heartbeat_count, entry->level, 
                entry->proxy_tei, entry->ability, entry->state, entry->end_sequence);
                //app_send_inspect_ack(entry);
                app_send_inspect_ack(entry);
        } else {
            app_printf("sta not exist\n");
        }  
    }
    uapi_free(EXT_MOD_ID_APP_COMMON, taginspect);
    uapi_free(EXT_MOD_ID_APP_COMMON, entry);
    uapi_free(EXT_MOD_ID_APP_COMMON, topo_head);
#if defined(ROUTER_WHITE_LIST_ENABLE)
    enable_whitelist();
#endif
#endif
    return EXT_ERR_SUCCESS;
}

td_s32 set_config_data_handle(EXT_CONST td_u8 *data, td_u16 data_len)
{
    
    //uapi_usr_reboot(EXT_SYS_REBOOT_CAUSE_USR0);
    if ((data == TD_NULL) || (data_len==0)) {
        return EXT_ERR_INVALID_PARAMETER;
    }
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    app_print_buf("set_config_data_handle", (td_u8 *)data, data_len);
    td_u8 ret = 0; 
    td_u8 tmp_mac[EXT_PLC_MAC_ADDR_LEN] = {0};
    td_u8 coo_mac[EXT_PLC_MAC_ADDR_LEN] = {0x0,0x0,0xc0,0xa8,0x1,0xcc};
    //td_u8 bad_mac[EXT_PLC_MAC_ADDR_LEN] = {0,0,0,0,0,0};

    Cmd_Config *setconfigdata = TD_NULL;
    setconfigdata = (Cmd_Config *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(Cmd_Config));
    if (setconfigdata == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    if (memcpy_s(setconfigdata, sizeof(Cmd_Config), data, data_len) != EOK) {
        uapi_free(EXT_MOD_ID_APP_COMMON, setconfigdata);
        return EXT_ERR_MEMCPY_FAIL;
    }

    if (memcpy_s(&coo_mac[2], EXT_PLC_MAC_ADDR_LEN-2, &setconfigdata->ipAddr, EXT_PLC_MAC_ADDR_LEN-2) != EOK) {
        uapi_free(EXT_MOD_ID_APP_COMMON, setconfigdata);
        return EXT_ERR_MEMCPY_FAIL;
    }

    //app_get_topo_info();
    app_printf("ipAddr: %08x\n", setconfigdata->ipAddr);
    //临时修改mac复位失效
    //uapi_set_cco_mac_addr(coo_mac);
    app_get_dev_addr_from_nv(tmp_mac);
    app_printf("app_get_dev_addr_from_nv:%02x:%02x:%02x:%02x:%02x:%02x\n", tmp_mac[0], tmp_mac[1],
            tmp_mac[2], tmp_mac[3], tmp_mac[4], tmp_mac[5]);
    /*        
    app_get_mac_addr_from_nv(tmp_mac);
    app_printf("app_get_mac_addr_from_nv:%02x:%02x:%02x:%02x:%02x:%02x\n", tmp_mac[0], tmp_mac[1],
            tmp_mac[2], tmp_mac[3], tmp_mac[4], tmp_mac[5]); 
    */       
    app_printf("new cco mac:%02x:%02x:%02x:%02x:%02x:%02x\n", coo_mac[0], coo_mac[1],
            coo_mac[2], coo_mac[3], coo_mac[4], coo_mac[5]);
    
    //!is_valid_mac_addr(nv.dev_addr)
    if (memcmp(tmp_mac, coo_mac, EXT_PLC_MAC_ADDR_LEN) != 0) {
        //if (memcmp(bad_mac, coo_mac, EXT_PLC_MAC_ADDR_LEN) != 0) {
        if (is_valid_mac_addr(coo_mac)) {
            //mac写入cco的工厂区，复位掉电后也一直起效
            ret = app_set_dev_addr_to_nv(coo_mac);//重新上电会失效
            app_printf("app_set_dev_addr_to_nv: ret=%d", ret);
            ret = app_get_dev_addr_from_nv(tmp_mac);
            app_printf("app_get_dev_addr_from_nv: ret=%d", ret);
            app_printf("app_get_dev_addr_from_nv:%02x:%02x:%02x:%02x:%02x:%02x\n", tmp_mac[0], tmp_mac[1],
                tmp_mac[2], tmp_mac[3], tmp_mac[4], tmp_mac[5]);
            //写入工厂区，不丢失
            /*
            app_set_mac_addr_to_nv(coo_mac);
            app_get_mac_addr_from_nv(tmp_mac);
            app_printf("app_get_mac_addr_from_nv:%02x:%02x:%02x:%02x:%02x:%02x\n", tmp_mac[0], tmp_mac[1],
                tmp_mac[2], tmp_mac[3], tmp_mac[4], tmp_mac[5]);
            */
            //只有MAC不同的时候才复位CCO
            uapi_usr_reboot(EXT_SYS_REBOOT_CAUSE_USR0);
            app_printf("set new Mac to CCO, CCO uapi_usr_reboot!!!\n");
        } else {
            app_printf("Bad Mac to CCO, CCO uapi_usr_reboot!!!\n");
        }
    } else {
        app_printf("same cco mac, uapi_get_my_mac:%02x:%02x:%02x:%02x:%02x:%02x\n", tmp_mac[0], tmp_mac[1],
            tmp_mac[2], tmp_mac[3], tmp_mac[4], tmp_mac[5]);
    }
    
    uapi_get_my_mac(tmp_mac, EXT_PLC_MAC_ADDR_LEN);
    app_printf("uapi_get_my_mac:%02x:%02x:%02x:%02x:%02x:%02x\n", tmp_mac[0], tmp_mac[1],
            tmp_mac[2], tmp_mac[3], tmp_mac[4], tmp_mac[5]);
    uapi_free(EXT_MOD_ID_APP_COMMON, setconfigdata);
    app_get_topo_info();
    
#if defined(ROUTER_WHITE_LIST_ENABLE)
    //uapi_set_white_list_switch(TD_TRUE, TD_FALSE, EXT_MAC_WHITE_LIST_CHANGE_REASON_APP_STOP_SEARCH);
    whitelist_num = app_get_whitelist_sta_num();
    whitelistStatus = uapi_get_white_list_switch(&whitelistStatus);
    app_printf("set_config_data_handle sta_num: %d, whitelistStatus: %d \n", whitelist_num, whitelistStatus);
    //uapi_set_white_list_switch(TD_TRUE,TD_TRUE, EXT_MAC_WHITE_LIST_CHANGE_REASON_APP_CON);
    //app_printf("set_config_data_handle sta_num: %d, whitelistStatus: %d \n", whitelist_num, whitelistStatus);
#endif
    //app_printf("CCO will reboot ... \n");
    //基站登录复位CCO
    //uapi_usr_reboot(EXT_SYS_REBOOT_CAUSE_USR0);
#endif
    return EXT_ERR_SUCCESS;
}

td_s32 app_router_config_white_list(EXT_CONST td_u8 *data, td_u16 data_len)
{

    if ((data == TD_NULL) || (data_len==0)) {
        return EXT_ERR_INVALID_PARAMETER;
    }
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    struct cmd_plc_white_list_config *whitelistcfg = TD_NULL;
    whitelistcfg = (struct cmd_plc_white_list_config *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(struct cmd_plc_white_list_config));
    if (whitelistcfg == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    if (memcpy_s(whitelistcfg, sizeof(struct cmd_plc_white_list_config), data, data_len) != EOK) {
        uapi_free(EXT_MOD_ID_APP_COMMON, whitelistcfg);
        return EXT_ERR_MEMCPY_FAIL;
    }
    app_printf("app_router_config_white_list: state: %d\n", whitelistcfg->state);
    switch(whitelistcfg->state) {
        case CCO_WHITE_LIST_DISABLE: {
            app_printf("app_router_config_white_list: CCO_WHITE_LIST_DISABLE \n");
            uapi_set_white_list_switch(TD_FALSE, TD_TRUE, EXT_MAC_WHITE_LIST_CHANGE_REASON_APP_CON);
            whitelist_num = app_get_whitelist_sta_num();
            whitelistStatus = uapi_get_white_list_switch(&whitelistStatus);
            app_printf("CCO_WHITE_LIST_DISABLE sta_num: %d, whitelistStatus: %d \n", whitelist_num, whitelistStatus);
            uapi_usr_reboot(EXT_SYS_REBOOT_CAUSE_USR0);
        }
            break; 
        case CCO_WHITE_LIST_ENABLE: {
            app_printf("app_router_config_white_list: CCO_WHITE_LIST_ENABLE \n");
            enable_whitelist();
            whitelistStatus = uapi_get_white_list_switch(&whitelistStatus);
            app_printf("after enable_whitelist sta_num: %d, whitelistStatus: %d \n", 
                app_get_whitelist_sta_num(), whitelistStatus);
        }
            break; 
        default:
            app_printf("app_router_config_white_list Unknown state: %d\n", whitelistcfg->state);
            break;

    }
    uapi_free(EXT_MOD_ID_APP_COMMON, whitelistcfg);
#endif
    return EXT_ERR_SUCCESS;
}
// uart---plc
td_s32 app_router_frame_handle_rx(EXT_CONST td_u8 *data, td_u16 data_len)
{

    td_u8 router_frame_cmd;
    //td_u8 broadcast_mac[EXT_PLC_MAC_ADDR_LEN] = {0xff,0xff,0xff,0xff,0xff,0xff}; 

    if ((data == TD_NULL) || (data_len==0)) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    
    router_frame_cmd = data[TYPE_OFFSET];
    app_printf("app_router_frame_handle_rx, magic: %02x,router_frame_cmd: %02x\n", 
        data[START_OFFSET], router_frame_cmd);
    if ((data[START_OFFSET] == FRAME_HEAD)) {
        switch(data[TYPE_OFFSET]) {
            case CMD_DATA: {
                //app_print_buf("image data", data, data_len);
                data_handle(data, data_len);
            }
                break;
            case CMD_RGB_CTRL_DATA: {
                rgb_ctrl_data_handle(data, data_len);
            }
                break;
            case CMD_PART_REFRESH_INFO: {
                part_refresh_info_handle(data, data_len);
            }
                break;
            case CMD_DATA_ACK: {
                
            }
                break;
            case CMD_MAC_ACK:
                break;
            case CMD_SET_LABELID: 
                app_printf("CMD_SET_LABELID, router_frame_cmd: %02x, mac:%02x:%02x:%02x:%02x:%02x:%02x\n", 
                    router_frame_cmd,app_default_tag_mac[0], app_default_tag_mac[1], app_default_tag_mac[2],app_default_tag_mac[3],
                    app_default_tag_mac[4],app_default_tag_mac[5]);
                    app_chl_tx_from_uart_to_plc(data, data_len, app_default_tag_mac);
                break;
            default:
                app_printf("Unknown cmd: %02x %02x\n", FRAME_HEAD, router_frame_cmd);
                break;
        }
    } else if ((data[START_OFFSET] ==FRAME_HEAD_CMD)) {
        switch(data[TYPE_OFFSET]) {
            case CMD_SENDWAKEUPCODE: {
                wakeup_code_handle(data, data_len);
            }
                break;
            case CMD_SENDPARTWAKEUPCODE: {
                wakeup_code_handle(data, data_len);
            }
                break;
            case CMD_PART_REFRESH_INFO: {
                part_refresh_info_handle(data, data_len);
            }
                break;
            case CMD_SETCONFIG: {
                //app_get_topo_info();
                //uapi_usr_reboot(EXT_SYS_REBOOT_CAUSE_USR0);
                set_config_data_handle(data, data_len);
            }
                break;
            case CMD_TAG_LOGIN_INFO: {

            }
                break;
            case CMD_SET_LABELID: 
                app_printf("CMD_SET_LABELID: mac:%02x:%02x:%02x:%02x:%02x:%02x\n", 
                    app_send_tag_mac[0], app_send_tag_mac[1], app_send_tag_mac[2],
                    app_send_tag_mac[3], app_send_tag_mac[4], app_send_tag_mac[5]);
                    app_chl_tx_from_uart_to_plc(data, data_len, app_default_tag_mac);
                break;
            case CMD_TAG_INSPECT: {
                app_inspect_handle(data, data_len);
            }
                break;
            case CMD_PLC_WHITE_LIST_SWITCH: {
                app_printf("CMD_PLC_WHITE_LIST_SWITCH: %02x %02x\n", 
                    FRAME_HEAD_CMD, CMD_PLC_WHITE_LIST_SWITCH);
                app_router_config_white_list(data, data_len);
            }
                break;
            default:
                app_printf("Unknown cmd: %02x %02x\n", FRAME_HEAD_CMD, router_frame_cmd);
                break;
        }

    } else {
        app_printf("Unknown Frame Head");
    }
       

    return EXT_ERR_SUCCESS;
}


#ifdef __cplusplus
}
#endif
