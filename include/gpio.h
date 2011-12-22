/*
 *==========================================================================
 *
 *      crc.h
 *
 *      Interface for the CRC algorithms.
 *
 *==========================================================================
 *####ECOSGPLCOPYRIGHTBEGIN####
 * -------------------------------------------
 * This file is part of eCos, the Embedded Configurable Operating System.
 * Copyright (C) 2002 Andrew Lunn
 *
 * eCos is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 or (at your option) any later version.
 *
 * eCos is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with eCos; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * As a special exception, if other files instantiate templates or use macros
 * or inline functions from this file, or you compile this file and link it
 * with other works to produce a work based on this file, this file does not
 * by itself cause the resulting work to be covered by the GNU General Public
 * License. However the source code for this file must still be made available
 * in accordance with section (3) of the GNU General Public License.
 *
 * This exception does not invalidate any other reasons why a work based on
 * this file might be covered by the GNU General Public License.
 *
 * Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
 * at http: *sources.redhat.com/ecos/ecos-license/
 * -------------------------------------------
 *####ECOSGPLCOPYRIGHTEND####
 *==========================================================================
 *#####DESCRIPTIONBEGIN####
 *
 * Author(s):    Andrew Lunn
 * Contributors: Andrew Lunn
 * Date:         2002-08-06
 * Purpose:
 * Description:
 *
 * This code is part of eCos (tm).
 *
 *####DESCRIPTIONEND####
 *
 *==========================================================================
 */

#ifndef _SERVICES_GPIO_H_
#define _SERVICES_GPIO_H_

#include <linux/types.h>


typedef struct
{
    char  gpio_name[32];
    int port;
    int port_num;
    int mul_sel;
    int pull;
    int drv_level;
    int data;
}
user_gpio_set_t;
/*
************************************************************************************************************
*
*                                             gpio_init
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    �� GPIO�����ʼ��������һ��GPIO����ַ
*
*
************************************************************************************************************
*/
extern  int      gpio_script_init(void);
/*
************************************************************************************************************
*
*                                             gpio_exit
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��  GPIO�����˳����޲�����ֱ���˳�
*
*
************************************************************************************************************
*/
extern  int      gpio_script_exit(void);
/*
************************************************************************************************************
*
*                                             gpio_Request
*
*    �������ƣ�
*
*    �����б� gpio_list �û����ݵ�ַ�����ڴ����û���GPIO������Ϣ
*               group_count_max  �û����ݵĸ����������ֵӦ�ô��ڻ��ߵ����û�ʵ�ʵ�GPIO����
*
*    ����ֵ  �� ����ɹ�������һ����������򷵻�0ֵ
*
*    ˵��    �� GPIO�����û����ݰ��սṹ�崫�ݣ�һ���ṹ�屣��һ��GPIO��Ϣ��
*
*
************************************************************************************************************
*/
extern  unsigned gpio_request                 (user_gpio_set_t *gpio_list,                                                              unsigned group_count_max               );
extern  unsigned gpio_request_ex(char *main_name, const char *sub_name);  //�豸����GPIO������չ�ӿ�
/*
************************************************************************************************************
*
*                                             gpio_Release
*
*    �������ƣ�
*
*    �����б�  p_handler  ���뵽�ľ��
*                if_release_to_default_status   �ͷź��״̬�������ͷź�״̬���䣬���Ա��ȫ����״̬�����Ա������ǰ��״̬
*
*    ����ֵ  ��
*
*    ˵��    �� �û�����ʹ��GPIO���ͷŵ���
*
*
************************************************************************************************************
*/
extern  int      gpio_release                 (unsigned p_handler,                                                                      int if_release_to_default_status       );
/*
************************************************************************************************************
*
*                                             gpio_get_all_pin_status
*
*    �������ƣ�
*
*    �����б� p_handler  ���뵽�ľ��
*               gpio_status  �����û����ݵĵ�ַ
*               gpio_count_max �����û����ݵĽṹ��ĸ����������ֵӦ�ô��ڻ��ߵ���ʵ�ʵ�GPIO����
*               if_get_from_hardware  ϣ����ȡ����GPIO��Ϣ��Դ��ʵ�ʵļĴ���״̬��������
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
extern  int      gpio_get_all_pin_status(unsigned p_handler, user_gpio_set_t *gpio_status, unsigned gpio_count_max, unsigned if_get_from_hardware);
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
extern  int      gpio_get_one_pin_status      (unsigned p_handler, user_gpio_set_t *gpio_status,             const char *gpio_name,     unsigned if_get_from_hardware          );
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
extern  int      gpio_set_one_pin_status      (unsigned p_handler, user_gpio_set_t *gpio_status,             const char *gpio_name,     unsigned if_set_to_current_input_status);
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
extern  int      gpio_set_one_pin_io_status   (unsigned p_handler, unsigned         if_set_to_output_status, const char *gpio_name                                             );
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
extern  int      gpio_set_one_pin_pull        (unsigned p_handler, unsigned         set_pull_status,         const char *gpio_name                                             );
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
extern  int      gpio_set_one_pin_driver_level(unsigned p_handler, unsigned         set_driver_level,        const char *gpio_name                                             );
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
extern  int      gpio_read_one_pin_value      (unsigned p_handler,                                           const char *gpio_name                                             );
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
extern  int      gpio_write_one_pin_value     (unsigned p_handler, unsigned         value_to_gpio,           const char *gpio_name                                             );





#endif /* _SERVICES_CRC_CRC_H_ */
