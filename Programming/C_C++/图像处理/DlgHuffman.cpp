// DlgHuffman.cpp : implementation file
//

#include "stdafx.h"
#include "ch1_1.h"
#include "DlgHuffman.h"
#include "DIBAPI.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHuffman dialog


CDlgHuffman::CDlgHuffman(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHuffman::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgHuffman)
	m_dEntropy = 0.0;
	m_dAvgCodeLen = 0.0;
	m_dEfficiency = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgHuffman::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHuffman)
	DDX_Control(pDX, IDC_LST_Table, m_lstTable);
	DDX_Text(pDX, IDC_EDIT1, m_dEntropy);
	DDX_Text(pDX, IDC_EDIT2, m_dAvgCodeLen);
	DDX_Text(pDX, IDC_EDIT3, m_dEfficiency);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHuffman, CDialog)
	//{{AFX_MSG_MAP(CDlgHuffman)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHuffman message handlers

BOOL CDlgHuffman::OnInitDialog() 
{
	
	// 字符串变量
	CString	str;
	
	// 循环变量
	LONG	i;
	LONG	j;
	LONG	k;
	
	// 中间变量
	FLOAT	fT;
	
	// ListCtrl的ITEM
	LV_ITEM lvitem;
	
	// 中间变量，保存ListCtrl中添加的ITEM编号
	int		iActualItem;
	
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
	
	// 保存计算中间结果的数组
	FLOAT *	fTemp;
	
	// 保存映射关系的数组
	int	*	iMap;
	
	// 分配内存
	fTemp = new FLOAT[m_iColorNum];
	iMap  = new int[m_iColorNum];
	m_strCode = new CString[m_iColorNum];
	
	// 初始化fTemp为m_fFreq
	for (i = 0; i < m_iColorNum; i ++)
	{
		// 赋值
		fTemp[i] = m_fFreq[i];
		iMap[i] = i;
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
				for (k = 0; k < m_iColorNum; k ++)
				{
					// 判断是否是fTemp[i]的子节点
					if (iMap[k] == i)
					{
						// 改变映射到节点i+1
						iMap[k] = i + 1;
					}
					else if (iMap[k] == i + 1)
					{
						// 改变映射到节点i
						iMap[k] = i;
					}
				}
			}
		}
	}
	
	//////////////////////////////////////////////////////////
	// 计算哈夫曼编码表
	
	// 找到概率大于0处才开始编码
	for (i = 0; i < m_iColorNum - 1; i ++)
	{
		// 判断概率是否大于0
		if (fTemp[i] > 0)
		{
			break;
		}
	}
	
	// 开始编码
	for (; i < m_iColorNum - 1; i ++)
	{
		// 更新m_strCode
		for (k = 0; k < m_iColorNum; k ++)
		{
			// 判断是否是fTemp[i]的子节点
			if (iMap[k] == i)
			{
				// 改变编码字符串
				m_strCode[k] = "1" + m_strCode[k];
			}
			else if (iMap[k] == i + 1)
			{
				// 改变编码字符串
				m_strCode[k] = "0" + m_strCode[k];
			}
		}
		
		// 概率最小的两个概率相加，保存在fTemp[i + 1]中
		fTemp[i + 1] += fTemp[i];
		
		// 改变映射关系
		for (k = 0; k < m_iColorNum; k ++)
		{
			// 判断是否是fTemp[i]的子节点
			if (iMap[k] == i)
			{
				// 改变映射到节点i+1
				iMap[k] = i + 1;
			}
		}
		
		// 重新排序
		for (j = i + 1; j < m_iColorNum - 1; j ++)
		{
			if (fTemp[j] > fTemp[j + 1])
			{
				// 互换
				fT = fTemp[j];
				fTemp[j] = fTemp[j + 1];
				fTemp[j + 1] = fT;
				
				// 更新映射关系
				for (k = 0; k < m_iColorNum; k ++)
				{
					// 判断是否是fTemp[i]的子节点
					if (iMap[k] == j)
					{
						// 改变映射到节点j+1
						iMap[k] = j + 1;
					}
					else if (iMap[k] == j + 1)
					{
						// 改变映射到节点j
						iMap[k] = j;
					}
				}
			}
			else
			{
				// 退出循环
				break;
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
	
	// 设置List控件样式
	m_lstTable.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	
	// 给List控件添加Header
	m_lstTable.InsertColumn(0, "灰度值", LVCFMT_LEFT, 60, 0);
	m_lstTable.InsertColumn(1, "出现频率", LVCFMT_LEFT, 78, 0);
	m_lstTable.InsertColumn(2, "哈夫曼编码", LVCFMT_LEFT, 110, 1);
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
		
		// 添加哈夫曼编码
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
