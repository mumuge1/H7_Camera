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

extern const uint8_t glyph_bitmap[];
extern const uint16_t unicode_list_1[];
extern const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[];
extern uint32_t FONT_SIZE,glyph_dsc_SIZE,unicode_SIZE;

void test1(void)
{
	res_flash = f_mount(&fs,FALSH_PATH,1);printf("RES:%d",res_flash);
	res_flash = f_open(&file,"1:test.txt",FA_READ);printf("RES:%d",res_flash);
	res_flash = f_read(&file,readbuffer2,TEST_SIZE,&fnum);printf("RES:%d",res_flash);
	//for(int i=0;i<TEST_SIZE;i++)
		printf("%s",readbuffer2);
	f_close(&file);
	f_unmount(FALSH_PATH);
}
void test(void)
{
	memset(WriteBuffer2,0x21,TEST_SIZE);
	res_flash = f_mount(&fs,FALSH_PATH,1);printf("RES:%d",res_flash);
	res_flash = f_unlink("1:test.txt");printf("RES:%d",res_flash);
	res_flash = f_open(&file,"1:test.txt",FA_READ|FA_WRITE|FA_CREATE_ALWAYS);printf("RES:%d",res_flash);
	res_flash = f_write(&file,WriteBuffer2,TEST_SIZE,&fnum);printf("RES:%d",res_flash);
	f_close(&file);
	res_flash = f_open(&file,"1:test.txt",FA_READ);printf("RES:%d",res_flash);
	res_flash = f_read(&file,readbuffer2,TEST_SIZE,&fnum);printf("RES:%d",res_flash);
//	for(int i=0;i<TEST_SIZE;i++)
//		printf("0x%x,",readbuffer2[i]);
	printf("%s",readbuffer2);
	f_close(&file);
	f_unmount(FALSH_PATH);
}
uint8_t* glyph_bitmap_1;
uint16_t* unicode_list_1_1;
lv_font_fmt_txt_glyph_dsc_t* glyph_dsc_1;
void Read_Font_From_FLASH(void)
{
	glyph_bitmap_1 = (uint8_t*) malloc(BITMAP_SIZE);
	unicode_list_1_1 = (uint16_t*)malloc(UNICODE_SIZE/sizeof(uint16_t));
	glyph_dsc_1 = (lv_font_fmt_txt_glyph_dsc_t*)malloc(DSC_SIZE/sizeof(lv_font_fmt_txt_glyph_dsc_t));
	
	
	res_flash = f_mount(&fs,FALSH_PATH,1);
	if(res_flash != FR_OK)
		printf("file system init error!!\n");
	else{
		//开始读取字体数据
		res_flash = f_open(&file,"1:my_Font_16/glyph_bitmap.bin",FA_READ);
		if(res_flash == FR_OK){
			printf("open:%d  ",res_flash);
			res_flash = f_read(&file,glyph_bitmap_1,f_size(&file),&fnum);
			printf("read:%d\n",res_flash);
		}
		else{
			printf("glyph_bitmap.bin read error!%d\n",res_flash);
		}
		f_close(&file);
		
		res_flash = f_open(&file,"1:my_Font_16/unicode_list_1.bin",FA_READ);
		if(res_flash == FR_OK){
			printf("open:%d  ",res_flash);
			res_flash = f_read(&file,unicode_list_1_1,f_size(&file),&fnum);
			printf("read:%d\n",res_flash);
		}
		else{
			printf("unicode_list_1.bin read error!%d\n",res_flash);
		}
		f_close(&file);
		
		res_flash = f_open(&file,"1:my_Font_16/glyph_dsc.bin",FA_READ);
		if(res_flash == FR_OK){
			printf("open:%d  ",res_flash);
			res_flash = f_read(&file,glyph_dsc_1,f_size(&file),&fnum);
			printf("read:%d\n",res_flash);
		}
		else{
			printf("glyph_dsc.bin read error!%d\n",res_flash);
		}
		f_close(&file);
	}
	f_unmount("0:");
}

