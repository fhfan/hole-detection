
// PHZNVisionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PHZNVision.h"
#include "PHZNVisionDlg.h"
#include "afxdialogex.h"
#include <Windows.h>
#include <ctime>
#include <vector>
#include <string>
#include <iostream>
#include "modbus.h"
#include <math.h>
#include <io.h>
#include <fcntl.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//CPHZNVisionDlg dlg;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPHZNVisionDlg 对话框



CPHZNVisionDlg::CPHZNVisionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPHZNVisionDlg::IDD, pParent)
	, time(0)
	, exp(15000)
	, rx(0)
	, ry(0)
	, time1(0)
	, exp1(15000)
	, rx1(0)
	, ry1(0)
	, rc(0.0048)
	, rc1(0.0048)
	, delay(0)
	, delay1(0)
	//, thres(255)
	//, thres1(255)
	, triggernum(0)
	, triggernum1(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPHZNVisionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT7, time);
	DDX_Text(pDX, IDC_EDIT8, exp);
	DDX_Text(pDX, IDC_EDIT10, rx);
	DDX_Text(pDX, IDC_EDIT11, ry);
	DDX_Control(pDX, IDC_CHECK3, trigger);
	//DDX_Control(pDX, IDC_CHECK4, preview);
	DDX_Text(pDX, IDC_EDIT1, time1);
	DDX_Text(pDX, IDC_EDIT6, exp1);
	DDX_Control(pDX, IDC_CHECK1, trigger1);
	//DDX_Control(pDX, IDC_CHECK2, preview1);
	DDX_Text(pDX, IDC_EDIT9, rx1);
	DDX_Text(pDX, IDC_EDIT12, ry1);
	DDX_Text(pDX, IDC_EDIT14, rc);
	DDX_Text(pDX, IDC_EDIT16, rc1);
	DDX_Text(pDX, IDC_EDIT2, delay);
	DDX_Text(pDX, IDC_EDIT3, delay1);
	DDX_Control(pDX, IDC_CHECK5, repaint);
	DDX_Control(pDX, IDC_CHECK6, repaint1);
	//DDX_Text(pDX, IDC_EDIT4, thres);
	//DDX_Text(pDX, IDC_EDIT5, thres1);
	DDX_Text(pDX, IDC_EDIT4, triggernum);
	DDX_Text(pDX, IDC_EDIT5, triggernum1);
}

BEGIN_MESSAGE_MAP(CPHZNVisionDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPHZNVisionDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPHZNVisionDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPHZNVisionDlg::OnBnClickedButton3)
	ON_MESSAGE(WM_UPDATEDATA, OnUpdateData)
	ON_MESSAGE(WM_UPDATEDATA1, OnUpdateData1)
	ON_BN_CLICKED(IDC_BUTTON4, &CPHZNVisionDlg::OnBnClickedButton4)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CPHZNVisionDlg 消息处理程序

BOOL CPHZNVisionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	CreateImageWindow();
	//LoadImageCorrectionFile();
	//CRect rect;
	//GetClientRect(&rect);     //取客户区大小  
	//InitConsoleWindow();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPHZNVisionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPHZNVisionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPHZNVisionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPHZNVisionDlg::InitConsoleWindow()
{
	AllocConsole();
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle, _O_TEXT);
	FILE *hf = _fdopen(hCrt, "w");
	*stdout = *hf;
}



void CPHZNVisionDlg::CreateImageWindow()
{
	HTuple HWindowID, HWindowID1;
	CRect Rect, Rect1;
	CWnd* pWnd = GetDlgItem(IDC_PIC);
	CWnd* pWnd1 = GetDlgItem(IDC_PIC1);
	HWindowID = (Hlong)pWnd->m_hWnd;//获取父窗口句柄
	HWindowID1 = (Hlong)pWnd1->m_hWnd;
	pWnd->GetWindowRect(&Rect);
	pWnd1->GetWindowRect(&Rect1);
	OpenWindow(0, 0, Rect.Width(), Rect.Height(), HWindowID, "visible", "", &hv_WindowID);
	OpenWindow(0, 0, Rect1.Width(), Rect1.Height(), HWindowID1, "visible", "", &hv_WindowID1);
}


void CPHZNVisionDlg::LoadImageCorrectionFile()
{
	////读取相机标定生成的文件
	string strCameraPath, strPosePath;
	AlgConfigTool.ReadCameraCorrectionConfig(CameraConfigPath, strCameraPath, strPosePath, CompensationValue);

	//////读取相机标定生成的文件
	//string strCameraPath, strPosePath;
	//strCameraPath = "./Calibration/camera12W.cal";
	//strPosePath = "./Calibration/pose12W.dat";

	ReadCamPar(strCameraPath.c_str(), &hv_CameraParameters);
	ReadPose(strPosePath.c_str(), &hv_CameraPose);
}

Mat HObject2Mat(HObject Hobj)
{
	Mat Image;
	HTuple htCh;

	HString cType;

	ConvertImageType(Hobj, &Hobj, "byte");

	CountChannels(Hobj, &htCh);

	Hlong wid = 0;

	Hlong hgt = 0;

	if (htCh[0].I() == 1)

	{

		HImage hImg(Hobj);

		void *ptr = hImg.GetImagePointer1(&cType, &wid, &hgt);//GetImagePointer1(Hobj, &ptr, &cType, &wid, &hgt);

		int W = (HTuple)wid;

		int H = (HTuple)hgt;

		Image.create(H, W, CV_8UC1);

		unsigned char *pdata = static_cast<unsigned char *>(ptr);

		memcpy(Image.data, pdata, W*H);

	}

	else if (htCh[0].I() == 3)

	{

		void *Rptr;

		void *Gptr;

		void *Bptr;

		HImage hImg(Hobj);

		hImg.GetImagePointer3(&Rptr, &Gptr, &Bptr, &cType, &wid, &hgt);

		int W = (HTuple)wid;

		int H = (HTuple)hgt;

		Image.create(H, W, CV_8UC3);

		vector<Mat> VecM(3);

		VecM[0].create(H, W, CV_8UC1);

		VecM[1].create(H, W, CV_8UC1);

		VecM[2].create(H, W, CV_8UC1);

		unsigned char *R = (unsigned char *)Rptr;

		unsigned char *G = (unsigned char *)Gptr;

		unsigned char *B = (unsigned char *)Bptr;

		memcpy(VecM[2].data, R, W*H);

		memcpy(VecM[1].data, G, W*H);

		memcpy(VecM[0].data, B, W*H);

		merge(VecM, Image);

	}
	return Image;
}


