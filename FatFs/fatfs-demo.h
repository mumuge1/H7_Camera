#ifndef FATFSDEMO
#define FATFSDEMO

#include "ff.h"
#include <stdio.h>
void fatfs_test(void);
FRESULT miscellaneous(void);
FRESULT scan_files (char* path);
FRESULT file_check(void);
void SD_fatfs(void);
void image2flash(uint8_t ofs, void* image,char* image_name);
#endif  /*FATFSDEMO*/
