#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "../../public/simple_math/math_utils.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	//unsigned short    bfType;  
	unsigned long    bfSize;
	unsigned short    bfReserved1;
	unsigned short    bfReserved2;
	unsigned long    bfOffBits;
} ClBitMapFileHeader;

typedef struct
{
	unsigned long  biSize;
	long   biWidth;
	long   biHeight;
	unsigned short   biPlanes;
	unsigned short   biBitCount;
	unsigned long  biCompression;
	unsigned long  biSizeImage;
	long   biXPelsPerMeter;
	long   biYPelsPerMeter;
	unsigned long   biClrUsed;
	unsigned long   biClrImportant;
} ClBitMapInfoHeader;

typedef struct
{
	unsigned char rgbBlue; //该颜色的蓝色分量  
	unsigned char rgbGreen; //该颜色的绿色分量  
	unsigned char rgbRed; //该颜色的红色分量  
	unsigned char rgbReserved; //保留值  
} ClRgbQuad;

typedef struct
{
	int width;
	int height;
	int channels;
	unsigned char* imageData;
}ClImage;

ClImage* clLoadImage(char* path);
bool clSaveImage(char* path, ClImage* bmpImg);


ClImage* clLoadImage(char* path)
{
	ClImage* bmpImg;
	FILE* pFile;
	unsigned short fileType;
	ClBitMapFileHeader bmpFileHeader;
	ClBitMapInfoHeader bmpInfoHeader;
	int channels = 1;
	int width = 0;
	int height = 0;
	int step = 0;
	int offset = 0;
	unsigned char pixVal;
	ClRgbQuad* quad;
	int i, j, k;

	bmpImg = (ClImage*)malloc(sizeof(ClImage));
	pFile = fopen(path, "rb");
	if (!pFile)
	{
		free(bmpImg);
		return NULL;
	}

	fread(&fileType, sizeof(unsigned short), 1, pFile);
	if (fileType == 0x4D42) //string "BM"
	{
		//printf("bmp file! \n");  

		fread(&bmpFileHeader, sizeof(ClBitMapFileHeader), 1, pFile);
		/*printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");
		printf("bmp文件头信息：\n");
		printf("文件大小：%d \n", bmpFileHeader.bfSize);
		printf("保留字：%d \n", bmpFileHeader.bfReserved1);
		printf("保留字：%d \n", bmpFileHeader.bfReserved2);
		printf("位图数据偏移字节数：%d \n", bmpFileHeader.bfOffBits);*/

		fread(&bmpInfoHeader, sizeof(ClBitMapInfoHeader), 1, pFile);
		/*printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");
		printf("bmp文件信息头\n");
		printf("结构体长度：%d \n", bmpInfoHeader.biSize);
		printf("位图宽度：%d \n", bmpInfoHeader.biWidth);
		printf("位图高度：%d \n", bmpInfoHeader.biHeight);
		printf("位图平面数：%d \n", bmpInfoHeader.biPlanes);
		printf("颜色位数：%d \n", bmpInfoHeader.biBitCount);
		printf("压缩方式：%d \n", bmpInfoHeader.biCompression);
		printf("实际位图数据占用的字节数：%d \n", bmpInfoHeader.biSizeImage);
		printf("X方向分辨率：%d \n", bmpInfoHeader.biXPelsPerMeter);
		printf("Y方向分辨率：%d \n", bmpInfoHeader.biYPelsPerMeter);
		printf("使用的颜色数：%d \n", bmpInfoHeader.biClrUsed);
		printf("重要颜色数：%d \n", bmpInfoHeader.biClrImportant);
		printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");*/

		if (bmpInfoHeader.biBitCount == 8)
		{
			//printf("该文件有调色板，即该位图为非真彩色\n\n");  
			channels = 1;
			width = bmpInfoHeader.biWidth;
			height = bmpInfoHeader.biHeight;
			offset = (channels * width) % 4;
			if (offset != 0)
			{
				offset = 4 - offset;
			}
			//bmpImg->mat = kzCreateMat(height, width, 1, 0);  
			bmpImg->width = width;
			bmpImg->height = height;
			bmpImg->channels = 1;
			bmpImg->imageData = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
			step = channels * width;

			quad = (ClRgbQuad*)malloc(sizeof(ClRgbQuad) * 256);
			fread(quad, sizeof(ClRgbQuad), 256, pFile);
			free(quad);

			for (i = 0; i < height; i++)
			{
				for (j = 0; j < width; j++)
				{
					fread(&pixVal, sizeof(unsigned char), 1, pFile);
					bmpImg->imageData[(height - 1 - i) * step + j] = pixVal;   //每次一个像素处理
				}
				if (offset != 0)
				{
					for (j = 0; j < offset; j++)
					{
						fread(&pixVal, sizeof(unsigned char), 1, pFile);
					}
				}
			}
		}
		else if (bmpInfoHeader.biBitCount == 24)
		{
			//printf("该位图为位真彩色\n\n");  
			channels = 3;
			width = bmpInfoHeader.biWidth;
			height = bmpInfoHeader.biHeight;

			bmpImg->width = width;
			bmpImg->height = height;
			bmpImg->channels = 3;
			bmpImg->imageData = (unsigned char*)malloc(sizeof(unsigned char) * width * 3 * height);
			step = channels * width;

			offset = (channels * width) % 4;
			if (offset != 0)
			{
				offset = 4 - offset;
			}

			fseek(pFile, bmpFileHeader.bfOffBits - sizeof(bmpInfoHeader) - sizeof(bmpFileHeader), SEEK_CUR);  //138-54?感觉应该没有138才对啊

			for (i = 0; i < height; i++)
			{
				for (j = 0; j < width; j++)
				{
					for (k = 0; k < 3; k++)
					{
						fread(&pixVal, sizeof(unsigned char), 1, pFile);
						bmpImg->imageData[(height - 1 - i) * step + j * 3 + k] = pixVal;  //
					}
					//kzSetMat(bmpImg->mat, height-1-i, j, kzScalar(pixVal[0], pixVal[1], pixVal[2]));  
				}
				if (offset != 0)
				{
					for (j = 0; j < offset; j++)
					{
						fread(&pixVal, sizeof(unsigned char), 1, pFile);
					}
				}
			}
		}
	}

	return bmpImg;
}

