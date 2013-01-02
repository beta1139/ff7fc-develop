#include "map_image.h"

MAPIMAGE::MAPIMAGE(char* map_image_name)
{
	p_map_image = NULL;
	m_data_size = -1;

	FILE* fp;
	if((fp = fopen(map_image_name, "rb")) == NULL){
		printf("map image open error (%s)\n", map_image_name);
		return ;
	}
	else{
		int temp_c;
		m_data_size = 0;
		while((temp_c = fgetc(fp) != EOF)){
			m_data_size++;
		}

		fseek(fp, 0, SEEK_SET);
		p_map_image = new UC[m_data_size];
		for(int i=0; i<m_data_size; i++){
			p_map_image[i] = fgetc(fp);
		}
		fclose(fp);
	}
	set_width();
	set_height();

	printf("<map image data>\n");
	printf("width           : %d\n", m_width);
	printf("height          : %d\n", m_height);
	printf("info header size: %d\n", get_bi_size());
	printf("bit count       : %d\n", get_bit_count());
	printf("\n");

	if(get_bi_size() != 40){ printf("error invalid info header!!\n"); exit(0); }
	if(get_bit_count() != 24){ printf("error input 24bit format bitmap image!!\n"); exit(0); }
}

int MAPIMAGE::get_bi_size(void) { return p_map_image[14]; }
int MAPIMAGE::get_bit_count(void) { return p_map_image[28]; }

void MAPIMAGE::set_height(void)
{
	m_height  = p_map_image[22];
	m_height += p_map_image[23]*256;
	m_height += p_map_image[24]*256*256;
	m_height += p_map_image[25]*256*256*256;
}

void MAPIMAGE::set_width(void)
{
	m_width  = p_map_image[18];
	m_width += p_map_image[19]*256;
	m_width += p_map_image[20]*256*256;
	m_width += p_map_image[21]*256*256*256;
}

// 一行分のデータ量
// m_width*3

UC MAPIMAGE::get_chip_r(int idx, int x_offset, int y_offset)
{
	int chip_x = idx%(m_width/16);
	int chip_y = idx/(m_width/16);

	UC r = p_map_image[m_data_size         - // 画像の終端
					   m_width*3           + // yを1line分戻す
					   chip_x*16*3         - // xのオフセット調整
					   chip_y*16*m_width*3 + // yを指定チップ分戻す
					   x_offset*3          -
					   y_offset*m_width*3];
	return r;
}

UC MAPIMAGE::get_chip_g(int idx, int x_offset, int y_offset)
{
	int chip_x = idx%(m_width/16);
	int chip_y = idx/(m_width/16);

	UC g = p_map_image[m_data_size         - // 画像の終端
					   m_width*3           + // yを1line分戻す
					   chip_x*16*3         - // xのオフセット調整
					   chip_y*16*m_width*3 + // yを指定チップ分戻す
					   x_offset*3          -
					   y_offset*m_width*3 + 1];
	return g;
}

UC MAPIMAGE::get_chip_b(int idx, int x_offset, int y_offset)
{
	int chip_x = idx%(m_width/16);
	int chip_y = idx/(m_width/16);

	UC b = p_map_image[m_data_size         - // 画像の終端
					   m_width*3           + // yを1line分戻す
					   chip_x*16*3         - // xのオフセット調整
					   chip_y*16*m_width*3 + // yを指定チップ分戻す
					   x_offset*3          -
					   y_offset*m_width*3 + 2];
	return b;
}

void MAPIMAGE::get_chip_data(int chip_idx, UC* data)
{
	for(int j=0; j<16; j++){
		for(int i=0; i<16; i++){
			data[i*3+j*16*3+0] = get_chip_r(chip_idx, i, j);
			data[i*3+j*16*3+1] = get_chip_g(chip_idx, i, j);
			data[i*3+j*16*3+2] = get_chip_b(chip_idx, i, j);
		}
	}
}

int MAPIMAGE::get_chip_num(void) { return ((m_width/16) * (m_height/16)); }

