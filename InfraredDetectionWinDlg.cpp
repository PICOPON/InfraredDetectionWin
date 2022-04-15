
// InfraredDetectionWinDlg.cpp: 实现文件


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "pch.h"
#include "framework.h"
#include "InfraredDetectionWin.h"
#include "InfraredDetectionWinDlg.h"
#include "afxdialogex.h"
#include "YoloPredict.h"


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CInfraredDetectionWinDlg 对话框



CInfraredDetectionWinDlg::CInfraredDetectionWinDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INFRAREDDETECTIONWIN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInfraredDetectionWinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, CBWorkMode);
	DDX_Control(pDX, IDC_COMBO1, CBFeatureSelect);
	DDX_Control(pDX, IDC_PROGRESS1, CProgress_Train);
	DDX_Control(pDX, IDC_LIST_Log, Clist_InfoLog);
	DDX_Control(pDX, IDC_EDIT2, EditDLClassConfThreshold);
	DDX_Control(pDX, IDC_EDIT3, EditDLNMSThreshold);
	DDX_Control(pDX, IDC_EDIT4, EditDLObjConfThreshold);
	DDX_Control(pDX, IDC_BUTTON1, Button_DLPrdt);
	DDX_Control(pDX, IDC_BUTTON10, Button_DLExit);
	DDX_Control(pDX, IDC_BUTTON8, Button_VSaliencyPrdt);
	DDX_Control(pDX, IDC_BUTTON9, Button_VSaliencyExit);
	DDX_Control(pDX, IDC_BUTTON2, Button_MLStartTrain);
	DDX_Control(pDX, IDC_BUTTON3, Button_MLSaveModel);
	DDX_Control(pDX, IDC_BUTTON7, Button_MLPredict);
	DDX_Control(pDX, IDC_EDIT5, MLNMSEdit);
	DDX_Control(pDX, IDC_EDIT1, MLConfEdit);
}

BEGIN_MESSAGE_MAP(CInfraredDetectionWinDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON8, &CInfraredDetectionWinDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON5, &CInfraredDetectionWinDlg::OnBnClickedButton5)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CInfraredDetectionWinDlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON2, &CInfraredDetectionWinDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CInfraredDetectionWinDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON7, &CInfraredDetectionWinDlg::OnBnClickedButton7)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CInfraredDetectionWinDlg::OnCbnSelchangeCombo1)
	ON_EN_CHANGE(IDC_EDIT1, &CInfraredDetectionWinDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CInfraredDetectionWinDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT2, &CInfraredDetectionWinDlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CInfraredDetectionWinDlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &CInfraredDetectionWinDlg::OnEnChangeEdit4)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON9, &CInfraredDetectionWinDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CInfraredDetectionWinDlg::OnBnClickedButton10)
	ON_EN_CHANGE(IDC_EDIT5, &CInfraredDetectionWinDlg::OnEnChangeEdit5)
END_MESSAGE_MAP()


// CInfraredDetectionWinDlg 消息处理程序

