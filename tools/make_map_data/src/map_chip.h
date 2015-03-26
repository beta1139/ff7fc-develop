#ifndef __MAP_CHIP__
#define __MAP_CHIP__

#include "common.h"

class MAPCHIP
{
	private:
		UC* p_chip_data;
		int m_data_size;
		int m_last_chip_idx;
		char* m_chip_name;
	public:
		MAPCHIP(char*);

		int get_bit_count(void);
		int get_bi_size(void);

		UC get_chip_col(int, int, int, int);

		UI get_width(void);
		UI get_height(void);

		void get_chip_data(int, UC*);
		int get_chip_num(void);

		int get_last_chip_idx(void);
		void add_chip(UC*);
		void save(void);
		void add_line(void);
	private:
		void set_height(UI);
		void set_chip_data(int, UC*);
		void set_bfSize(UI);
		UI   get_bfSize(void);
};

#endif /* __MAP_CHIP__ */

