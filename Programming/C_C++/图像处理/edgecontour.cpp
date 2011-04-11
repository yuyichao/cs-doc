// ************************************************************************
//  文件名：edgecontour.cpp
//
//  图像边缘与轮廓运算API函数库：
//
//  RobertDIB()		- robert边缘检测运算
//  SobelDIB()		- sobel边缘检测运算
//  PrewittDIB()	- prewitt边缘检测运算
//  KirschDIB()		- kirsch边缘检测运算
//  GaussDIB()		- gauss边缘检测运算
//  HoughDIB()		- 利用Hough变换检测平行直线
//  ContourDIB()	- 轮廓提取
//  TraceDIB()		- 轮廓跟踪
//  FillDIB()		- 种子填充算法1
//  Fill2DIB()		- 种子填充算法2

//
// ************************************************************************

#include "stdafx.h"
#include "edgecontour.h"
#include "TemplateTrans.h"
#include "DIBAPI.h"

#include <math.h>
#include <direct.h>

/*************************************************************************
 *
 * 函数名称：
 *   RobertDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 * 返回值:
 *   BOOL               - 边缘检测成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用Robert边缘检测算子对图像进行边缘检测运算。
 * 
 * 要求目标图像为灰度图像。
 ************************************************************************/

BOOL WINAPI RobertDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
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
	double result;
	unsigned char pixel[4];

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


	//使用水平方向的结构元素进行腐蚀
	for(j = lHeight-1; j > 0; j--)
	{
		for(i = 0;i <lWidth-1; i++)
		{
			//由于使用2×2的模板，为防止越界，所以不处理最下边和最右边的两列像素

			// 指向源图像第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lWidth * j + i;

			// 指向目标图像第j行，第i个象素的指针			
			lpDst = (char *)lpNewDIBBits + lWidth * j + i;

			//取得当前指针处2*2区域的像素值，注意要转换为unsigned char型
			pixel[0] = (unsigned char)*lpSrc;
			pixel[1] = (unsigned char)*(lpSrc + 1);
			pixel[2] = (unsigned char)*(lpSrc - lWidth);
			pixel[3] = (unsigned char)*(lpSrc - lWidth + 1);

			//计算目标图像中的当前点
			 result = sqrt(( pixel[0] - pixel[3] )*( pixel[0] - pixel[3] ) + \
				  ( pixel[1] - pixel[2] )*( pixel[1] - pixel[2] ));
			*lpDst = (unsigned char)result;
		
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
 *   SobelDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 * 返回值:
 *   BOOL               - 边缘检测成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用Sobel边缘检测算子对图像进行边缘检测运算。
 * 
 * 要求目标图像为灰度图像。
 ************************************************************************/

BOOL WINAPI SobelDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	
	// 指向缓存图像的指针
	LPSTR	lpDst1;
	LPSTR	lpDst2;
	
	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits1;
	HLOCAL	hNewDIBBits1;
	LPSTR	lpNewDIBBits2;
	HLOCAL	hNewDIBBits2;

	//循环变量
	long i;
	long j;

	// 模板高度
	int		iTempH;
	
	// 模板宽度
	int		iTempW;
	
	// 模板系数
	FLOAT	fTempC;
	
	// 模板中心元素X坐标
	int		iTempMX;
	
	// 模板中心元素Y坐标
	int		iTempMY;
	
	//模板数组
	FLOAT aTemplate[9];

	// 暂时分配内存，以保存新图像
	hNewDIBBits1 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits1 = (char * )LocalLock(hNewDIBBits1);

	// 暂时分配内存，以保存新图像
	hNewDIBBits2 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits2 = (char * )LocalLock(hNewDIBBits2);

	// 拷贝源图像到缓存图像中
	lpDst1 = (char *)lpNewDIBBits1;
	memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
	lpDst2 = (char *)lpNewDIBBits2;
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

	// 设置Sobel模板参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	aTemplate[0] = -1.0;
	aTemplate[1] = -2.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 2.0;
	aTemplate[8] = 1.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits1, lWidth, lHeight, 
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// 设置Sobel模板参数
	aTemplate[0] = -1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = -2.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 2.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = 1.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight, 
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewDIBBits1 + lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewDIBBits2 + lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 复制经过模板运算后的图像到源图像
	memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits1);
	LocalFree(hNewDIBBits1);

	LocalUnlock(hNewDIBBits2);
	LocalFree(hNewDIBBits2);
	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   PrewittDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 * 返回值:
 *   BOOL               - 边缘检测成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用Prewitt边缘检测算子对图像进行边缘检测运算。
 * 
 * 要求目标图像为灰度图像。
 ************************************************************************/

BOOL WINAPI PrewittDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	
	// 指向缓存图像的指针
	LPSTR	lpDst1;
	LPSTR	lpDst2;
	
	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits1;
	HLOCAL	hNewDIBBits1;
	LPSTR	lpNewDIBBits2;
	HLOCAL	hNewDIBBits2;

	//循环变量
	long i;
	long j;

	// 模板高度
	int		iTempH;
	
	// 模板宽度
	int		iTempW;
	
	// 模板系数
	FLOAT	fTempC;
	
	// 模板中心元素X坐标
	int		iTempMX;
	
	// 模板中心元素Y坐标
	int		iTempMY;
	
	//模板数组
	FLOAT aTemplate[9];

	// 暂时分配内存，以保存新图像
	hNewDIBBits1 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits1 = (char * )LocalLock(hNewDIBBits1);

	// 暂时分配内存，以保存新图像
	hNewDIBBits2 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits2 = (char * )LocalLock(hNewDIBBits2);

	// 拷贝源图像到缓存图像中
	lpDst1 = (char *)lpNewDIBBits1;
	memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
	lpDst2 = (char *)lpNewDIBBits2;
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);


	// 设置Prewitt模板参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	aTemplate[0] = -1.0;
	aTemplate[1] = -1.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 1.0;
	aTemplate[8] = 1.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits1, lWidth, lHeight, 
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// 设置Prewitt模板参数
	aTemplate[0] = 1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 1.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = -1.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight, 
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewDIBBits1 + lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewDIBBits2 + lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 复制经过模板运算后的图像到源图像
	memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits1);
	LocalFree(hNewDIBBits1);

	LocalUnlock(hNewDIBBits2);
	LocalFree(hNewDIBBits2);
	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   KirschDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 * 返回值:
 *   BOOL               - 边缘检测成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用kirsch边缘检测算子对图像进行边缘检测运算。
 * 
 * 要求目标图像为灰度图像。
 ************************************************************************/

