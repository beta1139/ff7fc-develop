#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp24bit.h"

BMP24BIT::BMP24BIT(char* file_name)
{
	FILE* fp = fopen(file_name, "rb");
	if(fp == NULL){ printf("file open error (%s)\n", file_name); exit(1); }

	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	fread(&m_bmp, sizeof(FileHeader)+sizeof(InfoHeader), 1, fp);

	m_w = (m_bmp.info_header.bcWidth[0])     +
		  (m_bmp.info_header.bcWidth[1]<<8)  +
		  (m_bmp.info_header.bcWidth[2]<<16) +
		  (m_bmp.info_header.bcWidth[3]<<24);

	m_h = (m_bmp.info_header.bcHeight[0])     +
		  (m_bmp.info_header.bcHeight[1]<<8)  +
		  (m_bmp.info_header.bcHeight[2]<<16) +
		  (m_bmp.info_header.bcHeight[3]<<24);

	m_bmp.data = (UC*)malloc(m_w*m_h*3);
	fread(m_bmp.data, m_w*m_h*3, 1, fp);

	fclose(fp);

	int bit_cnt = ((m_bmp.info_header.bcBitCount[0]) + (m_bmp.info_header.bcBitCount[1] << 8));
	if(bit_cnt != 24)
	{
		printf("%s is not 24 bit (%x)\n", file_name, bit_cnt);
		exit(1);
	}
}

BMP24BIT::~BMP24BIT(void)
{
	;
}

int BMP24BIT::get_w(void)
{
	return m_w;
}

int BMP24BIT::get_h(void)
{
	return m_h;
}

UI BMP24BIT::get_col(int x, int y)
{
	UI col_data = 0;

	col_data = ((m_bmp.data[m_w*m_h*3 - m_w*3 + x*3 - y*m_w*3 + 2] << 16) |
			    (m_bmp.data[m_w*m_h*3 - m_w*3 + x*3 - y*m_w*3 + 1] <<  8) |
			    (m_bmp.data[m_w*m_h*3 - m_w*3 + x*3 - y*m_w*3 + 0] <<  0));

	return col_data;
}

void BMP24BIT::set_col(int x, int y, UI col)
{
	m_bmp.data[m_w*m_h*3 - m_w*3 + x*3 - y*m_w*3 + 2] = (col >> 16) & 0xff;
	m_bmp.data[m_w*m_h*3 - m_w*3 + x*3 - y*m_w*3 + 1] = (col >>  8) & 0xff;
	m_bmp.data[m_w*m_h*3 - m_w*3 + x*3 - y*m_w*3 + 0] = (col >>  0) & 0xff;
}

void BMP24BIT::save_bmp(char* file_name)
{
	FILE* fp = fopen(file_name, "wb");
	if(fp == NULL){ printf("file open error (%s)\n", file_name); exit(1); }

	fwrite(&m_bmp, sizeof(FileHeader)+sizeof(InfoHeader), 1, fp);
	fwrite(&m_bmp.data[0], m_w*m_h*3, 1, fp);

	fclose(fp);
}

