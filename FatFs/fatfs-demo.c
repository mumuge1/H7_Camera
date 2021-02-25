#include "fatfs-demo.h"
#include "ff.h"
#include <stdlib.h>
#include "lvgl.h"
FRESULT res_flash;
FIL fnew,file;
FATFS fs,fs0;
UINT fnum;
#define TEST_SIZE 512
uint8_t readbuffer[1024]= {0};
uint8_t WriteBuffer[] = "this a fatfs!111111111111";
uint8_t WriteBuffer1[] = "this another fatfs!";
uint8_t WriteBuffer2[TEST_SIZE];
uint8_t readbuffer2[TEST_SIZE];
uint8_t workspace[4096],workspace0[4096];

void test_sd(void)
{
	res_flash = f_open(&file,"1:myFont.bin",FA_READ);printf("RES:%d",res_flash);
	printf("SIZE:%d",f_size(&file));
	f_close(&file);
}

uint8_t image1[7339];
#include "decode_dma.h"
#include "lcd.h"
extern UART_HandleTypeDef huart1;
void test1(void)
{
	res_flash = f_mount(&fs,FALSH_PATH,1);
	res_flash = f_open(&file,"1:th.jpg",FA_READ);
	res_flash = f_read(&file,image1,7339,&fnum);
	Jpeg_HWDecodingEnd = 0;
	JPEG_Decode_DMA((uint32_t)image1, sizeof(image1) , (uint32_t)image1_decode);
	while(Jpeg_HWDecodingEnd==0);
//	printf("image:%d,%d,%d,%d,%d\n",1,160*80,160,80,7);
//	HAL_UART_Transmit(&huart1,image1_decode,160*80,0xffff);	
//	HAL_Delay(1000);
//	HAL_UART_Transmit(&huart1,image1_decode,160*80,0xffff);
	ST7735_FillRGBRect(&st7735_pObj,0,0,image1_decode,160,80);
	f_close(&file);
	f_unmount(FALSH_PATH);
}