BOOL WINAPI KirschDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	
	// 指向缓存图像的指针
	LPSTR	lpDst1;
	LPSTR	lpDst2;
	
	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits1;
	HLOCAL	hNewDIBBits1;
	LPSTR	lpNewDIBBits2;
	HLOCAL	hNewDIBBits2;

	//循环变量
	long i;
	long j;

	// 模板高度
	int		iTempH;
	
	// 模板宽度
	int		iTempW;
	
	// 模板系数
	FLOAT	fTempC;
	
	// 模板中心元素X坐标
	int		iTempMX;
	
	// 模板中心元素Y坐标
	int		iTempMY;
	
	//模板数组
	FLOAT aTemplate[9];

	// 暂时分配内存，以保存新图像
	hNewDIBBits1 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits1 = (char * )LocalLock(hNewDIBBits1);

	// 暂时分配内存，以保存新图像
	hNewDIBBits2 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits2 = (char * )LocalLock(hNewDIBBits2);

	// 拷贝源图像到缓存图像中
	lpDst1 = (char *)lpNewDIBBits1;
	memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
	lpDst2 = (char *)lpNewDIBBits2;
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);


	// 设置Kirsch模板1参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	aTemplate[0] = 5.0;
	aTemplate[1] = 5.0;
	aTemplate[2] = 5.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = -3.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits1, lWidth, lHeight, 
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// 设置Kirsch模板2参数
	aTemplate[0] = -3.0;
	aTemplate[1] = 5.0;
	aTemplate[2] = 5.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 5.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = -3.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight, 
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewDIBBits1 + lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewDIBBits2 + lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

	// 设置Kirsch模板3参数
	aTemplate[0] = -3.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = 5.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 5.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = 5.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight, 
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewDIBBits1 + lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewDIBBits2 + lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);
	
	// 设置Kirsch模板4参数
	aTemplate[0] = -3.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 5.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = 5.0;
	aTemplate[8] = 5.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight, 
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewDIBBits1 + lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewDIBBits2 + lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);
	
	// 设置Kirsch模板5参数
	aTemplate[0] = -3.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = 5.0;
	aTemplate[7] = 5.0;
	aTemplate[8] = 5.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight, 
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);
	//求两幅缓存图像的最大值
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewDIBBits1 + lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewDIBBits2 + lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);
	
	// 设置Kirsch模板6参数
	aTemplate[0] = -3.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = 5.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = 5.0;
	aTemplate[7] = 5.0;
	aTemplate[8] = -3.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight, 
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewDIBBits1 + lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewDIBBits2 + lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);
	
	// 设置Kirsch模板7参数
	aTemplate[0] = 5.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = 5.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = 5.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = -3.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight, 
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewDIBBits1 + lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewDIBBits2 + lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);
	
	// 设置Kirsch模板8参数
	aTemplate[0] = 5.0;
	aTemplate[1] = 5.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = 5.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = -3.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight, 
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth-1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char *)lpNewDIBBits1 + lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char *)lpNewDIBBits2 + lWidth * j + i;
			
			if(*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		
		}
	}

	// 复制经过模板运算后的图像到源图像
	memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits1);
	LocalFree(hNewDIBBits1);

	LocalUnlock(hNewDIBBits2);
	LocalFree(hNewDIBBits2);
	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *  GaussDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 * 返回值:
 *   BOOL               - 边缘检测成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用高斯拉普拉斯边缘检测算子对图像进行边缘检测运算。
 * 
 * 要求目标图像为灰度图像。
 ************************************************************************/