void Font2FALSH(void)//字体烧到flash
{

	res_flash = f_mount(&fs,FALSH_PATH,1);
	
	if(res_flash != FR_OK)
		printf("file system init error!!\n");
	else{
		res_flash = f_mkdir("1:my_Font_16");
		printf("目录创建:%d\n",res_flash);
		 
//		res_flash = f_unlink("1:my_Font_16/glyph_bitmap.bin");printf("删除1：%d",res_flash);
//		res_flash = f_unlink("1:my_Font_16/unicode_list_1.bin");printf("删除2：%d",res_flash);
//		res_flash = f_unlink("1:my_Font_16/glyph_dsc.bin");printf("删除3：%d",res_flash);
		//开始烧录glyph_bitmap.bin   
		res_flash = f_open(&file, "1:my_Font_16/glyph_bitmap.bin",FA_CREATE_ALWAYS | FA_WRITE );
		if(res_flash == FR_OK){
			res_flash = f_write(&file,glyph_bitmap,FONT_SIZE,&fnum);
			if(res_flash == FR_OK)
				printf("glyph_bitmap.bin write ok!%d,size:%d\n",res_flash,fnum);
			else{
				printf("glyph_bitmap.bin write error!%d\n",res_flash);
			}
		}
		else{
			printf("glyph_bitmap.bin create or open error!%d\n",res_flash);
		}
		f_close(&file);
		//开始烧录unicode_list_1.bin
		res_flash = f_open(&file, "1:my_Font_16/unicode_list_1.bin",FA_CREATE_ALWAYS | FA_WRITE );
		if(res_flash == FR_OK){
			res_flash = f_write(&file,unicode_list_1,UNICODE_SIZE,&fnum);
			if(res_flash == FR_OK)
				printf("unicode_list_1.bin write ok!%d,size:%d\n",res_flash,fnum);
			else{
				printf("unicode_list_1.bin write error!%d\n",res_flash);
			}
		}
		else{
			printf("unicode_list_1.bin create or open error!%d\n",res_flash);
		}
		f_close(&file);		
		//开始烧录glyph_dsc.bin
		res_flash = f_open(&file, "1:my_Font_16/glyph_dsc.bin",FA_CREATE_ALWAYS | FA_WRITE );
		if(res_flash == FR_OK){
			res_flash = f_write(&file,glyph_dsc,DSC_SIZE,&fnum);
			if(res_flash == FR_OK)
				printf("glyph_dsc.bin write ok!%d,size:%d\n",res_flash,fnum);
			else{
				printf("glyph_dsc.bin write error!%d\n",res_flash);
			}
		}
		else{
			printf("glyph_dsc.bin create or open error!%d\n",res_flash);
		}
		f_close(&file);	
	}
	f_unmount("0:");
}
void Read_Font_From_SD(void)
{
	glyph_bitmap_1 = (uint8_t*) malloc(BITMAP_SIZE);
	//unicode_list_1_1 = (uint16_t*)malloc(UNICODE_SIZE/sizeof(uint16_t));
	//glyph_dsc_1 = (lv_font_fmt_txt_glyph_dsc_t*)malloc(DSC_SIZE/sizeof(lv_font_fmt_txt_glyph_dsc_t));
	res_flash = f_mount(&fs0,"0:",1);
	if(res_flash != FR_OK)
		printf("file system init error!!\n");
	else{
		//开始读取字体数据
		res_flash = f_open(&file,"0:my_Font_16/glyph_bitmap.bin",FA_READ);
		if(res_flash != FR_OK)
			printf("glyph_bitmap.bin read error!%d\n",res_flash);
		else{
			printf("open:%d  ",res_flash);
			res_flash = f_read(&file,glyph_bitmap_1,f_size(&file),&fnum);
			printf("read:%d\n",res_flash);
		}
		f_close(&file);
		
		res_flash = f_open(&file,"0:my_Font_16/unicode_list_1.bin",FA_READ);
		if(res_flash != FR_OK)
			printf("unicode_list_1.bin read error!%d\n",res_flash);
		else{
			printf("open:%d  ",res_flash);
			res_flash = f_read(&file,unicode_list_1_1,f_size(&file),&fnum);
			printf("read:%d\n",res_flash);
		}
		f_close(&file);
		
		res_flash = f_open(&file,"0:my_Font_16/glyph_dsc.bin",FA_READ);
		if(res_flash != FR_OK)
			printf("glyph_dsc.bin read error!%d\n",res_flash);
		else{
			printf("open:%d  ",res_flash);
			res_flash = f_read(&file,glyph_dsc_1,f_size(&file),&fnum);
			printf("read:%d\n",res_flash);
		}
		f_close(&file);
	}
//	scan_files("0:");
	f_mount(NULL,"0:",0);
}

