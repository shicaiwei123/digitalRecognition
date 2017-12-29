//#include <iostream>
//
//using namespace std;
//
//int main(int argc, char *argv[])
//{
//	char sz[] = "Hello, World!";	//Hover mouse over "sz" while debugging to see its contents
//	cout << sz << endl;	//<================= Put a breakpoint here
//	return 0;
//}
//
//opencv3.2.0，官方提供的默认是64位，vs14编译器，如果是其他版本的编译器，需要自己cmake编译opencv源码
//根据选择的调试模型，debug或者release，选择不同的把不同的lib文件导入附加库，不要同时导入，会出问题
//以d结尾的是debug模式的
#include<iostream>
#include <opencv2/core/core.hpp>           // cv::Mat  
#include <opencv2/highgui/highgui.hpp>     // cv::imread()  
#include <opencv2/imgproc/imgproc.hpp>     // cv::Canny()  
#include <string>
#include <string.h>
#include <time.h>
#include <opencv2/ml/ml.hpp>

using std::cout;
using std::cin;
using std::endl;
using namespace cv;
//#define openCVDebug
#define  gestureRegnize
#ifdef openCVDebug
//openCV测试
int main()
{
	//路径名是\\或者 / , 而不是\,//否则会读不到图片,也可以直接放在工作区间
	cv::Mat imagesrc = cv::imread("digits.png");   //读取图像  
	if(!imagesrc.data)
	{
		cout << "no image can be load!" << endl;
		return 0;
	}

	double fScale = 0.5;  //缩放系数  

	//计算目标图像的大小  
Size dsize = Size(imagesrc.cols*fScale, imagesrc.rows*fScale);
	Mat imagedst = Mat(dsize, CV_32S);
	resize(imagesrc, imagedst, dsize);

	cv::namedWindow("openCV");  //创建窗口  
	cv::imshow("openCV", imagedst);  //显示图像  
	cv::waitKey();
	//cv::imwrite("lena.bmp", imagesrc);//保存图像  

	return 1;
}

#endif

#ifdef gestureRegnize
//手势识别

int main()
{
	//MAT是c++的一个类，实例化对象就是一个个矩阵，封装了很多方法，许多matlab中对矩阵的操作在其方法中都可以找打
	//只不过可能名字不一样需要变换一下
	Mat img = imread("digits.png");
	Mat gray;
	//cvtCorlor是一个转换图片颜色空间的函数，提供多种转换的方式，注意opencv中读入图片后，图片是以BGR的方式储存的
	//而不是RGB
	cvtColor(img, gray, CV_BGR2GRAY);
	int b = 20;
	int m = gray.rows / b;       //原图为1000*2000
	int n = gray.cols / b;       //裁剪为5000个20*20的小图块
	Mat data, labels;       //特征矩阵
	for(int i = 0 ; i < n ; i++)
	{
		int offsetCol = i*b;     //列上的偏移量
		for(int j = 0 ; j < m ; j++)
		{
			int offsetRow = j*b;      //行上的偏移量
									//截取20*20的小块
			Mat tmp;
			gray(Range(offsetRow, offsetRow + b), Range(offsetCol, offsetCol + b)).copyTo(tmp);
			data.push_back(tmp.reshape(0, 1));      //序列化后放入特征矩阵
			labels.push_back((int)j / 5);      //对应的标注
		}

	}
	data.convertTo(data, CV_32F);     //uchar型转换为cv_32f
	int samplesNum = data.rows;
	int trainNum = 3000;
	Mat trainData, trainLabels;
	trainData = data(Range(0, trainNum), Range::all());       //前3000个样本为训练数据
	trainLabels = labels(Range(0, trainNum), Range::all());

	//注意机器学习模型是在ml的变量空间之中的，而ml又是在cv的变量空间之中的，如果要使用机器学习模型仅仅
	//using namespace cv是不够的，还要using namespace cv::ml,或者在机器学习模型前面加上ml::
	//机器学习模型是一个模板类，在Ptr<>中，根据自己的需要设定不同的选项，构建不同的机器学习模型
	////使用KNN算法训练模型
	//int K = 5;

	//Ptr<ml::KNearest> model = ml::KNearest::create();
	//Ptr<ml::TrainData> tData = ml::TrainData::create(trainData, ml::ROW_SAMPLE, trainLabels);

	//model->setDefaultK(K);
	//model->setIsClassifier(true);
	//model->train(tData);
	//model->save("F:\\github\\opencvTest\\opencvTest\\knn_knowledge.yml");
	//导入训练好的模型
	Ptr<ml::KNearest> model = Algorithm::load<ml::KNearest>("knn_knowledge.yml");

	//预测分类
	float start = clock();
	double train_hr = 0, test_hr = 0;
	Mat response;
	// compute prediction error on train and test data
	for(int i = 0 ; i < samplesNum ; i++)
	{
		Mat sample = data.row(i);
		float r = model->predict(sample);       //对所有行进行预测
											  //预测结果与原结果相比，相等为1，不等为0
		r = std::abs(r - labels.at<int>(i)) <= FLT_EPSILON ? 1.f : 0.f;

		if (i < trainNum)
			train_hr += r;      //累积正确数
		else
			test_hr += r;
	}

	test_hr /= samplesNum - trainNum;
	train_hr = trainNum > 0 ? train_hr / trainNum : 1.;

	printf("accuracy: train = %.1f%%, test = %.1f%%\n",
		train_hr*100.,
		test_hr*100.);
	float end = clock();    //用于计算模型的运算时的使用时间
	printf("time=%4f\n", end - start);
	waitKey(0);
	return 0;
}
#endif
