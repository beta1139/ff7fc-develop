#ifndef __MAP_DATA__
#define __MAP_DATA__

#include "common.h"

class MAPDATA
{
	private:
		UC* p_map_data;
		int m_map_id;
		int m_map_w;
		int m_map_h;
		int m_data_size;
	public:
		MAPDATA(int, int, int);
		int get_map_id(void){ return m_map_id; }
		int get_map_w(void){ return m_map_w; }
		int get_map_h(void){ return m_map_h; }

		void set_header(void);
		void set_chip_data(int, int, int);
		void save(void);
};

#endif /* __MAP_DATA__ */
