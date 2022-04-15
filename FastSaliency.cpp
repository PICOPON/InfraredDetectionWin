#include "pch.h"
#include "FastSaliency.h"

Mat FastSaliency::preProcess(const Mat& inputArray, int kSize)
{
    medianBlur(inputArray, inputArray, kSize);
    return inputArray;
}

Mat FastSaliency::facetDerivative(const Mat& inputArray, double alpha)
{
	Mat w3, w9, w10, k2, k3, k7, k8, k9, k10;

	Mat w2 = (Mat_<double>(5, 5) << -2, -2, -2, -2, -2,
									-1, -1, -1, -1, -1,
									0, 0, 0, 0, 0,
									1, 1, 1, 1, 1,
									2, 2, 2, 2, 2) / 50;
	Mat w7 = (Mat_<double>(5, 5) << -1, -1, -1, -1, -1,
									2, 2, 2, 2, 2,
									0, 0, 0, 0, 0,
									-2, -2, -2, -2, -2,
									1, 1, 1, 1, 1) / 60;
	Mat w8 = (Mat_<double>(5, 5) << -4, -2, 0, 2, 4,
									2, 1, 0, -1, -2,
									4, 2, 0, -2, -4,
									2, 1, 0, -1, -2,
									-4, -2, 0, 2, 4) / 140;
	transpose(w2, w3);
	transpose(w8, w9);
	transpose(w7, w10);

	filter2D(inputArray, k2, inputArray.depth(), w2);
	filter2D(inputArray, k3, inputArray.depth(), w3);
	filter2D(inputArray, k7, inputArray.depth(), w7);
	filter2D(inputArray, k8, inputArray.depth(), w8);
	filter2D(inputArray, k9, inputArray.depth(), w9);
	filter2D(inputArray, k10, inputArray.depth(), w10);

	return (k2 - k7 * 17 / 5 - 2 * k9) * sin(alpha * CV_PI / 180) + (k3 - k10 * 17 / 5 - 2 * k8) * cos(alpha * CV_PI / 180);
}

Mat FastSaliency::saliencyMap(const Mat& inputArray)
{
	Mat outputArray;
	Mat facetKern = (Mat_<double>(5, 5) << -4, -1, 0, -1, -4,
											-1, 2, 3, 2, -1,
											0, 3, 4, 3, 0,
											-1, 2, 3, 2, -1,
											-4, -1, 0, -1, -4) / 70;
	// 高通滤波得到 gradient magnitude map
	filter2D(inputArray, outputArray, inputArray.depth(), facetKern);
	// 平方增强得到 enhanced map
	pow(outputArray, 2, outputArray);
	// 高斯滤波得到 high SCR map
	// sigma = 1.5 from experiments
	GaussianBlur(outputArray, outputArray, Size(3, 3), 1.5);

	// 自适应全局熵阈值分割 
	outputArray = maxEntropySegMentation(outputArray);
	//double minVal, maxVal;
	//minMaxLoc(outputArray, &minVal, &maxVal);
	//threshold(outputArray, outputArray,0.35*maxVal,255,ThresholdTypes::THRESH_BINARY);
	return outputArray;
}

Mat FastSaliency::maxEntropySegMentation(Mat inputImage)
{
	const int channels[1] = { 0 };
	const int histSize[1] = { 256 };
	float pranges[2] = { 0,256 };
	const float* ranges[1] = { pranges };
	MatND hist;
	calcHist(&inputImage, 1, channels, Mat(), hist, 1, histSize, ranges);
	float maxentropy = 0;
	int max_index = 0;
	Mat result;
	for (int i = 0; i < 256; i++)
	{
		float cur_entropy = caculateCurrentEntropy(hist, i);
		if (cur_entropy > maxentropy)
		{
			maxentropy = cur_entropy;
			max_index = i;
		}
	}
	threshold(inputImage, result, max_index, 255, THRESH_BINARY);
	return result;
}

float FastSaliency::caculateCurrentEntropy(Mat hist, int threshold)
{
	float BackgroundSum = 0, targetSum = 0;
	const float* pDataHist = (float*)hist.ptr<float>(0);
	for (int i = 0; i < 256; i++)
	{
		//累计背景值
		if (i < threshold)
		{
			BackgroundSum += pDataHist[i];
		}
		//累计目标值
		else
		{
			targetSum += pDataHist[i];
		}
	}
	float BackgroundEntropy = 0, targetEntropy = 0;
	for (int i = 0; i < 256; i++)
	{
		//计算背景熵
		if (i < threshold)
		{
			if (pDataHist[i] == 0)
				continue;
			float ratio1 = pDataHist[i] / BackgroundSum;
			//计算当前能量熵
			BackgroundEntropy += -ratio1 * logf(ratio1);
		}
		else  //计算目标熵
		{
			if (pDataHist[i] == 0)
				continue;
			float ratio2 = pDataHist[i] / targetSum;
			targetEntropy += -ratio2 * logf(ratio2);
		}
	}
	return (targetEntropy + BackgroundEntropy);
}
