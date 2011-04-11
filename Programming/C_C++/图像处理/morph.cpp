// ************************************************************************
//  文件名：morph.cpp
//
//  图像形态学变换API函数库：
//
//  ErosionDIB()    - 图像腐蚀
//  DilationDIB()	- 图像膨胀
//  OpenDIB()		- 图像开运算
//  CloseDIB()		- 图像闭运算
//  ThiningDIB()	- 图像细化
//
// ************************************************************************


#include "stdafx.h"
#include "morph.h"
#include "DIBAPI.h"

#include <math.h>
#include <direct.h>

/*************************************************************************
 *
 * 函数名称：
 *   ErosiontionDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *   int   nMode		- 腐蚀方式，0表示水平方向，1表示垂直方向，2表示自定义结构元素。
 *	 int   structure[3][3]
						- 自定义的3×3结构元素。
 *
 * 返回值:
 *   BOOL               - 腐蚀成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行腐蚀运算。结构元素为水平方向或垂直方向的三个点，中间点位于原点；
 * 或者由用户自己定义3×3的结构元素。
 * 
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/

BOOL WINAPI ErosionDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode , int structure[3][3])
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
	int  n;
	int  m;

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


	if(nMode == 0)
	{
		//使用水平方向的结构元素进行腐蚀
		for(j = 0; j <lHeight; j++)
		{
			for(i = 1;i <lWidth-1; i++)
			{
				//由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if(pixel != 255 && *lpSrc != 0)
					return FALSE;
				
				//目标图像中的当前点先赋成黑色
				*lpDst = (unsigned char)0;

				//如果源图像中当前点自身或者左右有一个点不是黑色，
				//则将目标图像中的当前点赋成白色
				for (n = 0;n < 3;n++ )
				{
					pixel = *(lpSrc+n-1);
					if (pixel == 255 )
					{
						*lpDst = (unsigned char)255;
						break;
					}
				}
				
			}
		}

	}
	else if(nMode == 1)
	{
		//使用垂直方向的结构元素进行腐蚀
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				//由于使用1×3的结构元素，为防止越界，所以不处理最上边和最下边的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if(pixel != 255 && *lpSrc != 0)
					return FALSE;

				//目标图像中的当前点先赋成黑色
				*lpDst = (unsigned char)0;

				//如果源图像中当前点自身或者上下有一个点不是黑色，
				//则将目标图像中的当前点赋成白色
				for (n = 0;n < 3;n++ )
				{
					pixel = *(lpSrc+(n-1)*lWidth);
					if (pixel == 255 )
					{
						*lpDst = (unsigned char)255;
						break;
					}
				}
				
			}
		}

	}
	else
	{
		//使用自定义的结构元素进行腐蚀
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				//由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素
				//和最上边和最下边的两列像素
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if(pixel != 255 && *lpSrc != 0)
					return FALSE;

				//目标图像中的当前点先赋成黑色
				*lpDst = (unsigned char)0;

				//如果原图像中对应结构元素中为黑色的那些点中有一个不是黑色，
				//则将目标图像中的当前点赋成白色
				//注意在DIB图像中内容是上下倒置的
				for (m = 0;m < 3;m++ )
				{
					for (n = 0;n < 3;n++)
					{
						if( structure[m][n] == -1)
							continue;
						pixel = *(lpSrc + ((2-m)-1)*lWidth + (n-1));
						if (pixel == 255 )
						{	
							*lpDst = (unsigned char)255;
							break;
						}
					}
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
 *   DilationDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *   int   nMode		- 膨胀方式，0表示水平方向，1表示垂直方向，2表示自定义结构元素。
 *	 int   structure[3][3]
						- 自定义的3×3结构元素。
 *
 * 返回值:
 *   BOOL               - 膨胀成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行膨胀运算。结构元素为水平方向或垂直方向的三个点，中间点位于原点；
 * 或者由用户自己定义3×3的结构元素。
 * 
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/


BOOL WINAPI DilationDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode , int structure[3][3])
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
	int  n;
	int  m;

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


	if(nMode == 0)
	{
		//使用水平方向的结构元素进行膨胀
		for(j = 0; j <lHeight; j++)
		{
			for(i = 1;i <lWidth-1; i++)
			{
				//由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if(pixel != 255 && pixel != 0)
					return FALSE;
				
				//目标图像中的当前点先赋成白色
				*lpDst = (unsigned char)255;

				//源图像中当前点自身或者左右只要有一个点是黑色，
				//则将目标图像中的当前点赋成黑色
				for (n = 0;n < 3;n++ )
				{
					pixel = *(lpSrc+n-1);
					if (pixel == 0 )
					{
						*lpDst = (unsigned char)0;
						break;
					}
				}
				
			}
		}

	}
	else if(nMode == 1)
	{
		//使用垂直方向的结构元素进行膨胀
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				//由于使用1×3的结构元素，为防止越界，所以不处理最上边和最下边的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if(pixel != 255 && *lpSrc != 0)
					return FALSE;

				//目标图像中的当前点先赋成白色
				*lpDst = (unsigned char)255;

				//源图像中当前点自身或者上下只要有一个点是黑色，
				//则将目标图像中的当前点赋成黑色
				for (n = 0;n < 3;n++ )
				{
					pixel = *(lpSrc+(n-1)*lWidth);
					if (pixel == 0 )
					{
						*lpDst = (unsigned char)0;
						break;
					}
				}
				
			}
		}

	}
	else
	{
		//使用自定义的结构元素进行膨胀
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				//由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素
				//和最上边和最下边的两列像素
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if(pixel != 255 && *lpSrc != 0)
					return FALSE;

				//目标图像中的当前点先赋成白色
				*lpDst = (unsigned char)255;

				//原图像中对应结构元素中为黑色的那些点中只要有一个是黑色，
				//则将目标图像中的当前点赋成黑色
				//注意在DIB图像中内容是上下倒置的
				for (m = 0;m < 3;m++ )
				{
					for (n = 0;n < 3;n++)
					{
						if( structure[m][n] == -1)
							continue;
						pixel = *(lpSrc + ((2-m)-1)*lWidth + (n-1));
						if (pixel == 0 )
						{	
							*lpDst = (unsigned char)0;
							break;
						}
					}
				}
				
			}
		}

	}
	// 复制膨胀后的图像
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
 *   OpenDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *   int   nMode		- 开运算方式，0表示水平方向，1表示垂直方向，2表示自定义结构元素。
 *	 int   structure[3][3]
						- 自定义的3×3结构元素。
 *
 * 返回值:
 *   BOOL               - 开运算成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行开运算。结构元素为水平方向或垂直方向的三个点，中间点位于原点；
 * 或者由用户自己定义3×3的结构元素。
 * 
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/

BOOL WINAPI OpenDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode , int structure[3][3])
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
	int  n;
	int  m;

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


	if(nMode == 0)
	{
		//使用水平方向的结构元素进行腐蚀
		for(j = 0; j <lHeight; j++)
		{
			for(i = 1;i <lWidth-1; i++)
			{
				//由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if(pixel != 255 && *lpSrc != 0)
					return FALSE;
				
				//目标图像中的当前点先赋成黑色
				*lpDst = (unsigned char)0;

				//如果源图像中当前点自身或者左右有一个点不是黑色，
				//则将目标图像中的当前点赋成白色
				for (n = 0;n < 3;n++ )
				{
					pixel = *(lpSrc+n-1);
					if (pixel == 255 )
					{
						*lpDst = (unsigned char)255;
						break;
					}
				}
				
			}
		}

	}
	else if(nMode == 1)
	{
		//使用垂直方向的结构元素进行腐蚀
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				//由于使用1×3的结构元素，为防止越界，所以不处理最上边和最下边的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if(pixel != 255 && *lpSrc != 0)
					return FALSE;

				//目标图像中的当前点先赋成黑色
				*lpDst = (unsigned char)0;

				//如果源图像中当前点自身或者上下有一个点不是黑色，
				//则将目标图像中的当前点赋成白色
				for (n = 0;n < 3;n++ )
				{
					pixel = *(lpSrc+(n-1)*lWidth);
					if (pixel == 255 )
					{
						*lpDst = (unsigned char)255;
						break;
					}
				}
				
			}
		}

	}
	else
	{
		//使用自定义的结构元素进行腐蚀
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				//由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素
				//和最上边和最下边的两列像素
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if(pixel != 255 && *lpSrc != 0)
					return FALSE;

				//目标图像中的当前点先赋成黑色
				*lpDst = (unsigned char)0;

				//如果原图像中对应结构元素中为黑色的那些点中有一个不是黑色，
				//则将目标图像中的当前点赋成白色
				//注意在DIB图像中内容是上下倒置的
				for (m = 0;m < 3;m++ )
				{
					for (n = 0;n < 3;n++)
					{
						if( structure[m][n] == -1)
							continue;
						pixel = *(lpSrc + ((2-m)-1)*lWidth + (n-1));
						if (pixel == 255 )
						{	
							*lpDst = (unsigned char)255;
							break;
						}
					}
				}
				
			}
		}

	}
	// 复制腐蚀后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// 重新初始化新分配的内存，设定初始值为255
	//lpDst = (char *)lpNewDIBBits;
	//memset(lpDst, (BYTE)255, lWidth * lHeight);


	if(nMode == 0)
	{
		//使用水平方向的结构元素进行膨胀
		for(j = 0; j <lHeight; j++)
		{
			for(i = 1;i <lWidth-1; i++)
			{
				//由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if(pixel != 255 && *lpSrc != 0)
					return FALSE;
				
				//目标图像中的当前点先赋成白色
				*lpDst = (unsigned char)255;

				//源图像中当前点自身或者左右只要有一个点是黑色，
				//则将目标图像中的当前点赋成黑色
				for (n = 0;n < 3;n++ )
				{
					pixel = *(lpSrc+n-1);
					if (pixel == 0 )
					{
						*lpDst = (unsigned char)0;
						break;
					}
				}
				
			}
		}

	}
	else if(nMode == 1)
	{
		//使用垂直方向的结构元素进行膨胀
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				//由于使用1×3的结构元素，为防止越界，所以不处理最上边和最下边的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if(pixel != 255 && *lpSrc != 0)
					return FALSE;

				//目标图像中的当前点先赋成白色
				*lpDst = (unsigned char)255;

				//源图像中当前点自身或者上下只要有一个点是黑色，
				//则将目标图像中的当前点赋成黑色
				for (n = 0;n < 3;n++ )
				{
					pixel = *(lpSrc+(n-1)*lWidth);
					if (pixel == 0 )
					{
						*lpDst = (unsigned char)0;
						break;
					}
				}
				
			}
		}

	}
	else
	{
		//使用自定义的结构元素进行膨胀
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				//由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素
				//和最上边和最下边的两列像素
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if(pixel != 255 && *lpSrc != 0)
					return FALSE;

				//目标图像中的当前点先赋成白色
				*lpDst = (unsigned char)255;

				//原图像中对应结构元素中为黑色的那些点中只要有一个是黑色，
				//则将目标图像中的当前点赋成黑色
				//注意在DIB图像中内容是上下倒置的
				for (m = 0;m < 3;m++ )
				{
					for (n = 0;n < 3;n++)
					{
						if( structure[m][n] == -1)
							continue;
						pixel = *(lpSrc + ((2-m)-1)*lWidth + (n-1));
						if (pixel == 0 )
						{	
							*lpDst = (unsigned char)0;
							break;
						}
					}
				}
				
			}
		}

	}
	// 复制膨胀后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);


	return TRUE;
}


/*************************************************************************
 *
 * 函数名称：
 *   CloseDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *   int   nMode		- 闭运算方式，0表示水平方向，1表示垂直方向，2表示自定义结构元素。
 *	 int   structure[3][3]
						- 自定义的3×3结构元素。
 *
 * 返回值:
 *   BOOL               - 闭运算成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行开运算。结构元素为水平方向或垂直方向的三个点，中间点位于原点；
 * 或者由用户自己定义3×3的结构元素。
 * 
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/

/*************************************************************************
 *
 * 函数名称：
 *   ThinDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 闭运算成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行细化运算。
 * 
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/

BOOL WINAPI CloseDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode , int structure[3][3])
{
		if (DilationDIB(lpDIBBits, lWidth, lHeight, nMode , structure))
		{

			if (ErosionDIB(lpDIBBits, lWidth, lHeight, nMode , structure))
			{
				// 返回
				return TRUE;

			}
		}
		return FALSE;
	
	// 返回
	return TRUE;
}

BOOL WINAPI ThiningDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	
	// 指向源图像的指针
	LPSTR	lpSrc;
	
	// 指向缓存图像的指针
	LPSTR	lpDst;
	
	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//脏标记
	BOOL bModified;

	//循环变量
	long i;
	long j;
	int  n;
	int  m;

	//四个条件
	BOOL bCondition1;
	BOOL bCondition2;
	BOOL bCondition3;
	BOOL bCondition4;

	//计数器
	unsigned char nCount;
	
	//像素值
	unsigned char pixel;

	//5×5相邻区域像素值
	unsigned char neighbour[5][5];

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

	bModified=TRUE;

	while(bModified)
	{

		bModified = FALSE;
		// 初始化新分配的内存，设定初始值为255
		lpDst = (char *)lpNewDIBBits;
		memset(lpDst, (BYTE)255, lWidth * lHeight);

		for(j = 2; j <lHeight-2; j++)
		{
			for(i = 2;i <lWidth-2; i++)
			{

				bCondition1 = FALSE;
				bCondition2 = FALSE;
				bCondition3 = FALSE;
				bCondition4 = FALSE;

				//由于使用5×5的结构元素，为防止越界，所以不处理外围的几行和几列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if(pixel != 255 && *lpSrc != 0)
					//return FALSE;
					continue;
				//如果源图像中当前点为白色，则跳过
				else if(pixel == 255)
					continue;

				//获得当前点相邻的5×5区域内像素值，白色用0代表，黑色用1代表
				for (m = 0;m < 5;m++ )
				{
					for (n = 0;n < 5;n++)
					{
						neighbour[m][n] =(255 - (unsigned char)*(lpSrc + ((4 - m) - 2)*lWidth + n - 2 )) / 255;
					}
				}
//				neighbour[][]
				//逐个判断条件。
				//判断2<=NZ(P1)<=6
				nCount =  neighbour[1][1] + neighbour[1][2] + neighbour[1][3] \
						+ neighbour[2][1] + neighbour[2][3] + \
						+ neighbour[3][1] + neighbour[3][2] + neighbour[3][3];
				if ( nCount >= 2 && nCount <=6)
					bCondition1 = TRUE;

				//判断Z0(P1)=1
				nCount = 0;
				if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
					nCount++;
				if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
					nCount++;
				if (neighbour[2][1] == 0 && neighbour[3][1] == 1)
					nCount++;
				if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
					nCount++;
				if (neighbour[3][2] == 0 && neighbour[3][3] == 1)
					nCount++;
				if (neighbour[3][3] == 0 && neighbour[2][3] == 1)
					nCount++;
				if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
					nCount++;
				if (neighbour[1][3] == 0 && neighbour[1][2] == 1)
					nCount++;
				if (nCount == 1)
					bCondition2 = TRUE;

				//判断P2*P4*P8=0 or Z0(p2)!=1
				if (neighbour[1][2]*neighbour[2][1]*neighbour[2][3] == 0)
					bCondition3 = TRUE;
				else
				{
					nCount = 0;
					if (neighbour[0][2] == 0 && neighbour[0][1] == 1)
						nCount++;
					if (neighbour[0][1] == 0 && neighbour[1][1] == 1)
						nCount++;
					if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
						nCount++;
					if (neighbour[2][1] == 0 && neighbour[2][2] == 1)
						nCount++;
					if (neighbour[2][2] == 0 && neighbour[2][3] == 1)
						nCount++;
					if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
						nCount++;
					if (neighbour[1][3] == 0 && neighbour[0][3] == 1)
						nCount++;
					if (neighbour[0][3] == 0 && neighbour[0][2] == 1)
						nCount++;
					if (nCount != 1)
						bCondition3 = TRUE;
				}

				//判断P2*P4*P6=0 or Z0(p4)!=1
				if (neighbour[1][2]*neighbour[2][1]*neighbour[3][2] == 0)
					bCondition4 = TRUE;
				else
				{
					nCount = 0;
					if (neighbour[1][1] == 0 && neighbour[1][0] == 1)
						nCount++;
					if (neighbour[1][0] == 0 && neighbour[2][0] == 1)
						nCount++;
					if (neighbour[2][0] == 0 && neighbour[3][0] == 1)
						nCount++;
					if (neighbour[3][0] == 0 && neighbour[3][1] == 1)
						nCount++;
					if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
						nCount++;
					if (neighbour[3][2] == 0 && neighbour[2][2] == 1)
						nCount++;
					if (neighbour[2][2] == 0 && neighbour[1][2] == 1)
						nCount++;
					if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
						nCount++;
					if (nCount != 1)
						bCondition4 = TRUE;
				}
				if(bCondition1 && bCondition2 && bCondition3 && bCondition4)
				{
					*lpDst = (unsigned char)255;
					bModified = TRUE;
				}
				else
				{
					*lpDst = (unsigned char)0;
				}
			}
		}
			// 复制腐蚀后的图像
			memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);


	}
	// 复制腐蚀后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;
}

