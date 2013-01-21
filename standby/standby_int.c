/*
**********************************************************************************************************************
*											        eGon
*						                     the Embedded System
*									       script parser sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : standby.c
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
#include "standby_i.h"
#include <asm/arch/gic.h>
/*
************************************************************************************************************
*
*                                             eGon2_int_enter_standby
*
*    �������ƣ�
*
*    �����б�
*
*
*
*    ����ֵ  ��
*
*    ˵��    ��	����standby
*				1) ���DMA PENDING������DMA enable
*				2) �ر�DMA AHB
*
************************************************************************************************************
*/
int standby_int_disable(void)
{
	asm("mrs r0, cpsr");
	asm("orr r0, r0, #(0x40|0x80)");
	asm("msr cpsr_c, r0");

    return 0;
}
/*
************************************************************************************************************
*
*                                             eGon2_int_exit_standby
*
*    �������ƣ�
*
*    �����б�
*
*
*
*    ����ֵ  ��
*
*    ˵��    ��	�˳�standby
*				1) ��DMA AHB
*				2) ���DMA PENDING���ָ�DMA enable
*
*
************************************************************************************************************
*/
int standby_int_enable(void)
{
	asm("mrs r0, cpsr");
	asm("bic r0, r0, #(0x40|0x80)");
	asm("msr cpsr_c, r0");

    return 0;
}
/*
*********************************************************************************************************
*										   EnableInt
*
* Description:  ʹ���ж�
*
* Arguments	 : irq_no     �жϺ�
*
* Returns	 :
*
*********************************************************************************************************
*/
static int standby_enable_irq(__u32 irq_no)
{
	__u32 reg_val;
	__u32 offset;

	if (irq_no >= GIC_IRQ_NUM)
	{
		return -1;
	}

	offset   = irq_no >> 5; // ��32
	reg_val  = readl(GIC_SET_EN(offset));
	reg_val |= 1 << (irq_no & 0x1f);
	writel(reg_val, GIC_SET_EN(offset));

    return 0;
}
/*
*********************************************************************************************************
*										   DisableInt
*
* Description:  ��ֹ�ж�
*
* Arguments	 : irq_no     �жϺ�
*
* Returns	 :
*
*********************************************************************************************************
*/
static __s32 standby_disable_irq(__u32 irq_no)
{
	__u32 reg_val;
	__u32 offset;

	if (irq_no >= GIC_IRQ_NUM)
	{
		return -1;
	}

	offset   = irq_no >> 5; // ��32
	reg_val  = readl(GIC_SET_EN(offset));
	reg_val &= ~(1 << (irq_no & 0x1f));
	writel(reg_val, GIC_SET_EN(offset));

    return 0;
}
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
void standby_gic_store(void)
{
	standby_enable_irq(AW_IRQ_NMI);
}
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
void standby_gic_restore(void)
{
	standby_disable_irq(AW_IRQ_NMI);
}
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
void standby_gic_clear_pengding(void)
{
	__u32 reg_val;
	__u32 offset;
	__u32 idnum;

	idnum = readl(GIC_INT_ACK_REG);

	writel(idnum, GIC_END_INT_REG);
	writel(idnum, GIC_DEACT_INT_REG);

	offset = idnum >> 5; // ��32
	reg_val = readl(GIC_PEND_CLR(offset));
	reg_val |= (1 << (idnum & 0x1f));
	writel(reg_val, GIC_PEND_CLR(offset));

	return ;
}
