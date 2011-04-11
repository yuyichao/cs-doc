// ch1_1Doc.cpp : implementation of the CCh1_1Doc class
//

#include "stdafx.h"
#include "ch1_1.h"

#include "ch1_1Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCh1_1Doc

IMPLEMENT_DYNCREATE(CCh1_1Doc, CDocument)

BEGIN_MESSAGE_MAP(CCh1_1Doc, CDocument)
	//{{AFX_MSG_MAP(CCh1_1Doc)
	ON_COMMAND(ID_FILE_REOPEN, OnFileReopen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCh1_1Doc construction/destruction

CCh1_1Doc::CCh1_1Doc()
{
	// 默认背景色，灰色
	m_refColorBKG = 0x00808080;	
	
	// 初始化变量
	m_hDIB = NULL;
	m_palDIB = NULL;
	m_sizeDoc = CSize(1,1);
}

CCh1_1Doc::~CCh1_1Doc()
{
	// 判断DIB对象是否存在
	if (m_hDIB != NULL)
	{
		// 清除DIB对象
		::GlobalFree((HGLOBAL) m_hDIB);
	}
	
	// 判断调色板是否存在
	if (m_palDIB != NULL)
	{
		// 清除调色板
		delete m_palDIB;
	}
}

BOOL CCh1_1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCh1_1Doc diagnostics

#ifdef _DEBUG
void CCh1_1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCh1_1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCh1_1Doc commands

BOOL CCh1_1Doc::CanCloseFrame(CFrameWnd* pFrame) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDocument::CanCloseFrame(pFrame);
}

void CCh1_1Doc::DeleteContents() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDocument::DeleteContents();
}

BOOL CCh1_1Doc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CFile file;
	CFileException fe;

	// 打开文件
	if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		// 失败
		ReportSaveLoadException(lpszPathName, &fe,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		
		// 返回FALSE
		return FALSE;
	}
	
	DeleteContents();
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 尝试调用ReadDIBFile()读取图像
	TRY
	{
		m_hDIB = ::ReadDIBFile(file);
	}
	CATCH (CFileException, eLoad)
	{
		// 读取失败
		file.Abort();
		
		// 恢复光标形状
		EndWaitCursor();
		
		// 报告失败
		ReportSaveLoadException(lpszPathName, eLoad,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		
		// 设置DIB为空
		m_hDIB = NULL;
		
		// 返回FALSE
		return FALSE;
	}
	END_CATCH
	
	// 初始化DIB
	InitDIBData();
	
	// 恢复光标形状
	EndWaitCursor();
	
	// 判断读取文件是否成功
	if (m_hDIB == NULL)
	{
		// 失败，可能非BMP格式
		CString strMsg;
		strMsg = "读取图像时出错！可能是不支持该类型的图像文件！";
		
		// 提示出错
		MessageBox(NULL, strMsg, "系统提示", MB_ICONINFORMATION | MB_OK);
		
		// 返回FALSE
		return FALSE;
	}
	
	// 设置文件名称
	SetPathName(lpszPathName);
	
	// 初始化胀标记为FALSE
	SetModifiedFlag(FALSE);
	
	// 返回TRUE
	return TRUE;
}

