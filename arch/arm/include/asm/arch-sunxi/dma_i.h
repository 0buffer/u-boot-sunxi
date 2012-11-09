/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON driver sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : dmac_i.h
*
* By      : Jerry
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#ifndef	_DMAC_I_H_
#define	_DMAC_I_H_

#include <asm/arch/cpu.h>


#define CFG_SW_DMA_MAX     16


#define CFG_SW_DMA_CHANNAL_BASE              (SUNXI_DMA_BASE + 0x100              )

#define SUNXI_DMA_LINK_NULL       (0xfffff800)

struct sunxi_dma_config
{
	unsigned int config;
	unsigned int source_addr;
	unsigned int dest_addr;
	unsigned int byte_count;
	unsigned int commit_para;
	unsigned int link;
	unsigned int reserved[2];
};

typedef struct sunxi_dma_channal
{
	int 			   			used;
	int                			num;
	int				   			reserved[2];
	struct sunxi_dma_config		dma_config;
}
sunxi_dma_channal_t;

struct sunxi_dma_regs
{
    volatile unsigned int inten0;
    volatile unsigned int inten1;
    volatile unsigned int reserved0[2];
    volatile unsigned int intsts0;
    volatile unsigned int intsts1;
	volatile unsigned int reserved1[2];
	volatile unsigned int reserved2[8];
	volatile unsigned int status;
};

struct __sw_dma_control
{
    volatile unsigned int enable;
    volatile unsigned int pause;
    volatile unsigned int start_addr;
    volatile unsigned int config;
	volatile unsigned int cur_source;
	volatile unsigned int cur_dest;
	volatile unsigned int bcnt_left;
	volatile unsigned int para;
};


typedef struct __dma_config_set
{
    unsigned int      src_drq_type     : 5;            //Դ��ַ�洢���ͣ���DRAM, SPI,NAND�ȣ��μ�  __ndma_drq_type_t
    unsigned int      src_addr_type    : 2;            //ԭ��ַ���� 0:����ģʽ  1:���ֲ���
    unsigned int      src_burst_length : 2;            //����һ��burst��� 0:1   1:4   2:8
    unsigned int      src_data_width   : 2;            //���ݴ����ȣ�0:һ�δ���8bit��1:һ�δ���16bit��2:һ�δ���32bit��3:����
    unsigned int      reserved0        : 5;
    unsigned int      dst_drq_type     : 5;            //Ŀ�ĵ�ַ�洢���ͣ���DRAM, SPI,NAND��
    unsigned int      dst_addr_type    : 2;            //Ŀ�ĵ�ַ���ͣ�����������߲���  0:����ģʽ  1:���ֲ���
    unsigned int      dst_burst_length : 2;            //����һ��burst��� ��0��Ӧ��1����1��Ӧ��4,
    unsigned int      dst_data_width   : 2;            //���ݴ����ȣ�0:һ�δ���8bit��1:һ�δ���16bit��2:һ�δ���32bit��3:����
    unsigned int      reserved1        : 5;
}
__dma_config_t;

typedef struct __dma_setting
{
	__dma_config_t  cfg;
	unsigned int	loop_mode;
	unsigned int	data_block_size;
	unsigned int	wait_cyc;
}
__dma_setting_t;


#endif	/* _EGON2_DMAC_I_H_ */

