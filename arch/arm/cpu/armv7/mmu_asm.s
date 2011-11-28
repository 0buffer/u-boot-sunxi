/*
;*****************************************************************************************************************
;                                                         eGON
;                                         the Embedded GO-ON Bootloader System
;
;                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
;											       All Rights Reserved
;
; File Name : mmu_asm.s
;
; Author : Gary.Wang
;
; Version : 1.1.0
;
; Date : 2009.03.20
;
; Description :
;
; Others : None at present.
;
;
; History :
;
;  <Author>        <time>       <version>      <description>
;
; Gary.Wang       2009.03.20      1.1.0        build the file
;
;******************************************************************************************************************
*/
#include <asm/arch/cpu.h>

/*
;*******************************************************************************
;��������: mmu_set_ttb
;����ԭ��: void mmu_set_ttb( uint32 ttb_base_addr )
;��������: set TTB
;��ڲ���: ttb base address
;�� �� ֵ: void
;��    ע:
;*******************************************************************************
*/
.global mmu_set_ttb
mmu_set_ttb:
	MCR p15, 0, r0, c2, c0, 0
	MOV pc, lr

/*
;*******************************************************************************
;��������: mmu_change_domain_access
;����ԭ��: s32   mmu_change_domain_access( uint32 value, uint32 mask )
;��������: Change Domain Access Register Values
;��ڲ���: value
;          mask
;�� �� ֵ: void
;��    ע:
;*******************************************************************************
*/
.global mmu_set_domain_access
mmu_set_domain_access:
	MRC     p15, 0, r0, c3, c0, 0
	LDR     r0, =0x55555555
	MCR     p15, 0, r0, c3, c0, 0
    MOV     pc, lr


/*
;*******************************************************************************
;��������: mmu_flush_TLB
;����ԭ��: void   mmu_flush_TLB( void )
;��������: clear TLB
;��ڲ���: void
;�� �� ֵ: void
;��    ע:
;*******************************************************************************
*/
.global mmu_flush_TLB
mmu_flush_TLB:
    MOV r0, #0
    MCR p15, 0, r0, c8, c7, 0
    MOV pc, lr

