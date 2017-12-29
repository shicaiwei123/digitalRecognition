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
//opencv3.2.0���ٷ��ṩ��Ĭ����64λ��vs14������������������汾�ı���������Ҫ�Լ�cmake����opencvԴ��
//����ѡ��ĵ���ģ�ͣ�debug����release��ѡ��ͬ�İѲ�ͬ��lib�ļ����븽�ӿ⣬��Ҫͬʱ���룬�������
//��d��β����debugģʽ��
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
//openCV����
int main()
{
	//·������\\���� / , ������\,//����������ͼƬ,Ҳ����ֱ�ӷ��ڹ�������
	cv::Mat imagesrc = cv::imread("digits.png");   //��ȡͼ��  
	if(!imagesrc.data)
	{
		cout << "no image can be load!" << endl;
		return 0;
	}

	double fScale = 0.5;  //����ϵ��  

	//����Ŀ��ͼ��Ĵ�С  
Size dsize = Size(imagesrc.cols*fScale, imagesrc.rows*fScale);
	Mat imagedst = Mat(dsize, CV_32S);
	resize(imagesrc, imagedst, dsize);

	cv::namedWindow("openCV");  //��������  
	cv::imshow("openCV", imagedst);  //��ʾͼ��  
	cv::waitKey();
	//cv::imwrite("lena.bmp", imagesrc);//����ͼ��  

	return 1;
}

#endif

#ifdef gestureRegnize
//����ʶ��

int main()
{
	//MAT��c++��һ���࣬ʵ�����������һ�������󣬷�װ�˺ܶ෽�������matlab�жԾ���Ĳ������䷽���ж������Ҵ�
	//ֻ�����������ֲ�һ����Ҫ�任һ��
	Mat img = imread("digits.png");
	Mat gray;
	//cvtCorlor��һ��ת��ͼƬ��ɫ�ռ�ĺ������ṩ����ת���ķ�ʽ��ע��opencv�ж���ͼƬ��ͼƬ����BGR�ķ�ʽ�����
	//������RGB
	cvtColor(img, gray, CV_BGR2GRAY);
	int b = 20;
	int m = gray.rows / b;       //ԭͼΪ1000*2000
	int n = gray.cols / b;       //�ü�Ϊ5000��20*20��Сͼ��
	Mat data, labels;       //��������
	for(int i = 0 ; i < n ; i++)
	{
		int offsetCol = i*b;     //���ϵ�ƫ����
		for(int j = 0 ; j < m ; j++)
		{
			int offsetRow = j*b;      //���ϵ�ƫ����
									//��ȡ20*20��С��
			Mat tmp;
			gray(Range(offsetRow, offsetRow + b), Range(offsetCol, offsetCol + b)).copyTo(tmp);
			data.push_back(tmp.reshape(0, 1));      //���л��������������
			labels.push_back((int)j / 5);      //��Ӧ�ı�ע
		}

	}
	data.convertTo(data, CV_32F);     //uchar��ת��Ϊcv_32f
	int samplesNum = data.rows;
	int trainNum = 3000;
	Mat trainData, trainLabels;
	trainData = data(Range(0, trainNum), Range::all());       //ǰ3000������Ϊѵ������
	trainLabels = labels(Range(0, trainNum), Range::all());

	//ע�����ѧϰģ������ml�ı����ռ�֮�еģ���ml������cv�ı����ռ�֮�еģ����Ҫʹ�û���ѧϰģ�ͽ���
	//using namespace cv�ǲ����ģ���Ҫusing namespace cv::ml,�����ڻ���ѧϰģ��ǰ�����ml::
	//����ѧϰģ����һ��ģ���࣬��Ptr<>�У������Լ�����Ҫ�趨��ͬ��ѡ�������ͬ�Ļ���ѧϰģ��
	////ʹ��KNN�㷨ѵ��ģ��
	//int K = 5;

	//Ptr<ml::KNearest> model = ml::KNearest::create();
	//Ptr<ml::TrainData> tData = ml::TrainData::create(trainData, ml::ROW_SAMPLE, trainLabels);

	//model->setDefaultK(K);
	//model->setIsClassifier(true);
	//model->train(tData);
	//model->save("F:\\github\\opencvTest\\opencvTest\\knn_knowledge.yml");
	//����ѵ���õ�ģ��
	Ptr<ml::KNearest> model = Algorithm::load<ml::KNearest>("knn_knowledge.yml");

	//Ԥ�����
	float start = clock();
	double train_hr = 0, test_hr = 0;
	Mat response;
	// compute prediction error on train and test data
	for(int i = 0 ; i < samplesNum ; i++)
	{
		Mat sample = data.row(i);
		float r = model->predict(sample);       //�������н���Ԥ��
											  //Ԥ������ԭ�����ȣ����Ϊ1������Ϊ0
		r = std::abs(r - labels.at<int>(i)) <= FLT_EPSILON ? 1.f : 0.f;

		if (i < trainNum)
			train_hr += r;      //�ۻ���ȷ��
		else
			test_hr += r;
	}

	test_hr /= samplesNum - trainNum;
	train_hr = trainNum > 0 ? train_hr / trainNum : 1.;

	printf("accuracy: train = %.1f%%, test = %.1f%%\n",
		train_hr*100.,
		test_hr*100.);
	float end = clock();    //���ڼ���ģ�͵�����ʱ��ʹ��ʱ��
	printf("time=%4f\n", end - start);
	waitKey(0);
	return 0;
}
#endif
