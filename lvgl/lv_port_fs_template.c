/**
 * @file lv_port_fs_templ.c
 *
 */

 /*Copy this file as "lv_port_fs.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_fs_template.h"
#include "lvgl.h"
#include "ff.h"
#include <stdlib.h>
#include <stdio.h>
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/* Create a type to store the required data about your file.
 * If you are using a File System library
 * it already should have a File type.
 * For example FatFS has `FIL`. In this case use `typedef FIL file_t`*/
typedef FIL file_t;		// 把FIL类型定义成file_t
typedef DIR dir_t; 		// 把DIR类型定义成dir_t
/**********************
 *  STATIC PROTOTYPES
 **********************/
static void fs_init(void);

static lv_fs_res_t fs_open (lv_fs_drv_t * drv, void * file_p, const char * path, lv_fs_mode_t mode);
static lv_fs_res_t fs_close (lv_fs_drv_t * drv, void * file_p);
static lv_fs_res_t fs_read (lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br);
static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw);
static lv_fs_res_t fs_seek (lv_fs_drv_t * drv, void * file_p, uint32_t pos);
//static lv_fs_res_t fs_size (lv_fs_drv_t * drv, void * file_p, uint32_t * size_p);
//static lv_fs_res_t fs_tell (lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p);
//static lv_fs_res_t fs_remove (lv_fs_drv_t * drv, const char *path);
//static lv_fs_res_t fs_trunc (lv_fs_drv_t * drv, void * file_p);
//static lv_fs_res_t fs_rename (lv_fs_drv_t * drv, const char * oldname, const char * newname);
//static lv_fs_res_t fs_free (lv_fs_drv_t * drv, uint32_t * total_p, uint32_t * free_p);
//static lv_fs_res_t fs_dir_open (lv_fs_drv_t * drv, void * rddir_p, const char *path);
//static lv_fs_res_t fs_dir_read (lv_fs_drv_t * drv, void * rddir_p, char *fn);
//static lv_fs_res_t fs_dir_close (lv_fs_drv_t * drv, void * rddir_p);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_fs_init(void)
{
    /*----------------------------------------------------
     * Initialize your storage device and File System
     * -------------------------------------------------*/
    fs_init();

    /*---------------------------------------------------
     * Register the file system interface in LVGL
     *--------------------------------------------------*/

    /* Add a simple drive to open images */
    lv_fs_drv_t fs_drv;
    lv_fs_drv_init(&fs_drv);

    /*Set up fields...*/
    fs_drv.file_size = sizeof(file_t);
    fs_drv.letter = '1';
    fs_drv.open_cb = fs_open;
    fs_drv.close_cb = fs_close;
    fs_drv.read_cb = fs_read;
    fs_drv.write_cb = fs_write;
    fs_drv.seek_cb = fs_seek;
//    fs_drv.tell_cb = fs_tell;
//    fs_drv.free_space_cb = fs_free;
//    fs_drv.size_cb = fs_size;
//    fs_drv.remove_cb = fs_remove;
//    fs_drv.rename_cb = fs_rename;
//    fs_drv.trunc_cb = fs_trunc;

//    fs_drv.rddir_size = sizeof(dir_t);
//    fs_drv.dir_close_cb = fs_dir_close;
//    fs_drv.dir_open_cb = fs_dir_open;
//    fs_drv.dir_read_cb = fs_dir_read;

    lv_fs_drv_register(&fs_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
FATFS FS;
/* Initialize your Storage device and File system. */
static void fs_init(void)
{
    /*E.g. for FatFS initialize the SD card and FatFS itself*/
	f_mount(&FS, "1:", 1);
	
    /*You code here*/
}

/**
 * Open a file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param path path to the file beginning with the driver letter (e.g. S:/folder/file.txt)
 * @param mode read: FS_MODE_RD, write: FS_MODE_WR, both: FS_MODE_RD | FS_MODE_WR
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_open (lv_fs_drv_t * drv, void * file_p, const char * path, lv_fs_mode_t mode)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;
	char* f_path;
	uint8_t opt_mode;
	sprintf(f_path,"1:%s",path);
	/* 文件操作方法，将FatFS的转换成LVGL的操作方法 */
    if(mode == LV_FS_MODE_WR) {
    	opt_mode = FA_OPEN_ALWAYS | FA_WRITE;
    } else if(mode == LV_FS_MODE_RD) {
    	opt_mode = FA_OPEN_EXISTING | FA_READ;
    } else if(mode == (LV_FS_MODE_WR | LV_FS_MODE_RD)) {
		opt_mode = FA_WRITE | FA_READ;
    }
	FRESULT fres = f_open((FIL*)file_p, f_path, opt_mode);
	if (fres != FR_OK) {
    	printf("f_open error (%d)\n", fres);
    	res = LV_FS_RES_NOT_IMP;
  	} else
    	res = LV_FS_RES_OK;
	
    return res;
}

/**
 * Close an opened file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable. (opened with lv_ufs_open)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_close (lv_fs_drv_t * drv, void * file_p)
{
    if (f_close((FIL*)file_p) != FR_OK)
    	return LV_FS_RES_NOT_IMP;
  	else
    	return LV_FS_RES_OK;
}

/**
 * Read data from an opened file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable.
 * @param buf pointer to a memory block where to store the read data
 * @param btr number of Bytes To Read
 * @param br the real number of read bytes (Byte Read)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_read (lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br)
{
	FRESULT fres = f_read((FIL*)file_p, buf, btr, br);
  	if (fres != FR_OK) {
    	printf("f_read error (%d)\n", fres);
    	return LV_FS_RES_NOT_IMP;
  	} else 
    	return LV_FS_RES_OK;
}

/**
 * Write into a file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param buf pointer to a buffer with the bytes to write
 * @param btr Bytes To Write
 * @param br the number of real written bytes (Bytes Written). NULL if unused.
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw)
{
  	FRESULT fres = f_write((FIL*)file_p, buf, btw, bw);
  	if (fres != FR_OK) {
    	printf("f_read error (%d)\n", fres);
    	return LV_FS_RES_NOT_IMP;
  	} else
    	return LV_FS_RES_OK;
}

/**
 * Set the read write pointer. Also expand the file size if necessary.
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable. (opened with lv_ufs_open )
 * @param pos the new position of read write pointer
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_seek (lv_fs_drv_t * drv, void * file_p, uint32_t pos)
{
    /* Add your code here*/
  	FRESULT fres = f_lseek((FIL*)file_p, pos);
 	 if (fres != FR_OK) {
    	printf("f_lseek error (%d)\n", fres);
    	return LV_FS_RES_NOT_IMP;
  	} else
    	return LV_FS_RES_OK;
}

#endif
