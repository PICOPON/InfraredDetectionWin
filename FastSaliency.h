#pragma once
using namespace std;
using namespace cv;

class FastSaliency
{
private:
	Mat maxEntropySegMentation(Mat inputImage);
	float caculateCurrentEntropy(Mat hist, int threshold);

public:
	Mat preProcess(const Mat& inputArray,int kSize);
	Mat facetDerivative(const Mat& inputArray, double alpha);
	Mat saliencyMap(const Mat& inputArray);
	void mapFusion(vector<Mat> inputArrays, Mat& outputArray);
};

