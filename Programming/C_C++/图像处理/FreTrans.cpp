
/**************************************************************************
 *  文件名：FreTrans.cpp
 *
 *  正交变换API函数库：
 *
 *  FFT()				- 快速付立叶变换
 *  IFFT()				- 快速付立叶反变换
 *  DCT()				- 离散余弦变换
 *  WALSH()				- 沃尔什－哈达玛变换
 *
 *  Fourier()			- 图像的付立叶变换
 *  DIBDct()			- 图像的离散余弦变换
 *  DIBWalsh()			- 图像的沃尔什－哈达玛变换
 *
 *************************************************************************/

#include "FreTrans.h"
#include "stdafx.h"
#include "DIBAPI.h"

#include <math.h>
#include <direct.h>
#include <complex>
using namespace std;

// 常数π
#define PI 3.1415926535

/*************************************************************************
 *
 * 函数名称：
 *   FFT()
 *
 * 参数:
 *   complex<double> * TD	- 指向时域数组的指针
 *   complex<double> * FD	- 指向频域数组的指针
 *   r						－2的幂数，即迭代次数
 *
 * 返回值:
 *   无。
 *
 * 说明:
 *   该函数用来实现快速付立叶变换。
 *
 ************************************************************************/
VOID WINAPI FFT(complex<double> * TD, complex<double> * FD, int r)
{
	// 付立叶变换点数
	LONG	count;
	
	// 循环变量
	int		i,j,k;
	
	// 中间变量
	int		bfsize,p;
	
	// 角度
	double	angle;
	
	complex<double> *W,*X1,*X2,*X;
	
	// 计算付立叶变换点数
	count = 1 << r;
	
	// 分配运算所需存储器
	W  = new complex<double>[count / 2];
	X1 = new complex<double>[count];
	X2 = new complex<double>[count];
	
	// 计算加权系数
	for(i = 0; i < count / 2; i++)
	{
		angle = -i * PI * 2 / count;
		W[i] = complex<double> (cos(angle), sin(angle));
	}
	
	// 将时域点写入X1
	memcpy(X1, TD, sizeof(complex<double>) * count);
	
	// 采用蝶形算法进行快速付立叶变换
	for(k = 0; k < r; k++)
	{
		for(j = 0; j < 1 << k; j++)
		{
			bfsize = 1 << (r-k);
			for(i = 0; i < bfsize / 2; i++)
			{
				p = j * bfsize;
				X2[i + p] = X1[i + p] + X1[i + p + bfsize / 2];
				X2[i + p + bfsize / 2] = (X1[i + p] - X1[i + p + bfsize / 2]) * W[i * (1<<k)];
			}
		}
		X  = X1;
		X1 = X2;
		X2 = X;
	}
	
	// 重新排序
	for(j = 0; j < count; j++)
	{
		p = 0;
		for(i = 0; i < r; i++)
		{
			if (j&(1<<i))
			{
				p+=1<<(r-i-1);
			}
		}
		FD[j]=X1[p];
	}
	
	// 释放内存
	delete W;
	delete X1;
	delete X2;
}

/*************************************************************************
 *
 * 函数名称：
 *   IFFT()
 *
 * 参数:
 *   complex<double> * FD	- 指向频域值的指针
 *   complex<double> * TD	- 指向时域值的指针
 *   r						－2的幂数
 *
 * 返回值:
 *   无。
 *
 * 说明:
 *   该函数用来实现快速付立叶反变换。
 *
 ************************************************************************/
VOID WINAPI IFFT(complex<double> * FD, complex<double> * TD, int r)
{
	// 付立叶变换点数
	LONG	count;
	
	// 循环变量
	int		i;
	
	complex<double> *X;
	
	// 计算付立叶变换点数
	count = 1 << r;
	
	// 分配运算所需存储器
	X = new complex<double>[count];
	
	// 将频域点写入X
	memcpy(X, FD, sizeof(complex<double>) * count);
	
	// 求共轭
	for(i = 0; i < count; i++)
	{
		X[i] = complex<double> (X[i].real(), -X[i].imag());
	}
	
	// 调用快速付立叶变换
	FFT(X, TD, r);
	
	// 求时域点的共轭
	for(i = 0; i < count; i++)
	{
		TD[i] = complex<double> (TD[i].real() / count, -TD[i].imag() / count);
	}
	
	// 释放内存
	delete X;
}

/*************************************************************************
 *
 * 函数名称：
 *   Fourier()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对图像进行付立叶变换。
 *
 ************************************************************************/
