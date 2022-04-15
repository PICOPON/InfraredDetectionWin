
// InfraredDetectionWinDlg.h: 头文件
//

#pragma once
//

#include "FastSaliency.h"
#include "MLDetector.h"
#include <opencv2/highgui/highgui_c.h>

using namespace cv;
using namespace std;
// CInfraredDetectionWinDlg 对话框
class CInfraredDetectionWinDlg : public CDialogEx
{
// 构造
public:
	CInfraredDetectionWinDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INFRAREDDETECTIONWIN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	enum class WorkMode { VisibleLight, Infrared, MANUAL };

	HICON m_hIcon;

	Mat srcImg;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:
	// 图像设备资源
	VideoCapture cap;

	WorkMode workMode;
	MLDetector* mlDetector;

	float MLPredictThreshold; 
	float MLNMSThreshold; 
	float DLClassConfThreshold;
	float DLNMSConfThreshold;
	float DLObjConfThreshold;
	boolean CapOpenFlag;

	CComboBox CBWorkMode;
	CComboBox CBFeatureSelect;
	HBRUSH m_hbrush;

	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnEnChangeEdit1();
	CProgressCtrl CProgress_Train;
	CListBox Clist_InfoLog;

	afx_msg void OnBnClickedButton1();
	CEdit EditDLClassConfThreshold;
	CEdit EditDLNMSThreshold;
	CEdit EditDLObjConfThreshold;
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	CButton Button_DLPrdt;
	CButton Button_DLExit;
	CButton Button_VSaliencyPrdt;
	CButton Button_VSaliencyExit;
	CButton Button_MLStartTrain;
	CButton Button_MLSaveModel;
	CButton Button_MLPredict;
	CEdit MLNMSEdit;
	CEdit MLConfEdit;
	afx_msg void OnEnChangeEdit5();
};
