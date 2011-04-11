// ************************************************************************
//  文件名：restore.cpp
//
//  图像复原API函数库：
//
//  BlurDIB()			- 图像模糊
//  InverseDIB()	    - 图像逆滤波
//  NoiseBlurDIB()		- 图像模糊加噪
//  WienerDIB()			- 图像维纳滤波
//	RandomNoiseDIB()	- 图像中加入随机噪声
//	SaltNoiseDIB()		- 图像中加入椒盐噪声
//  fourn()				- n维FFT
//
// *************************************************************************

#include "stdafx.h"
#include "restore.h"
#include "DIBAPI.h"

#include <math.h>
#include <direct.h>

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

/*************************************************************************
 *
 * 函数名称：
 *   BlurDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 平移成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对DIB图像进行模糊操作。
 *
 ************************************************************************/

BOOL WINAPI BlurDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	// 指向源图像的指针
	LPSTR	lpSrc;
	
	//循环变量
	long i;
	long j;

	//像素值
	unsigned char pixel;

	// 图像每行的字节数
	LONG lLineBytes;

	//用于做FFT的数组
	double *fftSrc,*fftKernel;
	//二维FFT的长度和宽度
	unsigned long nn[3];
	//图像归一化因子
	double MaxNum;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	double dPower = log((double)lLineBytes)/log(2.0);
	if(dPower != (int) dPower)
	{
		return false;
	}
	dPower = log((double)lHeight)/log(2.0);
	if(dPower != (int) dPower)
	{
		return false;
	}

	fftSrc = new double [lHeight*lLineBytes*2+1];
	fftKernel = new double [lHeight*lLineBytes*2+1];

	nn[1] = lHeight;
	nn[2] = lLineBytes;

	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;
	
			pixel = (unsigned char)*lpSrc;

			fftSrc[(2*lLineBytes)*j + 2*i + 1] = (double)pixel;
			fftSrc[(2*lLineBytes)*j + 2*i + 2] = 0.0;
	
			if(i < 5 && j < 5)
			{
				fftKernel[(2*lLineBytes)*j + 2*i + 1] = 1/25.0;
			}
			else
			{
				fftKernel[(2*lLineBytes)*j + 2*i + 1] = 0.0;
			}
			fftKernel[(2*lLineBytes)*j + 2*i + 2] = 0.0;
		}
	}

	//对源图像进行FFT
	fourn(fftSrc,nn,2,1);
	//对卷积核图像进行FFT
	fourn(fftKernel,nn,2,1);

	//频域相乘
	for (i = 1;i <lHeight*lLineBytes*2;i+=2)
	{
		fftSrc[i] = fftSrc[i] * fftKernel[i] - fftSrc[i+1] * fftKernel[i+1];
		fftSrc[i+1] = fftSrc[i] * fftKernel[i+1] + fftSrc[i+1] * fftKernel[i];
	}

	//对结果图像进行反FFT
	fourn(fftSrc,nn,2,-1);

	//确定归一化因子
	MaxNum = 0.0;
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			fftSrc[(2*lLineBytes)*j + 2*i + 1] = 
				sqrt(fftSrc[(2*lLineBytes)*j + 2*i + 1] * fftSrc[(2*lLineBytes)*j + 2*i + 1]\
						+fftSrc[(2*lLineBytes)*j + 2*i + 2] * fftSrc[(2*lLineBytes)*j + 2*i + 2]);
			if( MaxNum < fftSrc[(2*lLineBytes)*j + 2*i + 1])
				MaxNum = fftSrc[(2*lLineBytes)*j + 2*i + 1];
		}
	}
	
	//转换为图像
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;
	
			*lpSrc = (unsigned char) (fftSrc[(2*lLineBytes)*j + 2*i + 1]*255.0/MaxNum);
		}
	}
	delete fftSrc;
	delete fftKernel;
	// 返回
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   RestoreDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 平移成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对BlurDIB()生成的DIB图像进行复原操作。
 *
 ************************************************************************/

