// Detectresult.cpp : 实现文件
//

#include "stdafx.h"
#include "PHZNVision.h"
#include "Detectresult.h"
#include "afxdialogex.h"
#include "ADO.h"

Detectresult odr;
ADO m_Ado;


// Detectresult 对话框

IMPLEMENT_DYNAMIC(Detectresult, CDialogEx)

Detectresult::Detectresult(CWnd* pParent /*=NULL*/)
: CDialogEx(Detectresult::IDD, pParent)
{

}

Detectresult::~Detectresult()
{
}

void Detectresult::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, dt);
}


BEGIN_MESSAGE_MAP(Detectresult, CDialogEx)
END_MESSAGE_MAP()


// Detectresult 消息处理程序


BOOL Detectresult::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rect;

	// 获取检测结果列表视图控件的位置和大小   
	odr.dt.GetClientRect(&rect);

	// 为列表视图控件添加全行选中和栅格风格   
	//m_programLangList.SetExtendedStyle(m_programLangList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 为列表视图控件添加六列   
	odr.dt.InsertColumn(0, _T("相机1x"), LVCFMT_CENTER, rect.Width() / 4, 0);
	odr.dt.InsertColumn(1, _T("相机1y"), LVCFMT_CENTER, rect.Width() / 4, 1);
	//odr.dt.InsertColumn(2, _T("相机1前16位触发"), LVCFMT_CENTER, rect.Width() / 8, 2);
	//odr.dt.InsertColumn(3, _T("相机1后16位触发"), LVCFMT_CENTER, rect.Width() / 8, 3);
	odr.dt.InsertColumn(2, _T("相机2x"), LVCFMT_CENTER, rect.Width() / 4, 2);
	odr.dt.InsertColumn(3, _T("相机2y"), LVCFMT_CENTER, rect.Width() / 4, 3);
	//odr.dt.InsertColumn(6, _T("相机2前16位触发"), LVCFMT_CENTER, rect.Width() / 8, 6);
	//odr.dt.InsertColumn(7, _T("相机2后16位触发"), LVCFMT_CENTER, rect.Width() / 8, 7);

	UpdateData(true);
	CString xj1x;
	CString xj1y;
	//CString trig1q;
	//CString trig1h;
	CString xj2x;
	CString xj2y;
	//CString trig2q;
	//CString trig2h;
	int idx = 0;
	try
	{
		//m_Ado.m_pRecordset.CreateInstance(__uuidof(Recordset));
		CString search_sql;
		search_sql.Format(_T("select * from detectInfo"));
		m_Ado.m_pRecordset = m_Ado.OpenRecordset(search_sql);
		//m_Ado.m_pRecordset = ((ADO*)(AfxGetMainWnd()))->m_pConnection->Execute(search_sql.AllocSysString(), NULL, adCmdText);
		while (!m_Ado.m_pRecordset->adoEOF)
		{
			xj1x = m_Ado.m_pRecordset->GetCollect("相机1x").bstrVal;
			xj1y = m_Ado.m_pRecordset->GetCollect("相机1y").bstrVal;
			//trig1q = m_Ado.m_pRecordset->GetCollect("相机1前16位触发").bstrVal;
			//trig1h = m_Ado.m_pRecordset->GetCollect("相机1后16位触发").bstrVal;
			xj2x = m_Ado.m_pRecordset->GetCollect("相机2x").bstrVal;
			xj2y = m_Ado.m_pRecordset->GetCollect("相机2y").bstrVal;
			//trig2q = m_Ado.m_pRecordset->GetCollect("相机2前16位触发").bstrVal;
			//trig2h = m_Ado.m_pRecordset->GetCollect("相机2后16位触发").bstrVal;

			odr.dt.InsertItem(idx, xj1x);
			odr.dt.SetItemText(idx, 1, xj1y);
			//odr.dt.SetItemText(idx, 2, trig1q);
			//odr.dt.SetItemText(idx, 3, trig1h);
			odr.dt.SetItemText(idx, 2, xj2x);
			odr.dt.SetItemText(idx, 3, xj2y);
			//odr.dt.SetItemText(idx, 6, trig2q);
			//odr.dt.SetItemText(idx, 7, trig2h);
			idx++;
			//AfxMessageBox(normalnum);
			//cout << strTime << "  " << totalnum << "  " << defectnum << "  " << normalnum << endl;
			m_Ado.m_pRecordset->MoveNext();
		}
		m_Ado.m_pRecordset->Close();
	}
	catch (_com_error e)
	{
		AfxMessageBox(_T("搜索失败！"));
		return TRUE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
