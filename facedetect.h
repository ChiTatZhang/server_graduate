#ifndef _FACEDETECT_H
#define _FACEDETECT_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <vector>
using namespace std;
using namespace cv;
 void detect_face(Mat &image,vector<Rect> *RFaceImg=NULL,vector<Mat> *mFaceImg=NULL);
#endif

