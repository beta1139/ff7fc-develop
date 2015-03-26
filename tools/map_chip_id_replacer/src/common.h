#ifndef __COMMON_H__
#define __COMMON_H__

typedef unsigned char  UC;
typedef unsigned short US;
typedef unsigned int   UI;

#define MAX_MAP_NUM (1000)

#define CHIP_W (16)
#define CHIP_H (16)

#define DBG_PRINT() {                                                            \
	FILE* fp;                                                                    \
	char dbg_str[100];                                                           \
	fp = fopen("dbg.txt", "a+");                                                 \
	sprintf(dbg_str, "[dbg] func_name:%s, line:%d\n", __FUNCTION__, __LINE__);   \
	fputs(dbg_str, fp);                                                          \
	fclose(fp);                                                                  \
}
#define DBG_PRINT_ARG(...) {                                                     \
	FILE* fp;                                                                    \
	fp = fopen("dbg.txt", "a+");                                                 \
	fprintf(fp, "[dbg] func_name:%s, line:%d >>> ", __FUNCTION__, __LINE__);     \
	fprintf(fp, __VA_ARGS__ );                                                   \
	fclose(fp);                                                                  \
}

#endif /* __COMMON_H__ */
