#include "pch.h"
#include "MLDetector.h"
#include "selective_search.hpp"

#include <io.h>
#include <fstream>



void MLDetector::Img2FeatureVec(Mat& inputArray, Mat& outputArray, Mat& cluster_centers, Ptr<FeatureDetector> detector, Ptr<DescriptorExtractor> descriptor)
{
    // 根据词典生成的词向量转换器
    outputArray = Mat::zeros(cv::Size(cluster_centers.rows, 1), CV_32FC1);
    Mat descriptor_trainImg;
    std::vector<KeyPoint> keyPoints_trainImg;
    detector->detect(inputArray, keyPoints_trainImg);
    descriptor->compute(inputArray, keyPoints_trainImg, descriptor_trainImg);

    for (int i = 0; i < descriptor_trainImg.rows; i++) {
        std::vector<double> distance_centers;
        for (int j = 0; j < cluster_centers.rows; j++) {
            double distance = norm(descriptor_trainImg.rowRange(i, i + 1), cluster_centers.rowRange(j, j + 1), NORM_L2);
            distance_centers.push_back(distance);
        }
        std::vector<double>::iterator biggest = std::max_element(std::begin(distance_centers), std::end(distance_centers));
        int pos_max = biggest - std::begin(distance_centers);
        outputArray.at<float>(Point(pos_max, 0)) += 1;
    }
    // 归一化
    outputArray = outputArray / descriptor_trainImg.rows;
}


MLDetector::MLDetector()
{
	this->numClusters = 9;
	this->objImgsPath = "./objSet/";
	this->trainImgsPath = "./trainSet/trainImgs/";
    this->svmModel = ml::SVM::create();

}

MLDetector::~MLDetector()
{
}

MLDetector::MLDetector(int numClusters)
{
    this->numClusters = numClusters;
    this->objImgsPath = "./objSet/";
    this->trainImgsPath = "./trainSet/trainImgs/";
    this->svmModel = ml::SVM::create();
}

MLDetector::MLDetector(string objImgsPath, string trainImgsPath, int numClusters)
{
	this->objImgsPath = objImgsPath;
	this->trainImgsPath = trainImgsPath;
	this->numClusters = numClusters;
    this->svmModel = ml::SVM::create();
}

void MLDetector::initBOWModel()
{
    //-------------------------------  初始化BOW模型  ------------------------------------------------------------------------
    //---------------------------------构建训练目标BOW模型特征簇--------------------------------------------------------------
    vector<Mat> obj_imgs;
    vector<string> obj_files;
    glob(this->objImgsPath, obj_files);

    for (auto i_pth : obj_files) {
        Mat obj_img_temp = imread(i_pth, ImreadModes::IMREAD_GRAYSCALE);
        obj_imgs.push_back(obj_img_temp);
    }

    vector<Mat> descriptor_objImgs;
    vector<vector<KeyPoint>> keyPoints_imgs;
    featureDetector->detect(obj_imgs, keyPoints_imgs);
    featureDescriptor->compute(obj_imgs, keyPoints_imgs, descriptor_objImgs);

    int avgPointNums = 0;
    for (auto dcr_img : descriptor_objImgs) {
        avgPointNums += dcr_img.rows;
    }
    // 设置聚类簇数为特征点数均值
    this->numClusters = int(avgPointNums / descriptor_objImgs.size());

    Mat mergedDescriptors_obj;
    vconcat(descriptor_objImgs, mergedDescriptors_obj);

    // 聚类生成全局词典 global_centers 
    Mat global_labels;
    // 聚类成numClusters个类, numClusters个主要特征描述词向量
    TermCriteria kmeans_accur(TermCriteria::EPS + TermCriteria::COUNT, 100, 1.0);
    kmeans(mergedDescriptors_obj, this->numClusters, global_labels, kmeans_accur, 5, KmeansFlags::KMEANS_RANDOM_CENTERS, this->global_centers);
    // 持久化全局词典 global_centers
    FileStorage fs("./LocalStorage/BOWDictionary.xml", FileStorage::WRITE);
    fs << "ex" << this->global_centers;
    fs.release();
}

