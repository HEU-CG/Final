
// Group3View.cpp: CGroup3View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Group3.h"
#endif

#include "Group3Doc.h"
#include "Group3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGroup3View

IMPLEMENT_DYNCREATE(CGroup3View, CView)

BEGIN_MESSAGE_MAP(CGroup3View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CGroup3View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CGroup3View 构造/析构

CGroup3View::CGroup3View() noexcept
{
	// TODO: 在此处添加构造代码

}

CGroup3View::~CGroup3View()
{
}

BOOL CGroup3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CGroup3View 绘图

void CGroup3View::OnDraw(CDC* /*pDC*/)
{
	CGroup3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CGroup3View 打印


void CGroup3View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CGroup3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CGroup3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CGroup3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CGroup3View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGroup3View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGroup3View 诊断

#ifdef _DEBUG
void CGroup3View::AssertValid() const
{
	CView::AssertValid();
}

void CGroup3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGroup3Doc* CGroup3View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGroup3Doc)));
	return (CGroup3Doc*)m_pDocument;
}
#endif //_DEBUG


// CGroup3View 消息处理程序
