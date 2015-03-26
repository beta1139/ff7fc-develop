#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include "mapdata.h"

MAPDATA::MAPDATA(char* file_name)
{
	FILE* fp = fopen(file_name, "rb");
	if(fp == NULL)
	{
		m_w = 0;
		m_h = 0;
		m_data = NULL;
		return ;
	}
	
	fseek(fp, 0L, SEEK_END);
	m_size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	m_data = (UC*)malloc(m_size);
	fread(m_data, m_size, 1, fp);

	fseek(fp, 0L, SEEK_SET);
	m_w = fgetc(fp);
	m_h = fgetc(fp);

	fclose(fp);
}

MAPDATA::~MAPDATA(void)
{
	if(m_data != NULL)
	{
		free(m_data);
		m_data = NULL;
	}
}

int MAPDATA::get_w(void)
{
	return (m_w*16);
}

int MAPDATA::get_h(void)
{
	return (m_h*15);
}

UI MAPDATA::get_chip_idx(int x, int y)
{
	int data_pos = 2+x*2+y*m_w*16*2;
	UI chip_idx = m_data[data_pos]*256 + m_data[data_pos+1];
	return chip_idx;
}

void MAPDATA::set_chip_idx(int x, int y, UI chip_idx)
{
	int data_pos = 2+x*2+y*m_w*16*2;
	m_data[data_pos]   = chip_idx/256;
	m_data[data_pos+1] = chip_idx%256;
}

void MAPDATA::save(char* file_name)
{
	FILE* fp = fopen(file_name, "wb");
	if(fp == NULL) { DBG_PRINT_ARG("%s", file_name); exit(1); }
	fwrite(m_data, m_size, 1, fp);
	fclose(fp);
}

