
// PHZNVisionDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "AlgConfigurationTool.h"

#define WM_UPDATEDATA WM_USER+5
#define WM_UPDATEDATA1 WM_USER+5
#define WM_UPDATEDATA2 WM_USER+5


using namespace cv;


// CPHZNVisionDlg 对话框
class CPHZNVisionDlg : public CDialogEx
{
// 构造
public:
	CPHZNVisionDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PHZNVision_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	HObject ho_Image;
	HObject ho_Rectangle;
	HObject ho_Circle;
	HObject ho_ImageReduced;
	HObject ho_Region;
	HObject ho_RegionFillUp;
	HObject ho_ConnectedRegions;
	HObject ho_ImageMean;
	HObject ho_SelectedRegions;
	HObject ho_SelectedRegions1;
	HObject ho_ObjectSelected;
	HObject ho_RegionOpening;
	HObject ho_RegionClosing;
	HObject ho_Contours;
	HObject ho_Image1;
	HObject ho_Rectangle1;
	HObject ho_Circle1;
	HObject ho_ImageReduced1;
	HObject ho_Region1;
	HObject ho_RegionFillUp1;
	HObject ho_ConnectedRegions1;
	HObject ho_SelectedRegions2;
	HObject ho_SelectedRegions3;
	HObject ho_ObjectSelected1;
	HObject ho_RegionOpening1;
	HObject ho_RegionClosing1;
	HObject ho_Contours1;
	HTuple hv_WindowID;
	HTuple hv_WindowID1;
	bool StartImageState;
	bool StartImageState1;
	bool StartImageState2;
	void CreateImageWindow();
	void InitConsoleWindow();
	afx_msg void OnBnClickedButton1();
	static UINT StartCameraTest(LPVOID pParam);
	static UINT StartCameraTest1(LPVOID pParam);
	static UINT StartCameraTest2(LPVOID pParam);
	LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	LRESULT OnUpdateData1(WPARAM wParam, LPARAM lParam);
	LRESULT OnUpdateData2(WPARAM wParam, LPARAM lParam);
	long time;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	double exp;
	double rx;
	double ry;
	CButton trigger;
	//CButton preview;
	long time1;
	double exp1;
	CButton trigger1;
	//CButton preview1;
	double rx1;
	double ry1;
	afx_msg void OnBnClickedButton4();
	double rc;
	double rc1;
	//加载图像矫正文件：相机内参文件、相机位姿文件
	void LoadImageCorrectionFile();
	int delay;
	int delay1;
	/*afx_msg void OnSize(UINT nType, int cx, int cy);
	CRect m_rect;
	void changeControlSize(CWnd* pWnd, int x, int y);*/
	CButton repaint;
	CButton repaint1;
	//double thres;
	//double thres1;
	int triggernum;
	int triggernum1;
};

