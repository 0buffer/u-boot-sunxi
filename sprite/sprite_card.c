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
#include <malloc.h>
#include "sparse/sparse.h"
#include <asm/arch/queue.h>
#include <asm/arch/sunxi_mbr.h>
#include "encrypt/encrypt.h"
#include "sprite_queue.h"
#include "sprite_download.h"
#include "sprite_verify.h"
#include "firmware/imgdecode.h"

#define  SPRITE_CARD_ONCE_DATA_DEAL    (512 * 1024)
#define  SPRITE_CARD_ONCE_SECTOR_DEAL  (SPRITE_CARD_ONCE_DATA_DEAL/512)

static void *imghd = NULL;
static void *imgitemhd = NULL;
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
uint sprite_card_firmware_start(void)
{
	return sunxi_partition_get_offset(1);
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
int sprite_card_firmware_probe(void)
{
	imghd = Img_Open("c:/imgfile");
	if(!imghd)
	{
		return -1;
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
int sprite_card_fetch_download_map(sunxi_download_info  *dl_map)
{
	imgitemhd = Img_OpenItem(imghd, "12345678", "1234567890dlinfo");
	if(!imgitemhd)
	{
		return -1;
	}
	debug("try to read item dl map\n");
	if(!Img_ReadItem(imghd, imgitemhd, (void *)dl_map, sizeof(sunxi_download_info)))
	{
		printf("sunxi sprite error : read dl map failed\n");

		return -1;
	}
	Img_CloseItem(imghd, imgitemhd);
	imgitemhd = NULL;

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

uint (* sunxi_sprite_decode)(void * ibuf, uint len);

int  (* sunxi_sprite_download_data)(void *this_buffer, void *next_buffer, uint len);

void *(* sunxi_sprite_next_buffer)(queue_data *qdata, void *buffer);

static uint decode_null(void * ibuf, uint len)
{
	debug("deocde null\n");
	return (uint)ibuf;
}

static uint decode_need(void * ibuf, uint len)
{
	char buffer[SPRITE_CARD_ONCE_DATA_DEAL];

	debug("deocde need\n");
	decode(ibuf, buffer, len);
	memcpy(ibuf, buffer, len);

	return (uint)ibuf;
}

static void *sunxi_sprite_decode_next_buffer(queue_data *qdata, void *buffer)
{
	return buffer;
}

static void *sunxi_sprite_nodecode_next_buffer(queue_data *qdata, void *buffer)
{
	return qdata->data;
}
#if 0
//int sunxi_sprite_card_download_part(sunxi_download_info *dl_map)
//{
//	long long part_datasize;      //��������Ĵ�С(�ֽڵ�λ)
//	long long part_tmpsize;       //��������Ĵ�С�����ڵݼ�������ʱ��ʾ�������
//    int  i = 0;
//    int  imgfile_start;
//    int  data_format;                //��ʽ
//    uint base_part_start, part_flash_size;    //������ʼ�����ʹ�С(������λ)
//    queue_data  *qdata, *next_qdata; //����ȡ��һ��buffer
//    uint origin_verify, active_verify; //У������
//    char  *base_buffer, *half_buffer;
//    char  *decode_buffer;
//    char  *next_buffer;
//
//	base_buffer = malloc(SPRITE_CARD_ONE_DATA_DEAL);
//	if(!base_buffer)
//	{
//		printf("sunxi sprite: fail to malloc memory for post-treatment\n");
//
//		return -1;
//	}
//	half_buffer = base_buffer + 512 * 1024;
//	qdata = next_qdata = NULL;
//
//    for(i=0;i<dl_map->download_count;i++)
//    {
//    	//����һ����������Ҫ��д��������
//    	//�������buffer
//    	//sunxi_queue_reset();
//    	//USB������Ҫ��������Ϊdl_filename������
//    	base_part_start = dl_map->one_part_info[i].addrlo;
//        part_flash_size = dl_map->one_part_info[i].lenlo;
//        part_tmpsize    = (long long)part_flash_size * 9;
//    	//��������ֱ�Ӷ�ȡ
//    	//���߶��������ж��Զ�ִ�еķ�ʽ����ͣ�����buffer�ռ�
//    	//�������ɣ�û����buffer���ã���������ʱ�жϲ�ѯ�Ƿ����¿ռ�
//		imgitemhd = Img_OpenItem(imghd, "RFSFAT16", (char *)dl_map->one_part_info[i].dl_filename);
//		if(!imgitemhd)
//		{
//			printf("sprite error: open part %s failed\n", dl_map->one_part_info[i].dl_filename);
//
//			goto _card_download_error;
//		}
//    	part_datasize = Img_GetItemSize(imghd, imgitemhd);
//        if(part_datasize > part_tmpsize)      //��������С�Ƿ�Ϸ�
//        {
//        	//data is larger than part size
//        	printf("sunxi sprite: data size is larger than part %s size\n", dl_map->one_part_info[i].dl_filename);
//
//        	goto _card_download_error;
//        }
//        //��ʼ��ȡ��������
//		imgfile_start = Img_GetItemStart(imghd, imgitemhd);
//		if(!imgfile_start)
//		{
//			printf("sunxi sprite: cant get part data imgfile_start\n");
//
//			goto _card_download_error;
//		}
//		//֪ͨ������������׼����imgfile_start��������ȡpart_datasize���ȵ�����
//		//sunxi_sprite_card_read_sequence(imgfile_start, (uint)((part_datasize + 511)/512));
//		//��ʼ��ȡ
////		if(sunxi_sprite_card_read_sequence())
////		{
////		    printf("sunxi sprite: cant read data from sdcard\n");
////
////		    goto _card_download_error;
////		}
//		//��ʼѭ��������������
//		part_tmpsize = part_datasize;
//		if(dl_map->one_part_info[i].encrypt)
//		{
//			//��Ҫ����
//			sunxi_sprite_decode = decode;
//			sunxi_sprite_next_buffer = sunxi_sprite_decode_next_buffer;
//			init_code();
//		}
//		else
//		{
//			//����Ҫ����
//			sunxi_sprite_decode = decode_null;
//			sunxi_sprite_next_buffer = sunxi_sprite_nodecode_next_buffer;
//		}
//		//���Ȼ�ȡһ��buffer�����ݣ������ж����ݸ�ʽ
//		while(sunxi_queue_isempty());       	//���buffer�����ǿյ���ȴ���ֱ����buffer����
//		sunxi_outqueue_query(qdata, next_qdata);		//ȡ��һ��buffer
//		decode_buffer = (char *)sunxi_sprite_decode((void *)qdata->data, (void *)half_buffer, qdata->len);
//		data_format = unsparse_probe((char *)decode_buffer, qdata->len, base_part_start);		//�ж����ݸ�ʽ
//		if(data_format == ANDROID_FORMAT_DETECT)
//		{
//			sunxi_sprite_download_data = unsparse_direct_write;
//		}
//		else if(data_format == ANDROID_FORMAT_BAD)
//		{
//			sunxi_sprite_download_raw_init(base_part_start);
//			sunxi_sprite_download_data = sunxi_sprite_download_raw;
//		}
//		//�ѵ�һ������д��ȥ
//		part_tmpsize -= qdata->len;
//		//���ݸ�ʽ�жϣ������sparse��ʽ
//		next_buffer = sunxi_sprite_next_buffer(next_qdata, half_buffer);
//		if(sunxi_sprite_download_data((void *)decode_buffer, (void *)next_buffer, qdata->len))
//		{
//			printf("sunxi sprite error: download data error\n");
//
//			goto _card_download_error;
//		}
//		sunxi_queue_out();
//		while(part_tmpsize)
//		{
//			if(!sunxi_queue_isfull())     //���buffer���в������ģ���������ڼ�����Ź̼�����
//			{
//				if(0 == sunxi_sprite_card_sequence_status()) //���������idle״̬������Զ��̼�����
//				{
//					//���ڣ�������idle״̬��ͬʱ��buffer���ã�����ÿ�����������ȡ����
//					if(sunxi_sprite_card_read_sequence())         //������޷���ȡ���ݣ��򷵻�ʧ��
//					{
//				    	printf("sunxi sprite: cant read sdcard by sequence\n");
//
//				    	goto _card_download_error;
//					}
//				}
//			}
//			//���ڴ����Ѿ���ɵ����ݣ�׼�����ܺ���¼
//			if(!sunxi_outqueue_query(qdata, next_qdata))    //���buffer�����п���ȡ��һ����Чbuffer
//			{
//				//����ִ�н��ܺ���,������û�м��ܣ���������Ҫ����
//				//����Ҫ����ʱ��real_dest_bufferָ��half_buffer
//				//������Ҫ����ʱ��real_dest_bufferָ��qdata.data
//				decode_buffer = (char *)sunxi_sprite_decode((void *)qdata->data, (void *)half_buffer, qdata->len);
//				//���ݸ�ʽ�жϣ������sparse��ʽ
//				next_buffer = sunxi_sprite_next_buffer(next_qdata, half_buffer);
//				if(sunxi_sprite_download_data((void *)decode_buffer, (void *)next_buffer, qdata->len))
//				{
//					printf("sunxi sprite error: download data error\n");
//
//					goto _card_download_error;
//				}
//				sunxi_queue_out();
//			}
//		}
//		exit_code();
//		Img_CloseItem(imghd, imgitemhd);
//		//У������
//        if(dl_map->one_part_info[i].vf_filename)
//        {
//        	imgitemhd = Img_OpenItem(imghd, "RFSFAT16", (char *)dl_map->one_part_info[i].vf_filename);
//			if(!imgitemhd)
//			{
//				printf("sprite error: open part %s failed\n", dl_map->one_part_info[i].vf_filename);
//				Img_CloseItem(imghd, imgitemhd);
//
//				continue;
//			}
//        	if(!Img_ReadItem(imghd, imgitemhd, (void *)&origin_verify, sizeof(int)))   //��������
//	        {
//	            printf("sprite update warning: fail to read data from %s\n", dl_map->one_part_info[i].vf_filename);
//				//Img_CloseItem(imghd, imgitemhd);
//	            continue;
//	        }
//        	if(data_format == ANDROID_FORMAT_BAD)
//        	{
//                active_verify = sunxi_sprite_part_rawdata_verify(base_part_start, part_datasize);
//            }
//            else
//            {
//            	active_verify = sunxi_sprite_part_sparsedata_verify();
//            }
//            if(origin_verify != active_verify)
//            {
//            	printf("sunxi sprite: part %s verify error\n", dl_map->one_part_info[i].dl_filename);
//            	printf("origin checksum=%x, active checksum=%x\n", origin_verify, active_verify);
//            	Img_CloseItem(imghd, imgitemhd);
//
//            	return -1;
//            }
//            Img_CloseItem(imghd, imgitemhd);
//        }
//        else
//        {
//        	printf("sunxi sprite: part %s not need to verify\n", dl_map->one_part_info[i].dl_filename);
//        }
//    }
//
//    return 0;
//_card_download_error:
//	return -1;
//}
#else
int sunxi_sprite_card_download_part(sunxi_download_info *dl_map)
{
	uint part_datasectors;   // ��Ҫ��д�ķ�������������
	uint part_tmpsectors;    // �ݼ�ֵ��Ϊ0ʱ��ʾ��д���
    int  i = 0;
    uint imgfile_start, tmp_file_start;
    uint read_one_sectors;
    int  data_format;                //��ʽ
    uint base_part_start, part_flash_size;    //������ʼ�����ʹ�С(������λ)
    uint origin_verify, active_verify; //У������
    char  *base_buffer = NULL, *half_buffer;
    char  *decode_buffer;
    int  ret = -1;

	base_buffer = (char *)malloc(SPRITE_CARD_ONCE_DATA_DEAL * 2);
	if(!base_buffer)
	{
		printf("sunxi sprite: fail to malloc memory for post-treatment\n");

		return -1;
	}
	half_buffer = base_buffer + SPRITE_CARD_ONCE_DATA_DEAL;
	//qdata = next_qdata = NULL;
	debug("total download part %d\n", dl_map->download_count);

    for(i=0;i<dl_map->download_count;i++)
    {
    	//����һ����������Ҫ��д��������
    	//�������buffer
    	//sunxi_queue_reset();
    	//USB������Ҫ��������Ϊdl_filename������
    	debug("download %d part %s\n", i, dl_map->one_part_info[i].dl_filename);
    	base_part_start = dl_map->one_part_info[i].addrlo;
        part_flash_size = dl_map->one_part_info[i].lenlo;
        //��������ֱ�Ӷ�ȡ
    	//���߶��������ж��Զ�ִ�еķ�ʽ����ͣ�����buffer�ռ�
    	//�������ɣ�û����buffer���ã���������ʱ�жϲ�ѯ�Ƿ����¿ռ�
    	imgitemhd = Img_OpenItem(imghd, "RFSFAT16", (char *)dl_map->one_part_info[i].dl_filename);
		if(!imgitemhd)
		{
			printf("sprite error: open part %s failed\n", dl_map->one_part_info[i].dl_filename);

			goto _card_download_error;
		}
		part_datasectors = Img_GetItemSize(imghd, imgitemhd)>>9;
    	debug("part size=%d\n", (uint)part_flash_size);
		debug("part data=%d\n", (uint)part_datasectors);
        if(part_datasectors > part_flash_size)      //��������С�Ƿ�Ϸ�
        {
        	//data is larger than part size
        	printf("sunxi sprite: data size is larger than part %s size\n", dl_map->one_part_info[i].dl_filename);

        	goto _card_download_error;
        }
        //��ʼ��ȡ��������
		imgfile_start = Img_GetItemStart(imghd, imgitemhd);
		if(!imgfile_start)
		{
			printf("sunxi sprite: cant get part data imgfile_start\n");

			goto _card_download_error;
		}
		debug("part data start sector=%d\n", imgfile_start);
		tmp_file_start = imgfile_start;
		//֪ͨ������������׼����imgfile_start��������ȡpart_datasize���ȵ�����
		//sunxi_sprite_card_read_sequence(imgfile_start, (uint)((part_datasize + 511)/512));
		//��ʼ��ȡ
//		if(sunxi_sprite_card_read_sequence())
//		{
//		    printf("sunxi sprite: cant read data from sdcard\n");
//
//		    goto _card_download_error;
//		}
		//��ʼѭ��������������
		part_tmpsectors = part_datasectors;
		if(dl_map->one_part_info[i].encrypt)
		{
			//��Ҫ����
			sunxi_sprite_decode = decode_need;
//			sunxi_sprite_next_buffer = sunxi_sprite_decode_next_buffer;
			init_code();
		}
		else
		{
			//����Ҫ����
			sunxi_sprite_decode = decode_null;
//			sunxi_sprite_next_buffer = sunxi_sprite_nodecode_next_buffer;
		}
		if(part_tmpsectors >= SPRITE_CARD_ONCE_DATA_DEAL)
		{
			read_one_sectors = SPRITE_CARD_ONCE_SECTOR_DEAL;
		}
		else
		{
			read_one_sectors = part_tmpsectors;
		}
		if(sunxi_flash_read(tmp_file_start, read_one_sectors, half_buffer) != read_one_sectors)
		{
			printf("sunxi sprite error : read sdcard block %d, total %d failed\n", tmp_file_start, read_one_sectors);

			goto _card_download_error;
		}
		//���Ȼ�ȡһ��buffer�����ݣ������ж����ݸ�ʽ
		decode_buffer = (char *)sunxi_sprite_decode(half_buffer, read_one_sectors * 512);
		data_format = unsparse_probe((char *)decode_buffer, read_one_sectors, base_part_start);		//�ж����ݸ�ʽ
		if(data_format == ANDROID_FORMAT_DETECT)
		{
			sunxi_sprite_download_data = unsparse_direct_write;
		}
		else if(data_format == ANDROID_FORMAT_BAD)
		{
			sunxi_sprite_download_raw_init(base_part_start);
			sunxi_sprite_download_data = sunxi_sprite_download_raw;
		}
		//�ѵ�һ������д��ȥ
		part_tmpsectors -= read_one_sectors;
		tmp_file_start += read_one_sectors;
		//���ݸ�ʽ�жϣ������sparse��ʽ
		debug("write first data\n");
		if(sunxi_sprite_download_data((void *)decode_buffer, (void *)decode_buffer, read_one_sectors))
		{
			printf("sunxi sprite error: download data error\n");

			goto _card_download_error;
		}
		debug("first data write ok\n");
		read_one_sectors = SPRITE_CARD_ONCE_SECTOR_DEAL;
		while(part_tmpsectors >= SPRITE_CARD_ONCE_DATA_DEAL)
		{
			debug("rest part sectors=%d, once sectors=%d\n", part_tmpsectors, SPRITE_CARD_ONCE_DATA_DEAL);
			if(sunxi_flash_read(tmp_file_start, read_one_sectors, half_buffer) != read_one_sectors)
			{
				printf("sunxi sprite error : read sdcard block %d, total %d failed\n", tmp_file_start, read_one_sectors);

				goto _card_download_error;
			}
			decode_buffer = (char *)sunxi_sprite_decode(half_buffer, read_one_sectors * 512);
			//���ڴ����Ѿ���ɵ����ݣ�׼�����ܺ���¼
				//���ݸ�ʽ�жϣ������sparse��ʽ
			if(sunxi_sprite_download_data((void *)decode_buffer, (void *)decode_buffer, read_one_sectors))
			{
				printf("sunxi sprite error: download data error\n");

				goto _card_download_error;
			}
		}
		exit_code();
		Img_CloseItem(imghd, imgitemhd);
		imgitemhd = NULL;
		//У������
		debug("part data download finish\n");
#if 1
        debug("try to read verify file %s\n", dl_map->one_part_info[i].vf_filename);
        if(dl_map->one_part_info[i].vf_filename)
        {
        	imgitemhd = Img_OpenItem(imghd, "RFSFAT16", (char *)dl_map->one_part_info[i].vf_filename);
			if(!imgitemhd)
			{
				printf("sprite error: open part %s failed\n", dl_map->one_part_info[i].vf_filename);
				Img_CloseItem(imghd, imgitemhd);
				imgitemhd = NULL;

				continue;
			}
			debug("img open ok\n");
        	if(!Img_ReadItem(imghd, imgitemhd, (void *)&origin_verify, sizeof(int)))   //��������
	        {
	            printf("sprite update warning: fail to read data from %s\n", dl_map->one_part_info[i].vf_filename);
				Img_CloseItem(imghd, imgitemhd);
				imgitemhd = NULL;

	            continue;
	        }
	        debug("img data read ok\n");
        	if(data_format == ANDROID_FORMAT_BAD)
        	{
                active_verify = sunxi_sprite_part_rawdata_verify(base_part_start, part_datasectors);
            }
            else
            {
            	active_verify = sunxi_sprite_part_sparsedata_verify();
            }
            if(origin_verify != active_verify)
            {
            	printf("origin checksum=%x, active checksum=%x\n", origin_verify, active_verify);
            	printf("sunxi sprite: part %s verify error\n", dl_map->one_part_info[i].dl_filename);
            	Img_CloseItem(imghd, imgitemhd);
            	imgitemhd = NULL;

            	goto _card_download_error;
            }
            Img_CloseItem(imghd, imgitemhd);
            imgitemhd = NULL;
        }
        else
        {
        	printf("sunxi sprite: part %s not need to verify\n", dl_map->one_part_info[i].dl_filename);
        }
#endif
    }
	debug("all download part ok\n");
	ret = 0;

_card_download_error:
	if(base_buffer)
	{
		free(base_buffer);
	}
	if(imgitemhd)
	{
		Img_CloseItem(imghd, imgitemhd);
		imgitemhd = NULL;
	}

	return ret;
}
#endif
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
int sunxi_sprite_deal_uboot(int production_media)
{
	char buffer[1024 * 1024];
	uint item_original_size;

    imgitemhd = Img_OpenItem(imghd, "BOOT    ", "UBOOT_0000000000");
    if(!imgitemhd)
    {
        printf("sprite update error: fail to open uboot item\n");
        return -1;
    }
    //uboot����
    item_original_size = Img_GetItemSize(imghd, imgitemhd);
    {
        printf("sprite update error: fail to get uboot item size\n");
        return -1;
    }
    /*��ȡuboot������*/
    if(!Img_ReadItem(imghd, imgitemhd, (void *)buffer, item_original_size))
    {
        printf("update error: fail to read data from for uboot\n");
        return -1;
    }
    Img_CloseItem(imghd, imgitemhd);
    imgitemhd = NULL;

    if(sunxi_sprite_download_uboot(buffer, production_media))
    {
    	printf("update error: fail to write uboot\n");
        return -1;
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
int sunxi_sprite_deal_boot0(int production_media)
{
	char buffer[1024 * 1024];
	uint item_original_size;

	if(production_media == 0)
	{
		imgitemhd = Img_OpenItem(imghd, "BOOT    ", "NANDBOOT0_000000");
	}
	else
	{
		imgitemhd = Img_OpenItem(imghd, "BOOT    ", "CARDBOOT0_000000");
	}
    if(!imgitemhd)
    {
        printf("sprite update error: fail to open boot0 item\n");
        return -1;
    }
    //boot0����
    item_original_size = Img_GetItemSize(imghd, imgitemhd);
    if(!item_original_size)
    {
        printf("sprite update error: fail to get boot0 item size\n");
        return -1;
    }
    /*��ȡboot0������*/
    if(!Img_ReadItem(imghd, imgitemhd, (void *)buffer, item_original_size))
    {
        printf("update error: fail to read data from for boot0\n");
        return -1;
    }
    Img_CloseItem(imghd, imgitemhd);
    imgitemhd = NULL;

    if(sunxi_sprite_download_boot0(buffer, production_media))
    {
    	printf("update error: fail to write boot0\n");
        return -1;
    }

    return 0;
}