BOOL CInfraredDetectionWinDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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
	ShowWindow(SW_NORMAL);
	this->m_hbrush = CreateSolidBrush(RGB(255, 255, 255));

	// 界面初始化

	CBWorkMode.AddString(_T("红外检测"));
	CBWorkMode.AddString(_T("可见光检测"));
	CBWorkMode.AddString(_T("人工识别"));
	CBWorkMode.SetCurSel(0);


	CBFeatureSelect.AddString(_T("SIFT特征"));
	CBFeatureSelect.AddString(_T("HOG特征"));
	CBFeatureSelect.SetCurSel(0);

	// SVM分类阈值初始化
	MLConfEdit.SetWindowTextW(CString("0.6"));
	MLNMSEdit.SetWindowTextW(CString("0.1"));

	// 深度学习分类阈值初始化
	EditDLClassConfThreshold.SetWindowTextW(CString("0.3"));
	EditDLNMSThreshold.SetWindowTextW(CString("0.5"));
	EditDLObjConfThreshold.SetWindowTextW(CString("0.3"));


	Button_VSaliencyPrdt.EnableWindow(false);
	Button_VSaliencyExit.EnableWindow(false);

	Button_DLPrdt.EnableWindow(false);
	Button_DLExit.EnableWindow(false);

	Button_MLStartTrain.EnableWindow(false);
	Button_MLPredict.EnableWindow(false);
	Button_MLSaveModel.EnableWindow(false);

	CProgress_Train.SetRange(0, 100);
	CProgress_Train.SetPos(0);

	Clist_InfoLog.AddString(CString("系统初始化完成"));
	Clist_InfoLog.SetCurSel(Clist_InfoLog.GetCount() - 1);

	this->CapOpenFlag = true;


	if(!PathFileExists(CString("./LocalStorage"))) CreateDirectory(CString("./LocalStorage"), NULL);
	if(!PathFileExists(CString("./objSet"))) CreateDirectory(CString("./objSet"), NULL);
	if(!PathFileExists(CString("./trainSet"))) CreateDirectory(CString("./trainSet"), NULL);
	if(!PathFileExists(CString("./trainSet/trainImgs"))) CreateDirectory(CString("./trainSet/trainImgs"), NULL);
	if(!PathFileExists(CString("./trainSet/labels.txt"))) CreateFile(CString("./trainSet/labels.txt"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CInfraredDetectionWinDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

//  如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CInfraredDetectionWinDlg::OnPaint()
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
HCURSOR CInfraredDetectionWinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}






void CInfraredDetectionWinDlg::OnBnClickedButton8()
{
	// 快速视觉显著性检测算法
	this->CapOpenFlag = true;
	Button_VSaliencyExit.EnableWindow(true);

	Clist_InfoLog.AddString(CString("视觉显著性检测算法启动"));
	Clist_InfoLog.SetCurSel(Clist_InfoLog.GetCount() - 1);

	destroyAllWindows();
	namedWindow("view", WINDOW_AUTOSIZE);

	HWND hWnd = (HWND)cvGetWindowHandle("view");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_STATIC_INFRERED)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);

	CRect rect;
	GetDlgItem(IDC_STATIC_INFRERED)->GetClientRect(&rect);
	Rect dst(rect.left, rect.top, rect.right, rect.bottom);

	// --------------------------   图像预处理  -----------------------------
	FastSaliency fastSaliency;
	// 空间域中值滤波消除椒盐噪声
	
	// this->srcImg = fastSaliency.preProcess(this->srcImg, 3);

	// this->srcImg = imread("./test2.JPG",ImreadModes::IMREAD_GRAYSCALE);


	Mat SaliencyMap1, SaliencyMap2, SaliencyMap3, SaliencyMap4;

	Mat frame;
	if (!cap.isOpened()) {
		cap.open(0);
	}
	while (cap.read(frame)) {
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		Mat outImg;
		vector<Mat> saliencyMaps;

		// ---------- 基于facet 模型的一阶方向导数预处理，得到 Derivative Subbands图 --------------------------
		// 分别计算0，45，90，135角度的方向导数
		SaliencyMap1 = fastSaliency.facetDerivative(frame, 0);
		SaliencyMap2 = fastSaliency.facetDerivative(frame, 45);
		SaliencyMap3 = fastSaliency.facetDerivative(frame, 90);
		SaliencyMap4 = fastSaliency.facetDerivative(frame, 135);

		saliencyMaps.push_back(SaliencyMap1);
		saliencyMaps.push_back(SaliencyMap2);
		saliencyMaps.push_back(SaliencyMap3);
		saliencyMaps.push_back(SaliencyMap4);

		// 多特征图融合
		for (auto i : saliencyMaps) {
			
		}
		// ----------------------  fast-saliency 显著性检测得到 Saliency map  ---------------------------------

		outImg = fastSaliency.saliencyMap(SaliencyMap1);

		/*
		// 提取可疑轮廓
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;

		findContours(outImg, contours, hierarchy, RetrievalModes::RETR_TREE, ContourApproximationModes::CHAIN_APPROX_SIMPLE, Point());

		// --------------------------    特征提取分类  -----------------------------
		// 轮廓特征提取，先验知识筛选

		// --------------------------    结果输出展示  -----------------------------
		// 检测结果图

		Clist_InfoLog.AddString(_T("共检测到")+ CString(to_string(contours.size()).c_str()) + _T("个可疑目标"));
		Clist_InfoLog.SetCurSel(Clist_InfoLog.GetCount() - 1);

		// 绘制轮廓
		drawContours(outImg, contours, -1, Scalar(0));

		for (int i = 0; i < contours.size(); i++) {
			Rect rect = boundingRect(contours[i]);
			rectangle(outImg, rect, Scalar(255), 1, 8);
			putText(outImg, to_string(i), Point(rect.x, rect.y), 1, 1, Scalar(255), 1);
		}
		*/
		resize(outImg, outImg, cv::Size(rect.Width(), rect.Height()));
		imshow("view", outImg);
		if (waitKey(30) == 'q' || !this->CapOpenFlag)
			break;
	}
	cap.release();
	destroyAllWindows();
}