void Font2SD(void)//字体烧到SD卡
{
	res_flash = f_mount(&fs0,"0:",1);
	
	if(res_flash != FR_OK)
		printf("file system init error!!\n");
	else{
		
		
		//开始烧录glyph_bitmap.bin    
		res_flash = f_open(&file, "0:my_Font_16/glyph_bitmap.bin",FA_CREATE_ALWAYS | FA_WRITE );
		if(res_flash != FR_OK)
			printf("glyph_bitmap.bin create or open error!\n");
		else{
			res_flash = f_write(&file,glyph_bitmap,FONT_SIZE,&fnum);
			if(res_flash != FR_OK)
				printf("glyph_bitmap.bin write error!\n");
			else{
				printf("glyph_bitmap.bin write ok!%d glyph_bitmap.bin size:%d\n",res_flash,fnum);
			}
		}
		f_close(&file);
		//开始烧录unicode_list_1.bin
		res_flash = f_open(&file, "0:my_Font_16/unicode_list_1.bin",FA_CREATE_ALWAYS | FA_WRITE );
		if(res_flash != FR_OK)
			printf("unicode_list_1.bin create or open error!\n");
		else{
			res_flash = f_write(&file,unicode_list_1,unicode_SIZE,&fnum);
			if(res_flash != FR_OK)
				printf("unicode_list_1.bin write error!\n");
			else{
				printf("unicode_list_1.bin write ok!%d unicode_list_1.bin size:%d\n",res_flash,fnum);
			}
		}
		f_close(&file);		
		//开始烧录glyph_dsc.bin
		res_flash = f_open(&file, "0:my_Font_16/glyph_dsc.bin",FA_CREATE_ALWAYS | FA_WRITE );
		if(res_flash != FR_OK)
			printf("glyph_dsc.bin create or open error!\n");
		else{
			res_flash = f_write(&file,glyph_dsc,glyph_dsc_SIZE,&fnum);
			if(res_flash != FR_OK)
				printf("glyph_dsc.bin write error!\n");
			else{
				printf("glyph_dsc.bin write ok!%d glyph_dsc.bin size:%d\n",res_flash,fnum);
			}
		}
		f_close(&file);	
	}
	f_mount(NULL,"0:",0);
}

