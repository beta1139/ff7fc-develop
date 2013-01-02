#include "main.h"
#include "map_chip.h"
#include "map_image.h"
#include "map_data.h"

void print_error_msg(void)
{
	printf("error!!\n");
	printf("make_map_image.exe -m <map image name> -c <mapchip image name> -o <id>\n");
}

int a_to_i(int c)
{
	switch(c)
	{
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'a': return 10;
		case 'b': return 11;
		case 'c': return 12;
		case 'd': return 13;
		case 'e': return 14;
		case 'f': return 15;
		default: printf("invalid map_id!! use 0-F c:%c\n", c); exit(0); }
}

bool is_same_chip(UC* chip_map, UC* chip_chip)
{
	for(int i=0; i<16*16*3; i++){
		if(((double)chip_chip[i] < chip_map[i]*0.9) || (((double)chip_chip[i] > chip_map[i]*1.1))){
			return false;
		}
	}
	return true;
}

int get_map_id(char* str)
{
	int map_id = 0;
	for(int i=0; i<strlen(str); i++){
		int c = str[strlen(str)-1-i];
		c = a_to_i(c);
		for(int j=0; j<i; j++){ c *= 10; }
		map_id += c;
	}
	if(map_id < 0 || 1000 <= map_id){
		printf("invalid map_id:%d\n", map_id);
		exit(0);
	}
	return map_id;
}


int main(int argc, char *argv[])
{
	if(argc < 7){ print_error_msg(); exit(0); }

	// search map_image_name
	char* p_map_image_name = NULL;
	for(int i=0; i<argc; i++){
		if(strcmp(argv[i], "-m") == 0){
			if(i+1 < argc){
				p_map_image_name = argv[i+1];
				break;
			}
			else{ print_error_msg(); exit(0); }
		}
	}
	if(p_map_image_name == NULL){ printf("error input mapdata\n"); exit(0); }
	MAPIMAGE mapimage(p_map_image_name);

	// search chip_name
	char* p_map_chip_name = NULL;
	for(int i=0; i<argc; i++){
		if(strcmp(argv[i], "-c") == 0){
			if(i+1 < argc){
				p_map_chip_name = argv[i+1];
				break;
			}
			else{ print_error_msg(); exit(0); }
		}
	}
	if(p_map_chip_name == NULL){ printf("error input mapchip\n"); exit(0); }
	MAPCHIP mapchip(p_map_chip_name);

	// search output data id
	int map_id;
	for(int i=0; i<argc; i++){
		if(strcmp(argv[i], "-o") == 0){
			if(i+1 < argc){
				map_id = get_map_id(argv[i+1]);
				break;
			}
			else{ print_error_msg(); exit(0); }
		}
	}
	MAPDATA mapdata(map_id, mapimage.get_width(), mapimage.get_height());

	//////////////////////////////////////////////////////////////////////////////////

	int chip_num_chip = mapchip.get_chip_num();
	int chip_num_map = mapimage.get_chip_num();

	UC chip_data_map[16*16*3];
	UC chip_data_chip[16*16*3];

	int found_num = 0;
	int not_found_num = 0;

	printf("=============== search chip idx ===============\n");

	for(int j=0; j<chip_num_map; j++){
		mapimage.get_chip_data(j, chip_data_map);

		int chip_idx = -1;
		for(int i=0; i<chip_num_chip; i++){
			mapchip.get_chip_data(i, chip_data_chip);

			if(is_same_chip(chip_data_map, chip_data_chip)){
			//if(memcmp(chip_data_map, chip_data_chip, sizeof(chip_data_map)) == 0){
				chip_idx = (i/16)*16 + i%16;
				found_num++;
				printf("chip found!!     (x:%4d, y:%4d, chip_id:%d)\n",
					   (j*16)%(mapimage.get_width()),
					   ((j*16)/(mapimage.get_width()))*16,
					   chip_idx);
				mapdata.set_chip_data(((j*16)%(mapimage.get_width()))/16,
									  ((j*16)/(mapimage.get_width())), i);
				break;
			}
		}
		if(chip_idx == -1){
			not_found_num++;
			printf("chip not found!! (x:%4d, y:%4d)\n",
				   (j*16)%(mapimage.get_width()),
				   ((j*16)/(mapimage.get_width()))*16);
			mapdata.set_chip_data(((j*16)%(mapimage.get_width()))/16,
								  ((j*16)/(mapimage.get_width())), 16);
		}
	}

	mapdata.save_map_data();

	printf("\n");
	printf("=============== result ===============\n");
	printf("found chip_num    : %d\n", found_num);
	printf("not found chip_num: %d\n", not_found_num);

	return 0;
} 

