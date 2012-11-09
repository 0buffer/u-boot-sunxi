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
#ifndef  __spare_head_h__
#define  __spare_head_h__

#include "nand_bsp.h"
/* work mode */
#define WORK_MODE_PRODUCT      (1<<4)
#define WORK_MODE_UPDATE       (1<<5)

#define WORK_MODE_BOOT			0x00	//��������
#define WORK_MODE_USB_PRODUCT	0x10	//����USB����
#define WORK_MODE_CARD_PRODUCT	0x11	//���ڿ�����
#define WORK_MODE_USB_UPDATE	0x20	//����USB����
#define WORK_MODE_OUTER_UPDATE	0x21	//�����ⲿ������

#define UBOOT_MAGIC				"uboot"
#define STAMP_VALUE             0x5F0A6C39
#define ALIGN_SIZE				8 * 1024
#define MAGIC_SIZE              8
#define STORAGE_BUFFER_SIZE     (256)

typedef struct _normal_gpio_cfg
{
    char      port;                       //�˿ں�
    char      port_num;                   //�˿��ڱ��
    char      mul_sel;                    //���ܱ��
    char      pull;                       //����״̬
    char      drv_level;                  //������������
    char      data;                       //�����ƽ
    char      reserved[2];                //����λ����֤����
}
normal_gpio_cfg;

//SD��������ݽṹ
typedef struct sdcard_spare_info_t
{
	int 			card_no[4];                   //��ǰ�����Ŀ����������
	int 			speed_mode[4];                //�����ٶ�ģʽ��0�����٣�����������
	int				line_sel[4];                  //�������ƣ�0: 1�ߣ�������4��
	int				line_count[4];                //��ʹ���ߵĸ���
}
sdcard_spare_info;

typedef struct _boot_dram_para_t
{
    unsigned int           dram_baseaddr;
    unsigned int           dram_clk;
    unsigned int           dram_type;
    unsigned int           dram_rank_num;
    unsigned int           dram_chip_density;
    unsigned int           dram_io_width;
    unsigned int		   dram_bus_width;
    unsigned int           dram_cas;
    unsigned int           dram_zq;
    unsigned int           dram_odt_en;
    unsigned int 		   dram_size;
    unsigned int           dram_tpr0;
    unsigned int           dram_tpr1;
    unsigned int           dram_tpr2;
    unsigned int           dram_tpr3;
    unsigned int           dram_tpr4;
    unsigned int           dram_tpr5;
    unsigned int 		   dram_emr1;
    unsigned int           dram_emr2;
    unsigned int           dram_emr3;
}boot_dram_para_t;

#endif


