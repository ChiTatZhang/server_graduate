#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include "facedetect.h"
using namespace cv;
using namespace std;
/*
 * 识别人脸
 * */
void detect_face(Mat &image,vector<Rect> *rFaceImg,vector<Mat> *mFaceImg)
{
	CascadeClassifier cascade;
	cascade.load("./model/haarcascade_frontalface_default.xml");	
	vector<Rect>faceImg;
	if(rFaceImg==NULL){
		rFaceImg = &faceImg;
	}
//	static vector<Mat> mFaceImg;
	Mat gray; 
	cvtColor(image, gray, CV_BGR2GRAY);
	equalizeHist(gray,gray);//直方图均衡化
//	namedWindow("灰度图");
//	imshow("灰度图", gray);
	cascade.detectMultiScale(gray,*rFaceImg,1.1,10,0,Size(0.2 * gray.cols, 0.2 * gray.rows), Size(0.9 * gray.cols, 0.9 * gray.rows));
//	static vector<Mat> mFaceImg;
	vector<Rect>::iterator rFace;
	vector<Mat>::iterator mFace;
	printf("%ld\r\n",(*rFaceImg).size());
	for (rFace =(*rFaceImg).begin(); rFace != (*rFaceImg).end(); rFace++)
	{
	/*	Point faceP1;
		Point faceP2;
		faceP1.x = face->x;
		faceP2.x = face->x+face->width;

		faceP1.y = face->y;
		faceP2.y = face->y+face->height;
	*/
		if(mFaceImg!=NULL)
		{
			Mat tmpImg=gray(*rFace);
			Mat saveImg;
			resize(tmpImg,saveImg,Size(92,112));
			(*mFaceImg).push_back(saveImg);
		}
		rectangle(image,*rFace, Scalar(19,213,255), 1,8);
	}
//	namedWindow("result");
//	imshow("result", image);
}
