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
		fseek(fp, 0, SEEK_END);
		m_data_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		p_map_image = (UC*)malloc(m_data_size);
		fread(p_map_image, m_data_size, 1, fp);

		fclose(fp);
	}

	printf("<map image data>\n");
	printf("width           : %d\n", get_width());
	printf("height          : %d\n", get_height());
	printf("info header size: %d\n", get_bi_size());
	printf("bit count       : %d\n", get_bit_count());
	printf("\n");

	if(get_bi_size() != 40){ printf("error invalid info header!!\n"); exit(0); }
	if(get_bit_count() != 24){ printf("error input 24bit format bitmap image!!\n"); exit(0); }
}

int MAPIMAGE::get_bi_size(void) { return p_map_image[14]; }
int MAPIMAGE::get_bit_count(void) { return p_map_image[28]; }

UI MAPIMAGE::get_height(void)
{
	UI height = 0;
	height  = (p_map_image[22] << 0);
	height += (p_map_image[23] << 8);
	height += (p_map_image[24] << 16);
	height += (p_map_image[25] << 24);
	return height;
}

UI MAPIMAGE::get_width(void)
{
	UI width = 0;
	width  = (p_map_image[18] << 0);
	width += (p_map_image[19] << 8);
	width += (p_map_image[20] << 16);
	width += (p_map_image[21] << 24);
	return width;
}

UC MAPIMAGE::get_chip_col(int idx, int x_offset, int y_offset, int col_offset)
{
	int chip_x = idx%(get_width()/16);
	int chip_y = idx/(get_width()/16);

	UC col = p_map_image[m_data_size         - // 画像の終端
					   get_width()*3           + // yを1line分戻す
					   chip_x*16*3         - // xのオフセット調整
					   chip_y*16*get_width()*3 + // yを指定チップ分戻す
					   x_offset*3          -
					   y_offset*get_width()*3 + col_offset];
	return col;
}

void MAPIMAGE::get_chip_data(int chip_idx, UC* data)
{
	for(int j=0; j<16; j++){
		for(int i=0; i<16; i++){
			data[i*3+j*16*3+0] = get_chip_col(chip_idx, i, j, 0);
			data[i*3+j*16*3+1] = get_chip_col(chip_idx, i, j, 1);
			data[i*3+j*16*3+2] = get_chip_col(chip_idx, i, j, 2);
		}
	}
}

int MAPIMAGE::get_chip_num(void)
{
	return ((get_width()/16) * (get_height()/16));
}

