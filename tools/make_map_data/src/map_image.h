#ifndef __MAP_IMAGE__
#define __MAP_IMAGE__

#include "main.h"

class MAPIMAGE
{
	private:
		UC* p_map_image;
		int m_data_size;
		int m_width;
		int m_height;
	public:
		MAPIMAGE(char*);

		int get_data_size(void){ return m_data_size; }
		int get_width(void){ return m_width; }
		int get_height(void){ return m_height; }
		int get_bit_count(void);
		int get_bi_size(void);

		UC get_chip_r(int, int, int);
		UC get_chip_g(int, int, int);
		UC get_chip_b(int, int, int);

		void set_width(void);
		void set_height(void);

		void get_chip_data(int, UC*);
		int get_chip_num(void);
};

#endif /* __MAP_IMAGE__ */
