// DlgShannon.cpp : implementation file
//

#include "stdafx.h"
#include "ch1_1.h"
#include "DlgShannon.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgShannon dialog


CDlgShannon::CDlgShannon(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgShannon::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgShannon)
	m_dEntropy = 0.0;
	m_dAvgCodeLen = 0.0;
	m_dEfficiency = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgShannon::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgShannon)
	DDX_Control(pDX, IDC_LST_Table, m_lstTable);
	DDX_Text(pDX, IDC_EDIT1, m_dEntropy);
	DDX_Text(pDX, IDC_EDIT2, m_dAvgCodeLen);
	DDX_Text(pDX, IDC_EDIT3, m_dEfficiency);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgShannon, CDialog)
	//{{AFX_MSG_MAP(CDlgShannon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgShannon message handlers

BOOL CDlgShannon::OnInitDialog() 
{
	
	// 字符串变量
	CString	str;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 中间变量
	FLOAT	fT;
	LONG	iTemp;
	
	// 保存计算中间结果的数组
	FLOAT *	fTemp;
	
	// 保存映射关系的数组
	LONG *	iMap;
	
	// 当前编码区间的频率和
	FLOAT	fTotal;
	
	// 计数（编码完成的个数）
	LONG	iCount = 0;
	
	// 频率和
	FLOAT	fSum;
	
	// 起始位置
	LONG	iStart;
	
	// 指向布尔型数组的指针
	BOOL	* bFinished;
	
	// 调用默认得OnInitDialog()函数
	CDialog::OnInitDialog();
	
	// 初始化变量
	m_dEntropy = 0.0;
	m_dAvgCodeLen = 0.0;
	
	// 计算图像熵
	for (i = 0; i < m_iColorNum; i ++)
	{
		// 判断概率是否大于0
		if (m_fFreq[i] > 0)
		{
			// 计算图像熵
			m_dEntropy -= m_fFreq[i] * log(m_fFreq[i]) / log(2.0);
		}
	}
	
	// 分配内存
	fTemp = new FLOAT[m_iColorNum];
	m_strCode = new CString[m_iColorNum];
	bFinished = new BOOL[m_iColorNum];
	iMap  = new LONG[m_iColorNum];
	
	fTotal = 0;
	
	// 初始化fTemp为m_fFreq, bFinished为FALSE
	for (i = 0; i < m_iColorNum; i ++)
	{
		// 赋值
		fTemp[i] = m_fFreq[i];
		
		// 初始化映射关系
		iMap[i] = i;
		
		// 初始化为FALSE
		bFinished[i] = FALSE;
		
		// 计算fTotal
		fTotal += m_fFreq[i];
	}
	
	// 用冒泡法对进行灰度值出现的概率排序，结果保存在数组fTemp中
	for (j = 0; j < m_iColorNum - 1; j ++)
	{
		for (i = 0; i < m_iColorNum - j - 1; i ++)
		{
			if (fTemp[i] > fTemp[i + 1])
			{
				// 互换
				fT = fTemp[i];
				fTemp[i] = fTemp[i + 1];
				fTemp[i + 1] = fT;
				
				// 更新映射关系
				iTemp = iMap[i];
				iMap[i] = iMap[i+1];
				iMap[i+1] = iTemp;
				
			}
		}
	}
	
	//////////////////////////////////////////////////////////
	// 计算香农－弗诺编码表
	
	// 找到概率大于0处才开始编码
	for (iStart = 0; iStart < m_iColorNum - 1; iStart ++)
	{
		// 判断概率是否大于0
		if (fTemp[iStart] > 0)
		{
			// 跳出
			break;
		}
	}
	
	// 初始化变量
	fSum = 0;
	str = "1";
	
	// 开始编码
	while(iCount < m_iColorNum)
	{
		// 初始化iCount为iStart
		iCount = iStart;
		
		// 循环编码
		for (i = iStart; i < m_iColorNum; i ++)
		{
			// 判断是否编码完成
			if (bFinished[i] == FALSE)
			{
				// 编码没有完成，继续编码
				
				// fSum加当前出现的频率
				fSum += fTemp[i];
				
				// 判断是否超出总和的一半
				if (fSum > fTotal/2.0)
				{
					// 超出，追加的字符改为0
					str = "0";
				}
				
				// 编码追加字符1或0
				m_strCode[iMap[i]] += str;
				
				// 判断是否编码完一段
				if (fSum == fTotal)
				{
					// 完成一部分编码，重新计算fTotal
					
					// 初始化fSum为0
					fSum = 0;
					
					// 判断是否是最后一个元素
					if (i == m_iColorNum - 1)
					{
						// 是最后，设置从起始点开始
						j = iStart;
					}
					else
					{
						// 不是最后，设置从下一个点开始
						j = i + 1;
					}
					
					// 保存j值
					iTemp = j;
					str = m_strCode[iMap[j]];
					
					// 计算下一段的fTotal
					fTotal = 0;
					for (; j < m_iColorNum; j++)
					{
						// 判断是否是同一段编码
						if ((m_strCode[iMap[j]].Right(1) != str.Right(1)) 
							|| (m_strCode[iMap[j]].GetLength() != str.GetLength()))
						{
							// 退出循环
							break;
						}
						
						// 累加
						fTotal += fTemp[j];
					}
					
					// 初始化str为1
					str = "1";
					
					// 判断是否该段长度为1
					if (iTemp + 1 == j)
					{
						// 是，表示该段编码已经完成
						bFinished[iTemp] = TRUE;
					}
				}
			}
			else
			{
				// iCount加1
				iCount ++;
				
				// 计算下一次循环的fTotal
				
				// 初始化fSum为0
				fSum = 0;
				
				// 判断是否是最后一个元素
				if (i == m_iColorNum - 1)
				{
					// 是最后，设置从起始点开始
					j = iStart;
				}
				else
				{
					// 不是最后，设置从下一个点开始
					j = i + 1;
				}
				
				// 保存j值
				iTemp = j;
				str = m_strCode[iMap[j]];
				
				// 计算下一段的fTotal
				fTotal = 0;
				for (; j < m_iColorNum; j++)
				{
					// 判断是否是同一段编码
					if ((m_strCode[iMap[j]].Right(1) != str.Right(1)) 
						|| (m_strCode[iMap[j]].GetLength() != str.GetLength()))
					{
						// 退出循环
						break;
					}
					
					// 累加
					fTotal += fTemp[j];
				}
				
				// 初始化str为1
				str = "1";
				
				// 判断是否该段长度为1
				if (iTemp + 1 == j)
				{
					// 是，表示该段编码已经完成
					bFinished[iTemp] = TRUE;
				}
			}
		}
	}
	
	// 计算平均码字长度
	for (i = 0; i < m_iColorNum; i ++)
	{
		// 累加
		m_dAvgCodeLen += m_fFreq[i] * m_strCode[i].GetLength();
	}
	
	// 计算编码效率
	m_dEfficiency = m_dEntropy / m_dAvgCodeLen;
	
	// 保存变动
	UpdateData(FALSE);
	
	//////////////////////////////////////////////////////////
	// 输出计算结果
	
	// ListCtrl的ITEM
	LV_ITEM lvitem;
	
	// 中间变量，保存ListCtrl中添加的ITEM编号
	int		iActualItem;
	
	// 设置List控件样式
	m_lstTable.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	
	// 给List控件添加Header
	m_lstTable.InsertColumn(0, "灰度值", LVCFMT_LEFT, 60, 0);
	m_lstTable.InsertColumn(1, "出现频率", LVCFMT_LEFT, 78, 0);
	m_lstTable.InsertColumn(2, "香农弗诺编码", LVCFMT_LEFT, 110, 1);
	m_lstTable.InsertColumn(3, "码字长度", LVCFMT_LEFT, 78, 2);
	
	// 设置样式为文本
	lvitem.mask = LVIF_TEXT;
	
	// 计算平均码字长度
	for (i = 0; i < m_iColorNum; i ++)
	{
		// 添加一项
		lvitem.iItem = m_lstTable.GetItemCount();
		str.Format("%u",i);
		lvitem.iSubItem = 0;
		lvitem.pszText= (LPTSTR)(LPCTSTR)str;
		iActualItem = m_lstTable.InsertItem(&lvitem);
		
		// 添加其它列
		lvitem.iItem = iActualItem;
		
		// 添加灰度值出现的频率
		lvitem.iSubItem = 1;
		str.Format("%f",m_fFreq[i]);
		lvitem.pszText = (LPTSTR)(LPCTSTR)str;
		m_lstTable.SetItem(&lvitem);
		
		// 添加香农弗诺编码
		lvitem.iSubItem = 2;
		lvitem.pszText = (LPTSTR)(LPCTSTR)m_strCode[i];
		m_lstTable.SetItem(&lvitem);
		
		// 添加码字长度
		lvitem.iSubItem = 3;
		str.Format("%u",m_strCode[i].GetLength());
		lvitem.pszText = (LPTSTR)(LPCTSTR)str;
		m_lstTable.SetItem(&lvitem);
	}	
	
	// 返回TRUE
	return TRUE;
}