void CInfraredDetectionWinDlg::OnBnClickedButton5()
{
	// 系统启动
	// 检查是否存在深度学习已训练模型
	if (PathFileExists(CString("class.names")) && PathFileExists(CString("coco.onnx"))) {
		Button_DLPrdt.EnableWindow(true);
	}else {
		MessageBox(CString("深度学习模型未发现"));
	}

	Clist_InfoLog.AddString(CString("系统启动完成"));
	Clist_InfoLog.SetCurSel(Clist_InfoLog.GetCount() - 1);

	Button_VSaliencyPrdt.EnableWindow(true);
	Button_MLStartTrain.EnableWindow(true);

	// 检查是否存在深度学习已训练模型
	if (PathFileExists(CString("./LocalStorage/SVMModel.xml")) && PathFileExists(CString("./LocalStorage/HOGModel.xml"))) {
		Button_MLPredict.EnableWindow(true);
	}
	else {
		MessageBox(CString("SVM训练模型未发现"));
	}

}


void CInfraredDetectionWinDlg::OnCbnSelchangeCombo2()
{
	// 工作模式选取
	switch (CBWorkMode.GetCurSel()) {
	case 0:
		this->workMode = WorkMode::Infrared;
		Clist_InfoLog.AddString(CString("红外模式"));
		break;
	case 1:
		this->workMode = WorkMode::VisibleLight;
		Clist_InfoLog.AddString(CString("可见光模式"));
		break;
	case 2:
		this->workMode = WorkMode::MANUAL;
		Clist_InfoLog.AddString(CString("人工模式"));
		break;
	}

	Clist_InfoLog.SetCurSel(Clist_InfoLog.GetCount() - 1);
}


void CInfraredDetectionWinDlg::OnBnClickedButton2()
{
	Clist_InfoLog.AddString(CString("模型训练准备中......"));

	Button_MLStartTrain.EnableWindow(false);

	try {
		if (this->mlDetector == nullptr) {
			this->mlDetector = new MLDetector();
		}
		MessageBox(CString("机器学习模型训练开始......"));

		// 机器学习模型训练
		switch (CBFeatureSelect.GetCurSel() + 1)
		{
		case 1:
			Clist_InfoLog.AddString(CString("SIFT特征模型训练开始......"));
			this->mlDetector->initBOWModel();
			CProgress_Train.SetPos(10);
			this->mlDetector->train(FeatureFlag::SIFTFlag);
			CProgress_Train.SetPos(100);
			Clist_InfoLog.AddString(CString("SVM分类器训练完成"));
			MessageBox(CString("SIFT特征模型训练完成"));
			break;
		case 2:
			Clist_InfoLog.AddString(CString("HOG特征模型训练开始......"));
			this->mlDetector->initHOGModel();
			CProgress_Train.SetPos(10);
			this->mlDetector->train(FeatureFlag::HOGFlag);
			CProgress_Train.SetPos(100);
			Clist_InfoLog.AddString(CString("SVM分类器训练完成"));
			MessageBox(CString("HOG特征模型训练完成"));
			break;
		default:
			MessageBox(CString("选项异常"));
			break;
		}
		Button_MLSaveModel.EnableWindow(true);
	}
	catch (const std::exception& e) {
		MessageBox(CString("数据集未准备好！"));
	}
	Button_MLStartTrain.EnableWindow(true);
	Clist_InfoLog.SetCurSel(Clist_InfoLog.GetCount() - 1);
}


