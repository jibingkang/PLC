/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Modify the drv directory, remove the first stage of the romization error,
 * delete the useless code to compile and adjust the dfx directory, remove the count
 * function driver part drv_dfx_uart.h code
 * Author: CompanyName
 * Create: 2012-04-04
 */

#ifndef __DRV_DFX_UART_H__
#define __DRV_DFX_UART_H__
#include <soc_types.h>
#include <soc_mdm_types.h>

td_u32 dfx_get_uart_stat(td_u16 id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option);

#endif