BOOL WINAPI GaussDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	
	// 指向缓存图像的指针
	LPSTR	lpDst1;
	LPSTR	lpDst2;
	
	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits1;
	HLOCAL	hNewDIBBits1;
	LPSTR	lpNewDIBBits2;
	HLOCAL	hNewDIBBits2;

	// 模板高度
	int		iTempH;
	
	// 模板宽度
	int		iTempW;
	
	// 模板系数
	FLOAT	fTempC;
	
	// 模板中心元素X坐标
	int		iTempMX;
	
	// 模板中心元素Y坐标
	int		iTempMY;
	
	//模板数组
	FLOAT aTemplate[25];

	// 暂时分配内存，以保存新图像
	hNewDIBBits1 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits1 = (char * )LocalLock(hNewDIBBits1);

	// 暂时分配内存，以保存新图像
	hNewDIBBits2 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits2 = (char * )LocalLock(hNewDIBBits2);

	// 拷贝源图像到缓存图像中
	lpDst1 = (char *)lpNewDIBBits1;
	memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
	lpDst2 = (char *)lpNewDIBBits2;
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);


	// 设置Gauss模板参数
	iTempW = 5;
	iTempH = 5;
	fTempC = 1.0;
	iTempMX = 3;
	iTempMY = 3;
	aTemplate[0] = -2.0;
	aTemplate[1] = -4.0;
	aTemplate[2] = -4.0;
	aTemplate[3] = -4.0;
	aTemplate[4] = -2.0;
	aTemplate[5] = -4.0;
	aTemplate[6] = 0.0;
	aTemplate[7] = 8.0;
	aTemplate[8] = 0.0;
	aTemplate[9] = -4.0;
	aTemplate[10] = -4.0;
	aTemplate[11] = 8.0;
	aTemplate[12] = 24.0;
	aTemplate[13] = 8.0;
	aTemplate[14] = -4.0;
	aTemplate[15] = -4.0;
	aTemplate[16] = 0.0;
	aTemplate[17] = 8.0;
	aTemplate[18] = 0.0;
	aTemplate[19] = -4.0;
	aTemplate[20] = -2.0;
	aTemplate[21] = -4.0;
	aTemplate[22] = -4.0;
	aTemplate[23] = -4.0;
	aTemplate[24] = -2.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits1, lWidth, lHeight, 
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// 复制经过模板运算后的图像到源图像
	memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits1);
	LocalFree(hNewDIBBits1);

	LocalUnlock(hNewDIBBits2);
	LocalFree(hNewDIBBits2);
	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   HoughDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 * 返回值:
 *   BOOL               - 运算成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对检测图像中的平行直线。如果图像中有两条平行的直线，则将这两条平行直线
 * 提取出来。
 * 
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/