void fatfs_test(void)
{
	memset(WriteBuffer2,0x21,TEST_SIZE);
    res_flash = f_mount(&fs,"1:",1);
    if (res_flash == FR_NO_FILESYSTEM) {
        printf("》FLASH还没有文件系统，即将进行格式化...\r\n");
        res_flash=f_mkfs("1:",0,workspace,4096);
        if (res_flash == FR_OK) {
            printf("》FLASH已成功格式化文件系统。\r\n");
            res_flash = f_mount(NULL,"1:",1);
            res_flash = f_mount(&fs,"1:",1);
        }else{
            printf("格式化失败%d\r\n",res_flash);
        }
    }else if(res_flash==FR_OK){
        printf("外部Flash挂载文件系统成功。(%d)\r\n",res_flash);
    }
    printf("\r\n****** 即将进行文件写入测试... ******\r\n");

    res_flash = f_open(&fnew, "1:FatFs读写测试文件.txt",

                       FA_CREATE_ALWAYS | FA_WRITE );

    if ( res_flash == FR_OK ) {
        printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
        res_flash=f_write(&fnew,WriteBuffer2,sizeof(WriteBuffer2),&fnum);
        if (res_flash==FR_OK) {
            printf("》文件写入成功，写入字节数据：%d\n",fnum);
            printf("》向文件写的数据为:\r\n%s\r\n",WriteBuffer2);
        } else {
            printf("！！文件写入失败：(%d)\n",res_flash);
        }
        f_close(&fnew);
    } else {
        printf("！！打开/创建文件失败%d\r\n",res_flash);
    }
}
FRESULT miscellaneous(void)
{
    DIR dir;
    FATFS *pfs;
    DWORD fre_clust, fre_sect, tot_sect;

    printf("\n***************Get flash info***************\r\n");
    res_flash = f_getfree("1:", &fre_clust, &pfs);

    /* 计算得到总的扇区个数和空扇区个数 */
    tot_sect = (pfs->n_fatent - 2) * pfs->csize;
    fre_sect = fre_clust * pfs->csize;
    /* 打印信息(4096 字节/扇区) */
    printf("》设备总空间：%10lu KB。\n》可用空间： %10lu KB。\n",
            tot_sect *4, fre_sect *4);
    printf("\n****************\r\n");
    res_flash = f_open(&fnew, "1:FatFs读写测试文件.txt",
                      FA_OPEN_EXISTING|FA_WRITE|FA_READ );
    if ( res_flash == FR_OK ) {

    /* 文件定位 */

    res_flash = f_lseek(&fnew,f_size(&fnew)-1);

    if (res_flash == FR_OK) {

    /* 格式化写入，参数格式类似printf函数 */
        f_write(&fnew,WriteBuffer1,sizeof(WriteBuffer1),&fnum);
        /* f_printf(&fnew,"\n在原来文件新添加一行内容\n");
         f_printf(&fnew,"》设备总空间：%10lu KB。\n》可用空间： %10lu KB。\n",tot_sect *4, fre_sect *4);*/
    /* 文件定位到文件起始位置 */
    res_flash = f_lseek(&fnew,0);
    /* 读取文件所有内容到缓存区 */
    res_flash = f_read(&fnew,readbuffer,f_size(&fnew),&fnum);
    if (res_flash == FR_OK) {
        printf("》文件内容：\n%s\n",readbuffer);
    }}
    f_close(&fnew);
    printf("\n********** 目录创建和重命名功能测试 **********\r\n");
    /* 尝试打开目录 */
    res_flash=f_opendir(&dir,"1:TestDir");
    if (res_flash!=FR_OK) {/* 打开目录失败，就创建目录 */
    res_flash=f_mkdir("1:TestDir");
    if(res_flash == FR_OK)  printf("创建目录TestDir成功!\n");
    } else {
        /* 如果目录已经存在，关闭它 */
        res_flash=f_closedir(&dir);
        printf("已存在目录TestDir!\n");
        /* 删除文件 */
        f_unlink("1:TestDir/testdir.txt");
    }
    if (res_flash==FR_OK) {
        /* 重命名并移动文件 */
        res_flash=f_rename("1:FatFs读写测试文件.txt","1:TestDir/testdir.txt");
    }
    } else {
        printf("!! 打开文件失败：%d\n",res_flash);
        printf("!! 或许需要再次运行'FatFs移植与读写测试'工程\n");
    }
    return res_flash;
}


FRESULT file_check(void)
{
    FILINFO fno;
    /* 获取文件信息 */
    res_flash=f_stat("1:TestDir/testdir.txt",&fno);
    if (res_flash==FR_OK) {
        printf("'testdir.txt'  file information:\n");
        printf(">>>file size: %d(字节)\n", fno.fsize);
        printf(">>>time: %u/%02u/%02u, %02u:%02u\n",
                (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31,
                fno.ftime >> 11, fno.ftime >> 5 & 63);
        printf(">>>attribute: %c%c%c%c%c\n\n",
                (fno.fattrib & AM_DIR) ? 'D' : '-', // 是一个目录
                (fno.fattrib & AM_RDO) ? 'R' : '-', // 只读文件
                (fno.fattrib & AM_HID) ? 'H' : '-', // 隐藏文件
                (fno.fattrib & AM_SYS) ? 'S' : '-', // 系统文件
                (fno.fattrib & AM_ARC) ? 'A' : '-'); // 档案文件
    }
    return res_flash;
}

UINT scan_files (char* path)
{
    FRESULT res; 
    FILINFO fno;//文件信息结构体
    DIR dir;
	UINT fnum = 0;
    res = f_opendir(&dir, path);
    if(res == FR_OK)
	{
        while(1)
		{
            res = f_readdir(&dir, &fno);
            if(res != FR_OK || fno.fname[0] == 0) break;
			fnum++;
        } 
    }
	return fnum;
}

