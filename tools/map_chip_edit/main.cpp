#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "common.h"
#include "bmp24bit.h"
#include "mapdata.h"
#include "col_tbl.h"

#define MAX_MAP_NUM (1000)
#define CHIP_W (16)
#define CHIP_H (16)

#include <list>
#include <algorithm>

using namespace std;

typedef list<UI>::iterator ITR;

bool is_fc_col(UI col)
{
	UI fc_col;
	for(int i=0; i<COL_DATA_NUM; i++)
	{
		fc_col = (col_tbl[i].r<<16) | (col_tbl[i].g<<8) | (col_tbl[i].b);
		if(fc_col == col)
		{
			return true;
		}
	}
	return false;
}

int main(void)
{
	BMP24BIT bmp("map_chip.bmp");

	int map_chip_num = (bmp.get_h()/CHIP_H) * (bmp.get_w()/CHIP_W);
	UI* chip_cnt = (UI*)(calloc(1, map_chip_num*sizeof(UI)));

	// check chip cnt
	char map_file_name[100];
	for(int i=0; i<MAX_MAP_NUM; i++)
	{
		sprintf(map_file_name, "./mapdata/mapdata_%03d", i);

		MAPDATA mapdata(map_file_name);
		if(mapdata.get_w() == 0) { continue; }

		system("cls");
		printf("check chip cnt: %03d\n", i);

		for(int y=0; y<mapdata.get_h(); y++)
		{
			for(int x=0; x<mapdata.get_w(); x++)
			{
				chip_cnt[mapdata.get_chip_idx(x, y)]++;
			}
		}
	}

	// draw unused chip
	int unused_chip_num = 0;
	for(int i=0; i<map_chip_num; i++)
	{
		system("cls");
		printf("drawing unused chip: %04d / %04d\n", i, map_chip_num-1);
		printf("unused chip num: %d\n", unused_chip_num);

		if(chip_cnt[i] == 0)
		{
			unused_chip_num++;

			for(int y=0; y<CHIP_H; y++)
			{
				for(int x=0; x<CHIP_W; x++)
				{
					bmp.set_col((i%CHIP_W)*CHIP_W+x, (i/CHIP_H)*CHIP_H+y,
								((col_tbl[0].r<<16) + (col_tbl[0].g<<8) + col_tbl[0].b));
				}
			}
		}
	}

	// draw color pallet
	for(int z=0; z<COL_DATA_NUM; z++)
	{
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<4; j++)
			{
				bmp.set_col(256-64+i+(z/4)*4, j+(z%4)*4, ((col_tbl[z].r<<16) + (col_tbl[z].g<<8) + col_tbl[z].b));
			}
		}
	}
	//bmp.save_bmp("map_chip_draw_unused.bmp");

	list<UI> used_col_tbl;
	ITR itr;

	UI col;
	for(int y=0; y<bmp.get_h(); y++)
	{
		system("cls");
		printf("checking used color: %d / %d\n", y, bmp.get_h()-1);

		for(int x=0; x<bmp.get_w(); x++)
		{
			col = bmp.get_col(x, y);

			itr = find(used_col_tbl.begin(), used_col_tbl.end(), col);
			if(itr == used_col_tbl.end())
			{
				used_col_tbl.push_back(col);
			}
		}
	}

	printf("used_col_tbl_num: %d\n", used_col_tbl.size());

	int fc_col_num = 0;
	int not_fc_col_num = 0;

	int draw_pos_x = 256-4;
	int draw_pos_y = 16;

	for(itr=used_col_tbl.begin(); itr!=used_col_tbl.end(); ++itr)
	{
		if(is_fc_col(*itr))
		{
			fc_col_num++;
		}
		else
		{
			not_fc_col_num++;

			for(int y=0; y<4; y++)
			{
				for(int x=0; x<4; x++)
				{
					bmp.set_col(draw_pos_x + x, draw_pos_y + y, *itr);
				}
			}
			draw_pos_y += 4;

			if(draw_pos_y == 16+4*4)
			{
				draw_pos_y = 16;
				draw_pos_x -= 4;
			}
		}
	}

	printf("fc_col_num: %d\n", fc_col_num);
	printf("not_fc_col_num: %d\n", not_fc_col_num);


	bmp.save_bmp("map_chip_out.bmp");

	return 0;
}