void MLDetector::initHOGModel()
{
    // -------------------------------------------------   初始化SIFT模型   ---------------------------------------------------------------------------
    this->hog = { Size(64, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9 };
}

void MLDetector::train(FeatureFlag featureFlag)
{
    //--------------------------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------  SVM分类器训练数据构建 -------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------------------------------------------
    //  SVM分类器输入特征矩阵
    Mat svmInputFeaturesVecs;
    //  读取训练图片构建特征矩阵
    vector<Mat> trainSetFeatureVecs;

    vector<string> train_files;
    glob(this->trainImgsPath, train_files);

    switch (featureFlag)
    {
    case FeatureFlag::SIFTFlag:
        try
        {
            for (auto i_pth : train_files) {
                Mat train_img_temp = imread(i_pth, ImreadModes::IMREAD_GRAYSCALE);
                Mat outFeatureSingleImg;
                Img2FeatureVec(train_img_temp, outFeatureSingleImg, this->global_centers, this->featureDetector, this->featureDescriptor);
                trainSetFeatureVecs.push_back(outFeatureSingleImg);
            }
            vconcat(trainSetFeatureVecs, svmInputFeaturesVecs);
        }
        catch (const std::exception& e)
        { }
        break;
    case FeatureFlag::HOGFlag:
        try
        {
            for (auto i_pth : train_files) {
                Mat resized_train_img;
                Mat train_img_temp = imread(i_pth, ImreadModes::IMREAD_GRAYSCALE);
                resize(train_img_temp, resized_train_img, Size(64, 128));
                vector<float> outVecSingleImg;
                this->hog.compute(resized_train_img, outVecSingleImg);
                Mat outFeatureSingleImg = Mat(outVecSingleImg);
                transpose(outFeatureSingleImg, outFeatureSingleImg);
                trainSetFeatureVecs.push_back(outFeatureSingleImg);
            }
            vconcat(trainSetFeatureVecs, svmInputFeaturesVecs);
        }
        catch (const std::exception& e)
        { }
        break;
    }

    //  SVM分类器输入标签矩阵
    Mat svmInputLabelsVecs = Mat::zeros(Size(1, svmInputFeaturesVecs.rows), CV_32SC1);
    ifstream in;
    // 读取已有的标签文本label.txt
    in.open("./trainSet/labels.txt",ios::app);

    if (in.is_open()) {
        string line_label;
        int i = 0;
       while(!in.eof()&& i < svmInputFeaturesVecs.rows) {
            if (in.peek() == '/n')  break;
            getline(in, line_label);
            svmInputLabelsVecs.at<int>(Point(0, i)) = std::stoi(line_label);
            i++;
        }
    }
   
    // 构建SVM分类器训练数据集
    Ptr<ml::TrainData> trainData = ml::TrainData::create(svmInputFeaturesVecs, ml::SampleTypes::ROW_SAMPLE, svmInputLabelsVecs);
    
    //--------------------------------------------------------------------------------------------------------------------------------
    // ---------------------------------------------------   SVM分类器训练   ----------------------------------------------------------
    //---------------------------------------------------------------------------------------------------------------------------------

    // SVM分类器模型初始化
    this->svmModel->setType(ml::SVM::C_SVC);
    this->svmModel->setC(0.1);
    this->svmModel->setGamma(1.0);
    this->svmModel->setKernel(ml::SVM::RBF);
    this->svmModel->setTermCriteria(TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 20000, FLT_EPSILON));
    
    // SVM分类器模型训练
    this->svmModel->trainAuto(trainData, 10);

}

void MLDetector::saveModel(string saveModelPath)
{
    //--------------------------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------  存储分类模型 -------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------------------------------------------
    this->svmModel->save(saveModelPath);
}

vector<Rect> MLDetector::predict(const Mat& inputArray,string saveModelPath, float nmsThreshold, float confThreshold, FeatureFlag featureFlag)
{

    //--------------------------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------  分类模型预测 -------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------------------------------------------
    
    // 加载SIFT特征模型参数
    Mat readMatrix;
    FileStorage fs("./LocalStorage/BOWDictionary.xml", FileStorage::READ);
    fs["ex"] >> this->global_centers;

    // 加载HOG特征模型参数
    this->hog = { Size(64, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9 };


     // 加载训练好的SVM分类器
    this->svmModel = ml::SVM::load(saveModelPath);


    // Selective Search生成候选框
    auto proposals = ss::selectiveSearch(inputArray,1,0.8,1000,10000,1000000,5);

    // SVM分类器预测
    vector<Rect> roiRects;
    for (auto&& rect : proposals)
    {
        Mat ImgROI = inputArray(rect);
        float response = -1.0f;

        switch (featureFlag)
        {
        case FeatureFlag::SIFTFlag:
            try
            {
                Mat test_vec;
                Img2FeatureVec(ImgROI, test_vec, this->global_centers, this->featureDetector, this->featureDescriptor);
                response = svmModel->predict(test_vec, noArray(),ml::StatModel::Flags::RAW_OUTPUT);
            }
            catch (const std::exception& e)
            {
                abort();
            }
            break;
        case FeatureFlag::HOGFlag:
            try
            {
                Mat dst_gray, resized_img;
                cvtColor(ImgROI, dst_gray, cv::COLOR_BGR2GRAY);
                resize(dst_gray, resized_img, Size(64, 128));
                vector<float> outVecSingleImg;
                this->hog.compute(resized_img, outVecSingleImg);
                Mat test_vec = Mat(outVecSingleImg);
                transpose(test_vec, test_vec);
                response = svmModel->predict(test_vec, noArray(), ml::StatModel::Flags::RAW_OUTPUT);
            }
            catch (const std::exception& e)
            {
                abort();
            }
            break;
        }

        float confidence = (1.0 / (1.0 + exp(-response)));

        if ( confidence >= confThreshold) {
           roiRects.push_back(rect);
       }
    }

    // 非极大值抑制
    nms::NMS(roiRects, nmsThreshold);

    return roiRects;

}
