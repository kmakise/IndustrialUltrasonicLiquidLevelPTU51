
#include "global.h"
		
		
		
FATFS fs;         /* Work area (file system object) for logical drive */
FIL fsrc, fdst;      /* file objects */
FRESULT res;
UINT br,bw;
char path0[512]="0:";
char buffer[4096];   /* file copy buffer */
uint8_t textFileBuffer[] = "中英文测试字符串 \r\nChinese and English test strings \r\n";
		



//  * @函数名称	scan_files
//  * @函数说明   搜索文件目录下所有文件 
//  * @输入参数   path: 根目录 
//  * @输出参数   无
//  * @返回参数   FRESULT
//  * @注意事项	无
FRESULT scan_files (
    char* path        /* Start node to be scanned (also used as work area) */
)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    int i;
    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
    static char lfn[_MAX_LFN + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif


    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) 
	{
        i = strlen(path);
        for (;;) 
		{
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fname[0] == '.') continue;             /* Ignore dot entry */
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
#else
            fn = fno.fname;
#endif
            if (fno.fattrib & AM_DIR) /* It is a directory */
			{                    
                sprintf(&path[i], "/%s", fn);
				printf("scan file - %s\n\r",path);
                res = scan_files(path);
                if (res != FR_OK) break;
                path[i] = 0;
            } 
			else                                 /* It is a file. */
			{                                       
                printf("scan file - %s/%s\n\r", path, fn);
            }
        }
    }
	else
	{
		printf("scan files error : %d\n\r",res);
	}

    return res;
}

//  * @函数名称	SD_TotalSize
//  * @函数说明   文件空间占用情况 
//  * @输入参数   无 
//  * @输出参数   无
//  * @返回参数   1: 成功 
//  				0: 失败
//  * @注意事项	无
int SD_TotalSize(char *path)
{
    FATFS *fs;
    DWORD fre_clust;        

    res = f_getfree(path, &fre_clust, &fs);  /* 必须是根目录，选择磁盘0 */
    if ( res==FR_OK ) 
    {
	  printf("\n\rget %s drive space.\n\r",path);
	  /* Print free space in unit of MB (assuming 512 bytes/sector) */
      printf("%d MB total drive space.\r\n"
           "%d MB available.\r\n",
           ( (fs->n_fatent - 2) * fs->csize ) / 2 /1024 , (fre_clust * fs->csize) / 2 /1024 );
		
	  return 1;
	}
	else
	{ 
	  printf("\n\rGet total drive space faild!\n\r");
	  return 0;   
	}
}

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
			
    TX3EN;		
	USART_SendData(USART3,(u8)ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
		
	return ch;
}


void SD_Wr(void)
{
	//挂载文件系统
	res = f_mount(0,&fs);
	if(res != FR_OK)
	{
		printf("mount filesystem 0 failed : %d\n\r",res);
	}
	//写文件测试
	printf("write file test......\n\r");
    res = f_open(&fdst, "0:/longfilenametest.txt", FA_CREATE_ALWAYS | FA_WRITE);
	if(res != FR_OK)
	{
		printf("open file error : %d\n\r",res);
	}
	else
	{
	    res = f_write(&fdst, textFileBuffer, sizeof(textFileBuffer), &bw);               /* Write it to the dst file */
		if(res == FR_OK)
		{
			printf("write data ok! %d\n\r",bw);
		}
		else
		{
			printf("write data error : %d\n\r",res);
		}
		/*close file */
		f_close(&fdst);
	}

	//读文件测试
	printf("read file test......\n\r");
    res = f_open(&fsrc, "0:/longfilenametest.txt", FA_OPEN_EXISTING | FA_READ);
    if(res != FR_OK)
	{
		printf("open file error : %d\n\r",res);
	}
	else
	{
	    res = f_read(&fsrc, buffer, sizeof(textFileBuffer), &br);     /* Read a chunk of src file */
		if(res==FR_OK)
		{
			printf("read data num : %d\n\r",br);
			printf("%s\n\r",buffer);
		}
		else
		{
			printf("read file error : %d\n\r",res);
		}
		/*close file */
		f_close(&fsrc);
	}
	//扫描已经存在的文件
	printf("\n\rbegin scan files path0......\n\r");
	scan_files(path0);
	SD_TotalSize(path0);//获取SD容量
}	



INT8U SD_WrEn = 1;
void SD_Mem(void)
{
	//配置SPI1接口
	MSD0_SPI_Configuration();
	
	while(1)
	{	
		if(SD_WrEn)
		{
			SD_WrEn = 0;
			
			SD_Wr();	
			
			RX3EN;	
			OSTimeDly(mS(500));			
		}
	}
}	
		
		