BOOL WINAPI Fourier(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	
	// 指向源图像的指针
	unsigned char*	lpSrc;
	
	// 中间变量
	double	dTemp;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 进行付立叶变换的宽度和高度（2的整数次方）
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 赋初值
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// 计算进行付立叶变换的宽度和高度（2的整数次方）
	while(w * 2 <= lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h * 2 <= lHeight)
	{
		h *= 2;
		hp++;
	}
	
	// 分配内存
	complex<double> *TD = new complex<double>[w * h];
	complex<double> *FD = new complex<double>[w * h];
	
	// 行
	for(i = 0; i < h; i++)
	{
		// 列
		for(j = 0; j < w; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 给时域赋值
			TD[j + w * i] = complex<double>(*(lpSrc), 0);
		}
	}
	
	for(i = 0; i < h; i++)
	{
		// 对y方向进行快速付立叶变换
		FFT(&TD[w * i], &FD[w * i], wp);
	}
	
	// 保存变换结果
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			TD[i + h * j] = FD[j + w * i];
		}
	}
	
	for(i = 0; i < w; i++)
	{
		// 对x方向进行快速付立叶变换
		FFT(&TD[i * h], &FD[i * h], hp);
	}
	
	// 行
	for(i = 0; i < h; i++)
	{
		// 列
		for(j = 0; j < w; j++)
		{
			// 计算频谱
			dTemp = sqrt(FD[j * h + i].real() * FD[j * h + i].real() + 
				         FD[j * h + i].imag() * FD[j * h + i].imag()) / 100;
			
			// 判断是否超过255
			if (dTemp > 255)
			{
				// 对于超过的，直接设置为255
				dTemp = 255;
			}
			
			// 指向DIB第(i<h/2 ? i+h/2 : i-h/2)行，第(j<w/2 ? j+w/2 : j-w/2)个象素的指针
			// 此处不直接取i和j，是为了将变换后的原点移到中心
			//lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * 
				(lHeight - 1 - (i<h/2 ? i+h/2 : i-h/2)) + (j<w/2 ? j+w/2 : j-w/2);
			
			// 更新源图像
			* (lpSrc) = (BYTE)(dTemp);
		}
	}
	
	// 删除临时变量
	delete TD;
	delete FD;
	
	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   DCT()
 *
 * 参数:
 *   double * f				- 指向时域值的指针
 *   double * F				- 指向频域值的指针
 *   r						－2的幂数
 *
 * 返回值:
 *   无。
 *
 * 说明:
 *   该函数用来实现快速离散余弦变换。该函数利用2N点的快速付立叶变换
 * 来实现离散余弦变换。
 *
 ************************************************************************/
VOID WINAPI DCT(double *f, double *F, int r)
{
	// 离散余弦变换点数
	LONG	count;
	
	// 循环变量
	int		i;
	
	// 中间变量
	double	dTemp;
	
	complex<double> *X;
	
	// 计算离散余弦变换点数
	count = 1<<r;
	
	// 分配内存
	X = new complex<double>[count*2];
	
	// 赋初值为0
	memset(X, 0, sizeof(complex<double>) * count * 2);
	
	// 将时域点写入数组X
	for(i=0;i<count;i++)
	{
		X[i] = complex<double> (f[i], 0);
	}
	
	// 调用快速付立叶变换
	FFT(X,X,r+1);
	
	// 调整系数
	dTemp = 1/sqrt(count);
	
	// 求F[0]
	F[0] = X[0].real() * dTemp;
	
	dTemp *= sqrt(2);
	
	// 求F[u]	
	for(i = 1; i < count; i++)
	{
		F[i]=(X[i].real() * cos(i*PI/(count*2)) + X[i].imag() * sin(i*PI/(count*2))) * dTemp;
	}
	
	// 释放内存
	delete X;
}

/*************************************************************************
 *
 * 函数名称：
 *   IDCT()
 *
 * 参数:
 *   double * F				- 指向频域值的指针
 *   double * f				- 指向时域值的指针
 *   r						－2的幂数
 *
 * 返回值:
 *   无。
 *
 * 说明:
 *   该函数用来实现快速离散余弦反变换。该函数也利用2N点的快速付立叶变换
 * 来实现离散余弦反变换。
 *
 ************************************************************************/