BOOL WINAPI RestoreDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	// 指向源图像的指针
	LPSTR	lpSrc;
	
	//循环变量
	long i;
	long j;

	//像素值
	unsigned char pixel;

	// 图像每行的字节数
	LONG lLineBytes;

	//用于做FFT的数组
	double *fftSrc,*fftKernel;
	double a,b,c,d;
	//二维FFT的长度和宽度
	unsigned long nn[3];
	//图像归一化因子
	double MaxNum;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	double dPower = log((double)lLineBytes)/log(2.0);
	if(dPower != (int) dPower)
	{
		return false;
	}
	dPower = log((double)lHeight)/log(2.0);
	if(dPower != (int) dPower)
	{
		return false;
	}

	fftSrc = new double [lHeight*lLineBytes*2+1];
	fftKernel = new double [lHeight*lLineBytes*2+1];

	nn[1] = lHeight;
	nn[2] = lLineBytes;

	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;
	
			pixel = (unsigned char)*lpSrc;

			fftSrc[(2*lLineBytes)*j + 2*i + 1] = (double)pixel;
			fftSrc[(2*lLineBytes)*j + 2*i + 2] = 0.0;
	
			if(i < 5 && j == 0)
			{
				fftKernel[(2*lLineBytes)*j + 2*i + 1] = 1/5.0;
			}
			else
			{
				fftKernel[(2*lLineBytes)*j + 2*i + 1] = 0.0;
			}
			fftKernel[(2*lLineBytes)*j + 2*i + 2] = 0.0;
		}
	}

	//对源图像进行FFT
	fourn(fftSrc,nn,2,1);
	//对卷积核图像进行FFT
	fourn(fftKernel,nn,2,1);

	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			a = fftSrc[(2*lLineBytes)*j + 2*i + 1];
			b = fftSrc[(2*lLineBytes)*j + 2*i + 2];
			c = fftKernel[(2*lLineBytes)*j + 2*i + 1];
			d = fftKernel[(2*lLineBytes)*j + 2*i + 2];
			if (c*c + d*d > 1e-3)
			{
				fftSrc[(2*lLineBytes)*j + 2*i + 1] = ( a*c + b*d ) / ( c*c + d*d );
				fftSrc[(2*lLineBytes)*j + 2*i + 2] = ( b*c - a*d ) / ( c*c + d*d );
			}
		}
	}

	//对结果图像进行反FFT
	fourn(fftSrc,nn,2,-1);

	//确定归一化因子
	MaxNum = 0.0;
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			fftSrc[(2*lLineBytes)*j + 2*i + 1] = 
				sqrt(fftSrc[(2*lLineBytes)*j + 2*i + 1] * fftSrc[(2*lLineBytes)*j + 2*i + 1]\
						+fftSrc[(2*lLineBytes)*j + 2*i + 2] * fftSrc[(2*lLineBytes)*j + 2*i + 2]);
			if( MaxNum < fftSrc[(2*lLineBytes)*j + 2*i + 1])
				MaxNum = fftSrc[(2*lLineBytes)*j + 2*i + 1];
		}
	}
	

	//转换为图像
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;

			*lpSrc = (unsigned char) (fftSrc[(2*lLineBytes)*j + 2*i + 1]*255.0/MaxNum);
		}
	}
	delete fftSrc;
	delete fftKernel;
	// 返回
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   NoiseBlurDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 模糊加噪操作成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对DIB图像进行模糊加噪操作。
 *
 ************************************************************************/

BOOL WINAPI NoiseBlurDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	// 指向源图像的指针
	LPSTR	lpSrc;
	
	//循环变量
	long i;
	long j;

	//像素值
	unsigned char pixel;

	// 图像每行的字节数
	LONG lLineBytes;

	//用于做FFT的数组
	double *fftSrc,*fftKernel;
	//二维FFT的长度和宽度
	unsigned long nn[3];
	//图像归一化因子
	double MaxNum;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	double dPower = log((double)lLineBytes)/log(2.0);
	if(dPower != (int) dPower)
	{
		return false;
	}
	dPower = log((double)lHeight)/log(2.0);
	if(dPower != (int) dPower)
	{
		return false;
	}

	fftSrc = new double [lHeight*lLineBytes*2+1];
	fftKernel = new double [lHeight*lLineBytes*2+1];

	nn[1] = lHeight;
	nn[2] = lLineBytes;

	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;
	
			pixel = (unsigned char)*lpSrc;

			fftSrc[(2*lLineBytes)*j + 2*i + 1] = (double)pixel;
			fftSrc[(2*lLineBytes)*j + 2*i + 2] = 0.0;
	
			if(i < 5 && j == 0)
			{
				fftKernel[(2*lLineBytes)*j + 2*i + 1] = 1/5.0;
			}
			else
			{
				fftKernel[(2*lLineBytes)*j + 2*i + 1] = 0.0;
			}
			fftKernel[(2*lLineBytes)*j + 2*i + 2] = 0.0;
		}
	}

	//对源图像进行FFT
	fourn(fftSrc,nn,2,1);
	//对卷积核图像进行FFT
	fourn(fftKernel,nn,2,1);

	//频域相乘
	for (i = 1;i <lHeight*lLineBytes*2;i+=2)
	{
		fftSrc[i] = fftSrc[i] * fftKernel[i] - fftSrc[i+1] * fftKernel[i+1];
		fftSrc[i+1] = fftSrc[i] * fftKernel[i+1] + fftSrc[i+1] * fftKernel[i];
	}

	//对结果图像进行反FFT
	fourn(fftSrc,nn,2,-1);

	//确定归一化因子	
	MaxNum = 0.0;
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			fftSrc[(2*lLineBytes)*j + 2*i + 1] = 
				sqrt(fftSrc[(2*lLineBytes)*j + 2*i + 1] * fftSrc[(2*lLineBytes)*j + 2*i + 1]\
						+fftSrc[(2*lLineBytes)*j + 2*i + 2] * fftSrc[(2*lLineBytes)*j + 2*i + 2]);
			if( MaxNum < fftSrc[(2*lLineBytes)*j + 2*i + 1])
				MaxNum = fftSrc[(2*lLineBytes)*j + 2*i + 1];
		}
	}
	
	//转换为图像，加噪
	char point;

	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			if ( i + j == ((int)((i+j)/8))*8)
			{
				point = -16;
			}
			else
			{
				point = 0;
			}

			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;

			*lpSrc = (unsigned char) (fftSrc[(2*lLineBytes)*j + 2*i + 1]*255.0/MaxNum + point);
		}
	}

	delete fftSrc;
	delete fftKernel;
	// 返回
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   WienerDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 维纳滤波复原操作成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对DIB图像进行维纳滤波复原操作。
 *
 ************************************************************************/

