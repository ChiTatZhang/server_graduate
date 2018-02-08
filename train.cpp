#include <iostream>
// #include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/face.hpp>
#include <opencv2/face/facerec.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <fstream>
#include "faceRecognise.h"
#include "database.h"
using namespace std;
using namespace cv; 
using namespace face;

int save_faceImage(Mat &image,const char *path,const int lable)
{
	vector<Mat> faceImg;
	vector<Mat>::iterator face;
	static int num =0;
	detect_face(image,NULL,&faceImg);
	cout<<"face:"<<faceImg.size()<<endl;
	char makeDir[64]={0};
	snprintf(makeDir,sizeof(makeDir),"mkdir %s",path);
	system(makeDir);
	imshow("img",image);
	if(faceImg.size()==1)
	{
		char tmppath[64]={0};
		snprintf(tmppath,sizeof(tmppath),"%s/%d.jpg",path,num);
		num++;
		cout<<"write_num: "<<num<<endl;
		imshow("save",faceImg[0]);
		waitKey(500);
		cout<<num<<endl;	
		cout<<tmppath;
		imwrite(tmppath,faceImg[0]);
		char sql[128]={0};
		sprintf(sql,"insert into imgpath (id,path) values(%d,'%s');",lable,tmppath);
		int ret=sqlite(sql);
		if(ret<0)
		{
			cout<<"data insert error"<<endl;
		}
	}
	cout<<"over"<<endl;
	return -1;
}


int add_face(VideoCapture &capture)
{
	int key=0;
	while(1)
	{
		int id=0;
		string name;
		cout<<"请输入id号"<<endl;
		cin>>id;
		cout<<"请输入名字"<<endl;
		cin>>name;
		while(key=waitKey(100))
		{
			Mat frame;
			Mat myFace;
			static int num=0;
			capture>>frame;
			vector<Rect> rec;
			detect_face(frame,&rec);
			if(key=='p')
			{
				if(rec.size()==1)
				{
					char path[64];
					sprintf(path,"./att_faces/%d",id);
					save_faceImage(frame,path,id);
					num++;
				}
				cout<<"已保存"<<num<<"张图片"<<endl;
			}
			imshow("video",frame);
			if(num>9){
				break;
			}

		}
		cout<<"继续添加请按n\n"<<"退出添加请按q"<<endl;
		string str;
		cin>>str;
		if(str=="q")
		{
			break;
		}
	}
}
int train()
{
	vector<Mat>imgs;
	vector<int>lab;
	char sql[128]="select id,path from imgpath";
	struct imgpath *ip;
	int num;
	int ret=sqlite(&ip,sql,&num);
	if(ret<0)
	{
		cout<<"imgpath select error"<<endl;
		return -1;
	}
	struct imgpath *tmpip=ip;
	for(int i=0;i<num;i++)
	{
		imgs.push_back(imread(tmpip->path,0));
		lab.push_back(tmpip->id);
		tmpip++;
	}
	free(ip);
	cout<<imgs.size()<<lab.size()<<endl;
	int MaxLab=lab[lab.size()-1];
	cout<<"maxlab: "<<MaxLab<<endl;
	cout<<"training...."<<endl;
	Ptr<FaceRecognizer> model =LBPHFaceRecognizer::create();
	model->train(imgs,lab);
	model->write("test.xml");
	return 0;	
}

int recognise(Mat &tmpframe,int &label,double &number)
{
	Ptr<FaceRecognizer> model =LBPHFaceRecognizer::create();
	model->read("test.xml");
	Mat testImg;
	vector<Rect> rec;
	detect_face(tmpframe,&rec);
	cout<<"detect_face rec.size()"<<rec.size()<<endl;
	if(rec.size()!=1)
		return -1;
	else
	{
		cvtColor(tmpframe,testImg,CV_BGR2GRAY);
		vector<Rect>::iterator pRec;
		for(pRec=rec.begin();pRec!=rec.end();pRec++){

			Mat faceImg=testImg(*pRec);
			Mat resizeImg;
			resize(faceImg,resizeImg,Size(92,112));

			model->predict(resizeImg, label, number);
			cout<<"label:" << label << endl;
			cout<<"num:" << number << endl;
		}
	}
	return 0;
}
#if 0
int main()
{
	//	VideoCapture capture(0);
	//	int key;

	//	int time=0;

	/*int trainRet=train();
	  if(trainRet>0)
	  cout<<"train fialed"<<endl;
	  else
	  cout<<"train success"<<endl;*/
	//	while(key=waitKey(100)){
	Mat tmpframe=imread("./att_faces/20180100/0.jpg");
	//		capture>>tmpframe;
	int lab;
	double num;
	recognise(tmpframe,lab,num);
	int predictLab=lab;
	double number=num;
	if((predictLab==20180101 || predictLab==20180100 || predictLab==42) && number<80){
		string name = "zhangzhida";
		cout<<name;
	}else if(predictLab==20180102 ||predictLab==45 && number<90){
		string name = "liuchangxin";
		cout<<name;
	}else
	{
		string name ="other";
		cout<<name;
	}	


	imshow("video",tmpframe);
	return 0;
}


#endif
