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

#define RGB 0
#define YUV 1

#define ALGORITHM YUV

double rgb_to_y(UI r, UI g, UI b)  { return (0.299*r + 0.587*g + 0.114*b); }
double rgb_to_u(UI r, UI g, UI b) { return ( -0.14713*r - 0.28886*g + 0.436*b); }
double rgb_to_v(UI r, UI g, UI b) { return (0.615*r - 0.51499*g - 0.10001*b); }

UI search_nearest_fc_col(UI col)
{
#if (ALGORITHM==RGB)
	UI org_r,org_g,org_b;
	org_r = (col >> 16) & 0x000000ff;
	org_g = (col >>  8) & 0x000000ff;
	org_b = (col >>  0) & 0x000000ff;

	double nearest_distance = 0x8FFFFFFF;
	UI nearest_col_idx = 0;

	UI r_dis, g_dis, b_dis;
	UI fc_r,fc_g,fc_b;

	for(int i=0; i<COL_DATA_NUM; i++){
		fc_r = col_tbl[i].r;
		fc_g = col_tbl[i].g;
		fc_b = col_tbl[i].b;

		r_dis = (fc_r-org_r)*(fc_r-org_r);
		g_dis = (fc_g-org_g)*(fc_g-org_g);
		b_dis = (fc_b-org_b)*(fc_b-org_b);

		double col_distance = r_dis + g_dis + b_dis;
		if(col_distance < nearest_distance){
			nearest_col_idx = i;
			nearest_distance = col_distance;
		}

		/*
		printf("org(%02x %02x %02x) col_tbl[%02d](%02x %02x %02x) distance: %u\n",
				org_r, org_g, org_b, i, fc_r, fc_g, fc_b, col_distance);
				*/
	}
#elif (ALGORITHM==YUV)
	UI org_r,org_g,org_b;
	org_r = (col >> 16) & 0x000000ff;
	org_g = (col >>  8) & 0x000000ff;
	org_b = (col >>  0) & 0x000000ff;
	double org_y  = rgb_to_y(org_r, org_g, org_b);
	double org_cb = rgb_to_u(org_r, org_g, org_b);
	double org_cr = rgb_to_v(org_r, org_g, org_b);

	double nearest_distance = 0x8FFFFFFF;
	UI nearest_col_idx = 0;

	double y_dis, cb_dis, cr_dis;

	UI fc_r,fc_g,fc_b;
	double fc_y, fc_cb, fc_cr;

	for(int i=0; i<COL_DATA_NUM; i++){
		fc_r = col_tbl[i].r;
		fc_g = col_tbl[i].g;
		fc_b = col_tbl[i].b;

		fc_y  = rgb_to_y(fc_r,  fc_g, fc_b);
		fc_cb = rgb_to_u(fc_r, fc_g, fc_b);
		fc_cr = rgb_to_v(fc_r, fc_g, fc_b);

		y_dis  = sqrt((fc_y - org_y)*(fc_y - org_y));
		cb_dis = sqrt((fc_cb-org_cb)*(fc_cb-org_cb));
		cr_dis = sqrt((fc_cr-org_cr)*(fc_cr-org_cr));

		double col_distance = y_dis + cb_dis + cr_dis;
		if(col_distance < nearest_distance){
			nearest_col_idx = i;
			nearest_distance = col_distance;
		}
	}
#endif /* ALGORITHM */
	/*
	printf("nearest_col_idx: %u\n", nearest_col_idx);
	exit(1);
	*/
	return ((col_tbl[nearest_col_idx].r<<16) + (col_tbl[nearest_col_idx].g<<8) + col_tbl[nearest_col_idx].b);
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
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<2; j++)
			{
				bmp.set_col(256-32+i+(z/4)*2, j+(z%4)*2, ((col_tbl[z].r<<16) + (col_tbl[z].g<<8) + col_tbl[z].b));
			}
		}
	}
	bmp.save_bmp("map_chip_draw_unused.bmp");

	// chg to fc color
	UI col;
	UI nearest_col;
	for(int y=0; y<bmp.get_h(); y++)
	{
		system("cls");
		printf("redraw with fc col: %d / %d\n", y, bmp.get_h()-1);

		for(int x=0; x<bmp.get_w(); x++)
		{
			col = bmp.get_col(x, y);
			nearest_col = search_nearest_fc_col(col);
			bmp.set_col(x, y, nearest_col);
		}
		//if(y==15) { break; }
	}
	bmp.save_bmp("map_chip_fc_col.bmp");

	return 0;
}