void CInfraredDetectionWinDlg::OnBnClickedButton3()
{
	switch (CBFeatureSelect.GetCurSel() + 1)
	{
	case 1:
		try {
			// 模型存储
			this->mlDetector->saveModel("./LocalStorage/SVMModel.xml");
			Clist_InfoLog.AddString(CString("SIFT特征SVM模型已存储"));
			MessageBox(CString("SIFT特征SVM模型已存储"));
		}
		catch (const std::exception& e) {
			MessageBox(CString(e.what()));
		}
		break;
	case 2:
		try {
			// 模型存储
			this->mlDetector->saveModel("./LocalStorage/HOGModel.xml");
			Clist_InfoLog.AddString(CString("HOG特征SVM模型已存储"));
			MessageBox(CString("HOG特征SVM模型已存储"));
		}
		catch (const std::exception& e) {
			MessageBox(CString(e.what()));
		}
		break;
	default:
		MessageBox(CString("选项异常"));
		break;
	}
	Button_MLPredict.EnableWindow(true);

	Clist_InfoLog.SetCurSel(Clist_InfoLog.GetCount() - 1);
}


void CInfraredDetectionWinDlg::OnBnClickedButton7()
{
	
	Clist_InfoLog.AddString(CString("模型预测开始......"));

	Mat src;

	// applyColorMap(this->srcImg, srcRGB, ColormapTypes::COLORMAP_JET);

	if (cap.isOpened()) {
		cap.read(src);
	}
	else {
		cap.open(0);
		cap.read(src);
	}
	vector<Rect> objRcts;

	if (this->mlDetector == nullptr) {
		this->mlDetector = new MLDetector();
	}

	switch (CBFeatureSelect.GetCurSel() + 1)
	{
	case 1:
		MessageBox(CString("SIFT特征模型预测"));
		try {
			objRcts = this->mlDetector->predict(src, "./LocalStorage/SVMModel.xml",this->MLNMSThreshold, this->MLPredictThreshold, FeatureFlag::SIFTFlag);
		}
		catch (const std::exception& e) {
			MessageBox(CString(e.what()));
		}
		break;
	case 2:
		MessageBox(CString("HOG特征模型预测"));
		try {
			objRcts = this->mlDetector->predict(src, "./LocalStorage/HOGModel.xml", this->MLNMSThreshold, this->MLPredictThreshold, FeatureFlag::HOGFlag);
		}
		catch (const std::exception& e) {
			MessageBox(CString(e.what()));
		}
		break;
	default:
		MessageBox(CString("选项异常"));
		break;
	}

	for (auto r : objRcts) {
		rectangle(src, r, Scalar(100, 0, 200), 2);
	}

	Clist_InfoLog.SetCurSel(Clist_InfoLog.GetCount() - 1);

	imshow("out", src);
	waitKey(0);

	cap.release();
}


void CInfraredDetectionWinDlg::OnCbnSelchangeCombo1()
{
	// 机器学习特征选取

	CBFeatureSelect.SetCurSel(CBFeatureSelect.GetCurSel());

	switch (CBFeatureSelect.GetCurSel())
	{
	case 0:
		  Clist_InfoLog.AddString(CString("SIFT特征检测模式"));
		  break;
	case 1:
		  Clist_InfoLog.AddString(CString("HOG特征检测模式"));
		  break;
	default:
		  Clist_InfoLog.AddString(CString("模式异常"));
		  break;
	}

	Clist_InfoLog.SetCurSel(Clist_InfoLog.GetCount() - 1);

}


