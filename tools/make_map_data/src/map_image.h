#ifndef __MAP_IMAGE__
#define __MAP_IMAGE__

#include "common.h"

class MAPIMAGE
{
	private:
		UC* p_map_image;
		int m_data_size;
	public:
		MAPIMAGE(char*);

		int get_data_size(void){ return m_data_size; }
		UI get_width(void);
		UI get_height(void);
		int get_bit_count(void);
		int get_bi_size(void);

		UC get_chip_col(int, int, int, int);

		void get_chip_data(int, UC*);
		int get_chip_num(void);
};

#endif /* __MAP_IMAGE__ */
