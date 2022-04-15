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

	// ������ȡ�㷨�����������Ӷ���
	Ptr<SIFT> featureDetector = SIFT::create();
	Ptr<SIFT> featureDescriptor = SIFT::create();
	HOGDescriptor hog;

	// SVMѵ��������
	Ptr<ml::SVM> svmModel;
	Mat global_centers; // BOWģ�͵ľ������

	// ������ת����(���ݴʵ�)
	void Img2FeatureVec(Mat& inputArray, Mat& outputArray, Mat& cluster_centers, Ptr<FeatureDetector> detector, Ptr<DescriptorExtractor> descriptor);

public:


	MLDetector();
	~MLDetector();
	MLDetector(int numClusters);
	MLDetector(string objImgsPath, string trainImgsPath, int numClusters);
	
	// ģ�ͳ�ʼ��
	void initBOWModel();
	void initHOGModel();
	// ģ�ʹ洢
	void saveModel(string saveModelPath);
	// ģ��ѵ��
	void train(FeatureFlag featureFlag);
	// ģ��Ԥ��
	vector<Rect> predict(const Mat& inputArray, string saveModelPath, float nmsThreshold, float confThreshold, FeatureFlag featureFlag);

};