bool clSaveImage(char* path, ClImage* bmpImg)
{
	FILE* pFile;
	unsigned short fileType;
	ClBitMapFileHeader bmpFileHeader;
	ClBitMapInfoHeader bmpInfoHeader;
	int step;
	int offset;
	unsigned char pixVal = '\0';
	int i, j;
	ClRgbQuad* quad;

	pFile = fopen(path, "wb");
	if (!pFile)
	{
		return false;
	}

	fileType = 0x4D42;
	fwrite(&fileType, sizeof(unsigned short), 1, pFile);

	if (bmpImg->channels == 3)//24位，通道，彩图  
	{
		step = bmpImg->channels * bmpImg->width;
		offset = step % 4;
		if (offset != 4)
		{
			step += 4 - offset;
		}

		bmpFileHeader.bfSize = bmpImg->height * step + 54;
		bmpFileHeader.bfReserved1 = 0;
		bmpFileHeader.bfReserved2 = 0;
		bmpFileHeader.bfOffBits = 54;
		fwrite(&bmpFileHeader, sizeof(ClBitMapFileHeader), 1, pFile);

		bmpInfoHeader.biSize = 40;
		bmpInfoHeader.biWidth = bmpImg->width;
		bmpInfoHeader.biHeight = bmpImg->height;
		bmpInfoHeader.biPlanes = 1;
		bmpInfoHeader.biBitCount = 24;
		bmpInfoHeader.biCompression = 0;
		bmpInfoHeader.biSizeImage = bmpImg->height * step;
		bmpInfoHeader.biXPelsPerMeter = 0;
		bmpInfoHeader.biYPelsPerMeter = 0;
		bmpInfoHeader.biClrUsed = 0;
		bmpInfoHeader.biClrImportant = 0;
		fwrite(&bmpInfoHeader, sizeof(ClBitMapInfoHeader), 1, pFile);

		for (i = bmpImg->height - 1; i > -1; i--)
		{
			for (j = 0; j < bmpImg->width; j++)
			{
				pixVal = bmpImg->imageData[i * bmpImg->width * 3 + j * 3];
				fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
				pixVal = bmpImg->imageData[i * bmpImg->width * 3 + j * 3 + 1];
				fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
				pixVal = bmpImg->imageData[i * bmpImg->width * 3 + j * 3 + 2];
				fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
			}
			if (offset != 0)
			{
				for (j = 0; j < offset; j++)
				{
					pixVal = 0;
					fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
				}
			}
		}
	}
	else if (bmpImg->channels == 1)//8位，单通道，灰度图  
	{
		step = bmpImg->width;
		offset = step % 4;
		if (offset != 4)
		{
			step += 4 - offset;
		}

		bmpFileHeader.bfSize = 54 + 256 * 4 + bmpImg->width;
		bmpFileHeader.bfReserved1 = 0;
		bmpFileHeader.bfReserved2 = 0;
		bmpFileHeader.bfOffBits = 54 + 256 * 4;
		fwrite(&bmpFileHeader, sizeof(ClBitMapFileHeader), 1, pFile);

		bmpInfoHeader.biSize = 40;
		bmpInfoHeader.biWidth = bmpImg->width;
		bmpInfoHeader.biHeight = bmpImg->height;
		bmpInfoHeader.biPlanes = 1;
		bmpInfoHeader.biBitCount = 8;
		bmpInfoHeader.biCompression = 0;
		bmpInfoHeader.biSizeImage = bmpImg->height * step;
		bmpInfoHeader.biXPelsPerMeter = 0;
		bmpInfoHeader.biYPelsPerMeter = 0;
		bmpInfoHeader.biClrUsed = 256;
		bmpInfoHeader.biClrImportant = 256;
		fwrite(&bmpInfoHeader, sizeof(ClBitMapInfoHeader), 1, pFile);

		quad = (ClRgbQuad*)malloc(sizeof(ClRgbQuad) * 256);
		for (i = 0; i < 256; i++)
		{
			quad[i].rgbBlue = i;
			quad[i].rgbGreen = i;
			quad[i].rgbRed = i;
			quad[i].rgbReserved = 0;
		}
		fwrite(quad, sizeof(ClRgbQuad), 256, pFile);
		free(quad);

		for (i = bmpImg->height - 1; i > -1; i--)
		{
			for (j = 0; j < bmpImg->width; j++)
			{
				pixVal = bmpImg->imageData[i * bmpImg->width + j];
				fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
			}
			if (offset != 0)
			{
				for (j = 0; j < offset; j++)
				{
					pixVal = 0;
					fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
				}
			}
		}
	}
	fclose(pFile);

	return true;
}