void CInfraredDetectionWinDlg::OnBnClickedButton1()
{
	this->CapOpenFlag = true;
	Button_DLExit.EnableWindow(true);

	Clist_InfoLog.AddString(CString("深度学习检测算法启动"));
	Clist_InfoLog.SetCurSel(Clist_InfoLog.GetCount() - 1);

	destroyAllWindows();
	namedWindow("view", WINDOW_AUTOSIZE);

	HWND hWnd = (HWND)cvGetWindowHandle("view");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_STATIC_COMSPIC)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);

	CRect rect;
	GetDlgItem(IDC_STATIC_COMSPIC)->GetClientRect(&rect);
	Rect dst(rect.left, rect.top, rect.right, rect.bottom);


	// 深度学习模型调用
	Net_config yolo_nets = { this->DLClassConfThreshold, this->DLNMSConfThreshold, this->DLObjConfThreshold,"coco.onnx" };
	YOLO yolo_model(yolo_nets);
	// string imgpath = "detection.jpg";
	// Mat srcimg = imread(imgpath);
	// yolo_model.detect(srcimg);
	Mat frame;
	if (!cap.isOpened()) {
		cap.open(0);
	}
	while (cap.read(frame))	{
		//static const string kWinName = "Deep learning object detection in ONNXRuntime";
		//namedWindow(kWinName, WINDOW_NORMAL);
		yolo_model.detect(frame);
		resize(frame, frame, cv::Size(rect.Width(), rect.Height()));
		imshow("view", frame);
		if (waitKey(30) == 'q' || !this->CapOpenFlag)
			break;
	}
	cap.release();
	destroyAllWindows();
}

void CInfraredDetectionWinDlg::OnEnChangeEdit1()
{
	// 机器学习置信度阈值更改
	CString T;
	MLConfEdit.GetWindowTextW(T);
	this->MLPredictThreshold = _ttof(T);
}

void CInfraredDetectionWinDlg::OnEnChangeEdit5()
{
	// 机器学习NMS阈值更改
	CString T;
	MLNMSEdit.GetWindowTextW(T);
	this->MLNMSThreshold = _ttof(T);
}


void CInfraredDetectionWinDlg::OnEnChangeEdit2()
{
	// 深度学习类别置信度阈值更改
	CString T;
	EditDLClassConfThreshold.GetWindowTextW(T);
	this->DLClassConfThreshold = _ttof(T);

	//Clist_InfoLog.AddString(CString(" 深度学习类别置信度阈值已修改！"));
	//Clist_InfoLog.SetCurSel(Clist_InfoLog.GetCount() - 1);
}


void CInfraredDetectionWinDlg::OnEnChangeEdit3()
{
	// 深度学习非极大值抑制置信度阈值更改
	CString T;
	EditDLNMSThreshold.GetWindowTextW(T);
	this->DLNMSConfThreshold = _ttof(T);

	//Clist_InfoLog.AddString(CString("深度学习非极大值抑制置信度阈值更改！"));
	//Clist_InfoLog.SetCurSel(Clist_InfoLog.GetCount() - 1);
}


void CInfraredDetectionWinDlg::OnEnChangeEdit4()
{
	// 深度学习目标置信度阈值更改
	CString T;
	EditDLObjConfThreshold.GetWindowTextW(T);
	this->DLObjConfThreshold = _ttof(T);

	//Clist_InfoLog.AddString(CString("深度学习目标置信度阈值更改！"));
	//Clist_InfoLog.SetCurSel(Clist_InfoLog.GetCount() - 1);
}


void CInfraredDetectionWinDlg::OnClose()
{
	// 退出前消息处理
	this->CapOpenFlag = false;
	
	if (cap.isOpened()) {
		cap.release();
		destroyAllWindows();
	}

	if (this->mlDetector != nullptr) {
		this->mlDetector->~MLDetector();
	}

	CDialogEx::OnClose();
}


HBRUSH CInfraredDetectionWinDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_LISTBOX) {
		pDC->SetBkColor(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 0, 0));
		return m_hbrush;
	}
	else {
		HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
		return hbr;
	}
}


void CInfraredDetectionWinDlg::OnBnClickedButton9()
{
	// 视觉显著性检测退出代码
	this->CapOpenFlag = false;
}


void CInfraredDetectionWinDlg::OnBnClickedButton10()
{
	// 机器学习检测退出代码
	this->CapOpenFlag = false;
}