void CCh1_1Doc::OnFileReopen() 
{
 	// 重新打开图像，放弃所有修改
 
	// 判断当前图像是否已经被改动
	if (IsModified())
	{
		// 提示用户该操作将丢失所有当前的修改
		if (MessageBox(NULL, "重新打开图像将丢失所有改动！是否继续？", "系统提示", MB_ICONQUESTION | MB_YESNO) == IDNO)
		{
			// 用户取消操作，直接返回
			return;
		}
	}
	
	CFile file;
 	CFileException fe;
 
 	CString strPathName;
  	
	// 获取当前文件路径
 	strPathName = GetPathName();
 
 	// 重新打开文件
 	if (!file.Open(strPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
 	{
 		// 失败
 		ReportSaveLoadException(strPathName, &fe,
 			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
 		
 		// 返回
 		return;
 	}
 	
 	// 更改光标形状
 	BeginWaitCursor();
 	
 	// 尝试调用ReadDIBFile()读取图像
 	TRY
 	{
 		m_hDIB = ::ReadDIBFile(file);
 	}
 	CATCH (CFileException, eLoad)
 	{
 		// 读取失败
 		file.Abort();
 		
 		// 恢复光标形状
 		EndWaitCursor();
 		
 		// 报告失败
 		ReportSaveLoadException(strPathName, eLoad,
 			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
 		
 		// 设置DIB为空
 		m_hDIB = NULL;
 		
 		// 返回
 		return;
 	}
 	END_CATCH
 	
 	// 初始化DIB
 	InitDIBData();
 	
 	// 判断读取文件是否成功
 	if (m_hDIB == NULL)
 	{
 		// 失败，可能非BMP格式
 		CString strMsg;
 		strMsg = "读取图像时出错！可能是不支持该类型的图像文件！";
 		
 		// 提示出错
 		MessageBox(NULL, strMsg, "系统提示", MB_ICONINFORMATION | MB_OK);
  		
 		// 恢复光标形状
 		EndWaitCursor();
 		
 		// 返回
 		return;
 	}
 	
 	// 初始化胀标记为FALSE
 	SetModifiedFlag(FALSE);
 	
 	// 刷新
 	UpdateAllViews(NULL);
  	
 	// 恢复光标形状
 	EndWaitCursor();
	
 	// 返回
 	return;
 	
}

BOOL CCh1_1Doc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	CFile file;
	CFileException fe;

	// 打开文件
	if (!file.Open(lpszPathName, CFile::modeCreate |
	  CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		// 失败
		ReportSaveLoadException(lpszPathName, &fe,
			TRUE, AFX_IDP_INVALID_FILENAME);
		
		// 返回FALSE
		return FALSE;
	}

	// 尝试调用SaveDIB保存图像
	BOOL bSuccess = FALSE;
	TRY
	{
		
		// 更改光标形状
		BeginWaitCursor();

		// 尝试保存图像
		bSuccess = ::SaveDIB(m_hDIB, file);

		// 关闭文件
		file.Close();
	}
	CATCH (CException, eSave)
	{
		// 失败
		file.Abort();
		
		// 恢复光标形状
		EndWaitCursor();
		ReportSaveLoadException(lpszPathName, eSave,
			TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		
		// 返回FALSE
		return FALSE;
	}
	END_CATCH

	// 恢复光标形状
	EndWaitCursor();
	
	// 重置胀标记为FALSE
	SetModifiedFlag(FALSE);

	if (!bSuccess)
	{
		// 保存失败，可能是其它格式的DIB，可以读取但是不能保存
		// 或者是SaveDIB函数有误

		CString strMsg;
		strMsg = "无法保存BMP图像！";
		
		// 提示出错
		MessageBox(NULL, strMsg, "系统提示", MB_ICONINFORMATION | MB_OK);
	}
	
	return bSuccess;
}

void CCh1_1Doc::ReplaceHDIB(HDIB hDIB)
{
	// 替换DIB，在功能粘贴中用到该函数
	
	// 判断DIB是否为空
	if (m_hDIB != NULL)
	{
		// 非空，则清除
		::GlobalFree((HGLOBAL) m_hDIB);
	}

	// 替换成新的DIB对象
	m_hDIB = hDIB;
	
}

void CCh1_1Doc::InitDIBData()
{
	// 初始化DIB对象

	// 判断调色板是否为空
	if (m_palDIB != NULL)
	{
		// 删除调色板对象
		delete m_palDIB;

		// 重置调色板为空
		m_palDIB = NULL;
	}
	
	// 如果DIB对象为空，直接返回
	if (m_hDIB == NULL)
	{
		// 返回
		return;
	}
	
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	
	// 判断图像是否过大
	if (::DIBWidth(lpDIB) > INT_MAX ||::DIBHeight(lpDIB) > INT_MAX)
	{
		::GlobalUnlock((HGLOBAL) m_hDIB);
		
		// 释放DIB对象
		::GlobalFree((HGLOBAL) m_hDIB);
		
		// 设置DIB为空
		m_hDIB = NULL;
		
		CString strMsg;
		strMsg = "BMP图像太大！";
		
		// 提示用户
		MessageBox(NULL, strMsg, "系统提示", MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 设置文档大小
	m_sizeDoc = CSize((int) ::DIBWidth(lpDIB), (int) ::DIBHeight(lpDIB));
	
	::GlobalUnlock((HGLOBAL) m_hDIB);
	
	// 创建新调色板
	m_palDIB = new CPalette;
	
	// 判断是否创建成功
	if (m_palDIB == NULL)
	{
		// 失败，可能是内存不足
		::GlobalFree((HGLOBAL) m_hDIB);
		
		// 设置DIB对象为空
		m_hDIB = NULL;
		
		// 返回
		return;
	}
	
	// 调用CreateDIBPalette来创建调色板
	if (::CreateDIBPalette(m_hDIB, m_palDIB) == NULL)
	{
		// 返回空，可能该DIB对象没有调色板
		
		// 删除
		delete m_palDIB;
		
		// 设置为空
		m_palDIB = NULL;
		
		// 返回
		return;
	}
}