void SD_fatfs(void)
{
	res_flash = f_mount(&fs0,"0:",1);
	if(res_flash == FR_NO_FILESYSTEM) {
        printf("》SD卡还没有文件系统，即将进行格式化...\r\n");
        //res_flash=f_mkfs("0:",0,workspace0,4096);
        if (res_flash == FR_OK) {
            printf("》SD卡已成功格式化文件系统。\r\n");
            res_flash = f_mount(NULL,"0:",0);
            res_flash = f_mount(&fs0,"0:",0);
        }else{
            printf("《《SD卡格式化失败。》》\r\n");
        }
    }else if(res_flash==FR_OK){
        printf("外部SD卡挂载文件系统成功。(%d)\r\n",res_flash);
    }
    printf("\r\n****** 即将进行文件写入测试... ******\r\n");

    res_flash = f_open(&file, "0:FatFs.txt",

                       FA_CREATE_ALWAYS | FA_WRITE );

    if ( res_flash == FR_OK ) {
        printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
        res_flash=f_write(&file,WriteBuffer,sizeof(WriteBuffer),&fnum);
        if (res_flash==FR_OK) {
            printf("》文件写入成功，写入字节数据：%d\n",fnum);
            printf("》向文件写的数据为:\r\n%s\r\n",WriteBuffer);
        } else {
            printf("！！文件写入失败：(%d)\n",res_flash);
        }
        f_close(&file);
    } else {
        printf("！！打开/创建文件失败%x\r\n",res_flash);
    }
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

    printf("\n*************** 设备信息获取 ***************\r\n");
    /* 获取设备信息和空簇大小 */
    res_flash = f_getfree("1:", &fre_clust, &pfs);

    /* 计算得到总的扇区个数和空扇区个数 */
    tot_sect = (pfs->n_fatent - 2) * pfs->csize;
    fre_sect = fre_clust * pfs->csize;
    /* 打印信息(4096 字节/扇区) */
    printf("》设备总空间：%10lu KB。\n》可用空间： %10lu KB。\n",
            tot_sect *4, fre_sect *4);
    printf("\n******** 文件定位和格式化写入功能测试 ********\r\n");
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

FRESULT scan_files (char* path)
{
    FRESULT res; //部分在递归过程被修改的变量，不用全局变量
    FILINFO fno;
    DIR dir;
    int i;
    char *fn; // 文件名
    res = f_opendir(&dir, path);
    if (res == FR_OK) {
        i = strlen(path);
        for (;;) {
            //读取目录下的内容，再读会自动读下一个文件
            res = f_readdir(&dir, &fno);
            //为空时表示所有项目读取完毕，跳出
            if (res != FR_OK || fno.fname[0] == 0) break;
            #if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
            #else
            fn = fno.fname;
            #endif
            //点表示当前目录，跳过
            if (*fn == '.') continue;
            //目录，递归读取
            if (fno.fattrib & AM_DIR) {
                //合成完整目录名
                sprintf(&path[i], "/%s", fn);
                //递归遍历
                res = scan_files(path);
                path[i] = 0;
                //打开失败，跳出循环
                if (res != FR_OK)
                    break;
            } else {
                printf("%s/%s\r\n", path, fn); //输出文件名
                /* 可以在这里提取特定格式的文件路径 */
            }//else
        } //for
    }
    return res;
	f_unmount(FALSH_PATH);
}


void image2flash(uint8_t ofs, void* image,char* image_name)
{
    char file_name[15];
    char path_image[15];
    FRESULT res_flash;
    FIL fil_image;
    UINT fnum_image;
    printf("\n********** 写入图片数据 **********\r\n");
    sprintf(file_name,"%s.bin",image_name);
    sprintf(path_image,"%d:%s.bin",ofs,image_name);
    res_flash = f_open(&fil_image, path_image,FA_CREATE_ALWAYS | FA_WRITE );
    if(res_flash == FR_OK){
        printf("<%s> create success!writing...\n",file_name);
        res_flash=f_write(&fil_image,image,40960,&fnum_image);
        if(res_flash == FR_OK){
            printf("<%s> write complete!!!\n",file_name);
            printf("Number of bytes written:%d\n",fnum_image);
        }
        else printf("write failed,Maybe you've written it!\n");
    }else{
        printf("write or create file <%s> failed!\n",file_name);
    }
    f_close(&fil_image);

}
/*
0100 0011 1010 1001
0100 0010 0010 1001
0100 0011 1010 1011*/
