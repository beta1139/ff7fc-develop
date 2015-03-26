#include "mapchip.h"

MAPCHIP::MAPCHIP(char* file_name) : BMP24BIT(file_name)
{
}

MAPCHIP::~MAPCHIP(void)
{
}

int MAPCHIP::get_map_chip_num(void)
{
	return (get_h()/CHIP_H) * (get_w()/CHIP_W);
}

void MAPCHIP::get_chip_data(int chip_idx, int* data)
{
	for(int y=0; y<CHIP_H; y++)
	{
		for(int x=0; x<CHIP_W; x++)
		{
			*(data+x+y*CHIP_W) = get_col((chip_idx%CHIP_W)*CHIP_W+x,
										 (chip_idx/CHIP_H)*CHIP_H+y);
		}
	}
}

