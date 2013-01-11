#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "mapdata.h"
#include "mapchip.h"

int main(void)
{
	MAPCHIP old_mapchip("map_chip.bmp");
	MAPCHIP new_mapchip("map_chip_new.bmp");

	int old_chip_num = old_mapchip.get_map_chip_num();
	int new_chip_num = new_mapchip.get_map_chip_num();

	UI* chip_id_replace_tbl = (UI*)(calloc(1, old_chip_num*sizeof(UI)));

	int old_chip_data[16][16] = {0};
	int new_chip_data[16][16] = {0};

	// chk chip idx
	for(int old_chip_idx=0; old_chip_idx<old_chip_num; old_chip_idx++)
	{
		system("cls");
		printf("checking chip idx: (%4d / %4d)\n", old_chip_idx, old_chip_num-1);

		old_mapchip.get_chip_data(old_chip_idx, &old_chip_data[0][0]);

		for(int new_chip_idx=0; new_chip_idx<new_chip_num; new_chip_idx++)
		{
			new_mapchip.get_chip_data(new_chip_idx, &new_chip_data[0][0]);
			if(memcmp(old_chip_data, new_chip_data, 16*16*sizeof(int)) == 0)
			{
				chip_id_replace_tbl[old_chip_idx] = new_chip_idx;
				break;
			}

			if(new_chip_idx == new_chip_num-1)
			{
				printf("map chip not found!! map_chip.bmp (x: %d, y: %d)\n", (old_chip_idx%16)*16, (old_chip_idx/16)*16);
				getchar();
				exit(1);
			}
		}
	}
	//for(int i=0; i<old_chip_num; i++) { DBG_PRINT_ARG("tbl[%4d]: %d\n", i, chip_id_replace_tbl[i]); }


	char map_file_name[100];
	UI new_chip_idx = 0;
	for(int i=0; i<MAX_MAP_NUM; i++)
	{
		sprintf(map_file_name, "./mapdata/mapdata_%03d", i);

		MAPDATA mapdata(map_file_name);
		if(mapdata.get_w() == 0) { continue; }

		system("cls");
		printf("replacing chip idx (mapdata_%03d)\n", i);

		for(int y=0; y<mapdata.get_h(); y++)
		{
			for(int x=0; x<mapdata.get_w(); x++)
			{
				new_chip_idx = chip_id_replace_tbl[mapdata.get_chip_idx(x, y)];
				mapdata.set_chip_idx(x, y, new_chip_idx);
			}
		}
		mapdata.save(map_file_name);
	}
	system("cls");
	printf("DONE!!\n");
	getchar();

	return 0;
}