BOOL WINAPI WienerDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	// 指向源图像的指针
	LPSTR	lpSrc;
	
	//循环变量
	long i;
	long j;

	//像素值
	unsigned char pixel;

	// 图像每行的字节数
	LONG lLineBytes;

	//用于做FFT的数组
	double *fftSrc,*fftKernel,*fftNoise;
	double a,b,c,d,e,f,multi;
	//二维FFT的长度和宽度
	unsigned long nn[3];
	//图像归一化因子
	double MaxNum;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	double dPower = log((double)lLineBytes)/log(2.0);
	if(dPower != (int) dPower)
	{
		return false;
	}
	dPower = log((double)lHeight)/log(2.0);
	if(dPower != (int) dPower)
	{
		return false;
	}

	fftSrc = new double [lHeight*lLineBytes*2+1];
	fftKernel = new double [lHeight*lLineBytes*2+1];
	fftNoise = new double [lHeight*lLineBytes*2+1];

	nn[1] = lHeight;
	nn[2] = lLineBytes;

	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;
	
			pixel = (unsigned char)*lpSrc;

			fftSrc[(2*lLineBytes)*j + 2*i + 1] = (double)pixel;
			fftSrc[(2*lLineBytes)*j + 2*i + 2] = 0.0;
	
			if(i < 5 && j == 0)
			{
				fftKernel[(2*lLineBytes)*j + 2*i + 1] = 1/5.0;
			}
			else
			{
				fftKernel[(2*lLineBytes)*j + 2*i + 1] = 0.0;
			}
			fftKernel[(2*lLineBytes)*j + 2*i + 2] = 0.0;
			if ( i + j == ((int)((i+j)/8))*8)
			{
				fftNoise [(2*lLineBytes)*j + 2*i + 1]= -16.0;
			}
			else
			{
				fftNoise [(2*lLineBytes)*j + 2*i + 1]= 0.0;
			}
			fftNoise[(2*lLineBytes)*j + 2*i + 2] = 0.0;

		}
	}

	srand((unsigned)time(NULL));
	
	//对源图像进行FFT
	fourn(fftSrc,nn,2,1);
	//对卷积核图像进行FFT
	fourn(fftKernel,nn,2,1);
	//对噪声图像进行FFT
	fourn(fftNoise,nn,2,1);

	for (i = 1;i <lHeight*lLineBytes*2;i+=2)
	{
			a = fftSrc[i];
			b = fftSrc[i+1];
			c = fftKernel[i];
			d = fftKernel[i+1];
			e = fftNoise[i];
			f = fftNoise[i+1];
			multi = (a*a + b*b)/(a*a + b*b - e*e - f*f);
			if (c*c + d*d > 1e-3)
			{
				fftSrc[i] = ( a*c + b*d ) / ( c*c + d*d ) / multi;
				fftSrc[i+1] = ( b*c - a*d ) / ( c*c + d*d )/multi;
			}
	}

	//对结果图像进行反FFT
	fourn(fftSrc,nn,2,-1);

	//确定归一化因子
	MaxNum = 0.0;
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			fftSrc[(2*lLineBytes)*j + 2*i + 1] = 
				sqrt(fftSrc[(2*lLineBytes)*j + 2*i + 1] * fftSrc[(2*lLineBytes)*j + 2*i + 1]\
						+fftSrc[(2*lLineBytes)*j + 2*i + 2] * fftSrc[(2*lLineBytes)*j + 2*i + 2]);
			if( MaxNum < fftSrc[(2*lLineBytes)*j + 2*i + 1])
				MaxNum = fftSrc[(2*lLineBytes)*j + 2*i + 1];
		}
	}
	
	//转换为图像
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;

			*lpSrc = (unsigned char) (fftSrc[(2*lLineBytes)*j + 2*i + 1]*255.0/MaxNum );
		}
	}

	delete fftSrc;
	delete fftKernel;
	delete fftNoise;
	// 返回
	return true;
}


