#include "map_chip.h"

MAPCHIP::MAPCHIP(char* chip_name)
{
	p_chip_data = NULL;
	m_data_size = -1;

	FILE* fp;
	if((fp = fopen(chip_name, "rb")) == NULL){
		printf("mapchip open error (%s)\n", chip_name);
		return ;
	}
	else{
		m_chip_name = (char*)malloc(strlen(chip_name)+1);
		strcpy(m_chip_name, chip_name);

		fseek(fp, 0, SEEK_END);
		m_data_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		p_chip_data = (UC*)malloc(m_data_size);
		fread(p_chip_data, m_data_size, 1, fp);

		fclose(fp);
	}

	printf("<map chip data>\n");
	printf("bfSize          : %d\n", get_bfSize());
	//printf("data_size       : %d\n", m_data_size);
	printf("width           : %d\n", get_width());
	printf("height          : %d\n", get_height());
	printf("info header size: %d\n", get_bi_size());
	printf("bit count       : %d\n", get_bit_count());
	printf("chip num        : %d\n", get_chip_num());
	printf("\n");

	if(get_bi_size() != 40){ printf("error invalid info header!!\n"); exit(0); }
	if(get_bit_count() != 24){ printf("error input 24bit format bitmap image!!\n"); exit(0); }

	m_last_chip_idx = get_chip_num();
}

int MAPCHIP::get_bi_size(void) { return p_chip_data[14]; }
int MAPCHIP::get_bit_count(void) { return p_chip_data[28]; }

void MAPCHIP::set_height(UI height)
{
	p_chip_data[22] = ((height >> 0)  & 0xff);
	p_chip_data[23] = ((height >> 8)  & 0xff);
	p_chip_data[24] = ((height >> 16) & 0xff);
	p_chip_data[25] = ((height >> 24) & 0xff);
}

UI MAPCHIP::get_height(void)
{
	UI height = 0;
	height  = (p_chip_data[22] << 0);
	height += (p_chip_data[23] << 8);
	height += (p_chip_data[24] << 16);
	height += (p_chip_data[25] << 24);
	return height;
}

UI MAPCHIP::get_width(void)
{
	UI width = 0;
	width  = (p_chip_data[18] << 0);
	width += (p_chip_data[19] << 8);
	width += (p_chip_data[20] << 16);
	width += (p_chip_data[21] << 24);
	return width;
}


// 一行分のデータ量
// 16*16*3

UC MAPCHIP::get_chip_col(int idx, int x_offset, int y_offset, int col_offset)
{
	int chip_x = idx%16;
	int chip_y = idx/16;

	int offset = m_data_size       - // 画像の終端
				 get_width()*3     + // yを1line分戻す
				 chip_x*16*3       - // xのオフセット調整
				 chip_y*16*get_width()*3 +// yを指定チップ分戻す
				 x_offset*3        -
				 y_offset*get_width()*3 + col_offset;

	if(offset < 0 || m_data_size <= offset)
	{
		printf("error offset: %d, data_size: %d, idx: %d, x_offset: %d, y_offset: %d, col_offset: %d\n",
				offset, m_data_size, idx, x_offset, y_offset, col_offset);
		exit(1);
	}

	UC col = p_chip_data[offset];

	return col;
}

void MAPCHIP::get_chip_data(int chip_idx, UC* data)
{
	for(int j=0; j<16; j++){
		for(int i=0; i<16; i++){
			data[i*3+j*16*3+0] = get_chip_col(chip_idx, i, j, 0);
			data[i*3+j*16*3+1] = get_chip_col(chip_idx, i, j, 1);
			data[i*3+j*16*3+2] = get_chip_col(chip_idx, i, j, 2);
		}
	}
}

void MAPCHIP::set_chip_data(int chip_idx, UC* data)
{
	int chip_x = chip_idx%16;
	int chip_y = chip_idx/16;

	int offset = 0;

	for(int j=0; j<16; j++){
		for(int i=0; i<16; i++){
			offset = m_data_size   -
					 get_width()*3 +
					 chip_x*16*3   -
					 chip_y*16*get_width()*3 +
					 i*3           -
					 j*get_width()*3;

			p_chip_data[offset + 0] = data[i*3+j*16*3+0];
			p_chip_data[offset + 1] = data[i*3+j*16*3+1];
			p_chip_data[offset + 2] = data[i*3+j*16*3+2];
		}
	}
}

int MAPCHIP::get_chip_num(void)
{
	return ((get_width()/16) * (get_height()/16));
}

int MAPCHIP::get_last_chip_idx(void)
{
	return m_last_chip_idx;
}

void MAPCHIP::add_line(void)
{
	// backup
	UC* backup_data = (UC*)malloc(m_data_size);
	memcpy(backup_data, p_chip_data, m_data_size);
	int backup_data_size = m_data_size;

	// chg size
	m_data_size += get_width()*16*3;
	set_bfSize(m_data_size);
	set_height(get_height() + 16);

	// realloc
	p_chip_data = (UC*)realloc(p_chip_data, m_data_size);

	// copy
	memset(p_chip_data+RGB_OFFSET, 0x75, get_width()*16*3);
	memcpy(p_chip_data+RGB_OFFSET+get_width()*16*3,
		   backup_data+RGB_OFFSET, backup_data_size-RGB_OFFSET);

	free(backup_data);
}

UI MAPCHIP::get_bfSize(void)
{
	UI size = 0;
	size  = p_chip_data[2] << 0;
	size += p_chip_data[3] << 8;
	size += p_chip_data[4] << 16;
	size += p_chip_data[5] << 24;
	return size;
}

void MAPCHIP::set_bfSize(UI size)
{
	p_chip_data[2] = ((size >> 0)  & 0xff);
	p_chip_data[3] = ((size >> 8)  & 0xff);
	p_chip_data[4] = ((size >> 16) & 0xff);
	p_chip_data[5] = ((size >> 24) & 0xff);
}

void MAPCHIP::add_chip(UC* data)
{
	if((m_last_chip_idx%16) == 0)
	{
		add_line();
	}

	set_chip_data(m_last_chip_idx, data);

	m_last_chip_idx++;
}

void MAPCHIP::save(void)
{
	FILE* fp;

	char str[100];
	strcpy(str, m_chip_name);

	str[strlen(m_chip_name)-4] = '_';
	str[strlen(m_chip_name)-3] = 'o';
	str[strlen(m_chip_name)-2] = 'u';
	str[strlen(m_chip_name)-1] = 't';
	str[strlen(m_chip_name)-0] = '.';
	str[strlen(m_chip_name)+1] = 'b';
	str[strlen(m_chip_name)+2] = 'm';
	str[strlen(m_chip_name)+3] = 'p';
	str[strlen(m_chip_name)+4] = '\0';

	if((fp = fopen(str, "wb")) == NULL){
		printf("file open error (file_name:%s)\n", str);
		exit(0);
	}
	fwrite(p_chip_data, m_data_size, 1, fp);
	fclose(fp);
}

