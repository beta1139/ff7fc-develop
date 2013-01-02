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
		int temp_c;
		m_data_size = 0;
		while((temp_c = fgetc(fp) != EOF)){
			m_data_size++;
		}

		fseek(fp, 0, SEEK_SET);
		p_chip_data = new UC[m_data_size];
		for(int i=0; i<m_data_size; i++){
			p_chip_data[i] = fgetc(fp);
		}
		fclose(fp);
	}
	set_width();
	set_height();

	printf("<map chip data>\n");
	printf("width           : %d\n", m_width);
	printf("height          : %d\n", m_height);
	printf("info header size: %d\n", get_bi_size());
	printf("bit count       : %d\n", get_bit_count());
	printf("chip num        : %d\n", get_chip_num());
	printf("\n");

	if(get_bi_size() != 40){ printf("error invalid info header!!\n"); exit(0); }
	if(get_bit_count() != 24){ printf("error input 24bit format bitmap image!!\n"); exit(0); }
}

int MAPCHIP::get_bi_size(void) { return p_chip_data[14]; }
int MAPCHIP::get_bit_count(void) { return p_chip_data[28]; }

void MAPCHIP::set_height(void)
{
	m_height  = p_chip_data[22];
	m_height += p_chip_data[23]*256;
	m_height += p_chip_data[24]*256*256;
	m_height += p_chip_data[25]*256*256*256;
}

void MAPCHIP::set_width(void)
{
	m_width  = p_chip_data[18];
	m_width += p_chip_data[19]*256;
	m_width += p_chip_data[20]*256*256;
	m_width += p_chip_data[21]*256*256*256;
}


// 一行分のデータ量
// 16*16*3

UC MAPCHIP::get_chip_r(int idx, int x_offset, int y_offset)
{
	int chip_x = idx%16;
	int chip_y = idx/16;

	UC r = p_chip_data[m_data_size       - // 画像の終端
					   16*3*16           + // yを1line分戻す
					   chip_x*16*3       - // xのオフセット調整
					   chip_y*16*16*16*3 +// yを指定チップ分戻す
					   x_offset*3        -
					   y_offset*16*16*3];
	return r;
}

UC MAPCHIP::get_chip_g(int idx, int x_offset, int y_offset)
{
	int chip_x = idx%16;
	int chip_y = idx/16;

	UC g = p_chip_data[m_data_size       - // 画像の終端
					   16*3*16           + // yを1line分戻す
					   chip_x*16*3       - // xのオフセット調整
					   chip_y*16*16*16*3 + // yを指定チップ分戻す
					   x_offset*3        -
					   y_offset*16*16*3 + 1];
	return g;
}

UC MAPCHIP::get_chip_b(int idx, int x_offset, int y_offset)
{
	int chip_x = idx%16;
	int chip_y = idx/16;

	UC b = p_chip_data[m_data_size       - // 画像の終端
					   16*3*16           + // yを1line分戻す
					   chip_x*16*3       - // xのオフセット調整
					   chip_y*16*16*16*3 + // yを指定チップ分戻す
					   x_offset*3        -
					   y_offset*16*16*3 + 2];
	return b;
}

void MAPCHIP::get_chip_data(int chip_idx, UC* data)
{
	for(int j=0; j<16; j++){
		for(int i=0; i<16; i++){
			data[i*3+j*16*3+0] = get_chip_r(chip_idx, i, j);
			data[i*3+j*16*3+1] = get_chip_g(chip_idx, i, j);
			data[i*3+j*16*3+2] = get_chip_b(chip_idx, i, j);
		}
	}
}

int MAPCHIP::get_chip_num(void) { return ((m_width/16) * (m_height/16)); }

