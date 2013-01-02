#include "map_data.h"


MAPDATA::MAPDATA(int map_id, int width, int height)
{
	m_map_id = map_id;

	if((width % 256) != 0){ printf("error map_width: %d, input the multiple of 256\n", width); exit(0); }
	m_map_w = (width/16)/16;
	if((height % 240) != 0){ printf("error map_height: %d, input the multiple of 240\n", height); exit(0); }
	m_map_h = (height/16)/15;

	m_data_size = 2 + (m_map_w*16)*(m_map_h*15)*12;
	p_map_data = (UC*)(calloc(1, m_data_size));

	set_header();

	printf("<map data>\n");
	printf("map_width       : %02x\n", p_map_data[0]);
	printf("map_height      : %02x\n", p_map_data[1]);
	printf("\n");
};

void MAPDATA::set_header(void)
{
	p_map_data[0] = m_map_w;
	p_map_data[1] = m_map_h;
}

void MAPDATA::set_chip_data(int x, int y, int map_idx)
{
	//printf("!!!!!! (x:%d, y:%d, map_idx:%d)\n", x, y, map_idx);

	int offset = 2+x*2+y*m_map_w*16*2;
	if(m_data_size <= offset){
		printf("invalid map_size (line:%d x:%d, y:%d, offset:%d)\n",
				__LINE__, x, y, offset);
		exit(0);
	}
	p_map_data[offset + 0] = map_idx / 256;
	p_map_data[offset + 1] = map_idx % 256;
}

void MAPDATA::save_map_data(void)
{
	FILE* fp_map_data;
	char map_data_name[30];
	sprintf(map_data_name, "mapdata_%03d", m_map_id);
	if((fp_map_data = fopen(map_data_name, "wb")) == NULL){
		printf("file open error (file_name:%s)\n", map_data_name);
		exit(0);
	}
	for(int i=0; i<m_data_size; i++){ fputc(p_map_data[i], fp_map_data); }
	fclose(fp_map_data);
}

