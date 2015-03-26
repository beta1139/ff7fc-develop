#ifndef __MAPCHIP_H__
#define __MAPCHIP_H__

#include "common.h"
#include "bmp24bit.h"

class MAPCHIP : public BMP24BIT
{
	private:
	public:
		MAPCHIP(char*);
		~MAPCHIP(void);
		int get_map_chip_num(void);
		void get_chip_data(int, int*);
};

#endif /* __BMP24BIT_H__ */

