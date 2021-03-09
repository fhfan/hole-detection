#pragma once
#include "afxcmn.h"


// Detectresult 对话框

class Detectresult : public CDialogEx
{
	DECLARE_DYNAMIC(Detectresult)

public:
	Detectresult(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Detectresult();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl dt;
};

extern Detectresult odr;
