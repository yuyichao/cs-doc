// ************************************************************************
//  文件名：detect.cpp
//
//  图像分析与检测API函数库：
//
//  ThresholdDIB()	- 图像阈值分割运算
//  AddMinusDIB()   - 图像加减运算
//  HprojectDIB()	- 图像水平投影
//  VprojectDIB()	- 图像垂直投影
//	TemplateDIB()	- 图像模板匹配运算
//
// ************************************************************************

#include "stdafx.h"
#include "detect.h"
#include "DIBAPI.h"

#include <math.h>
#include <direct.h>

/*************************************************************************
 *
 * 函数名称：
 *   ThresholdDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 运算成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行阈值分割运算。
 * 
 ************************************************************************/

BOOL WINAPI ThresholdDIB(LPSTR lpDIBBits,LONG lWidth, LONG lHeight)
{
	
	// 指向源图像的指针
	LPSTR	lpSrc;
	
	// 指向缓存图像的指针
	LPSTR	lpDst;
	
	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//循环变量
	long i;
	long j;

	//像素值
	unsigned char pixel;

	//直方图数组
	long lHistogram[256];

	//阈值，最大灰度值与最小灰度值，两个区域的平均灰度值
	unsigned char iThreshold,iNewThreshold,iMaxGrayValue,iMinGrayValue,iMean1GrayValue,iMean2GrayValue;

	//用于计算区域灰度平均值的中间变量
	long lP1,lP2,lS1,lS2;

	//迭代次数
	int iIterationTimes;

	// 图像每行的字节数
	LONG lLineBytes;

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为255
	lpDst = (char *)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	for (i = 0; i < 256;i++)
	{
		lHistogram[i]=0;
	}

	//获得直方图
	iMaxGrayValue = 0;
	iMinGrayValue = 255;
	for (i = 0;i < lWidth ;i++)
	{
		for(j = 0;j < lHeight ;j++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;
	
			pixel = (unsigned char)*lpSrc;
			
			lHistogram[pixel]++;
			//修改最大，最小灰度值
			if(iMinGrayValue > pixel)
			{
				iMinGrayValue = pixel;
			}
			if(iMaxGrayValue < pixel)
			{
				iMaxGrayValue = pixel;
			}
		}
	}

	//迭代求最佳阈值
	iNewThreshold = (iMinGrayValue + iMaxGrayValue)/2;
	iThreshold = 0;
	
	for(iIterationTimes = 0; iThreshold != iNewThreshold && iIterationTimes < 100;iIterationTimes ++)
	{
		iThreshold = iNewThreshold;
		lP1 =0;
		lP2 =0;
		lS1 = 0;
		lS2 = 0;
		//求两个区域的灰度平均值
		for (i = iMinGrayValue;i < iThreshold;i++)
		{
			lP1 += lHistogram[i]*i;
			lS1 += lHistogram[i];
		}
		iMean1GrayValue = (unsigned char)(lP1 / lS1);
		for (i = iThreshold+1;i < iMaxGrayValue;i++)
		{
			lP2 += lHistogram[i]*i;
			lS2 += lHistogram[i];
		}
		iMean2GrayValue = (unsigned char)(lP2 / lS2);
		iNewThreshold =  (iMean1GrayValue + iMean2GrayValue)/2;
	}

	//根据阈值将图像二值化
	for (i = 0;i < lWidth ;i++)
	{
		for(j = 0;j < lHeight ;j++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;
	
			// 指向目标图像倒数第j行，第i个象素的指针			
			lpDst = (char *)lpNewDIBBits + lLineBytes * j + i;

			pixel = (unsigned char)*lpSrc;
			
			if(pixel <= iThreshold)
			{
				*lpDst = (unsigned char)0;
			}
			else
			{
				*lpDst = (unsigned char)255;
			}
		}
	}

	// 复制图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   AddMinusDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LPSTR lpDIBBitsBK  - 指向背景DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *	 bool  bAddMinus	- 为true时执行加运算，否则执行减运算。
 *
 * 返回值:
 *   BOOL               - 运算成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对两幅图像进行加减运算。
 * 
 * 要求目标图像为255个灰度值的灰度图像。
 ************************************************************************/

BOOL WINAPI AddMinusDIB(LPSTR lpDIBBits, LPSTR lpDIBBitsBK, LONG lWidth, LONG lHeight ,bool bAddMinus)
{
	
	// 指向源图像的指针
	LPSTR	lpSrc,lpSrcBK;
	
	// 指向缓存图像的指针
	LPSTR	lpDst;
	
	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//循环变量
	long i;
	long j;

	//像素值
	unsigned char pixel,pixelBK;

	// 图像每行的字节数
	LONG lLineBytes;

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为255
	lpDst = (char *)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lWidth ;i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;
			lpSrcBK = (char *)lpDIBBitsBK + lLineBytes * j + i;
	
			// 指向目标图像倒数第j行，第i个象素的指针			
			lpDst = (char *)lpNewDIBBits + lLineBytes * j + i;

			pixel = (unsigned char)*lpSrc;
			pixelBK = (unsigned char)*lpSrcBK;
			if(bAddMinus)
				*lpDst = pixel + pixelBK > 255 ? 255 : pixel + pixelBK;
			else
				*lpDst = pixel - pixelBK < 0 ? 0 : pixel - pixelBK;


		}
	}

			
	// 复制腐蚀后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   HprojectDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 运算成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对两幅图像进行水平投影运算。
 * 
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/

