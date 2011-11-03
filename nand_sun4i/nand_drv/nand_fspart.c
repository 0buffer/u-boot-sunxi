/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON file system sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : fspart
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
#include <common.h>
#include <asm/arch/nand_fspart.h>
#include <asm/arch/nand_bsp.h>

typedef struct tag_CRC32_DATA
{
	unsigned int CRC;				//int�Ĵ�С��32λ
	unsigned int CRC_32_Tbl[256];	//�����������
}CRC32_DATA_t;

char BOOTFSMBR_buf[1024] __attribute__ ((packed));
/*
************************************************************************************************************
*
*                                             _calc_crc32
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
static __u32 _calc_crc32(void * buffer, __u32 length)
{
	__u32 i, j;
	CRC32_DATA_t crc32;		//
	__u32 CRC32 = 0xffffffff; //���ó�ʼֵ

	crc32.CRC = 0;

	for( i = 0; i < 256; ++i)//��++i�����Ч��
	{
		crc32.CRC = i;
		for( j = 0; j < 8 ; ++j)
		{
			//���ѭ��ʵ���Ͼ�����"���㷨"����ȡCRC��У����
			if(crc32.CRC & 1)
				crc32.CRC = (crc32.CRC >> 1) ^ 0xEDB88320;
			else //0xEDB88320����CRC-32������ʽ��ֵ
				crc32.CRC >>= 1;
		}
		crc32.CRC_32_Tbl[i] = crc32.CRC;
	}

	CRC32 = 0xffffffff; //���ó�ʼֵ
    for( i = 0; i < length; ++i)
    {
        CRC32 = crc32.CRC_32_Tbl[(CRC32^((unsigned char*)buffer)[i]) & 0xff] ^ (CRC32>>8);
    }

    //return CRC32;
	return CRC32^0xffffffff;
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
int FS_getpart_count(void)
{
	MBR        *mbr  = (MBR*)BOOTFSMBR_buf;

	return mbr->PartCount;
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
int FS_getpart_start(int part_index)
{
	MBR        *mbr  = (MBR*)BOOTFSMBR_buf;

	if(part_index >= mbr->PartCount)
	{
		return -1;
	}

	return mbr->array[part_index].addrlo;
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
int FS_getpart_capacity(int part_index)
{
	MBR        *mbr  = (MBR*)BOOTFSMBR_buf;

	if(part_index >= mbr->PartCount)
	{
		return -1;
	}

	return mbr->array[part_index].lenlo;
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
int bootfs_scan_part(void)
{
   int i, part_index = 0;
   MBR    *mbr;

   NAND_LogicRead(0, MBR_SIZE >> 9 , BOOTFSMBR_buf);

   mbr = (MBR*)BOOTFSMBR_buf;
   if(!strncmp((const char*)mbr->magic, MBR_MAGIC, 8))
   {
        int crc = 0;

        crc = _calc_crc32(&mbr->version, MBR_SIZE-4);
        if(crc == mbr->crc32)
        {
            if(mbr->PartCount < 32)
            {
                PARTITION* pe = mbr->array;

                for(i=0; i<mbr->PartCount && i < MBR_MAX_PART_COUNT; i++, pe++)
                {
                    unsigned long long  len, addr;

                    addr = pe->addrhi;
                    addr = (addr << 32) | pe->addrlo;
                    len  = pe->lenhi;
                    len  = (len << 32) | pe->lenlo;

					if(!len)
					{
						len = NAND_GetDiskSize() - addr;
						pe->lenlo = len & 0xffffffff;
					}
                    part_index ++;
                }
            }
        }
   	}

	return part_index;
}