//void math_utils::Save_bmp(const vector<char>& data)
//{
//    BITMAPINFOHEADER bmiHdr; //定义信息头        
//    bmiHdr.biSize = sizeof(BITMAPINFOHEADER);
//    bmiHdr.biWidth = nImgW;
//    bmiHdr.biHeight = nImgH;
//    bmiHdr.biPlanes = 1;
//    bmiHdr.biBitCount = 24;
//    bmiHdr.biCompression = BI_RGB;
//    bmiHdr.biSizeImage = nImgW * nImgH * 3;
//    bmiHdr.biXPelsPerMeter = 0;
//    bmiHdr.biYPelsPerMeter = 0;
//    bmiHdr.biClrUsed = 0;
//    bmiHdr.biClrImportant = 0;
//    FILE* fp = fopen(filename, "wb");
//    if (fp)
//    {
//        BITMAPFILEHEADER fheader = { 0 };
//        fheader.bfType = 'M' << 8 | 'B';
//        fheader.bfSize = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) + pHeader->biSizeImage;
//        fheader.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
//        fwrite(&fheader, 1, sizeof(fheader), fp);
//        fwrite(pHeader, 1, sizeof(BITMAPINFOHEADER), fp);
//        fwrite(data, 1, bmiHdr.biSizeImage, fp);
//        fclose(fp);
//        return TRUE;
//    }
//    else
//        return FALSE;
//}