//Mat HImageToMat(HalconCpp::HObject &H_img)
//{
//	Mat cv_img;
//	HalconCpp::HTuple channels, w, h;
//
//	HalconCpp::ConvertImageType(H_img, &H_img, "byte");
//	HalconCpp::CountChannels(H_img, &channels);
//
//	if (channels.I() == 1)
//	{
//		HalconCpp::HTuple pointer;
//		GetImagePointer1(H_img, &pointer, nullptr, &w, &h);
//		int width = w.I(), height = h.I();
//		int size = width * height;
//		cv_img = Mat::zeros(height, width, CV_8UC1);
//		memcpy(cv_img.data, (void*)(pointer.L()), size);
//	}
//
//	else if (channels.I() == 3)
//	{
//		HalconCpp::HTuple pointerR, pointerG, pointerB;
//		HalconCpp::GetImagePointer3(H_img, &pointerR, &pointerG, &pointerB, nullptr, &w, &h);
//		int width = w.I(), height = h.I();
//		int size = width * height;
//		cv_img = Mat::zeros(height, width, CV_8UC3);
//		uchar* R = (uchar*)(pointerR.L());
//		uchar* G = (uchar*)(pointerG.L());
//		uchar* B = (uchar*)(pointerB.L());
//		for (int i = 0; i < height; ++i)
//		{
//			uchar *p = cv_img.ptr<uchar>(i);
//			for (int j = 0; j < width; ++j)
//			{
//				p[3 * j] = B[i * width + j];
//				p[3 * j + 1] = G[i * width + j];
//				p[3 * j + 2] = R[i * width + j];
//			}
//		}
//	}
//	return cv_img;
//}
//
//
//Mat HImageToMat1(HalconCpp::HObject &H_img)
//{
//	Mat cv_img;
//	HalconCpp::HTuple channels, w, h;
//
//	HalconCpp::ConvertImageType(H_img, &H_img, "byte");
//	HalconCpp::CountChannels(H_img, &channels);
//
//	if (channels.I() == 1)
//	{
//		HalconCpp::HTuple pointer;
//		GetImagePointer1(H_img, &pointer, nullptr, &w, &h);
//		int width = w.I(), height = h.I();
//		int size = width * height;
//		cv_img = Mat::zeros(height, width, CV_8UC1);
//		memcpy(cv_img.data, (void*)(pointer.L()), size);
//	}
//
//	else if (channels.I() == 3)
//	{
//		HalconCpp::HTuple pointerR, pointerG, pointerB;
//		HalconCpp::GetImagePointer3(H_img, &pointerR, &pointerG, &pointerB, nullptr, &w, &h);
//		int width = w.I(), height = h.I();
//		int size = width * height;
//		cv_img = Mat::zeros(height, width, CV_8UC3);
//		uchar* R = (uchar*)(pointerR.L());
//		uchar* G = (uchar*)(pointerG.L());
//		uchar* B = (uchar*)(pointerB.L());
//		for (int i = 0; i < height; ++i)
//		{
//			uchar *p = cv_img.ptr<uchar>(i);
//			for (int j = 0; j < width; ++j)
//			{
//				p[3 * j] = B[i * width + j];
//				p[3 * j + 1] = G[i * width + j];
//				p[3 * j + 2] = R[i * width + j];
//			}
//		}
//	}
//	return cv_img;
//}


Mat HObject2Mat1(HObject Hobj)
{
	Mat Image;
	HTuple htCh;

	HString cType;

	ConvertImageType(Hobj, &Hobj, "byte");

	CountChannels(Hobj, &htCh);

	Hlong wid = 0;

	Hlong hgt = 0;

	if (htCh[0].I() == 1)

	{

		HImage hImg(Hobj);

		void *ptr = hImg.GetImagePointer1(&cType, &wid, &hgt);//GetImagePointer1(Hobj, &ptr, &cType, &wid, &hgt);

		int W = (HTuple)wid;

		int H = (HTuple)hgt;

		Image.create(H, W, CV_8UC1);

		unsigned char *pdata = static_cast<unsigned char *>(ptr);

		memcpy(Image.data, pdata, W*H);

	}

	else if (htCh[0].I() == 3)

	{

		void *Rptr;

		void *Gptr;

		void *Bptr;

		HImage hImg(Hobj);

		hImg.GetImagePointer3(&Rptr, &Gptr, &Bptr, &cType, &wid, &hgt);

		int W = (HTuple)wid;

		int H = (HTuple)hgt;

		Image.create(H, W, CV_8UC3);

		vector<Mat> VecM(3);

		VecM[0].create(H, W, CV_8UC1);

		VecM[1].create(H, W, CV_8UC1);

		VecM[2].create(H, W, CV_8UC1);

		unsigned char *R = (unsigned char *)Rptr;

		unsigned char *G = (unsigned char *)Gptr;

		unsigned char *B = (unsigned char *)Bptr;

		memcpy(VecM[2].data, R, W*H);

		memcpy(VecM[1].data, G, W*H);

		memcpy(VecM[0].data, B, W*H);

		merge(VecM, Image);

	}
	return Image;
}


HObject Mat2HObject(const Mat &image)
{
	HObject Hobj = HObject();
	int hgt = image.rows;
	int wid = image.cols;
	int i;
	//  CV_8UC3  
	if (image.type() == CV_8UC3)
	{
		vector<cv::Mat> imgchannel;
		split(image, imgchannel);
		cv::Mat imgB = imgchannel[0];
		cv::Mat imgG = imgchannel[1];
		cv::Mat imgR = imgchannel[2];
		uchar* dataR = new uchar[hgt*wid];
		uchar* dataG = new uchar[hgt*wid];
		uchar* dataB = new uchar[hgt*wid];
		for (i = 0; i < hgt; i++)
		{
			memcpy(dataR + wid * i, imgR.data + imgR.step*i, wid);
			memcpy(dataG + wid * i, imgG.data + imgG.step*i, wid);
			memcpy(dataB + wid * i, imgB.data + imgB.step*i, wid);
		}
		GenImage3(&Hobj, "byte", wid, hgt, (Hlong)dataR, (Hlong)dataG, (Hlong)dataB);
		delete[]dataR;
		delete[]dataG;
		delete[]dataB;
		dataR = NULL;
		dataG = NULL;
		dataB = NULL;
	}
	//  CV_8UC1  
	else if (image.type() == CV_8UC1)
	{
		uchar* data = new uchar[hgt*wid];
		for (i = 0; i < hgt; i++)
			memcpy(data + wid * i, image.data + image.step*i, wid);
		GenImage1(&Hobj, "byte", wid, hgt, (Hlong)data);
		delete[] data;
		data = NULL;
	}
	return Hobj;
}


//Mat label(HObject ho_Image, int upvalue, int lowvalue, int b, int g, int r)
//{
//	Mat srcImage, colImage;
//	int j, k;
//	Mat grayImage;
//	vector<Mat> channels(3);
//	srcImage = HObject2Mat(ho_Image);
//	if (srcImage.channels() == 1)
//	{
//		for (j = 0; j < 3; j++)
//			channels[j] = srcImage;
//		merge(channels, colImage);
//		//channels.clear();
//		for (j = 0; j < srcImage.rows; j++)
//		{
//			for (k = 0; k < srcImage.cols; k++)
//			{
//				if (srcImage.at<uchar>(j, k)>upvalue || srcImage.at<uchar>(j, k)<lowvalue)
//				{
//					colImage.at<Vec3b>(j, k)[0] = b;
//					colImage.at<Vec3b>(j, k)[1] = g;
//					colImage.at<Vec3b>(j, k)[2] = r;
//				}
//
//			}
//		}
//		return colImage;
//	}
//	else
//	{
//		cvtColor(srcImage, grayImage, CV_BGR2GRAY);   //RGB 012
//		for (j = 0; j < grayImage.rows; j++)
//		{
//			for (k = 0; k < grayImage.cols; k++)
//			{
//				if (grayImage.at<uchar>(j, k)>upvalue || grayImage.at<uchar>(j, k)<lowvalue)
//				{
//					srcImage.at<Vec3b>(j, k)[0] = b;
//					srcImage.at<Vec3b>(j, k)[1] = g;
//					srcImage.at<Vec3b>(j, k)[2] = r;
//				}
//			}
//		}
//		return srcImage;
//	}
//}

void deletefiles(HTuple dir)
{
	HTuple  hv_Files, hv_Index, hv_strLen, hv_Pos;
	HTuple  hv_Pos1;

	ListFiles(dir, (HTuple("files").Append("directories")),
		&hv_Files);
	{
		HTuple end_val1 = (hv_Files.TupleLength()) - 1;
		HTuple step_val1 = 1;
		for (hv_Index = 0; hv_Index.Continue(end_val1, step_val1); hv_Index += step_val1)
		{
			hv_strLen = HTuple(hv_Files[hv_Index]).TupleStrlen();
			hv_Pos = HTuple(hv_Files[hv_Index]).TupleStrrchr(".");
			if (0 != (-1 == hv_Pos))
			{
				hv_Pos1 = HTuple(hv_Files[hv_Index]).TupleStrrchr("\\");
				if (0 != ((hv_Pos1 + 1) == hv_strLen))
				{
					deletefiles(hv_Files[hv_Index]);
				}
			}
			else
			{
				DeleteFile(HTuple(hv_Files[hv_Index]));
			}
		}
	}
}

