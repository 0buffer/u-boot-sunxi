/*
 * (C) Copyright 2012
 *     wangflord@allwinnertech.com
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program;
 *
 */

#include <config.h>
#include <common.h>

download_info  dl_map;

uint (* sunxi_sprite_probe_part_data_size)(char *style, char *filename);

int (* sunxi_sprite_fetch_part_data_start)(char *style, char *filename);
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
int sunxi_sprite_download_init(int workmode)
{
	//��ʼ�����У����ڶ�ȡ����
    if(sunxi_queue_init())
    {
    	printf("sunxi queue init fail\n");

    	return -1;
    }
	//������������ģʽ������������usb����
	if(workmode == WORK_MODE_CARD_PRODUCT)
	{
		//������ģʽ
		//��ʼ��������������Ҫ�õ���Ӳ��������LED��CODEC
		//;
		//���Կ��Ϲ̼��ĺϷ���
		if(sprite_card_firmware_probe())
	    {
	    	printf("sunxi sprite firmware probe fail\n");

	    	return -1;
	    }
	    //��ȡdl_map�ļ�������ָ�����ص�����
	    if(sprite_card_fetch_download_map(&dl_map))
	    {
	    	return -1;
	    }
	    sunxi_sprite_card_download_part(&dl_map);
	}
	else if(workmode == WORK_MODE_USB_PRODUCT)
	{
		//usb����ģʽ
		//��ʼ��USBӲ�����˴�����USB����������ʾfel������Ȼ��ʼ��������
//		if(sprite_usb_hardware_init())
//		{
//			printf("sunxi sprite: usb hardware init fail\n");
//
//			return -1;
//		}
//		//��ȡdl_map�ļ�������ָ�����ص�����
//	    if(sprite_usb_fetch_download_map(&dl_map))
//	    {
//	    	return -1;
//	    }
//	    sunxi_sprite_probe_part_data_size = sunxi_sprite_usb_probe_part_data_size;
	}
	else
	{
		//����ģʽ�����ӵ�
		return -1;
	}
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
int sunxi_sprite_erase_flash()
{
	int need_erase_flash;
    char buf[1024 * 1024];
    char *tmp_buf;
    sunxi_mbr  *local_mbr;
    int i;
	//��ȡ������Ϣ���鿴�Ƿ���Ҫ����flash
    ret = script_parser_fetch("nand_para", "erase_flash", &need_erase_flash, 1);
    if((!ret) && (need_erase_flash))
    {
    	//check if need to protect part data
    	//��ʼ��flash�������������ȥ�Թ�
    	//����ʼ��ʧ�ܵ�ʱ��ֱ�Ӳ�����������˽������
    	if(sprite_flash_init())
    	{
    		sprite_flash_erase();
    		return 0;
    	}
    	//�������������ϵ�MBR
    	if(sprite_flash_read(0, 1024 * 1024/512, buf) == 0)
    	{
    		printf("read local mbr fail\n");
    	}
    	//У��MBR
    	tmp_buf = buf;
    	for(i=0;i<SUNXI_MBR_MAX_COUNT;i++)
        {
        	local_mbr = (sunxi_mbr *)tmp_buf;
        	if(crc32(0, tmp_buf + 4, SUNXI_MBR_SIZE - 4) != local_mbr->crc32)
        	{
        		printf("bad mbr table\n");
        		tmp_buf += SUNXI_MBR_SIZE;
        	}
	        else
	        {
	        	sprite_store_part_data(local_mbr);
	            break;
	        }
	    }
	    sprite_flash_erase();
	    if(i != SUNXI_MBR_MAX_COUNT)   //this means there is no correct sunxi mbr copy, then consider the flash has no need to protect data
	    {
	    	sprite_restore_part_data();
	    }
        sprite_flash_exit();
    }

    return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
int sunxi_sprite_hardware_scan(char *buffer)
{
	;
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
int sunxi_sprite_mode(int workmode)
{
	int production_meida;
	char  storage_info[256];

	//��ȡ��ǰ������������nand���߿�
	production_meida = uboot_spare_head.boot_data.storage_type;
	//��ʼ������Ӳ��
	if(sunxi_sprite_download_init(workmode))
	{
		return -1;
	}
	//����flash
	sunxi_sprite_erase_flash();
	sunxi_sprite_hardware_scan(storage_info);
	//��д��������
	if(workmode == WORK_MODE_CARD_PRODUCT)
	{
		if(sunxi_sprite_card_download_part())
		{
			return -1;
		}
		//��дuboot
		if(sunxi_sprite_card_download_uboot(production_meida, storage_info))
		{
			return -1;
		}
		//��дboot0
		if(sunxi_sprite_card_download_boot0(production_meida, storage_info))
		{
			return -1;
		}
	}
	else
	{
		;
	}
	//��д����

	return 0;
}