BOOL WINAPI HoughDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
		
	// 指向源图像的指针
	LPSTR	lpSrc;
	
	// 指向缓存图像的指针
	LPSTR	lpDst;
	
	// 指向变换域的指针
	LPSTR   lpTrans;

	// 图像每行的字节数
	LONG lLineBytes;
	
	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//指向变换域的指针
	LPSTR	lpTransArea;
	HLOCAL	hTransArea;

	//变换域的尺寸
	int iMaxDist;
	int iMaxAngleNumber;

	//变换域的坐标
	int iDist;
	int iAngleNumber;

	//循环变量
	long i;
	long j;

	//像素值
	unsigned char pixel;

	//存储变换域中的两个最大值
	MaxValue MaxValue1;
	MaxValue MaxValue2;

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

	//计算变换域的尺寸
	//最大距离
	iMaxDist = (int) sqrt(lWidth*lWidth + lHeight*lHeight);

	//角度从0－180，每格2度
	iMaxAngleNumber = 90;

	//为变换域分配内存
	hTransArea = LocalAlloc(LHND, lWidth * lHeight * sizeof(int));

	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpTransArea = (char * )LocalLock(hTransArea);
		
	// 初始化新分配的内存，设定初始值为0
	lpTrans = (char *)lpTransArea;
	memset(lpTrans, 0, lWidth * lHeight * sizeof(int));

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{

			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;

			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = (unsigned char)*lpSrc;

			//目标图像中含有0和255外的其它灰度值
			if(pixel != 255 && *lpSrc != 0)
				return FALSE;

			//如果是黑点，则在变换域的对应各点上加1
			if(pixel == 0)
			{
				//注意步长是2度
				for(iAngleNumber=0; iAngleNumber<iMaxAngleNumber; iAngleNumber++)
				{
					iDist = (int) fabs(i*cos(iAngleNumber*2*pi/180.0) + \
						j*sin(iAngleNumber*2*pi/180.0));
				
					//变换域的对应点上加1
					*(lpTransArea+iDist*iMaxAngleNumber+iAngleNumber) = \
						*(lpTransArea+iDist*iMaxAngleNumber+iAngleNumber) +1;
				}
			}
		
		}
	}
				
	//找到变换域中的两个最大值点
	MaxValue1.Value=0;
	MaxValue2.Value=0;
	
	//找到第一个最大值点
	for (iDist=0; iDist<iMaxDist;iDist++)
	{
		for(iAngleNumber=0; iAngleNumber<iMaxAngleNumber; iAngleNumber++)
		{
			if((int)*(lpTransArea+iDist*iMaxAngleNumber+iAngleNumber)>MaxValue1.Value)
			{
				MaxValue1.Value = (int)*(lpTransArea+iDist*iMaxAngleNumber+iAngleNumber);
				MaxValue1.Dist = iDist;
				MaxValue1.AngleNumber = iAngleNumber;
			}

		}
	}

	//将第一个最大值点附近清零
	for (iDist = -9;iDist < 10;iDist++)
	{
		for(iAngleNumber=-1; iAngleNumber<2; iAngleNumber++)
		{
			if(iDist+MaxValue1.Dist>=0 && iDist+MaxValue1.Dist<iMaxDist \
				&& iAngleNumber+MaxValue1.AngleNumber>=0 && iAngleNumber+MaxValue1.AngleNumber<=iMaxAngleNumber)
			{
				*(lpTransArea+(iDist+MaxValue1.Dist)*iMaxAngleNumber+\
					(iAngleNumber+MaxValue1.AngleNumber))=0;
			}
		}
	}

	//找到第二个最大值点
	for (iDist=0; iDist<iMaxDist;iDist++)
	{
		for(iAngleNumber=0; iAngleNumber<iMaxAngleNumber; iAngleNumber++)
		{
			if((int)*(lpTransArea+iDist*iMaxAngleNumber+iAngleNumber)>MaxValue2.Value)
			{
				MaxValue2.Value = (int)*(lpTransArea+iDist*iMaxAngleNumber+iAngleNumber);
				MaxValue2.Dist = iDist;
				MaxValue2.AngleNumber = iAngleNumber;
			}

		}
	}


	//判断两直线是否平行
	if(abs(MaxValue1.AngleNumber-MaxValue2.AngleNumber)<=2)
	{
		//两直线平行，在缓存图像中重绘这两条直线
		for(j = 0; j <lHeight; j++)
		{
			for(i = 0;i <lWidth; i++)
			{	

				// 指向缓存图像倒数第j行，第i个象素的指针			
				lpDst = (char *)lpNewDIBBits + lLineBytes * j + i;	

				//如果该点在某一条平行直线上，则在缓存图像上将该点赋为黑

				//在第一条直线上
				iDist = (int) fabs(i*cos(MaxValue1.AngleNumber*2*pi/180.0) + \
							j*sin(MaxValue1.AngleNumber*2*pi/180.0));
				if (iDist == MaxValue1.Dist)
					*lpDst = (unsigned char)0;
			
				//在第二条直线上
				iDist = (int) fabs(i*cos(MaxValue2.AngleNumber*2*pi/180.0) + \
							j*sin(MaxValue2.AngleNumber*2*pi/180.0));
				if (iDist == MaxValue2.Dist)
					*lpDst = (unsigned char)0;
			}
		}
	}

	// 复制腐蚀后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 释放内存
	LocalUnlock(hTransArea);
	LocalFree(hTransArea);

	// 返回
	return TRUE;

}