VOID WINAPI IDCT(double *F, double *f, int r)
{
	// 离散余弦反变换点数
	LONG	count;
	
	// 循环变量
	int		i;
	
	// 中间变量
	double	dTemp, d0;
	
	complex<double> *X;
	
	// 计算离散余弦变换点数
	count = 1<<r;
	
	// 分配内存
	X = new complex<double>[count*2];
	
	// 赋初值为0
	memset(X, 0, sizeof(complex<double>) * count * 2);
	
	// 将频域变换后点写入数组X
	for(i=0;i<count;i++)
	{
		X[i] = complex<double> (F[i] * cos(i*PI/(count*2)), F[i] * sin(i*PI/(count*2)));
	}
	
	// 调用快速付立叶反变换
	IFFT(X,X,r+1);
	
	// 调整系数
	dTemp = sqrt(2.0/count);
	d0 = (sqrt(1.0/count) - dTemp) * F[0];
	
	// 计算f(x)
	for(i = 0; i < count; i++)
	{
		f[i] = d0 + X[i].real()* dTemp * 2 * count;
	}
	
	// 释放内存
	delete X;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBDct()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对图像进行离散余弦变换。
 *
 ************************************************************************/
BOOL WINAPI DIBDct(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	
	// 指向源图像的指针
	unsigned char*	lpSrc;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 进行付立叶变换的宽度和高度（2的整数次方）
	LONG	w;
	LONG	h;
	
	// 中间变量
	double	dTemp;
	
	int		wp;
	int		hp;
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 赋初值
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// 计算进行离散余弦变换的宽度和高度（2的整数次方）
	while(w * 2 <= lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h * 2 <= lHeight)
	{
		h *= 2;
		hp++;
	}
	
	// 分配内存
	double *f = new double[w * h];
	double *F = new double[w * h];
	
	// 行
	for(i = 0; i < h; i++)
	{
		// 列
		for(j = 0; j < w; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 给时域赋值
			f[j + i * w] = *(lpSrc);
		}
	}
	
	for(i = 0; i < h; i++)
	{
		// 对y方向进行离散余弦变换
		DCT(&f[w * i], &F[w * i], wp);
	}
	
	// 保存计算结果
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			f[j * h + i] = F[j + w * i];
		}
	}
	
	for(j = 0; j < w; j++)
	{
		// 对x方向进行离散余弦变换
		DCT(&f[j * h], &F[j * h], hp);
	}
	
	// 行
	for(i = 0; i < h; i++)
	{
		// 列
		for(j = 0; j < w; j++)
		{
			// 计算频谱
			dTemp = fabs(F[j*h+i]);
			
			// 判断是否超过255
			if (dTemp > 255)
			{
				// 对于超过的，直接设置为255
				dTemp = 255;
			}
			
			// 指向DIB第y行，第x个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 更新源图像
			* (lpSrc) = (BYTE)(dTemp);
		}
	}
	
	// 释放内存
	delete f;
	delete F;

	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   WALSH()
 *
 * 参数:
 *   double * f				- 指向时域值的指针
 *   double * F				- 指向频域值的指针
 *   r						－2的幂数
 *
 * 返回值:
 *   无。
 *
 * 说明:
 *   该函数用来实现快速沃尔什-哈达玛变换。
 *
 ************************************************************************/

VOID WINAPI WALSH(double *f, double *F, int r)
{
	// 沃尔什-哈达玛变换点数
	LONG	count;
	
	// 循环变量
	int		i,j,k;
	
	// 中间变量
	int		bfsize,p;
	
	double *X1,*X2,*X;
	
	// 计算快速沃尔什变换点数
	count = 1 << r;
	
	// 分配运算所需的数组
	X1 = new double[count];
	X2 = new double[count];
	
	// 将时域点写入数组X1
	memcpy(X1, f, sizeof(double) * count);
	
	// 蝶形运算
	for(k = 0; k < r; k++)
	{
		for(j = 0; j < 1<<k; j++)
		{
			bfsize = 1 << (r-k);
			for(i = 0; i < bfsize / 2; i++)
			{
				p = j * bfsize;
				X2[i + p] = X1[i + p] + X1[i + p + bfsize / 2];
				X2[i + p + bfsize / 2] = X1[i + p] - X1[i + p + bfsize / 2];
			}
		}
		
		// 互换X1和X2  
		X = X1;
		X1 = X2;
		X2 = X;
	}
	
	// 调整系数
	for(j = 0; j < count; j++)
	{
		p = 0;
		for(i = 0; i < r; i++)
		{
			if (j & (1<<i))
			{
				p += 1 << (r-i-1);
			}
		}

		F[j] = X1[p] / count;
	}
	
	// 释放内存
	delete X1;
	delete X2;
}

