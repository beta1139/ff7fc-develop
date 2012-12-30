#ifndef __MAPDATA_H__
#define __MAPDATA_H__

#include "common.h"

class MAPDATA
{
	private:
		UC* m_data;
		int m_w;
		int m_h;
		int m_size;
	public:
		MAPDATA(char*);
		~MAPDATA(void);
		int get_w(void);
		int get_h(void);
		UI get_chip_idx(int, int);
		void set_chip_idx(int, int, UI);
		void save(char*);
};

#endif /* __MAPDATA_H__ */