double DistancePP(HalconCpp::HTuple hv_CameraParameters, HalconCpp::HTuple hv_CameraPose,
	double TargetThickness, double PointerRow1, double PointerColumn1, double PointerRow2, double PointerColumn2)
{
	HalconCpp::HTuple hv_TargetCameraPose;
	SetOriginPose(hv_CameraPose, 0.0, 0.0, TargetThickness, &hv_TargetCameraPose);

	HalconCpp::HTuple hv_ImageRows;
	hv_ImageRows.Clear();
	hv_ImageRows.Append(HalconCpp::HTuple(PointerRow1));
	hv_ImageRows.Append(HalconCpp::HTuple(PointerRow2));

	HalconCpp::HTuple hv_ImageColumns;
	hv_ImageColumns.Clear();
	hv_ImageColumns.Append(HalconCpp::HTuple(PointerColumn1));
	hv_ImageColumns.Append(HalconCpp::HTuple(PointerColumn2));

	HalconCpp::HTuple hv_WorldX, hv_WorldY;
	ImagePointsToWorldPlane(hv_CameraParameters, hv_TargetCameraPose, hv_ImageRows, hv_ImageColumns, "mm", &hv_WorldX, &hv_WorldY);
	// Determine the distance in world coordinate [mm]
	HalconCpp::HTuple hv_Distance;
	DistancePp(HalconCpp::HTuple(hv_WorldY[0]), HalconCpp::HTuple(hv_WorldX[0]),
		HalconCpp::HTuple(hv_WorldY[1]), HalconCpp::HTuple(hv_WorldX[1]), &hv_Distance);

	double distance = hv_Distance[0].D(); //[mm]
	return distance;
}

bool flag = false, flag1 = false, flag2 = false;
bool judg1 = true, judg2 = true;
bool judg11 = true, judg21 = true;
int thread1, thread2;
int trignum = 0, trignum1 = 0;
HTuple hv_Row11, hv_Column11, hv_Phi11, hv_Length11, hv_Length211, hv_Usedthreshold, hv_Usedthreshold1;
HTuple hv_Row12, hv_Column12, hv_Phi12, hv_Length12, hv_Length212;
HTuple hv_Row21, hv_Column21, hv_Phi21, hv_Length21, hv_Length221;
HTuple hv_Row22, hv_Column22, hv_Phi22, hv_Length22, hv_Length222;
HTuple hv_Row11t, hv_Column11t, hv_Phi11t, hv_Length11t, hv_Length211t;
HTuple hv_Row12t, hv_Column12t, hv_Phi12t, hv_Length12t, hv_Length212t;
HTuple hv_Row21t, hv_Column21t, hv_Phi21t, hv_Length21t, hv_Length221t;
HTuple hv_Row22t, hv_Column22t, hv_Phi22t, hv_Length22t, hv_Length222t;
HTuple hv_AcqHandle, hv_AcqHandle1;
HTuple m_ImageWidth, m_ImageWidth1, m_ImageHeight, m_ImageHeight1;

void CPHZNVisionDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_CHECK3)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK5)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK6)->EnableWindow(TRUE);
	if (flag == false)
	{
		AfxBeginThread(StartCameraTest, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		flag = true;
	}
	if (flag1 == false)
	{
		AfxBeginThread(StartCameraTest1, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		flag1 = true;
	}
	if (flag2 == false)
	{
		AfxBeginThread(StartCameraTest2, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		//flag2 = true;
	}
}


UINT CPHZNVisionDlg::StartCameraTest(LPVOID pParam)
{
	// Local control variables
	HTuple end_val36, step_val36, hv_Classes, hv_lightdark, hv_Width, hv_Height;
	HTuple hv_Information, hv_Values, hv_Number, hv_I, hv_Radius, hv_StartPhi, hv_EndPhi, hv_PointOrder, hv_Area;
	HTuple hv_Row, hv_Column, hv_MetrologyHandle1, hv_Index, hv_Parameter1, hv_Mean, hv_Deviation;
	thread1 = 0;
	clock_t st, et;
	Mat roi;
	//int s, rowmax, colmax, rowindex, colindex;
	vector<Vec3f> pcircles;
	bool trijudg;
	try
	{
		InfoFramegrabber("HMV3rdParty", "device", &hv_Information, &hv_Values);
		OpenFramegrabber("HMV3rdParty", 0, 0, 0, 0, 0, 0, "progressive", -1, "default", -1, "false", "default", hv_Values[0].S(), 0, -1, &hv_AcqHandle);
	}
	catch (...)
	{
		//CloseFramegrabber(hv_AcqHandle);
		return 0;
	}
	CPHZNVisionDlg *pDlg = (CPHZNVisionDlg*)pParam;
	pDlg->SendMessage(WM_UPDATEDATA, TRUE);
	if (pDlg->trigger.GetCheck() == 1)
	{
		SetFramegrabberParam(hv_AcqHandle, "TriggerSelector", "FrameStart");
		SetFramegrabberParam(hv_AcqHandle, "TriggerMode", "On");
		SetFramegrabberParam(hv_AcqHandle, "TriggerSource", "Line1");
		SetFramegrabberParam(hv_AcqHandle, "grab_timeout", 1000);
	}
	else
		SetFramegrabberParam(hv_AcqHandle, "TriggerMode", "Off");
	//相机参数
	SetFramegrabberParam(hv_AcqHandle, "ExposureTime", pDlg->exp);
	//SetFramegrabberParam(hv_AcqHandle, "GainRaw", pDlg->gain);
	//SetFramegrabberParam(hv_AcqHandle, "Gamma", pDlg->gamma);
	if (judg11 == true)
	{
		GrabImageStart(hv_AcqHandle, -1);
		//ClearWindow(pDlg->hv_WindowID);
		//SetFramegrabberParam(hv_AcqHandle, "grab_timeout", -1);
		//GrabImage(&pDlg->ho_Image, hv_AcqHandle);   //同步拉流
		GrabImageAsync(&pDlg->ho_Image, hv_AcqHandle, -1);
		//GetImageSize(pDlg->ho_Image, &hv_Width, &hv_Height);
		GetImagePointer1(pDlg->ho_Image, NULL, NULL, &m_ImageWidth, &m_ImageHeight);     //和加载静态图像相同
		//Mat m_Image = HObject2Mat(pDlg->ho_Image);
		judg11 = false;
	}
	if (pDlg->repaint.GetCheck() == 1)
		judg1 = true;
	SetPart(pDlg->hv_WindowID, 0, 0, m_ImageHeight - 1, m_ImageWidth - 1);
	pDlg->StartImageState = true;
	SetColor(pDlg->hv_WindowID, "red");
	SetDraw(pDlg->hv_WindowID, "margin");
	while (pDlg->StartImageState)
	{
		//GrabImage(&pDlg->ho_Image, hv_AcqHandle);
		trijudg = false;
		if (pDlg->trigger.GetCheck() == 1)
		{
			SetFramegrabberParam(hv_AcqHandle, "TriggerSelector", "FrameStart");
			SetFramegrabberParam(hv_AcqHandle, "TriggerMode", "On");
			SetFramegrabberParam(hv_AcqHandle, "TriggerSource", "Line1");
			SetFramegrabberParam(hv_AcqHandle, "grab_timeout", 1000);
			trijudg = true;
		}
		else
			SetFramegrabberParam(hv_AcqHandle, "TriggerMode", "Off");
		try
		{
			GrabImageAsync(&pDlg->ho_Image, hv_AcqHandle, -1);
		}
		catch (...)
		{
			if (pDlg->StartImageState == false)
				break;
			else
				continue;
		}
		st = clock();
		//thread1 = 0;
		DispObj(pDlg->ho_Image, pDlg->hv_WindowID);
		if (pDlg->repaint.GetCheck() != 1 && judg1 == true)
			continue;
		//Mat srcImage = HObject2Mat(pDlg->ho_Image);
		/*if (judg1==true)
			DrawRectangle2(pDlg->hv_WindowID, &hv_Row11, &hv_Column11, &hv_Phi11, &hv_Length11, &hv_Length211);
		GenRectangle2(&pDlg->ho_Rectangle, hv_Row11, hv_Column11, hv_Phi11, hv_Length11, hv_Length211);
		DispRectangle2(pDlg->hv_WindowID, hv_Row11, hv_Column11, hv_Phi11, hv_Length11, hv_Length211);
		ReduceDomain(pDlg->ho_Image, pDlg->ho_Rectangle, &pDlg->ho_ImageReduced);
		if (pDlg->rc.GetCheck() == 1)
			hv_lightdark = "dark";
		else
			hv_lightdark = "light";
		Binaryrchold(pDlg->ho_ImageReduced, &pDlg->ho_Region, "smooth_histo", hv_lightdark, &hv_Usedrchold);
		FillUp(pDlg->ho_Region, &pDlg->ho_RegionFillUp);
		SmallestRectangle2(pDlg->ho_RegionFillUp, &hv_Row11t, &hv_Column11t, &hv_Phi11t, &hv_Length11t, &hv_Length211t);
		DispCircle(pDlg->hv_WindowID, hv_Row11t, hv_Column11t, 2);*/
		if (judg1 == true)
		{
			DrawRectangle1(pDlg->hv_WindowID, &hv_Row11, &hv_Column11, &hv_Row12, &hv_Column12);
			judg1 = false;
		}
		GenRectangle1(&pDlg->ho_Rectangle, hv_Row11, hv_Column11, hv_Row12, hv_Column12);
		DispRectangle1(pDlg->hv_WindowID, hv_Row11, hv_Column11, hv_Row12, hv_Column12);
		/*ReduceDomain(pDlg->ho_Image, pDlg->ho_Rectangle, &pDlg->ho_ImageReduced);
		hv_Row = hv_Row12;
		hv_Column = hv_Column12;
		hv_Radius = hv_Length12 / 2.0;
		CreateMetrologyModel(&hv_MetrologyHandle1);
		AddMetrologyObjectGeneric(hv_MetrologyHandle1, "circle", (hv_Row.TupleConcat(hv_Column)).TupleConcat(hv_Radius),
			hv_Radius, 5, 1, 30, HalconCpp::HTuple(), HalconCpp::HTuple(), &hv_Index);
		SetMetrologyObjectParam(hv_MetrologyHandle1, "all", "measure_transition", "positive");
		SetMetrologyObjectParam(hv_MetrologyHandle1, "all", "num_measures", 20);
		SetMetrologyObjectParam(hv_MetrologyHandle1, "all", "num_instances", 2);
		ApplyMetrologyModel(pDlg->ho_ImageReduced, hv_MetrologyHandle1);
		GetMetrologyObjectResult(hv_MetrologyHandle1, "all", "all", "result_type", "all_param",
			&hv_Parameter1);
		GetMetrologyObjectResultContour(&pDlg->ho_Contours, hv_MetrologyHandle1, "all", "all",
			1.5);
		ClearMetrologyModel(hv_MetrologyHandle1);
		if (hv_Parameter1.Length() == 0)
		{
			pDlg->rx = 100;
			pDlg->ry = 100;
		}
		else if (hv_Parameter1.Length() == 3)
		{
			hv_Column12t = hv_Parameter1[1];
			hv_Row12t = hv_Parameter1[0];
			hv_Radius = hv_Parameter1[2];
			DispCircle(pDlg->hv_WindowID, hv_Row12t, hv_Column12t, hv_Radius);
			pDlg->rx = round((pDlg->x1 + (hv_Column12t.D() - hv_Width.I() / 2.0)*pDlg->rc) * 1000) / 1000;
			pDlg->ry = round((pDlg->y1 + (hv_Height.I() / 2.0 - hv_Row12t.D())*pDlg->rc) * 1000) / 1000;
		}
		else if (hv_Parameter1.Length() == 6)
		{
			int i;
			if (hv_Parameter1[2] > hv_Parameter1[5])
			{
				i = 0;
			}
			else
			{
				i = 1;
			}
			hv_Column12t = hv_Parameter1[1 + 3 * i];
			hv_Row12t = hv_Parameter1[0 + 3 * i];
			hv_Radius = hv_Parameter1[2 + 3 * i];
			DispCircle(pDlg->hv_WindowID, hv_Row12t, hv_Column12t, hv_Radius);
			pDlg->rx = round((pDlg->x1 + (hv_Column12t.D() - hv_Width.I() / 2.0)*pDlg->rc) * 1000) / 1000;
			pDlg->ry = round((pDlg->y1 + (hv_Height.I() / 2.0 - hv_Row12t.D())*pDlg->rc) * 1000) / 1000;
		}*/
		ReduceDomain(pDlg->ho_Image, pDlg->ho_Rectangle, &pDlg->ho_ImageReduced);
		//CropDomain(pDlg->ho_ImageReduced, &pDlg->ho_ImageReduced);
		GaussFilter(pDlg->ho_ImageReduced, &pDlg->ho_ImageReduced, 3);
		Intensity(pDlg->ho_Rectangle, pDlg->ho_ImageReduced, &hv_Mean, &hv_Deviation);
		if (hv_Deviation.D()<10)
		{
			pDlg->rx = 100;
			pDlg->ry = 100;
		}
		else
		{
			BinaryThreshold(pDlg->ho_ImageReduced, &pDlg->ho_Region, "max_separability", "light", &hv_Usedthreshold);
			AreaCenter(pDlg->ho_Region, &hv_Area, &hv_Row, &hv_Column);
			pDlg->rx = round(hv_Column.D()*pDlg->rc * 1000) / 1000;
			pDlg->ry = round(hv_Row.D()*pDlg->rc * 1000) / 1000;
			DispCircle(pDlg->hv_WindowID, hv_Row, hv_Column, HTuple(0.75 / pDlg->rc));
		}
		//cout << hv_Deviation.D() << endl;
		//BinaryThreshold(pDlg->ho_ImageReduced, &pDlg->ho_Region, "max_separability", "light", &hv_Usedthreshold);
		//Threshold(pDlg->ho_ImageReduced, &pDlg->ho_Region, pDlg->thres, 255);
		//CountObj(pDlg->ho_Region, &hv_Number);
		//cout << hv_Number.I() << endl;
		//AreaCenter(pDlg->ho_Region, &hv_Area, &hv_Row, &hv_Column);
		//if (hv_Row.D() == 0 && hv_Column.D()==0)
		//{
		//	pDlg->rx = 100;
		//	pDlg->ry = 100;
		//}
		//else
		//{
		//	//AreaCenter(pDlg->ho_Region, &hv_Area, &hv_Row, &hv_Column);
		//	pDlg->rx = round(hv_Column.D()*pDlg->rc * 1000) / 1000;
		//	pDlg->ry = round(hv_Row.D()*pDlg->rc * 1000) / 1000;
		//	DispCircle(pDlg->hv_WindowID, hv_Row, hv_Column, HTuple(0.75 / pDlg->rc));
		//}
		
		//AutoThreshold(pDlg->ho_ImageReduced, &pDlg->ho_Region, 15);
		//BinaryThreshold(pDlg->ho_ImageReduced, &pDlg->ho_Region, "smooth_histo", "light", &hv_Usedthreshold);
		//Connection(pDlg->ho_Region, &pDlg->ho_ConnectedRegions);
		//RegionToMean(pDlg->ho_ConnectedRegions, pDlg->ho_ImageReduced, &pDlg->ho_ImageMean);
		//roi = HObject2Mat(pDlg->ho_ImageMean);
		//threshold(pDlg->ho_ImageReduced, &pDlg->ho_ImageReduced, 128, 255);
		//threshold(roi, roi, pDlg->thres, 255, CV_THRESH_BINARY);
		//rowmax = 0; colmax = 0;
		//for (int i = 0; i < roi.rows; i++)
		//{
		//	s = 0;
		//	uchar* pdata = roi.ptr<uchar>(i);
		//	for (int j = 0; j < roi.cols; j++)
		//	{
		//		s += *pdata++;
		//	}
		//	if (s>=rowmax)
		//	{
		//		rowmax = s;
		//		rowindex = i;
		//	}
		//	/*else
		//		break;*/
		//}
		//roi = roi.t();
		//for (int i = 0; i < roi.rows; i++)
		//{
		//	s = 0;
		//	uchar* pdata = roi.ptr<uchar>(i);
		//	for (int j = 0; j < roi.cols; j++)
		//	{
		//		s += *pdata++;
		//	}
		//	if (s >= colmax)
		//	{
		//		colmax = s;
		//		colindex = i;
		//	}
		//	/*else
		//		break;*/
		//}
		//Roberts(pDlg->ho_ImageReduced, &pDlg->ho_ImageReduced, "gradient_sum");
		//Autothreshold(pDlg->ho_ImageReduced, &pDlg->ho_ImageReduced, 15);
		////Binaryrchold(pDlg->ho_ImageReduced, &pDlg->ho_ImageReduced, "smooth_histo", "dark", &hv_Usedrchold);
		//Connection(pDlg->ho_ImageReduced, &pDlg->ho_ImageReduced);
		//SelectShape(pDlg->ho_ImageReduced, &pDlg->ho_ImageReduced, "area", "and", HTuple(int(M_PI*pow(0.5 / pDlg->rc, 2))), HTuple(int(M_PI*pow(1 / pDlg->rc, 2))));
		////SelectShape(pDlg->ho_ImageReduced, &pDlg->ho_ImageReduced, "circularity", "and", 0.6, 1);
		//CountObj(pDlg->ho_ImageReduced, &hv_Number);
		//ShapeTrans(pDlg->ho_ImageReduced, &pDlg->ho_ImageReduced, "outer_circle");
		//AreaCenter(pDlg->ho_ImageReduced, &hv_Area, &hv_Row12t, &hv_Column12t);
		//hv_Radius = HTuple(pow(hv_Area.D() / M_PI, 0.5));
		//DispCircle(pDlg->hv_WindowID, hv_Row12t, hv_Column12t, hv_Radius);

		//roi = HObject2Mat(pDlg->ho_ImageReduced);
		//HoughCircles(roi, pcircles, CV_HOUGH_GRADIENT, 1, 10, pDlg->rc, 35/*, int(0.5 / pDlg->rc), int(1 / pDlg->rc)*/);
		//for (int i = 0; i < pcircles.size(); i++)
		//{
		//	/*cout << pcircles[i][1] << endl;
		//	cout << pcircles[i][0] << endl;
		//	cout << pcircles[i][2] << endl;*/
		//	DispCircle(pDlg->hv_WindowID, HTuple(pcircles[i][1]), HTuple(pcircles[i][0]), HTuple(pcircles[i][2]));
		//}
		//DispCircle(pDlg->hv_WindowID, HTuple(hv_Row11.D() + rowindex), HTuple(hv_Column11.D()+colindex), HTuple(0.75 / pDlg->rc));
		//if (pcircles.size() != 0)
		//{
		//	//pDlg->rx = round((pDlg->x1 + (pcircles[pcircles.size() - 1][0] - hv_Width.I() / 2.0)*pDlg->rc) * 1000) / 1000;
		//	//pDlg->ry = round((pDlg->y1 + (hv_Height.I() / 2.0 - pcircles[pcircles.size() - 1][1])*pDlg->rc) * 1000) / 1000;
		//	pDlg->rx = round(DistancePP(hv_CameraParameters, hv_CameraPose, TargetThickness,
		//		pcircles[pcircles.size() - 1][1], pcircles[pcircles.size() - 1][0],
		//		pcircles[pcircles.size() - 1][1], 0) * 1000) / 1000;
		//	pDlg->ry = round(DistancePP(hv_CameraParameters, hv_CameraPose, TargetThickness,
		//		pcircles[pcircles.size() - 1][1], pcircles[pcircles.size() - 1][0],
		//		0, pcircles[pcircles.size() - 1][0]) * 1000) / 1000;
		//}
		/*if (colindex == roi.rows - 1)
		{
			pDlg->rx = 100;
			pDlg->ry = 100;
		}
		else
		{
			pDlg->rx = round((hv_Column11.D() + colindex)*pDlg->rc * 1000) / 1000;
			pDlg->ry = round((hv_Row11.D() + rowindex)*pDlg->rc * 1000) / 1000;
		}*/
		/*else
		{
			pDlg->rx = 100;
			pDlg->ry = 100;
		}
		pcircles.clear();*/
		et = clock();
		//cout << "rx:" << pDlg->rx << endl;
		//cout << "ry:" << pDlg->ry << endl;
		/*if (pDlg->rc.GetCheck() == 1)
			hv_lightdark = "dark";
		else
			hv_lightdark = "light";
		Binaryrchold(pDlg->ho_ImageReduced, &pDlg->ho_Region, "smooth_histo", hv_lightdark, &hv_Usedrchold);*/
		/*FillUp(pDlg->ho_Region, &pDlg->ho_RegionFillUp);
		SmallestRectangle2(pDlg->ho_RegionFillUp, &hv_Row12t, &hv_Column12t, &hv_Phi12t, &hv_Length12t, &hv_Length212t);
		DispCircle(pDlg->hv_WindowID, hv_Row12t, hv_Column12t, 2);*/
		/*Autorchold(pDlg->ho_ImageReduced, &pDlg->ho_Region, 15);
		Connection(pDlg->ho_Region, &pDlg->ho_ConnectedRegions);
		SelectShape(pDlg->ho_ConnectedRegions, &pDlg->ho_SelectedRegions, "area", "and", 3.1415926*pow(0.5 / pDlg->rc, 2), 3.1415926*pow(1 / pDlg->rc, 2));
		SelectShape(pDlg->ho_SelectedRegions, &pDlg->ho_SelectedRegions1, "circularity", "and", 0.6, 1);
		CountObj(pDlg->ho_SelectedRegions1, &hv_Number);
		HTuple end_val30 = hv_Number;
		HTuple step_val30 = 1;
		for (hv_I = 1; hv_I.Continue(end_val30, step_val30); hv_I += step_val30)
		{
			SelectObj(pDlg->ho_SelectedRegions1, &pDlg->ho_ObjectSelected, hv_I);
			OpeningCircle(pDlg->ho_ObjectSelected, &pDlg->ho_RegionOpening, 5.5);
			ClosingCircle(pDlg->ho_RegionOpening, &pDlg->ho_RegionClosing, 5.5);
			GenContourRegionXld(pDlg->ho_RegionClosing, &pDlg->ho_Contours, "border");
			FitCircleContourXld(pDlg->ho_Contours, "ahuber", -1, 2, 0, 3, 2, &hv_Row12t, &hv_Column12t,
				&hv_Radius, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);
		}
		DispCircle(pDlg->hv_WindowID, hv_Row12t, hv_Column12t, hv_Radius);
		cout << hv_Row12t.D() << endl;
		cout << hv_Column12t.D() << endl;
		cout << hv_Radius.D() << endl;*/
		/*if (pDlg->preview.GetCheck() != 1)
		{
			pDlg->rx = round((pDlg->x1 + (hv_Column12t.D() - hv_Width.I() / 2.0)*pDlg->rc) * 1000) / 1000;
			pDlg->ry = round((pDlg->y1 + (hv_Height.I() / 2.0 - hv_Row12t.D())*pDlg->rc) * 1000) / 1000;
			thread1 = 1;
		}*/
		/*if (pDlg->preview.GetCheck() != 1)
		{
			if (hv_Number.I() != 0)
			{
				pDlg->rx = round((pDlg->x1 + (hv_Column12t.D() - hv_Width.I() / 2.0)*pDlg->rc) * 1000) / 1000;
				pDlg->ry = round((pDlg->y1 + (hv_Height.I() / 2.0 - hv_Row12t.D())*pDlg->rc) * 1000) / 1000;
			}
			else
			{
				pDlg->rx = 100;
				pDlg->ry = 100;
			}
			thread1 = 1;
		}*/
		//cout << "rx:" << pDlg->rx << endl;
		//cout << "ry:" << pDlg->ry << endl;
		/*if (pDlg->preview.GetCheck() != 1)
		{*/
		thread1 = 1;
		//}
		pDlg->time = et - st;
		if (trijudg == true)
		{
			trignum++;
			pDlg->triggernum = trignum;
		}
		pDlg->SendMessage(WM_UPDATEDATA, FALSE);
		if (pDlg->trigger.GetCheck() != 1)
			Sleep(pDlg->delay);
	}
	CloseFramegrabber(hv_AcqHandle); // 结束线程
	return 0;
}


UINT CPHZNVisionDlg::StartCameraTest1(LPVOID pParam)
{
	// Local control variables
	HTuple end_val36, step_val36, hv_Classes, hv_lightdark, hv_Width, hv_Height;
	HTuple hv_Information, hv_Values, hv_Number, hv_I, hv_Radius, hv_StartPhi, hv_EndPhi, hv_PointOrder, hv_Area, hv_Row, hv_Column;
	HTuple hv_Mean, hv_Deviation;
	thread2 = 0;
	clock_t st, et;
	Mat roi;
	//int s, rowmax, colmax, rowindex, colindex;
	vector<Vec3f> pcircles;
	bool trijudg;
	//OpenFramegrabber("HMV3rdParty", 0, 0, 0, 0, 0, 0, "progressive", -1, "default", -1, "false", "default", "DahuaTechnology:5M03DF0PAK00003", 0, -1, &hv_AcqHandle1);
	try
	{
		InfoFramegrabber("HMV3rdParty", "device", &hv_Information, &hv_Values);
		OpenFramegrabber("HMV3rdParty", 0, 0, 0, 0, 0, 0, "progressive", -1, "default", -1, "false", "default", hv_Values[1].S(), 0, -1, &hv_AcqHandle1);
	}
	catch (...)
	{
		//CloseFramegrabber(hv_AcqHandle1);
		return 0;
	}
	CPHZNVisionDlg *pDlg = (CPHZNVisionDlg*)pParam;
	pDlg->SendMessage(WM_UPDATEDATA1, TRUE);
	if (pDlg->trigger1.GetCheck() == 1)
	{
		SetFramegrabberParam(hv_AcqHandle1, "TriggerSelector", "FrameStart");
		SetFramegrabberParam(hv_AcqHandle1, "TriggerMode", "On");
		SetFramegrabberParam(hv_AcqHandle1, "TriggerSource", "Line1");
		SetFramegrabberParam(hv_AcqHandle1, "grab_timeout", 1000);
	}
	else
		SetFramegrabberParam(hv_AcqHandle1, "TriggerMode", "Off");
	//相机参数
	SetFramegrabberParam(hv_AcqHandle1, "ExposureTime", pDlg->exp1);
	//SetFramegrabberParam(hv_AcqHandle1, "GainRaw", pDlg->gain);
	//SetFramegrabberParam(hv_AcqHandle1, "Gamma", pDlg->gamma);
	if (judg21 == true)
	{
		GrabImageStart(hv_AcqHandle1, -1);
		//ClearWindow(pDlg->hv_WindowID);
		//SetFramegrabberParam(hv_AcqHandle1, "grab_timeout", -1);
		//GrabImage(&pDlg->ho_Image1, hv_AcqHandle1);   //同步拉流
		GrabImageAsync(&pDlg->ho_Image1, hv_AcqHandle1, -1);
		//GetImageSize(pDlg->ho_Image1, &hv_Width, &hv_Height);
		GetImagePointer1(pDlg->ho_Image1, NULL, NULL, &m_ImageWidth1, &m_ImageHeight1);     //和加载静态图像相同
		judg21 = false;
	}
	if (pDlg->repaint1.GetCheck() == 1)
		judg2 = true;
	//Mat m_Image = HObject2Mat(pDlg->ho_Image);
	SetPart(pDlg->hv_WindowID1, 0, 0, m_ImageHeight1 - 1, m_ImageWidth1 - 1);
	pDlg->StartImageState1 = true;
	SetColor(pDlg->hv_WindowID1, "red");
	SetDraw(pDlg->hv_WindowID1, "margin");
	while (pDlg->StartImageState1)
	{
		//GrabImage(&pDlg->ho_Image1, hv_AcqHandle1);
		trijudg = false;
		if (pDlg->trigger1.GetCheck() == 1)
		{
			SetFramegrabberParam(hv_AcqHandle1, "TriggerSelector", "FrameStart");
			SetFramegrabberParam(hv_AcqHandle1, "TriggerMode", "On");
			SetFramegrabberParam(hv_AcqHandle1, "TriggerSource", "Line1");
			SetFramegrabberParam(hv_AcqHandle1, "grab_timeout", 1000);
			trijudg = true;
		}
		else
			SetFramegrabberParam(hv_AcqHandle1, "TriggerMode", "Off");
		try
		{
			GrabImageAsync(&pDlg->ho_Image1, hv_AcqHandle1, -1);
		}
		catch (...)
		{
			if (pDlg->StartImageState1 == false)
				break;
			else
				continue;
		}
		st = clock();
		//thread2 = 0;
		DispObj(pDlg->ho_Image1, pDlg->hv_WindowID1);
		if (pDlg->repaint1.GetCheck() != 1 && judg2 == true)
			continue;
		//Mat srcImage = HObject2Mat(pDlg->ho_Image);
		/*if (judg2 == true)
			DrawRectangle2(pDlg->hv_WindowID1, &hv_Row21, &hv_Column21, &hv_Phi21, &hv_Length21, &hv_Length221);
		GenRectangle2(&pDlg->ho_Rectangle1, hv_Row21, hv_Column21, hv_Phi21, hv_Length21, hv_Length221);
		DispRectangle2(pDlg->hv_WindowID1, hv_Row21, hv_Column21, hv_Phi21, hv_Length21, hv_Length221);
		ReduceDomain(pDlg->ho_Image1, pDlg->ho_Rectangle1, &pDlg->ho_ImageReduced1);
		if (pDlg->rc1.GetCheck() == 1)
			hv_lightdark = "dark";
		else
			hv_lightdark = "light";
		Binaryrchold(pDlg->ho_ImageReduced1, &pDlg->ho_Region1, "smooth_histo", hv_lightdark, &hv_Usedrchold1);
		FillUp(pDlg->ho_Region1, &pDlg->ho_RegionFillUp1);
		SmallestRectangle2(pDlg->ho_RegionFillUp1, &hv_Row21t, &hv_Column21t, &hv_Phi21t, &hv_Length21t, &hv_Length221t);
		DispCircle(pDlg->hv_WindowID1, hv_Row21t, hv_Column21t, 2);*/
		if (judg2 == true)
		{
			DrawRectangle1(pDlg->hv_WindowID1, &hv_Row21, &hv_Column21, &hv_Row22, &hv_Column22);
			judg2 = false;
		}
		GenRectangle1(&pDlg->ho_Rectangle1, hv_Row21, hv_Column21, hv_Row22, hv_Column22);
		DispRectangle1(pDlg->hv_WindowID1, hv_Row21, hv_Column21, hv_Row22, hv_Column22);
		ReduceDomain(pDlg->ho_Image1, pDlg->ho_Rectangle1, &pDlg->ho_ImageReduced1);
		//CropDomain(pDlg->ho_ImageReduced1, &pDlg->ho_ImageReduced1);
		GaussFilter(pDlg->ho_ImageReduced1, &pDlg->ho_ImageReduced1, 3);
		Intensity(pDlg->ho_Rectangle1, pDlg->ho_ImageReduced1, &hv_Mean, &hv_Deviation);
		if (hv_Deviation.D()<10)
		{
			pDlg->rx1 = 100;
			pDlg->ry1 = 100;
		}
		else
		{
			BinaryThreshold(pDlg->ho_ImageReduced1, &pDlg->ho_Region1, "max_separability", "light", &hv_Usedthreshold1);
			AreaCenter(pDlg->ho_Region1, &hv_Area, &hv_Row, &hv_Column);
			pDlg->rx1 = round(hv_Column.D()*pDlg->rc1 * 1000) / 1000;
			pDlg->ry1 = round(hv_Row.D()*pDlg->rc1 * 1000) / 1000;
			DispCircle(pDlg->hv_WindowID1, hv_Row, hv_Column, HTuple(0.75 / pDlg->rc1));
		}
		//Threshold(pDlg->ho_ImageReduced1, &pDlg->ho_Region1, pDlg->thres1, 255);
		//AreaCenter(pDlg->ho_Region1, &hv_Area, &hv_Row, &hv_Column);
		//if (hv_Row.D() == 0 && hv_Column.D() == 0)
		//{
		//	pDlg->rx1 = 100;
		//	pDlg->ry1 = 100;
		//}
		//else
		//{
		//	//AreaCenter(pDlg->ho_Region, &hv_Area, &hv_Row, &hv_Column);
		//	pDlg->rx1 = round(hv_Column.D()*pDlg->rc1 * 1000) / 1000;
		//	pDlg->ry1 = round(hv_Row.D()*pDlg->rc1 * 1000) / 1000;
		//	DispCircle(pDlg->hv_WindowID1, hv_Row, hv_Column, HTuple(0.75 / pDlg->rc1));
		//}
		//threshold(pDlg->ho_ImageReduced1, &pDlg->ho_ImageReduced1, 128, 255);
		//roi = HObject2Mat1(pDlg->ho_ImageReduced1);
		//threshold(roi, roi, pDlg->thres1, 255, CV_THRESH_BINARY);
		//rowmax = 0; colmax = 0;
		//for (int i = 0; i < roi.rows; i++)
		//{
		//	s = 0;
		//	uchar* pdata = roi.ptr<uchar>(i);
		//	for (int j = 0; j < roi.cols; j++)
		//	{
		//		s += *pdata++;
		//	}
		//	if (s >= rowmax)
		//	{
		//		rowmax = s;
		//		rowindex = i;
		//	}
		//	/*else
		//		break;*/
		//}
		//roi = roi.t();
		//for (int i = 0; i < roi.rows; i++)
		//{
		//	s = 0;
		//	uchar* pdata = roi.ptr<uchar>(i);
		//	for (int j = 0; j < roi.cols; j++)
		//	{
		//		s += *pdata++;
		//	}
		//	if (s >= colmax)
		//	{
		//		colmax = s;
		//		colindex = i;
		//	}
		//	/*else
		//		break;*/
		//}
		//Roberts(pDlg->ho_ImageReduced1, &pDlg->ho_ImageReduced1, "gradient_sum");
		////Autorchold(pDlg->ho_ImageReduced1, &pDlg->ho_ImageReduced1, 15);
		//Binaryrchold(pDlg->ho_ImageReduced1, &pDlg->ho_ImageReduced1, "smooth_histo", "dark", &hv_Usedrchold1);
		//Connection(pDlg->ho_ImageReduced1, &pDlg->ho_ImageReduced1);
		//SelectShape(pDlg->ho_ImageReduced1, &pDlg->ho_ImageReduced1, "area", "and", HTuple(3.1415926*pow(0.5 / pDlg->rc1, 2)), HTuple(3.1415926*pow(1 / pDlg->rc1, 2)));
		////SelectShape(pDlg->ho_ImageReduced1, &pDlg->ho_ImageReduced1, "circularity", "and", 0.6, 1);
		//CountObj(pDlg->ho_ImageReduced1, &hv_Number);
		//ShapeTrans(pDlg->ho_ImageReduced1, &pDlg->ho_ImageReduced1, "inner_circle");
		//AreaCenter(pDlg->ho_ImageReduced1, &hv_Area, &hv_Row22t, &hv_Column22t);
		//hv_Radius = HTuple(pow(hv_Area.D() / 3.1415926, 0.5));
		//DispCircle(pDlg->hv_WindowID1, hv_Row22t, hv_Column22t, hv_Radius);
		//roi = HObject2Mat1(pDlg->ho_ImageReduced1);
		//HoughCircles(roi, pcircles, CV_HOUGH_GRADIENT, 1, 10, pDlg->rc1, 35/*, int(0.5 / pDlg->rc1), int(1 / pDlg->rc1)*/);
		//for (int i = 0; i < pcircles.size(); i++)
		//{
		//	//cout << pcircles[i][1] << endl;
		//	//cout << pcircles[i][0] << endl;
		//	//cout << pcircles[i][2] << endl;
		//	DispCircle(pDlg->hv_WindowID1, HTuple(pcircles[i][1]), HTuple(pcircles[i][0]), HTuple(pcircles[i][2]));
		//}
		//DispCircle(pDlg->hv_WindowID1, HTuple(hv_Row21.D() + rowindex), HTuple(hv_Column21.D() + colindex), HTuple(0.75 / pDlg->rc1));
		//if (pcircles.size() != 0)
		//{
		//	//pDlg->rx1 = round((pDlg->x2 + (pcircles[pcircles.size() - 1][0] - hv_Width.I() / 2.0)*pDlg->rc1) * 1000) / 1000;
		//	//pDlg->ry1 = round((pDlg->y2 + (hv_Height.I() / 2.0 - pcircles[pcircles.size() - 1][1])*pDlg->rc1) * 1000) / 1000;
		//	pDlg->rx1 = round(DistancePP(hv_CameraParameters, hv_CameraPose, TargetThickness,
		//		pcircles[pcircles.size() - 1][1], pcircles[pcircles.size() - 1][0],
		//		pcircles[pcircles.size() - 1][1], 0) * 1000) / 1000;
		//	pDlg->ry1 = round(DistancePP(hv_CameraParameters, hv_CameraPose, TargetThickness,
		//		pcircles[pcircles.size() - 1][1], pcircles[pcircles.size() - 1][0],
		//		0, pcircles[pcircles.size() - 1][0]) * 1000) / 1000;
		//}
		//else
		//{
		//	pDlg->rx1 = 100;
		//	pDlg->ry1 = 100;
		//}
		//pcircles.clear();
		/*if (colindex == roi.rows - 1)
		{
			pDlg->rx1 = 100;
			pDlg->ry1 = 100;
		}
		else
		{
			pDlg->rx1 = round((hv_Column21.D() + colindex)*pDlg->rc1 * 1000) / 1000;
			pDlg->ry1 = round((hv_Row21.D() + rowindex)*pDlg->rc1 * 1000) / 1000;
		}*/
		et = clock();
		//cout << "rx1:" << pDlg->rx1 << endl;
		//cout << "ry1:" << pDlg->ry1 << endl;
		/*if (pDlg->rc1.GetCheck() == 1)
			hv_lightdark = "dark";
		else
			hv_lightdark = "light";
		Binaryrchold(pDlg->ho_ImageReduced1, &pDlg->ho_Region1, "smooth_histo", hv_lightdark, &hv_Usedrchold1);*/
		/*FillUp(pDlg->ho_Region1, &pDlg->ho_RegionFillUp1);
		SmallestRectangle2(pDlg->ho_RegionFillUp1, &hv_Row22t, &hv_Column22t, &hv_Phi22t, &hv_Length22t, &hv_Length222t);
		DispCircle(pDlg->hv_WindowID1, hv_Row22t, hv_Column22t, 2);*/
		/*Autorchold(pDlg->ho_ImageReduced1, &pDlg->ho_Region1, 15);
		Connection(pDlg->ho_Region1, &pDlg->ho_ConnectedRegions1);
		SelectShape(pDlg->ho_ConnectedRegions1, &pDlg->ho_SelectedRegions2, "area", "and", 3.1415926*pow(0.5 / pDlg->rc1, 2), 3.1415926*pow(1 / pDlg->rc1, 2));
		SelectShape(pDlg->ho_SelectedRegions2, &pDlg->ho_SelectedRegions3, "circularity", "and", 0.6, 1);
		CountObj(pDlg->ho_SelectedRegions3, &hv_Number);
		HTuple end_val30 = hv_Number;
		HTuple step_val30 = 1;
		for (hv_I = 1; hv_I.Continue(end_val30, step_val30); hv_I += step_val30)
		{
			SelectObj(pDlg->ho_SelectedRegions3, &pDlg->ho_ObjectSelected1, hv_I);
			OpeningCircle(pDlg->ho_ObjectSelected1, &pDlg->ho_RegionOpening1, 5.5);
			ClosingCircle(pDlg->ho_RegionOpening1, &pDlg->ho_RegionClosing1, 5.5);
			GenContourRegionXld(pDlg->ho_RegionClosing1, &pDlg->ho_Contours1, "border");
			FitCircleContourXld(pDlg->ho_Contours1, "ahuber", -1, 2, 0, 3, 2, &hv_Row22t, &hv_Column22t,
				&hv_Radius, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);
		}
		DispCircle(pDlg->hv_WindowID1, hv_Row22t, hv_Column22t, hv_Radius);
		cout << hv_Row22t.D() << endl;
		cout << hv_Column22t.D() << endl;
		cout << hv_Radius.D() << endl;*/
		//cout << "rx:" << pDlg->rx << endl;
		//cout << "ry:" << pDlg->ry << endl;
		/*if (pDlg->preview1.GetCheck() != 1)
		{*/
		thread2 = 1;
		//}
		/*if (pDlg->preview1.GetCheck() != 1)
		{
			if (hv_Number.I() != 0)
			{
				pDlg->rx = round((pDlg->x1 + (hv_Column22t.D() - hv_Width.I() / 2.0)*pDlg->rc1) * 1000) / 1000;
				pDlg->ry = round((pDlg->y1 + (hv_Height.I() / 2.0 - hv_Row22t.D())*pDlg->rc1) * 1000) / 1000;
			}
			else
			{
				pDlg->rx1 = 100;
				pDlg->ry1 = 100;
			}
			thread2 = 1;
		}*/
		pDlg->time1 = et - st;
		if (trijudg == true)
		{
			trignum1++;
			pDlg->triggernum1 = trignum1;
		}
		pDlg->SendMessage(WM_UPDATEDATA, FALSE);
		if (pDlg->trigger1.GetCheck() != 1)
			Sleep(pDlg->delay1);
	}
	CloseFramegrabber(hv_AcqHandle1); // 结束线程
	return 0;
}

UINT CPHZNVisionDlg::StartCameraTest2(LPVOID pParam)
{
	CPHZNVisionDlg *pDlg = (CPHZNVisionDlg*)pParam;
	/*if (pDlg->trigger.GetCheck() == 1 || pDlg->trigger1.GetCheck() == 1)
		pDlg->StartImageState2 = true;
	else
		pDlg->StartImageState2 = false;*/
	pDlg->SendMessage(WM_UPDATEDATA2, TRUE);
	modbus* mb = new modbus("192.168.0.250", 502);
	// set slave id
	mb->modbus_set_slave_id(1);
	// connect with the server
	mb->modbus_connect();
	while (true)
	{
		if (pDlg->trigger.GetCheck() == 1 || pDlg->trigger1.GetCheck() == 1)
		{
			cout << "thread1:" << thread1 << endl;
			cout << "thread2:" << thread2 << endl;
			if (thread1 == 1 && thread2 == 1)
			{
				uint16_t write_registers[8] = { int(pDlg->rx), abs(pDlg->rx - int(pDlg->rx)) * 1000, int(pDlg->ry), abs(pDlg->ry - int(pDlg->ry)) * 1000, int(pDlg->rx1), abs(pDlg->rx1 - int(pDlg->rx1)) * 1000, int(pDlg->ry1), abs(pDlg->ry1 - int(pDlg->ry1)) * 1000 };
				mb->modbus_write_registers(0001, 8, write_registers);
				thread1 = 0; thread2 = 0;
			}
			if (thread1 == 1 && thread2 == 0)
			{
				uint16_t write_registers[4] = { int(pDlg->rx), abs(pDlg->rx - int(pDlg->rx)) * 1000, int(pDlg->ry), abs(pDlg->ry - int(pDlg->ry)) * 1000 };
				mb->modbus_write_registers(0001, 4, write_registers);
				thread1 = 0;
			}
		}
	}
	return 0;
}


LRESULT CPHZNVisionDlg::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
	UpdateData(wParam);
	return 0;
}

LRESULT CPHZNVisionDlg::OnUpdateData1(WPARAM wParam, LPARAM lParam)
{
	UpdateData(wParam);
	return 0;
}

LRESULT CPHZNVisionDlg::OnUpdateData2(WPARAM wParam, LPARAM lParam)
{
	UpdateData(wParam);
	return 0;
}

void CPHZNVisionDlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	StartImageState = flag = false;
	//GetDlgItem(IDC_CHECK3)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK3)->EnableWindow(TRUE);
}


