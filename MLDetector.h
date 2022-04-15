#pragma once


using namespace std;
using namespace cv;

enum FeatureFlag { SIFTFlag, HOGFlag };

class MLDetector
{
private:
	int numClusters;
	string objImgsPath;
	string trainImgsPath;

	// 特征提取算法和特征描述子定义
	Ptr<SIFT> featureDetector = SIFT::create();
	Ptr<SIFT> featureDescriptor = SIFT::create();
	HOGDescriptor hog;

	// SVM训练器定义
	Ptr<ml::SVM> svmModel;
	Mat global_centers; // BOW模型的聚类簇数

	// 词向量转化器(根据词典)
	void Img2FeatureVec(Mat& inputArray, Mat& outputArray, Mat& cluster_centers, Ptr<FeatureDetector> detector, Ptr<DescriptorExtractor> descriptor);

public:


	MLDetector();
	~MLDetector();
	MLDetector(int numClusters);
	MLDetector(string objImgsPath, string trainImgsPath, int numClusters);
	
	// 模型初始化
	void initBOWModel();
	void initHOGModel();
	// 模型存储
	void saveModel(string saveModelPath);
	// 模型训练
	void train(FeatureFlag featureFlag);
	// 模型预测
	vector<Rect> predict(const Mat& inputArray, string saveModelPath, float nmsThreshold, float confThreshold, FeatureFlag featureFlag);

};

