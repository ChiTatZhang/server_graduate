#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
//#include <opencv2/contrib/contrib.hpp>
#include <iostream>

#include "facedetect.h"

using namespace cv;
using namespace std;

int main()
{
	CascadeClassifier cascade;
	cascade.load("haarcascade_frontalface_default.xml");
	Mat image = imread("timg.jpeg");
//	waitKey(0);
	Mat temImage,dstImage;
	temImage=image;
//	resize(temImage,dstImage,Size(temImage.cols/2,temImage.rows/2));
	Mat resultImg = Mat::zeros(image.size(),CV_8UC3);
	for(int row = 0; row < image.rows; row++){
		for(int col = 0; col < image.cols; col++){
			const uchar* currentRow = image.ptr(row);
			uchar *myrow = resultImg.ptr(row);
			if(image.channels() == 1){
				myrow[col] = 255 - currentRow[col];
			}else if(image.channels() == 3){
				myrow[col * 3] = 255 - currentRow[col * 3];
				myrow[col * 3 + 1] = 255 - currentRow[col * 3 + 1];
				myrow[col * 3 + 1] = 255 - currentRow[col * 3 + 1];
			}else{
				printf("image type is unknown\r\n");
			}
		}
	}
	namedWindow("反相");
	imshow("反相",resultImg);
//	imwrite("反相.jpg",resultImg);
	detect_face(image, cascade);
	waitKey(0);
	return 0;
}
