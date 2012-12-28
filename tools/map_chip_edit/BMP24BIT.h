#ifndef __BMP24BIT_H__
#define __BMP24BIT_H__

#include "common.h"

class BMP24BIT
{
	private:
		typedef struct
		{
			UC bfType[2];
			UC bfSize[4];
			UC rsv1[2];
			UC rsv2[2];
			UC bfOffBits[4];
		}FileHeader;
		
		typedef struct
		{
			UC bcSize[4];
			UC bcWidth[4];
			UC bcHeight[4];
			UC bcPlanes[2];
			UC bcBitCount[2];
		
			UC bcCompression[4];
			UC bcSizeImage[4];
			UC bcXPixPerMeter[4];
			UC bcYPixPerMeter[4];
			UC bcClrUsed[4];
			UC bcCirImportant[4];
		}InfoHeader;
		
		typedef struct
		{
			FileHeader file_header;
			InfoHeader info_header;
			UC*        data;
		}BITMAP_FILE;

	private:
		BITMAP_FILE m_bmp;
		int m_w;
		int m_h;
	public:
		BMP24BIT(char*);
		~BMP24BIT(void);
		int get_w(void);
		int get_h(void);
		UI get_col(int, int);
		void set_col(int, int, UI);
		void save_bmp(char*);
};

#endif /* __BMP24BIT_H__ */
