
/**************************************************************************
 *  文件名：GeoTrans.cpp
 *
 *  图像几何变换API函数库：
 *
 *  TranslationDIB1()   - 图像平移
 *  TranslationDIB()    - 图像平移
 *  MirrorDIB()		    - 图像镜像
 *  TransposeDIB()		- 图像转置
 *  ZoomDIB()			- 图像缩放
 *  RotateDIB()			- 图像旋转
 *
 *************************************************************************/

#include "stdafx.h"
#include "geotrans.h"
#include "DIBAPI.h"

#include <math.h>
#include <direct.h>

/*************************************************************************
 *
 * 函数名称：
 *   TranslationDIB1()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *   LONG  lXOffset     - X轴平移量（象素数）
 *   LONG  lYOffset     - Y轴平移量（象素数）
 *
 * 返回值:
 *   BOOL               - 平移成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来水平移动DIB图像。函数不会改变图像的大小，移出的部分图像
 * 将截去，空白部分用白色填充。
 *
 ************************************************************************/

BOOL WINAPI TranslationDIB1(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, LONG lXOffset, LONG lYOffset)
{
	// 指向源图像的指针
	LPSTR	lpSrc;
	
	// 指向要复制区域的指针
	LPSTR	lpDst;
	
	// 指向复制图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	// 象素在新DIB中的坐标
	LONG	i;
	LONG	j;
	
	// 象素在源DIB中的坐标
	LONG	i0;
	LONG	j0;
	
	// 图像每行的字节数
	LONG lLineBytes;
	
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
	
	// 每行
	for(i = 0; i < lHeight; i++)
	{
		// 每列
		for(j = 0; j < lWidth; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			// 注意由于DIB中图像第一行其实保存在最后一行的位置，因此lpDst
			// 值不是(char *)lpNewDIBBits + lLineBytes * i + j，而是
			// (char *)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j
			lpDst = (char *)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 计算该象素在源DIB中的坐标
			i0 = i - lXOffset;
			j0 = j - lYOffset;
			
			// 判断是否在源图范围内
			if( (j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
			{
				// 指向源DIB第i0行，第j0个象素的指针
				// 同样要注意DIB上下倒置的问题
				lpSrc = (char *)lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;
				
				// 复制象素
				*lpDst = *lpSrc;
			}
			else
			{
				// 对于源图中没有的象素，直接赋值为255
				* ((unsigned char*)lpDst) = 255;
			}
			
		}
	}
	
	// 复制平移后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);
	
	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	
	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   TranslationDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *   LONG  lXOffset     - X轴平移量（象素数）
 *   LONG  lYOffset     - Y轴平移量（象素数）
 *
 * 返回值:
 *   BOOL               - 平移成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来水平移动DIB图像。函数不会改变图像的大小，移出的部分图像
 * 将截去，空白部分用白色填充。
 *
 ************************************************************************/

BOOL WINAPI TranslationDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, LONG lXOffset, LONG lYOffset)
{
	// 平移后剩余图像在源图像中的位置（矩形区域）
	CRect	rectSrc;
	
	// 平移后剩余图像在新图像中的位置（矩形区域）
	CRect	rectDst;
	
	// 指向源图像的指针
	LPSTR	lpSrc;
	
	// 指向要复制区域的指针
	LPSTR	lpDst;
	
	// 指向复制图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;
	
	// 指明图像是否全部移去可视区间
	BOOL	bVisible;

	// 循环变量
	LONG	i;
	
	// 图像每行的字节数
	LONG lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 赋初值
	bVisible = TRUE;
	
	// 计算rectSrc和rectDst的X坐标
	if (lXOffset <= -lWidth)
	{
		// X轴方向全部移出可视区域
		bVisible = FALSE;
	}
	else if (lXOffset <= 0)
	{
		// 移动后，有图区域左上角X坐标为0
		rectDst.left = 0;
		
		// 移动后，有图区域右下角X坐标为lWidth - |lXOffset| = lWidth + lXOffset
		rectDst.right = lWidth + lXOffset;
	}
	else if (lXOffset < lWidth)
	{
		// 移动后，有图区域左上角X坐标为lXOffset
		rectDst.left = lXOffset;
		
		// 移动后，有图区域右下角X坐标为lWidth
		rectDst.right = lWidth;
	}
	else
	{
		// X轴方向全部移出可视区域
		bVisible = FALSE;
	}
	
	// 平移后剩余图像在源图像中的X坐标
	rectSrc.left = rectDst.left - lXOffset;
	rectSrc.right = rectDst.right - lXOffset;
	
	//  计算rectSrc和rectDst的Y坐标
	if (lYOffset <= -lHeight)
	{
		// Y轴方向全部移出可视区域
		bVisible = FALSE;
	}
	else if (lYOffset <= 0)
	{
		// 移动后，有图区域左上角Y坐标为0
		rectDst.top = 0;
		
		// 移动后，有图区域右下角Y坐标为lHeight - |lYOffset| = lHeight + lYOffset
		rectDst.bottom = lHeight + lYOffset;
	}
	else if (lYOffset < lHeight)
	{
		// 移动后，有图区域左上角Y坐标为lYOffset
		rectDst.top = lYOffset;
		
		// 移动后，有图区域右下角Y坐标为lHeight
		rectDst.bottom = lHeight;
	}
	else
	{
		// X轴方向全部移出可视区域
		bVisible = FALSE;
	}
	
	// 平移后剩余图像在源图像中的Y坐标
	rectSrc.top = rectDst.top - lYOffset;
	rectSrc.bottom = rectDst.bottom - lYOffset;
	
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
	
	// 如果有部分图像可见
	if (bVisible)
	{

		// 平移图像
		for(i = 0; i < (rectSrc.bottom - rectSrc.top); i++)
		{
			// 要复制区域的起点，注意由于DIB图像内容是上下倒置的，第一行内容是保存在最后
			// 一行，因此复制区域的起点不是lpDIBBits + lLineBytes * (i + rectSrc.top) + 
			// rectSrc.left，而是 lpDIBBits + lLineBytes * (lHeight - i - rectSrc.top - 1) + 
			// rectSrc.left。
			
			lpSrc = (char *)lpDIBBits + lLineBytes * (lHeight - i - rectSrc.top - 1) + rectSrc.left;
			
			// 要目标区域的起点
			// 同样注意上下倒置的问题。
			lpDst = (char *)lpNewDIBBits + lLineBytes * (lHeight - i - rectDst.top - 1) + 
				rectDst.left;
			
			// 拷贝每一行，宽度为rectSrc.right - rectSrc.left
			memcpy(lpDst, lpSrc, rectSrc.right - rectSrc.left);
			
		}
	}

	// 复制平移后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);
	
	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	
	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   MirrorDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *   BOOL  bDirection   - 镜像的方向，TRUE表示水平镜像，FALSE表示垂直镜像
 *
 * 返回值:
 *   BOOL               - 镜像成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来镜像DIB图像。可以指定镜像的方式是水平还是垂直。
 *
 ************************************************************************/

BOOL WINAPI MirrorDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bDirection)
{
	
	// 指向源图像的指针
	LPSTR	lpSrc;
	
	// 指向要复制区域的指针
	LPSTR	lpDst;
	
	// 指向复制图像的指针
	LPSTR	lpBits;
	HLOCAL	hBits;

	// 循环变量
	LONG	i;
	LONG	j;
	
	// 图像每行的字节数
	LONG lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 暂时分配内存，以保存一行图像
	hBits = LocalAlloc(LHND, lLineBytes);
	if (hBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpBits = (char * )LocalLock(hBits);
	
	// 判断镜像方式
	if (bDirection)
	{
		// 水平镜像
		
		// 针对图像每行进行操作
		for(i = 0; i < lHeight; i++)
		{
			// 针对每行图像左半部分进行操作
			for(j = 0; j < lWidth / 2; j++)
			{
				
				// 指向倒数第i行，第j个象素的指针
				lpSrc = (char *)lpDIBBits + lLineBytes * i + j;
				
				// 指向倒数第i行，倒数第j个象素的指针
				lpDst = (char *)lpDIBBits + lLineBytes * (i + 1) - j;
				
				// 备份一个象素
				*lpBits = *lpDst;
				
				// 将倒数第i行，第j个象素复制到倒数第i行，倒数第j个象素
				*lpDst = *lpSrc;
				
				// 将倒数第i行，倒数第j个象素复制到倒数第i行，第j个象素
				*lpSrc = *lpBits;
			}
			
		}
	}
	else
	{
		// 垂直镜像

		// 针对上半图像进行操作
		for(i = 0; i < lHeight / 2; i++)
		{
			
			// 指向倒数第i行象素起点的指针
			lpSrc = (char *)lpDIBBits + lLineBytes * i;
			
			// 指向第i行象素起点的指针
			lpDst = (char *)lpDIBBits + lLineBytes * (lHeight - i - 1);
			
			// 备份一行，宽度为lWidth
			memcpy(lpBits, lpDst, lLineBytes);
			
			// 将倒数第i行象素复制到第i行
			memcpy(lpDst, lpSrc, lLineBytes);
			
			// 将第i行象素复制到倒数第i行
			memcpy(lpSrc, lpBits, lLineBytes);
			
		}
	}
	
	// 释放内存
	LocalUnlock(hBits);
	LocalFree(hBits);
	
	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   TransposeDIB()
 *
 * 参数:
 *   LPSTR lpDIB		- 指向源DIB的指针
 *
 * 返回值:
 *   BOOL               - 转置成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来转置DIB图像，即图像x、y坐标互换。函数将不会改变图像的大小，
 * 但是图像的高宽将互换。
 *
 ************************************************************************/

BOOL WINAPI TransposeDIB(LPSTR lpDIB)
{
	
	// 图像的宽度和高度
	LONG	lWidth;
	LONG	lHeight;
	
	// 指向源图像的指针
	LPSTR	lpDIBBits;
	
	// 指向源象素的指针
	LPSTR	lpSrc;
	
	// 指向转置图像对应象素的指针
	LPSTR	lpDst;

	// 指向转置图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;
	
	// 指向BITMAPINFO结构的指针（Win3.0）
	LPBITMAPINFOHEADER lpbmi;
	
	// 指向BITMAPCOREINFO结构的指针
	LPBITMAPCOREHEADER lpbmc;

	// 循环变量
	LONG	i;
	LONG	j;
	
	// 图像每行的字节数
	LONG lLineBytes;
	
	// 新图像每行的字节数
	LONG lNewLineBytes;

	// 获取指针
	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	// 找到源DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 获取图像的"宽度"（4的倍数）
	lWidth = ::DIBWidth(lpDIB);
	
	// 获取图像的高度
	lHeight = ::DIBHeight(lpDIB);
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 计算新图像每行的字节数
	lNewLineBytes = WIDTHBYTES(lHeight * 8);
	
	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lNewLineBytes);

	// 判断是否内存分配失败
	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);
	
	// 针对图像每行进行操作
	for(i = 0; i < lHeight; i++)
	{
		// 针对每行图像每列进行操作
		for(j = 0; j < lWidth; j++)
		{
			
			// 指向源DIB第i行，第j个象素的指针
			lpSrc = (char *)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 指向转置DIB第j行，第i个象素的指针
			// 注意此处lWidth和lHeight是源DIB的宽度和高度，应该互换
			lpDst = (char *)lpNewDIBBits + lNewLineBytes * (lWidth - 1 - j) + i;
			
			// 复制象素
			*lpDst = *lpSrc;
			
		}
		
	}
	
	// 复制转置后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lNewLineBytes);
	
	// 互换DIB中图像的高宽
	if (IS_WIN30_DIB(lpDIB))
	{
		// 对于Windows 3.0 DIB
		lpbmi->biWidth = lHeight;
		
		lpbmi->biHeight = lWidth;
	}
	else
	{
		// 对于其它格式的DIB
		lpbmc->bcWidth = (unsigned short) lHeight;
		lpbmc->bcHeight = (unsigned short) lWidth;
	}
	
	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	
	// 返回
	return TRUE;
}


/*************************************************************************
 *
 * 函数名称：
 *   ZoomDIB()
 *
 * 参数:
 *   LPSTR lpDIB		- 指向源DIB的指针
 *   float fXZoomRatio	- X轴方向缩放比率
 *   float fYZoomRatio	- Y轴方向缩放比率
 *
 * 返回值:
 *   HGLOBAL            - 缩放成功返回新DIB句柄，否则返回NULL。
 *
 * 说明:
 *   该函数用来缩放DIB图像，返回新生成DIB的句柄。
 *
 ************************************************************************/

HGLOBAL WINAPI ZoomDIB(LPSTR lpDIB, float fXZoomRatio, float fYZoomRatio)
{
	
	// 源图像的宽度和高度
	LONG	lWidth;
	LONG	lHeight;
	
	// 缩放后图像的宽度和高度
	LONG	lNewWidth;
	LONG	lNewHeight;
	
	// 缩放后图像的宽度（lNewWidth'，必须是4的倍数）
	LONG	lNewLineBytes;
	
	// 指向源图像的指针
	LPSTR	lpDIBBits;
	
	// 指向源象素的指针
	LPSTR	lpSrc;
	
	// 缩放后新DIB句柄
	HDIB	hDIB;
	
	// 指向缩放图像对应象素的指针
	LPSTR	lpDst;
	
	// 指向缩放图像的指针
	LPSTR	lpNewDIB;
	LPSTR	lpNewDIBBits;
	
	// 指向BITMAPINFO结构的指针（Win3.0）
	LPBITMAPINFOHEADER lpbmi;
	
	// 指向BITMAPCOREINFO结构的指针
	LPBITMAPCOREHEADER lpbmc;
	
	// 循环变量（象素在新DIB中的坐标）
	LONG	i;
	LONG	j;
	
	// 象素在源DIB中的坐标
	LONG	i0;
	LONG	j0;
	
	// 图像每行的字节数
	LONG lLineBytes;
	
	// 找到源DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 获取图像的宽度
	lWidth = ::DIBWidth(lpDIB);
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 获取图像的高度
	lHeight = ::DIBHeight(lpDIB);
	
	// 计算缩放后的图像实际宽度
	// 此处直接加0.5是由于强制类型转换时不四舍五入，而是直接截去小数部分
	lNewWidth = (LONG) (::DIBWidth(lpDIB) * fXZoomRatio + 0.5);
	
	// 计算新图像每行的字节数
	lNewLineBytes = WIDTHBYTES(lNewWidth * 8);
	
	// 计算缩放后的图像高度
	lNewHeight = (LONG) (lHeight * fYZoomRatio + 0.5);
	
	// 分配内存，以保存新DIB
	hDIB = (HDIB) ::GlobalAlloc(GHND, lNewLineBytes * lNewHeight + *(LPDWORD)lpDIB + ::PaletteSize(lpDIB));
	
	// 判断是否内存分配失败
	if (hDIB == NULL)
	{
		// 分配内存失败
		return NULL;
	}
	
	// 锁定内存
	lpNewDIB =  (char * )::GlobalLock((HGLOBAL) hDIB);
	
	// 复制DIB信息头和调色板
	memcpy(lpNewDIB, lpDIB, *(LPDWORD)lpDIB + ::PaletteSize(lpDIB));
	
	// 找到新DIB象素起始位置
	lpNewDIBBits = ::FindDIBBits(lpNewDIB);
	
	// 获取指针
	lpbmi = (LPBITMAPINFOHEADER)lpNewDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpNewDIB;
	
	// 更新DIB中图像的高度和宽度
	if (IS_WIN30_DIB(lpNewDIB))
	{
		// 对于Windows 3.0 DIB
		lpbmi->biWidth = lNewWidth;
		lpbmi->biHeight = lNewHeight;
	}
	else
	{
		// 对于其它格式的DIB
		lpbmc->bcWidth = (unsigned short) lNewWidth;
		lpbmc->bcHeight = (unsigned short) lNewHeight;
	}
	
	// 针对图像每行进行操作
	for(i = 0; i < lNewHeight; i++)
	{
		// 针对图像每列进行操作
		for(j = 0; j < lNewWidth; j++)
		{
			
			// 指向新DIB第i行，第j个象素的指针
			// 注意此处宽度和高度是新DIB的宽度和高度
			lpDst = (char *)lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;
			
			// 计算该象素在源DIB中的坐标
			i0 = (LONG) (i / fYZoomRatio + 0.5);
			j0 = (LONG) (j / fXZoomRatio + 0.5);
			
			// 判断是否在源图范围内
			if( (j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
			{
				
				// 指向源DIB第i0行，第j0个象素的指针
				lpSrc = (char *)lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;
				
				// 复制象素
				*lpDst = *lpSrc;
			}
			else
			{
				// 对于源图中没有的象素，直接赋值为255
				* ((unsigned char*)lpDst) = 255;
			}
			
		}
		
	}
	
	// 返回
	return hDIB;
}

/*************************************************************************
 *
 * 函数名称：
 *   RotateDIB()
 *
 * 参数:
 *   LPSTR lpDIB		- 指向源DIB的指针
 *   int iRotateAngle	- 旋转的角度（0-360度）
 *
 * 返回值:
 *   HGLOBAL            - 旋转成功返回新DIB句柄，否则返回NULL。
 *
 * 说明:
 *   该函数用来以图像中心为中心旋转DIB图像，返回新生成DIB的句柄。
 * 调用该函数会自动扩大图像以显示所有的象素。函数中采用最邻近插
 * 值算法进行插值。
 *
 ************************************************************************/

HGLOBAL WINAPI RotateDIB(LPSTR lpDIB, int iRotateAngle)
{
	
	// 源图像的宽度和高度
	LONG	lWidth;
	LONG	lHeight;
	
	// 旋转后图像的宽度和高度
	LONG	lNewWidth;
	LONG	lNewHeight;
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 旋转后图像的宽度（lNewWidth'，必须是4的倍数）
	LONG	lNewLineBytes;
	
	// 指向源图像的指针
	LPSTR	lpDIBBits;
	
	// 指向源象素的指针
	LPSTR	lpSrc;
	
	// 旋转后新DIB句柄
	HDIB	hDIB;
	
	// 指向旋转图像对应象素的指针
	LPSTR	lpDst;
	
	// 指向旋转图像的指针
	LPSTR	lpNewDIB;
	LPSTR	lpNewDIBBits;
	
	// 指向BITMAPINFO结构的指针（Win3.0）
	LPBITMAPINFOHEADER lpbmi;
	
	// 指向BITMAPCOREINFO结构的指针
	LPBITMAPCOREHEADER lpbmc;
	
	// 循环变量（象素在新DIB中的坐标）
	LONG	i;
	LONG	j;
	
	// 象素在源DIB中的坐标
	LONG	i0;
	LONG	j0;
	
	// 旋转角度（弧度）
	float	fRotateAngle;
	
	// 旋转角度的正弦和余弦
	float	fSina, fCosa;
	
	// 源图四个角的坐标（以图像中心为坐标系原点）
	float	fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;
	
	// 旋转后四个角的坐标（以图像中心为坐标系原点）
	float	fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
	
	// 两个中间常量
	float	f1,f2;
	
	// 找到源DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 获取图像的"宽度"（4的倍数）
	lWidth = ::DIBWidth(lpDIB);
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 获取图像的高度
	lHeight = ::DIBHeight(lpDIB);
	
	// 将旋转角度从度转换到弧度
	fRotateAngle = (float) RADIAN(iRotateAngle);
	
	// 计算旋转角度的正弦
	fSina = (float) sin((double)fRotateAngle);
	
	// 计算旋转角度的余弦
	fCosa = (float) cos((double)fRotateAngle);
	
	// 计算原图的四个角的坐标（以图像中心为坐标系原点）
	fSrcX1 = (float) (- (lWidth  - 1) / 2);
	fSrcY1 = (float) (  (lHeight - 1) / 2);
	fSrcX2 = (float) (  (lWidth  - 1) / 2);
	fSrcY2 = (float) (  (lHeight - 1) / 2);
	fSrcX3 = (float) (- (lWidth  - 1) / 2);
	fSrcY3 = (float) (- (lHeight - 1) / 2);
	fSrcX4 = (float) (  (lWidth  - 1) / 2);
	fSrcY4 = (float) (- (lHeight - 1) / 2);
	
	// 计算新图四个角的坐标（以图像中心为坐标系原点）
	fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;
	
	// 计算旋转后的图像实际宽度
	lNewWidth  = (LONG) ( max( fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2) ) + 0.5);
	
	// 计算新图像每行的字节数
	lNewLineBytes = WIDTHBYTES(lNewWidth * 8);
	
	// 计算旋转后的图像高度
	lNewHeight = (LONG) ( max( fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2) )  + 0.5);
	
	// 两个常数，这样不用以后每次都计算了
	f1 = (float) (-0.5 * (lNewWidth - 1) * fCosa - 0.5 * (lNewHeight - 1) * fSina
		+ 0.5 * (lWidth  - 1));
	f2 = (float) ( 0.5 * (lNewWidth - 1) * fSina - 0.5 * (lNewHeight - 1) * fCosa
		+ 0.5 * (lHeight - 1));
	
	// 分配内存，以保存新DIB
	hDIB = (HDIB) ::GlobalAlloc(GHND, lNewLineBytes * lNewHeight + *(LPDWORD)lpDIB + ::PaletteSize(lpDIB));
	
	// 判断是否内存分配失败
	if (hDIB == NULL)
	{
		// 分配内存失败
		return NULL;
	}

	// 锁定内存
	lpNewDIB =  (char * )::GlobalLock((HGLOBAL) hDIB);
	
	// 复制DIB信息头和调色板
	memcpy(lpNewDIB, lpDIB, *(LPDWORD)lpDIB + ::PaletteSize(lpDIB));
	
	// 找到新DIB象素起始位置
	lpNewDIBBits = ::FindDIBBits(lpNewDIB);
	
	// 获取指针
	lpbmi = (LPBITMAPINFOHEADER)lpNewDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpNewDIB;

	// 更新DIB中图像的高度和宽度
	if (IS_WIN30_DIB(lpNewDIB))
	{
		// 对于Windows 3.0 DIB
		lpbmi->biWidth = lNewWidth;
		lpbmi->biHeight = lNewHeight;
	}
	else
	{
		// 对于其它格式的DIB
		lpbmc->bcWidth = (unsigned short) lNewWidth;
		lpbmc->bcHeight = (unsigned short) lNewHeight;
	}
	
	// 针对图像每行进行操作
	for(i = 0; i < lNewHeight; i++)
	{
		// 针对图像每列进行操作
		for(j = 0; j < lNewWidth; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			// 注意此处宽度和高度是新DIB的宽度和高度
			lpDst = (char *)lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;
			
			// 计算该象素在源DIB中的坐标
			i0 = (LONG) (-((float) j) * fSina + ((float) i) * fCosa + f2 + 0.5);
			j0 = (LONG) ( ((float) j) * fCosa + ((float) i) * fSina + f1 + 0.5);
			
			// 判断是否在源图范围内
			if( (j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
			{
				// 指向源DIB第i0行，第j0个象素的指针
				lpSrc = (char *)lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;
				
				// 复制象素
				*lpDst = *lpSrc;
			}
			else
			{
				// 对于源图中没有的象素，直接赋值为255
				* ((unsigned char*)lpDst) = 255;
			}
			
		}
		
	}
	
	// 返回
	return hDIB;
}


/*************************************************************************
 *
 * 函数名称：
 *   RotateDIB2()
 *
 * 参数:
 *   LPSTR lpDIB		- 指向源DIB的指针
 *   int iRotateAngle	- 旋转的角度（0-360度）
 *
 * 返回值:
 *   HGLOBAL            - 旋转成功返回新DIB句柄，否则返回NULL。
 *
 * 说明:
 *   该函数用来以图像中心为中心旋转DIB图像，返回新生成DIB的句柄。
 * 调用该函数会自动扩大图像以显示所有的象素。函数中采用双线性插
 * 值算法进行插值。
 *
 ************************************************************************/

HGLOBAL WINAPI RotateDIB2(LPSTR lpDIB, int iRotateAngle)
{
	
	// 源图像的宽度和高度
	LONG	lWidth;
	LONG	lHeight;
	
	// 旋转后图像的宽度和高度
	LONG	lNewWidth;
	LONG	lNewHeight;
	
	// 旋转后图像的宽度（lNewWidth'，必须是4的倍数）
	LONG	lNewLineBytes;
	
	// 指向源图像的指针
	LPSTR	lpDIBBits;
	
	// 旋转后新DIB句柄
	HDIB	hDIB;
	
	// 指向旋转图像对应象素的指针
	LPSTR	lpDst;
	
	// 指向旋转图像的指针
	LPSTR	lpNewDIB;
	LPSTR	lpNewDIBBits;
	
	// 指向BITMAPINFO结构的指针（Win3.0）
	LPBITMAPINFOHEADER lpbmi;
	
	// 指向BITMAPCOREINFO结构的指针
	LPBITMAPCOREHEADER lpbmc;
	
	// 循环变量（象素在新DIB中的坐标）
	LONG	i;
	LONG	j;
	
	// 象素在源DIB中的坐标
	FLOAT	i0;
	FLOAT	j0;
	
	// 旋转角度（弧度）
	float	fRotateAngle;
	
	// 旋转角度的正弦和余弦
	float	fSina, fCosa;
	
	// 源图四个角的坐标（以图像中心为坐标系原点）
	float	fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;
	
	// 旋转后四个角的坐标（以图像中心为坐标系原点）
	float	fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
	
	// 两个中间常量
	float	f1,f2;
	
	// 找到源DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 获取图像的宽度
	lWidth = ::DIBWidth(lpDIB);
	
	// 获取图像的高度
	lHeight = ::DIBHeight(lpDIB);
	
	// 将旋转角度从度转换到弧度
	fRotateAngle = (float) RADIAN(iRotateAngle);
	
	// 计算旋转角度的正弦
	fSina = (float) sin((double)fRotateAngle);
	
	// 计算旋转角度的余弦
	fCosa = (float) cos((double)fRotateAngle);
	
	// 计算原图的四个角的坐标（以图像中心为坐标系原点）
	fSrcX1 = (float) (- (lWidth  - 1) / 2);
	fSrcY1 = (float) (  (lHeight - 1) / 2);
	fSrcX2 = (float) (  (lWidth  - 1) / 2);
	fSrcY2 = (float) (  (lHeight - 1) / 2);
	fSrcX3 = (float) (- (lWidth  - 1) / 2);
	fSrcY3 = (float) (- (lHeight - 1) / 2);
	fSrcX4 = (float) (  (lWidth  - 1) / 2);
	fSrcY4 = (float) (- (lHeight - 1) / 2);
	
	// 计算新图四个角的坐标（以图像中心为坐标系原点）
	fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;
	
	// 计算旋转后的图像实际宽度
	lNewWidth  = (LONG) ( max( fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2) ) + 0.5);
	lNewLineBytes = WIDTHBYTES(lNewWidth * 8);
	
	// 计算旋转后的图像高度
	lNewHeight = (LONG) ( max( fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2) )  + 0.5);
	
	// 两个常数，这样不用以后每次都计算了
	f1 = (float) (-0.5 * (lNewWidth - 1) * fCosa - 0.5 * (lNewHeight - 1) * fSina
		+ 0.5 * (lWidth  - 1));
	f2 = (float) ( 0.5 * (lNewWidth - 1) * fSina - 0.5 * (lNewHeight - 1) * fCosa
		+ 0.5 * (lHeight - 1));
	
	// 分配内存，以保存新DIB
	hDIB = (HDIB) ::GlobalAlloc(GHND, lNewLineBytes * lNewHeight + *(LPDWORD)lpDIB + ::PaletteSize(lpDIB));
	
	// 判断是否内存分配失败
	if (hDIB == NULL)
	{
		// 分配内存失败
		return NULL;
	}

	// 锁定内存
	lpNewDIB =  (char * )::GlobalLock((HGLOBAL) hDIB);
	
	// 复制DIB信息头和调色板
	memcpy(lpNewDIB, lpDIB, *(LPDWORD)lpDIB + ::PaletteSize(lpDIB));
	
	// 找到新DIB象素起始位置
	lpNewDIBBits = ::FindDIBBits(lpNewDIB);
	
	// 获取指针
	lpbmi = (LPBITMAPINFOHEADER)lpNewDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpNewDIB;

	// 更新DIB中图像的高度和宽度
	if (IS_WIN30_DIB(lpNewDIB))
	{
		// 对于Windows 3.0 DIB
		lpbmi->biWidth = lNewWidth;
		lpbmi->biHeight = lNewHeight;
	}
	else
	{
		// 对于其它格式的DIB
		lpbmc->bcWidth = (unsigned short) lNewWidth;
		lpbmc->bcHeight = (unsigned short) lNewHeight;
	}
	
	// 针对图像每行进行操作
	for(i = 0; i < lNewHeight; i++)
	{
		// 针对图像每列进行操作
		for(j = 0; j < lNewWidth; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			// 注意此处宽度和高度是新DIB的宽度和高度
			lpDst = (char *)lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;
			
			// 计算该象素在源DIB中的坐标
			i0 = -((float) j) * fSina + ((float) i) * fCosa + f2;
			j0 =  ((float) j) * fCosa + ((float) i) * fSina + f1;
			
			// 利用双线性插值算法来估算象素值
			*lpDst = Interpolation (lpDIBBits, lWidth, lHeight, j0, i0);
			
		}
		
	}
	
	// 返回
	return hDIB;
}


/*************************************************************************
 *
 * 函数名称：
 *   Interpolation()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *   FLOAT x			- 插值元素的x坐标
 *   FLOAT y		    - 插值元素的y坐标
 *
 * 返回值:
 *   unsigned char      - 返回插值计算结果。
 *
 * 说明:
 *   该函数利用双线性插值算法来估算象素值。对于超出图像范围的象素，
 * 直接返回255。
 *
 ************************************************************************/

unsigned char WINAPI Interpolation (LPSTR lpDIBBits, LONG lWidth, LONG lHeight, FLOAT x, FLOAT y)
{
	
	// 四个最临近象素的坐标(i1, j1), (i2, j1), (i1, j2), (i2, j2)
	LONG	i1, i2;
	LONG	j1, j2;
	
	// 四个最临近象素值
	unsigned char	f1, f2, f3, f4;
	
	// 二个插值中间值
	unsigned char	f12, f34;
	
	// 定义一个值，当象素坐标相差小于改值时认为坐标相同
	FLOAT			EXP;
	
	// 图像每行的字节数
	LONG lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 赋值
	EXP = (FLOAT) 0.0001;
	
	// 计算四个最临近象素的坐标
	i1 = (LONG) x;
	i2 = i1 + 1;
	j1 = (LONG) y;
	j2 = j1 + 1;
	
	// 根据不同情况分别处理
	if( (x < 0) || (x > lWidth - 1) || (y < 0) || (y > lHeight - 1))
	{
		// 要计算的点不在源图范围内，直接返回255。
		return 255;
	}
	else
	{
		if (fabs(x - lWidth + 1) <= EXP)
		{
			// 要计算的点在图像右边缘上
			if (fabs(y - lHeight + 1) <= EXP)
			{
				// 要计算的点正好是图像最右下角那一个象素，直接返回该点象素值
				f1 = *((unsigned char *)lpDIBBits + lLineBytes * (lHeight - 1 - j1) + i1);
				return f1;
			}
			else
			{
				// 在图像右边缘上且不是最后一点，直接一次插值即可
				f1 = *((unsigned char *)lpDIBBits + lLineBytes * (lHeight - 1 - j1) + i1);
				f3 = *((unsigned char *)lpDIBBits + lLineBytes * (lHeight - 1 - j1) + i2);
				
				// 返回插值结果
				return ((unsigned char) (f1 + (y -j1) * (f3 - f1)));
			}
		}
		else if (fabs(y - lHeight + 1) <= EXP)
		{
			// 要计算的点在图像下边缘上且不是最后一点，直接一次插值即可
			f1 = *((unsigned char *)lpDIBBits + lLineBytes * (lHeight - 1 - j1) + i1);
			f2 = *((unsigned char *)lpDIBBits + lLineBytes * (lHeight - 1 - j2) + i1);
			
			// 返回插值结果
			return ((unsigned char) (f1 + (x -i1) * (f2 - f1)));
		}
		else
		{
			// 计算四个最临近象素值
			f1 = *((unsigned char *)lpDIBBits + lLineBytes * (lHeight - 1 - j1) + i1);
			f2 = *((unsigned char *)lpDIBBits + lLineBytes * (lHeight - 1 - j2) + i1);
			f3 = *((unsigned char *)lpDIBBits + lLineBytes * (lHeight - 1 - j1) + i2);
			f4 = *((unsigned char *)lpDIBBits + lLineBytes * (lHeight - 1 - j2) + i2);
			
			// 插值1
			f12 = (unsigned char) (f1 + (x - i1) * (f2 - f1));
			
			// 插值2
			f34 = (unsigned char) (f3 + (x - i1) * (f4 - f3));
			
			// 插值3
			return ((unsigned char) (f12 + (y -j1) * (f34 - f12)));
		}
	}
}