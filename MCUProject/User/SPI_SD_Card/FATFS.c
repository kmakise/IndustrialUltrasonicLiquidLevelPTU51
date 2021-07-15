
#include "global.h"
		
		
		
FATFS fs;         /* Work area (file system object) for logical drive */
FIL fsrc, fdst;      /* file objects */
FRESULT res;
UINT br,bw;
char path0[512]="0:";
char buffer[4096];   /* file copy buffer */
uint8_t textFileBuffer[] = "��Ӣ�Ĳ����ַ��� \r\nChinese and English test strings \r\n";
		



//  * @��������	scan_files
//  * @����˵��   �����ļ�Ŀ¼�������ļ� 
//  * @�������   path: ��Ŀ¼ 
//  * @�������   ��
//  * @���ز���   FRESULT
//  * @ע������	��
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

//  * @��������	SD_TotalSize
//  * @����˵��   �ļ��ռ�ռ����� 
//  * @�������   �� 
//  * @�������   ��
//  * @���ز���   1: �ɹ� 
//  				0: ʧ��
//  * @ע������	��
int SD_TotalSize(char *path)
{
    FATFS *fs;
    DWORD fre_clust;        

    res = f_getfree(path, &fre_clust, &fs);  /* �����Ǹ�Ŀ¼��ѡ�����0 */
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
	//�����ļ�ϵͳ
	res = f_mount(0,&fs);
	if(res != FR_OK)
	{
		printf("mount filesystem 0 failed : %d\n\r",res);
	}
	//д�ļ�����
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

	//���ļ�����
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
	//ɨ���Ѿ����ڵ��ļ�
	printf("\n\rbegin scan files path0......\n\r");
	scan_files(path0);
	SD_TotalSize(path0);//��ȡSD����
}	



INT8U SD_WrEn = 1;
void SD_Mem(void)
{
	//����SPI1�ӿ�
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
		
		