void CPHZNVisionDlg::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码
	StartImageState = StartImageState1 = StartImageState2 = false;
	//AfxGetMainWnd()->SendMessage(WM_CLOSE);
	HANDLE Process = GetCurrentProcess();
	TerminateProcess(Process, 0);
}




void CPHZNVisionDlg::OnBnClickedButton4()
{
	// TODO:  在此添加控件通知处理程序代码
	StartImageState1 = flag1 = false;
	//GetDlgItem(IDC_CHECK1)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK1)->EnableWindow(TRUE);
}


//void CPHZNVisionDlg::OnSize(UINT nType, int cx, int cy)
//{
//	CDialogEx::OnSize(nType, cx, cy);
//	// TODO:  在此处添加消息处理程序代码
//	if (nType != SIZE_MINIMIZED)
//	{
//		CWnd *pWnd = GetWindow(GW_CHILD);
//		while (pWnd)
//		{
//			changeControlSize(pWnd, cx, cy);
//			pWnd = pWnd->GetNextWindow();
//		}
//		//重新获取界面大小
//		GetClientRect(&m_rect);
//	}
//}


//void CPHZNVisionDlg::changeControlSize(CWnd* pWnd, int x, int y)
//
//{
//	if (pWnd != NULL)
//	{
//		CRect rect;
//		pWnd->GetWindowRect(&rect);
//		ScreenToClient(&rect);
//
//		//保存原高度，供下拉框使用
//		int height = rect.Height();
//		rect.left = rect.left*x / m_rect.Width();
//		rect.top = rect.top*y / m_rect.Height();
//		rect.right = rect.right*x / m_rect.Width();
//		rect.bottom = rect.bottom*y / m_rect.Height();
//
//		//如果是下拉框，则不改变其高度
//		TCHAR szClass[65] = { 0 };
//		if (GetClassName(pWnd->m_hWnd, szClass, 64) && _tcsicmp(szClass, _T("COMBOBOX")) == 0)
//		{
//			pWnd->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), height, SWP_NOZORDER | SWP_SHOWWINDOW);
//		}
//		else
//
//			pWnd->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);
//	}
//}