/*************************************************************************
 *
 * 函数名称：
 *   IWALSH()
 *
 * 参数:
 *   double * F				- 指向频域值的指针
 *   double * f				- 指向时域值的指针
 *   r						－2的幂数
 *
 * 返回值:
 *   无。
 *
 * 说明:
 *   该函数用来实现快速沃尔什-哈达玛反变换。
 *
 ************************************************************************/

VOID WINAPI IWALSH(double *F, double *f, int r)
{
	// 变换点数
	LONG	count;
	
	// 循环变量
	int		i;
	
	// 计算变换点数
	count = 1 << r;
	
	// 调用快速沃尔什－哈达玛变换进行反变换
	WALSH(F, f, r);
	
	// 调整系数
	for(i = 0; i < count; i++)
	{
		f[i] *= count;
	}
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBWalsh()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对图像进行沃尔什-哈达玛变换。函数首先对图像每列进行一维
 * 沃尔什－哈达玛变换，然后对变换结果的每行进行一维沃尔什－哈达玛变换。
 *
 ************************************************************************/

BOOL WINAPI DIBWalsh(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	
	// 指向源图像的指针
	unsigned char*	lpSrc;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 进行付立叶变换的宽度和高度（2的整数次方）
	LONG	w;
	LONG	h;
	
	// 中间变量
	double	dTemp;
	
	int		wp;
	int		hp;
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 赋初值
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// 计算进行离散余弦变换的宽度和高度（2的整数次方）
	while(w * 2 <= lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h * 2 <= lHeight)
	{
		h *= 2;
		hp++;
	}
	
	// 分配内存
	double *f = new double[w * h];
	double *F = new double[w * h];
	
	// 行
	for(i = 0; i < h; i++)
	{
		// 列
		for(j = 0; j < w; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 给时域赋值
			f[j + i * w] = *(lpSrc);
		}
	}
	
	for(i = 0; i < h; i++)
	{
		// 对y方向进行沃尔什-哈达玛变换
		WALSH(f + w * i, F + w * i, wp);
	}
	
	// 保存计算结果
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			f[j * h + i] = F[j + w * i];
		}
	}
	
	for(j = 0; j < w; j++)
	{
		// 对x方向进行沃尔什-哈达玛变换
		WALSH(f + j * h, F + j * h, hp);
	}
	
	// 行
	for(i = 0; i < h; i++)
	{
		// 列
		for(j = 0; j < w; j++)
		{
			// 计算频谱
			dTemp = fabs(F[j * h + i] * 1000);
			
			// 判断是否超过255
			if (dTemp > 255)
			{
				// 对于超过的，直接设置为255
				dTemp = 255;
			}
			
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 更新源图像
			* (lpSrc) = (BYTE)(dTemp);
		}
	}
	
	//释放内存
	delete f;
	delete F;

	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBWalsh1()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对图像进行沃尔什-哈达玛变换。于上面不同的是，此处是将二维
 * 矩阵转换成一个列向量，然后对该列向量进行一次一维沃尔什-哈达玛变换。
 *
 ************************************************************************/

BOOL WINAPI DIBWalsh1(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	
	// 指向源图像的指针
	unsigned char*	lpSrc;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 进行付立叶变换的宽度和高度（2的整数次方）
	LONG	w;
	LONG	h;
	
	// 中间变量
	double	dTemp;
	
	int		wp;
	int		hp;
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 赋初值
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// 计算进行离散余弦变换的宽度和高度（2的整数次方）
	while(w * 2 <= lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h * 2 <= lHeight)
	{
		h *= 2;
		hp++;
	}
	
	// 分配内存
	double *f = new double[w * h];
	double *F = new double[w * h];
	
	// 列
	for(i = 0; i < w; i++)
	{
		// 行
		for(j = 0; j < h; j++)
		{
			// 指向DIB第j行，第i个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;
			
			// 给时域赋值
			f[j + i * w] = *(lpSrc);
		}
	}
	
	// 调用快速沃尔什－哈达玛变换
	WALSH(f, F, wp + hp);
	
	// 列
	for(i = 0; i < w; i++)
	{
		// 行
		for(j = 0; j < h; j++)
		{
			// 计算频谱
			dTemp = fabs(F[i * w + j] * 1000);
			
			// 判断是否超过255
			if (dTemp > 255)
			{
				// 对于超过的，直接设置为255
				dTemp = 255;
			}
			
			// 指向DIB第j行，第i个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;
			
			// 更新源图像
			* (lpSrc) = (BYTE)(dTemp);
		}
	}
	
	//释放内存
	delete f;
	delete F;

	// 返回
	return TRUE;
}