BOOL WINAPI HprojectDIB(LPSTR lpDIBBits,LONG lWidth, LONG lHeight)
{
	
	// 指向源图像的指针
	LPSTR	lpSrc;
	
	// 指向缓存图像的指针
	LPSTR	lpDst;
	
	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//循环变量
	long i;
	long j;

	//图像中每行内的黑点个数
	long lBlackNumber;

	//像素值
	unsigned char pixel;

	// 图像每行的字节数
	LONG lLineBytes;

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为255
	lpDst = (char *)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	for (j = 0;j < lHeight ;j++)
	{
		lBlackNumber = 0;
		for(i = 0;i < lWidth ;i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;
	
			pixel = (unsigned char)*lpSrc;

			if (pixel != 255 && pixel != 0)
			{
				return false;
			}
			if(pixel == 0)
			{
				lBlackNumber++;
			}
		}
		for(i = 0;i < lBlackNumber ;i++)
		{	
			// 指向目标图像倒数第j行，第i个象素的指针			
			lpDst = (char *)lpNewDIBBits + lLineBytes * j + i;

			*lpDst = (unsigned char)0;
		}		
	}

			
	// 复制投影图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   VprojectDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 运算成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对两幅图像进行垂直投影运算。
 * 
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/

BOOL WINAPI VprojectDIB(LPSTR lpDIBBits,LONG lWidth, LONG lHeight)
{
	
	// 指向源图像的指针
	LPSTR	lpSrc;
	
	// 指向缓存图像的指针
	LPSTR	lpDst;
	
	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//循环变量
	long i;
	long j;

	//图像中每行内的黑点个数
	long lBlackNumber;

	//像素值
	unsigned char pixel;

	// 图像每行的字节数
	LONG lLineBytes;

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为255
	lpDst = (char *)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	for (i = 0;i < lWidth ;i++)
	{
		lBlackNumber = 0;
		for(j = 0;j < lHeight ;j++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;
	
			pixel = (unsigned char)*lpSrc;

			if (pixel != 255 && pixel != 0)
			{
				return false;
			}
			if(pixel == 0)
			{
				lBlackNumber++;
			}
		}
		for(j = 0;j < lBlackNumber ;j++)
		{	
			// 指向目标图像倒数第j行，第i个象素的指针			
			lpDst = (char *)lpNewDIBBits + lLineBytes * j + i;

			*lpDst = (unsigned char)0;
		}		
	}

			
	// 复制投影图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   TemplateMatchDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LPSTR lpDIBBitsBK  - 指向背景DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *   LONG  lTemplateWidth       - 模板图像宽度（象素数）
 *   LONG  lTemplateHeight      - 模板图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 运算成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行模板匹配运算。
 * 
 * 要求目标图像为255个灰度值的灰度图像。
 ************************************************************************/

BOOL WINAPI TemplateMatchDIB (LPSTR lpDIBBits, LPSTR lpTemplateDIBBits, LONG lWidth, LONG lHeight,
							  LONG lTemplateWidth,LONG lTemplateHeight)
{	
	// 指向源图像的指针
	LPSTR	lpSrc,lpTemplateSrc;
	
	// 指向缓存图像的指针
	LPSTR	lpDst;
	
	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//循环变量
	long i;
	long j;
	long m;
	long n;

	//中间结果
	double dSigmaST;
	double dSigmaS;
	double dSigmaT;

	//相似性测度
	double R;

	//最大相似性测度
	double MaxR;

	//最大相似性出现位置
	long lMaxWidth;
	long lMaxHeight;

	//像素值
	unsigned char pixel;
	unsigned char templatepixel;

	// 图像每行的字节数
	LONG lLineBytes,lTemplateLineBytes;

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为255
	lpDst = (char *)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	lTemplateLineBytes = WIDTHBYTES(lTemplateWidth * 8);

	//计算dSigmaT
	dSigmaT = 0;
	for (n = 0;n < lTemplateHeight ;n++)
	{
		for(m = 0;m < lTemplateWidth ;m++)
		{
			// 指向模板图像倒数第j行，第i个象素的指针			
			lpTemplateSrc = (char *)lpTemplateDIBBits + lTemplateLineBytes * n + m;
			templatepixel = (unsigned char)*lpTemplateSrc;
			dSigmaT += (double)templatepixel*templatepixel;
		}
	}

	//找到图像中最大相似性的出现位置
	MaxR = 0.0;
	for (j = 0;j < lHeight - lTemplateHeight +1 ;j++)
	{
		for(i = 0;i < lWidth - lTemplateWidth + 1;i++)
		{
			dSigmaST = 0;
			dSigmaS = 0;
	
			for (n = 0;n < lTemplateHeight ;n++)
			{
				for(m = 0;m < lTemplateWidth ;m++)
				{
					// 指向源图像倒数第j+n行，第i+m个象素的指针			
					lpSrc  = (char *)lpDIBBits + lLineBytes * (j+n) + (i+m);
			
					// 指向模板图像倒数第n行，第m个象素的指针			
					lpTemplateSrc  = (char *)lpTemplateDIBBits + lTemplateLineBytes * n + m;

					pixel = (unsigned char)*lpSrc;
					templatepixel = (unsigned char)*lpTemplateSrc;

					dSigmaS += (double)pixel*pixel;
					dSigmaST += (double)pixel*templatepixel;
				}
			}
			//计算相似性
			R = dSigmaST / ( sqrt(dSigmaS)*sqrt(dSigmaT));
			//与最大相似性比较
			if (R > MaxR)
			{
				MaxR = R;
				lMaxWidth = i;
				lMaxHeight = j;
			}
		}
	}

	//将最大相似性出现区域部分复制到目标图像
	for (n = 0;n < lTemplateHeight ;n++)
	{
		for(m = 0;m < lTemplateWidth ;m++)
		{
			lpTemplateSrc = (char *)lpTemplateDIBBits + lTemplateLineBytes * n + m;
			lpDst = (char *)lpNewDIBBits + lLineBytes * (n+lMaxHeight) + (m+lMaxWidth);
			*lpDst = *lpTemplateSrc;
		}
	}
	
	// 复制图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;
}
