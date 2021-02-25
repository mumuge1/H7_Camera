#ifndef FATFSDEMO
#define FATFSDEMO

#include "ff.h"
#include <stdio.h>

#define BITMAP_SIZE 1385850
#define UNICODE_SIZE 42000
#define DSC_SIZE 167992
#define SD_PATH     "0:"
#define FALSH_PATH  "1:"
#define FONT_16_DIR "my_Font_16"
void test1(void);
void fatfs_test(void);
FRESULT miscellaneous(void);
UINT scan_files(char* path);
FRESULT file_check(void);
void SD_fatfs(void);
void Font2SD(void);
void image2flash(uint8_t ofs, void* image,char* image_name);
void Read_Font_From_SD(void);
void test(void);
void Font2FALSH(void);
void test_sd(void);
void Read_Font_From_FLASH(void);
#endif  /*FATFSDEMO*/