BOOL fourn(double * data, unsigned long nn[], int ndim, int isign)
{
	int idim;
	unsigned long i1,i2,i3,i2rev,i3rev,ip1,ip2,ip3,ifp1,ifp2;
	unsigned long ibit,k1,k2,n,nprev,nrem,ntot;
	double tempi,tempr;
	double theta,wi,wpi,wpr,wr,wtemp;

	for (ntot=1,idim=1;idim<=ndim;idim++)
		ntot *= nn[idim];
	nprev=1;
	for (idim=ndim;idim>=1;idim--) {
		n=nn[idim];
		nrem=ntot/(n*nprev);
		ip1=nprev << 1;
		ip2=ip1*n;
		ip3=ip2*nrem;
		i2rev=1;
		for (i2=1;i2<=ip2;i2+=ip1) {
			if (i2 < i2rev) {
				for (i1=i2;i1<=i2+ip1-2;i1+=2) {
					for (i3=i1;i3<=ip3;i3+=ip2) {
						i3rev=i2rev+i3-i2;
						SWAP(data[i3],data[i3rev]);
						SWAP(data[i3+1],data[i3rev+1]);
					}
				}
			}
			ibit=ip2 >> 1;
			while (ibit >= ip1 && i2rev > ibit) {
				i2rev -= ibit;
				ibit >>= 1;
			}
			i2rev += ibit;
		}
		ifp1=ip1;
		while (ifp1 < ip2) {
			ifp2=ifp1 << 1;
			theta=isign*6.28318530717959/(ifp2/ip1);
			wtemp=sin(0.5*theta);
			wpr = -2.0*wtemp*wtemp;
			wpi=sin(theta);
			wr=1.0;
			wi=0.0;
			for (i3=1;i3<=ifp1;i3+=ip1) {
				for (i1=i3;i1<=i3+ip1-2;i1+=2) {
					for (i2=i1;i2<=ip3;i2+=ifp2) {
						k1=i2;
						k2=k1+ifp1;
						tempr=wr*data[k2]-wi*data[k2+1];
						tempi=wr*data[k2+1]+wi*data[k2];
						data[k2]=data[k1]-tempr;
						data[k2+1]=data[k1+1]-tempi;
						data[k1] += tempr;
						data[k1+1] += tempi;
					}
				}
				wr=(wtemp=wr)*wpr-wi*wpi+wr;
				wi=wi*wpr+wtemp*wpi+wi;
			}
			ifp1=ifp2;
		}
		nprev *= n;
	}
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   RandomNoiseDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 模糊操作成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对DIB图像进行模糊操作。
 *
 ************************************************************************/

BOOL WINAPI RandomNoiseDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	// 指向源图像的指针
	LPSTR	lpSrc;
	
	//循环变量
	long i;
	long j;

	// 图像每行的字节数
	LONG lLineBytes;

	//像素值
	unsigned char pixel;

	//噪声
	BYTE NoisePoint;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	//生成伪随机种子
	srand((unsigned)time(NULL));

	//在图像中加噪
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			NoisePoint=rand()/1024;

			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;
			
			//取得像素值
			pixel = (unsigned char)*lpSrc;

			*lpSrc = (unsigned char)(pixel*224/256 + NoisePoint);
		}
	}
	// 返回
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   SaltNoiseDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 模糊操作成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对DIB图像进行模糊操作。
 *
 ************************************************************************/

BOOL WINAPI SaltNoiseDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	// 指向源图像的指针
	LPSTR	lpSrc;
	
	//循环变量
	long i;
	long j;

	// 图像每行的字节数
	LONG lLineBytes;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	//生成伪随机种子
	srand((unsigned)time(NULL));

	//在图像中加噪
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			if(rand()>31500)
			{
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char *)lpDIBBits + lLineBytes * j + i;
				
				//图像中当前点置为黑
				*lpSrc = 0;
			}
		}
	}
	// 返回
	return true;
}

#undef SWAP
