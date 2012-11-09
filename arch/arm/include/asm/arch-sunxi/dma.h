/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm driver sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : wboot_dma
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
#ifndef	_DMA_H_
#define	_DMA_H_


//================================
//======    DMA ����     =========
//================================

/* DMA ��������  */
#define DMAC_CFG_CONTINUOUS_ENABLE              (0x01)	//(0x01<<29)
#define DMAC_CFG_CONTINUOUS_DISABLE             (0x00)	//(0x01<<29)

//* DMA ʱ�� */
#define	DMAC_CFG_WAIT_1_DMA_CLOCK				(0x00)	//(0x00<<26)
#define	DMAC_CFG_WAIT_2_DMA_CLOCK				(0x01)	//(0x01<<26)
#define	DMAC_CFG_WAIT_3_DMA_CLOCK				(0x02)	//(0x02<<26)
#define	DMAC_CFG_WAIT_4_DMA_CLOCK				(0x03)	//(0x03<<26)
#define	DMAC_CFG_WAIT_5_DMA_CLOCK				(0x04)	//(0x04<<26)
#define	DMAC_CFG_WAIT_6_DMA_CLOCK				(0x05)	//(0x05<<26)
#define	DMAC_CFG_WAIT_7_DMA_CLOCK				(0x06)	//(0x06<<26)
#define	DMAC_CFG_WAIT_8_DMA_CLOCK				(0x07)	//(0x07<<26)

/* DMA ����Ŀ�Ķ� ���� */
/* DMA Ŀ�Ķ� ������ */
#define	DMAC_CFG_DEST_DATA_WIDTH_8BIT			(0x00)	//(0x00<<24)
#define	DMAC_CFG_DEST_DATA_WIDTH_16BIT			(0x01)	//(0x01<<24)
#define	DMAC_CFG_DEST_DATA_WIDTH_32BIT			(0x02)	//(0x02<<24)

/* DMA Ŀ�Ķ� ͻ������ģʽ */
#define	DMAC_CFG_DEST_1_BURST       			(0x00)	//(0x00<<23)
#define	DMAC_CFG_DEST_4_BURST		    		(0x01)	//(0x01<<23)

/* DMA Ŀ�Ķ� ��ַ�仯ģʽ */
#define	DMAC_CFG_DEST_ADDR_TYPE_LINEAR_MODE		(0x00)	//(0x00<<21)
#define	DMAC_CFG_DEST_ADDR_TYPE_IO_MODE 		(0x01)	//(0x01<<21)
#define	DMAC_CFG_DEST_ADDR_TYPE_HPAGE_MODE 		(0x02)	//(0x02<<21)
#define	DMAC_CFG_DEST_ADDR_TYPE_VPAGE_MODE 		(0x03)	//(0x03<<21)


/* DMA ����Դ�� ���� */
/* DMA Դ�� ������ */
#define	DMAC_CFG_SRC_DATA_WIDTH_8BIT			(0x00)	//(0x00<<8)
#define	DMAC_CFG_SRC_DATA_WIDTH_16BIT			(0x01)	//(0x01<<8)
#define	DMAC_CFG_SRC_DATA_WIDTH_32BIT			(0x02)	//(0x02<<8)

/* DMA Դ�� ͻ������ģʽ */
#define	DMAC_CFG_SRC_1_BURST       				(0x00)	//(0x00<<7)
#define	DMAC_CFG_SRC_4_BURST		    		(0x01)	//(0x01<<7)

/* DMA Դ�� ��ַ�仯ģʽ */
#define	DMAC_CFG_SRC_ADDR_TYPE_LINEAR_MODE		(0x00)	//(0x00<<5)
#define	DMAC_CFG_SRC_ADDR_TYPE_IO_MODE 			(0x01)	//(0x01<<5)
#define	DMAC_CFG_SRC_ADDR_TYPE_HPAGE_MODE 		(0x02)	//(0x02<<5)
#define	DMAC_CFG_SRC_ADDR_TYPE_VPAGE_MODE 		(0x03)	//(0x03<<5)


/* DMA ����Ŀ�Ķ� ���� */
#define	DMAC_CFG_DEST_TYPE_SRAM					(0x00)
#define	DMAC_CFG_DEST_TYPE_DRAM		    		(0x01)

#define	DMAC_CFG_DEST_TYPE_CODEC	    		(15)

#define	DMAC_CFG_DEST_TYPE_OTG_EP1	    		(17)
#define	DMAC_CFG_DEST_TYPE_OTG_EP2	    		(18)
#define	DMAC_CFG_DEST_TYPE_OTG_EP3	    		(19)
#define	DMAC_CFG_DEST_TYPE_OTG_EP4	    		(20)
#define	DMAC_CFG_DEST_TYPE_OTG_EP5	    		(21)
/* DMA ����Դ�� ���� */
#define	DMAC_CFG_SRC_TYPE_SRAM					(0x00)
#define	DMAC_CFG_SRC_TYPE_DRAM		    	   	(0x01)

#define	DMAC_CFG_SRC_TYPE_CODEC	    			(15)

#define	DMAC_CFG_SRC_TYPE_OTG_EP1	    		(17)
#define	DMAC_CFG_SRC_TYPE_OTG_EP2	    		(18)
#define	DMAC_CFG_SRC_TYPE_OTG_EP3	    		(19)
#define	DMAC_CFG_SRC_TYPE_OTG_EP4	    		(20)
#define	DMAC_CFG_SRC_TYPE_OTG_EP5	    		(21)


extern    unsigned int 	sunxi_dma_request			(unsigned int dmatype);
extern    int 			sunxi_dma_release			(unsigned int hdma);
extern    int 			sunxi_dma_setting			(unsigned int hdma, void *cfg);
extern    int 			sunxi_dma_start			     (unsigned int hdma, unsigned int saddr, unsigned int daddr, unsigned int bytes);
extern    int 			sunxi_dma_stop			     (unsigned int hdma);
extern    int 			sunxi_dma_querystatus		(unsigned int hdma);



#endif	//_DMA_H_

/* end of _DMA_H_ */