/*************************************************************************
 *
 * 函数名称：
 *   Fill2DIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 * 返回值:
 *   BOOL               - 种子填充成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行种子填充运算。
 * 
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/

BOOL WINAPI Fill2DIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	
	// 指向源图像的指针
	LPSTR	lpSrc;

	//循环变量
	long i;

	//像素值
	unsigned char pixel;

	//左右边界像素位置
	int xl,xr;

	//是否已填充至边界
	BOOL bFilll,bFillr;

	//种子堆栈及指针
	Seed Seeds[10];
	int StackPoint;

	//当前像素位置
	int iCurrentPixelx,iCurrentPixely;
	int iBufferPixelx,iBufferPixely;

	//初始化种子
	Seeds[1].Height = lHeight / 2;
	Seeds[1].Width = lWidth / 2;
	StackPoint = 1;

	while( StackPoint != 0)
	{
		//取出种子
		iCurrentPixelx = Seeds[StackPoint].Width;
		iCurrentPixely = Seeds[StackPoint].Height;
		StackPoint--;
//		if(Seed2.Height== 75)
//		{
//			return true;
//			i++;
//		}
		bFilll = true;
		bFillr = true;
		//填充种子所在的行
		//保存种子像素的位置
		iBufferPixelx = iCurrentPixelx;
		iBufferPixely = iCurrentPixely;
		//先向左填充
		while(bFilll)
		{
			lpSrc = (char *)lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx;
			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = (unsigned char)*lpSrc;

			//目标图像中含有0和255外的其它灰度值
			if(pixel != 255 && pixel != 0)
				return FALSE;
			//遇到边界
			if(pixel == 0)
			{
				bFilll = false;
				xl=iCurrentPixelx+1;
			}
			else
			{
				*lpSrc = (unsigned char)0;
				iCurrentPixelx--;
				//防止越界
				if(iCurrentPixelx<0)
				{	
					bFilll = false;
					iCurrentPixelx = 0;
					xl = 0; 
				}
			}		
		}
		//再向右填充
		//取回种子像素的位置
		iCurrentPixelx = iBufferPixelx+1;
		if(iCurrentPixelx>lWidth)
		{	
			bFillr = false;
			iCurrentPixelx = lWidth;
			xr = lWidth; 
		}
		iCurrentPixely = iBufferPixely;
		while(bFillr)
		{
			lpSrc = (char *)lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx;
			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = (unsigned char)*lpSrc;

			//目标图像中含有0和255外的其它灰度值
			if(pixel != 255 && pixel != 0)
				return FALSE;
			//遇到边界
			if(pixel == 0)
			{
				bFillr = false;
				xr=iCurrentPixelx-1;
			}
			else
			{
				*lpSrc = (unsigned char)0;
				iCurrentPixelx++;
				//防止越界
				if(iCurrentPixelx>lWidth)
				{	
					bFillr = false;
					iCurrentPixelx = lWidth;
					xr = lWidth; 
				}
			}		
		}
		//上、下两条扫描线是否全为边界象素或已填充过
		//先看上面的扫描线
		iCurrentPixely--;
		if(iCurrentPixely < 0)
		{
			iCurrentPixely = 0;
		}
		for (i = xr; i>= xl;i--)
		{
			lpSrc = (char *)lpDIBBits + lWidth * iCurrentPixely + i;
			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = (unsigned char)*lpSrc;

			//有未填充的像素，将新的种子压入堆栈
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely;
				Seeds[StackPoint].Width = i;
				break;
			}				
		}
		//再看下面的扫描线
		iCurrentPixely+=2;
		if(iCurrentPixely > lHeight)
		{
			iCurrentPixely = lHeight;
		}
		for (i = xr; i>= xl;i--)
		{
			lpSrc = (char *)lpDIBBits + lWidth * iCurrentPixely + i;
			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = (unsigned char)*lpSrc;

			//有未填充的像素，将新的种子压入堆栈
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely;
				Seeds[StackPoint].Width = i;
				break;
			}				
		}


	}

	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   FillDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 * 返回值:
 *   BOOL               - 种子填充成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行种子填充运算。
 * 
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/

BOOL WINAPI FillDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	
	// 指向源图像的指针
	LPSTR	lpSrc;

	//像素值
	unsigned char pixel;

	//种子堆栈及指针
	Seed *Seeds;
	int StackPoint;

	//当前像素位置
	int iCurrentPixelx,iCurrentPixely;

	//初始化种子
	Seeds = new Seed[lWidth*lHeight];
	Seeds[1].Height = lHeight / 2;
	Seeds[1].Width = lWidth / 2;
	StackPoint = 1;

	while( StackPoint != 0)
	{
		//取出种子
		iCurrentPixelx = Seeds[StackPoint].Width;
		iCurrentPixely = Seeds[StackPoint].Height;
		StackPoint--;

		lpSrc = (char *)lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx;
		//取得当前指针处的像素值，注意要转换为unsigned char型
		pixel = (unsigned char)*lpSrc;

		//目标图像中含有0和255外的其它灰度值
		if(pixel != 255 && pixel != 0)
			return FALSE;

		//将当前点涂黑
		*lpSrc = (unsigned char)0;

		//判断左面的点，如果为白，则压入堆栈
		//注意防止越界
		if(iCurrentPixelx > 0)
		{
			lpSrc = (char *)lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx - 1;
			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = (unsigned char)*lpSrc;
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely;
				Seeds[StackPoint].Width = iCurrentPixelx - 1;
			}
			//目标图像中含有0和255外的其它灰度值
			if(pixel != 255 && pixel != 0)
				return FALSE;
		}

		//判断上面的点，如果为白，则压入堆栈
		//注意防止越界
		if(iCurrentPixely < lHeight - 1)
		{
			lpSrc = (char *)lpDIBBits + lWidth * (iCurrentPixely + 1) + iCurrentPixelx;
			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = (unsigned char)*lpSrc;
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely + 1;
				Seeds[StackPoint].Width = iCurrentPixelx;
			}
			//目标图像中含有0和255外的其它灰度值
			if(pixel != 255 && pixel != 0)
				return FALSE;
		}

		//判断右面的点，如果为白，则压入堆栈
		//注意防止越界
		if(iCurrentPixelx < lWidth - 1)
		{
			lpSrc = (char *)lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx + 1;
			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = (unsigned char)*lpSrc;
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely;
				Seeds[StackPoint].Width = iCurrentPixelx + 1;
			}
			//目标图像中含有0和255外的其它灰度值
			if(pixel != 255 && pixel != 0)
				return FALSE;
		}

		//判断下面的点，如果为白，则压入堆栈
		//注意防止越界
		if(iCurrentPixely > 0)
		{
			lpSrc = (char *)lpDIBBits + lWidth * (iCurrentPixely - 1) + iCurrentPixelx;
			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = (unsigned char)*lpSrc;
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely - 1;
				Seeds[StackPoint].Width = iCurrentPixelx;
			}
			//目标图像中含有0和255外的其它灰度值
			if(pixel != 255 && pixel != 0)
				return FALSE;
		}
	}

	//释放堆栈
	delete Seeds;
	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   ContourDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 * 返回值:
 *   BOOL               - 运算成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行轮廓提取运算。
 * 
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/

BOOL WINAPI ContourDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
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
	unsigned char n,e,s,w,ne,se,nw,sw;

	//像素值
	unsigned char pixel;

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
	for(j = 1; j <lHeight-1; j++)
	{
		for(i = 1;i <lWidth-1; i++)
		{
			
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lWidth * j + i;
			
			// 指向目标图像倒数第j行，第i个象素的指针			
			lpDst = (char *)lpNewDIBBits + lWidth * j + i;
			
			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = (unsigned char)*lpSrc;

			//目标图像中含有0和255外的其它灰度值
//			if(pixel != 255 && pixel != 0)
//				return FALSE;
			if(pixel == 0)
			{
				*lpDst = (unsigned char)0;
				nw = (unsigned char)*(lpSrc + lWidth -1);
				n  = (unsigned char)*(lpSrc + lWidth );
				ne = (unsigned char)*(lpSrc + lWidth +1);
				w = (unsigned char)*(lpSrc -1);
				e = (unsigned char)*(lpSrc +1);
				sw = (unsigned char)*(lpSrc - lWidth -1);
				s  = (unsigned char)*(lpSrc - lWidth );
				se = (unsigned char)*(lpSrc - lWidth +1);
				//如果相邻的八个点都是黑点
				if(nw+n+ne+w+e+sw+s+se==0)
				{
					*lpDst = (unsigned char)255;
				}
			}
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
 *   TraceDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 * 返回值:
 *   BOOL               - 运算成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行轮廓跟踪运算。
 * 
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/

BOOL WINAPI TraceDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	
	// 指向源图像的指针
	LPSTR	lpSrc;
	
	// 指向缓存图像的指针
	LPSTR	lpDst;
	
	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	// 图像每行的字节数
	LONG lLineBytes;
	
	//循环变量
	long i;
	long j;

	//像素值
	unsigned char pixel;

	//是否找到起始点及回到起始点
	bool bFindStartPoint;

	//是否扫描到一个边界点
	bool bFindPoint;

	//起始边界点与当前边界点
	Point StartPoint,CurrentPoint;

	//八个方向和起始扫描方向
	int Direction[8][2]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};
	int BeginDirect;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为255
	lpDst = (char *)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lLineBytes * lHeight);

	//先找到最左上方的边界点
	bFindStartPoint = false;
	for (j = 0;j < lHeight && !bFindStartPoint;j++)
	{
		for(i = 0;i < lWidth && !bFindStartPoint;i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;
			
			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = (unsigned char)*lpSrc;
			
			if(pixel == 0)
			{
				bFindStartPoint = true;

				StartPoint.Height = j;
				StartPoint.Width = i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char *)lpNewDIBBits + lLineBytes * j + i;	
				*lpDst = (unsigned char)0;
			}		
		}
	}

	//由于起始点是在左下方，故起始扫描沿左上方向
	BeginDirect = 0;
	//跟踪边界
	bFindStartPoint = false;
	//从初始点开始扫描
	CurrentPoint.Height = StartPoint.Height;
	CurrentPoint.Width = StartPoint.Width;
	while(!bFindStartPoint)
	{
		bFindPoint = false;
		while(!bFindPoint)
		{
			//沿扫描方向查看一个像素
			lpSrc = (char *)lpDIBBits + lLineBytes * ( CurrentPoint.Height + Direction[BeginDirect][1])
				+ (CurrentPoint.Width + Direction[BeginDirect][0]);
			pixel = (unsigned char)*lpSrc;
			if(pixel == 0)
			{
				bFindPoint = true;
				CurrentPoint.Height = CurrentPoint.Height + Direction[BeginDirect][1];
				CurrentPoint.Width = CurrentPoint.Width + Direction[BeginDirect][0];
				if(CurrentPoint.Height == StartPoint.Height && CurrentPoint.Width == StartPoint.Width)
				{
					bFindStartPoint = true;
				}
				lpDst = (char *)lpNewDIBBits + lLineBytes * CurrentPoint.Height + CurrentPoint.Width;
				*lpDst = (unsigned char)0;
				//扫描的方向逆时针旋转两格
				BeginDirect--;
				if(BeginDirect == -1)
					BeginDirect = 7;
				BeginDirect--;
				if(BeginDirect == -1)
					BeginDirect = 7;
			}
			else
			{
				//扫描方向顺时针旋转一格
				BeginDirect++;
				if(BeginDirect == 8)
					BeginDirect = 0;

			}

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


