#include <iostream>
#include <sys/types.h>//socket
#include <sys/socket.h>//socket
#include <netinet/in.h>//sockaddr_in
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "database.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/face.hpp>
#include <opencv2/face/facerec.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <fstream>
#include "faceRecognise.h"
#include <pthread.h>
#include <time.h>

using namespace std;
using namespace cv;
using namespace face;
#define SERVER_PORT 8888
#define SIZE 1024
#define LISTEN_SIZE 5
#define PHOTO_SIZE 1024*6
Mat currentImage;
pthread_mutex_t imageLock=PTHREAD_MUTEX_INITIALIZER;
struct receive{

	int flag;
	//	char img[SIZE];
	//int size;
	char sql[512];
};
struct send{
	int flag;
	int num;
};
typedef struct photo{
	int id;
	int size;
	char buf[PHOTO_SIZE];
}Photo;
int initSocket()
{
	struct sockaddr_in server_addr;
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr= htons(INADDR_ANY);
	server_addr.sin_port = htons(SERVER_PORT);

	//创建套接字
	int fd= socket(AF_INET,SOCK_STREAM,0);
	if(fd < 0)
	{
		perror("create socket failed:");
		return -1;
	}
	//设置地址复用
	int opt=1;
	setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	//绑定ip和端口号
	int ret=bind(fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(ret < 0)
	{
		perror("server bind failed:");
		close(fd);
		return -1;
	}

	//监听
	ret=listen(fd,LISTEN_SIZE);
	if(ret < 0)
	{
		perror("server listen failed:");
		close(fd);
		return -1;
	}
	return fd;
}

static void * handle_request(void * s)
{
	pthread_detach(pthread_self());
	int newId=*((int *)s);
	char buf[SIZE]={0};
	int ret = recv(newId,buf,sizeof(struct receive),0);
	if(ret<=0)
	{
		perror("server recieve data failed:");
		close(newId);
		return NULL;
	}
	else
		cout<<"recieve success"<<endl;
	cout<<"size "<<sizeof(struct receive)<<endl;
	cout<<"ret "<<ret<<endl;
	struct receive rec;
	struct send sd;
	sd.flag=-1;
	sd.num=0;
	memset(&rec,0,sizeof(rec));
	memcpy(&rec,buf,ret);
	memset(buf,0,SIZE);
	cout<<rec.flag<<endl;
	//	cout<<rec.sql<<endl;
	switch(rec.flag){
	case 0://传输图片验证登录
		{
			Photo photo;

			for(int i=0;i<3;i++)
			{
				char filename[32];
				sprintf(filename,"./facedetect/face%d.jpg",i);
				ret=recv(newId,(char *)&photo,sizeof(photo),0);	
				cout<<ret<<endl;
				ofstream file;
				cout<<photo.size<<endl;
				file.open(filename,ios::out|ios::binary);
				file.write(photo.buf,photo.size);
				file.close();
			}

			int cmp_label[3]={0};
			int label;
			double number;
			Ptr<FaceRecognizer> model =LBPHFaceRecognizer::create();
			model->read("test.xml");
			for(int i=0;i<3;i++)
			{
				char path[32]={0};
				sprintf(path,"./facedetect/face%d.jpg",i);
				Mat face=imread(path,0);
				//resize(face,face,Size(92,112));
				model->predict(face, label, number);
				if(number<85)
				{
					cmp_label[i]=label;
					cout<<label;
				}
			}
			if(cmp_label[0]==label && cmp_label[1]==label && cmp_label[2]==label)
			{
				cout<<label;
				struct staffinfo *staff=NULL;
				int num=0;
				char sql[128];
				sprintf(sql,"select * from staffinfo where id=%d;",label);
				ret=sqlite(&staff,sql,&num);
				sd.flag=ret;
				sd.num=num;
				cout<<ret<<"  "<<num<<endl;
				memcpy(buf,&sd,sizeof(sd));
				ret = send(newId,buf,sizeof(sd),0);
				if(ret<0)
					perror("server send data failed:");
				else if(num>0)
				{
					cout<<"send success!"<<endl;
					int size=sizeof(struct staffinfo)*num;
					char *buff=(char *)staff;	
					//memcpy(buff,staff,sizeof(struct straffinfo)*num);
					ret = send(newId,buff,size,0);
					cout<<"send"<<size<<endl;
					if(ret<0)
						perror("server send data failed:");
					else
						cout<<"send success!"<<endl;
					free(staff);
				}
			}

			close(newId);
			break;

		}
	case 1://控制器件
		{
			sd.flag=0;
			memcpy(buf,&sd,sizeof(sd));
			ret = send(newId,buf,sizeof(sd),0);
			if(ret<0)
				perror("server send data failed:");
			else
				cout<<"send success!"<<endl;
			close(newId);
			break;
		}

	case 2://增删改
		{
			ret=sqlite(rec.sql);
			sd.flag=ret;
			cout<<sd.flag;
			memcpy(buf,&sd,sizeof(sd));
			ret = send(newId,buf,sizeof(sd),0);
			if(ret<0)
				perror("server send data failed:");
			else
				cout<<"send success!"<<endl;
			close(newId);
			break;
		}
	case 3://员工信息查询
		{
			struct staffinfo *staff=NULL;
			int num=0;
			ret=sqlite(&staff,rec.sql,&num);
			sd.flag=ret;
			sd.num=num;
			cout<<ret<<"  "<<num<<endl;
			memcpy(buf,&sd,sizeof(sd));
			ret = send(newId,buf,sizeof(sd),0);
			if(ret<0)
				perror("server send data failed:");
			else
			{
				cout<<"send success!"<<endl;
				int size=sizeof(struct staffinfo)*num;
				char *buff=(char *)staff;	
				//memcpy(buff,staff,sizeof(struct straffinfo)*num);
				ret = send(newId,buff,size,0);
				cout<<"send"<<size<<endl;
				if(ret<0)
					perror("server send data failed:");
				else
					cout<<"send success!"<<endl;
				free(staff);
			}
			close(newId);
			break;
		}
	case 4://出勤情况查询
		{
			struct attendence *atten=NULL;
			int num=0;
			ret=sqlite(&atten,rec.sql,&num);
			sd.flag=ret;
			sd.num=num;
			memcpy(buf,&sd,sizeof(sd));
			ret = send(newId,buf,sizeof(sd),10);
			if(ret<0)
				perror("server send data failed:");
			else{
				cout<<"send success!"<<endl;
				int size=sizeof(struct attendence)*num;
				char *buff=(char *)atten;
				//memcpy(buff,staff,sizeof(struct straffinfo)*num);
				ret = send(newId,buff,size,0);
				cout<<"send"<<ret<<endl;
				if(ret<0)
					perror("server send data failed:");
				else
					cout<<"send success!"<<endl;
				free(atten);
			}
			close(newId);
			break;
		}
	case 5://出入情况查询
		{
			struct iochalk *io=NULL;
			int num=0;
			ret=sqlite(&io,rec.sql,&num);
			sd.flag=ret;
			sd.num=num;
			memcpy(buf,&sd,sizeof(sd));
			ret = send(newId,buf,sizeof(sd),0);
			if(ret<0)
				perror("server send data failed:");
			else{
				cout<<"send success!"<<endl;
				int size=sizeof(struct iochalk)*num;
				char *buff=(char *)io;
				//memcpy(buff,io,sizeof(struct straffinfo)*num);
				ret = send(newId,buff,size,0);
				if(ret<0)
					perror("server send data failed:");
				else
					cout<<"send success!"<<endl;
				free(io);
			}
			close(newId);
			break;
		}
	case 6://加班查询
		{
			struct overtime *ot=NULL;
			int num=0;
			ret=sqlite(&ot,rec.sql,&num);
			sd.flag=ret;
			sd.num=num;
			memcpy(buf,&sd,sizeof(sd));
			ret = send(newId,buf,sizeof(sd),0);
			if(ret<0)
				perror("server send data failed:");
			else{
				cout<<"send success!"<<endl;
				int size=sizeof(struct overtime)*num;
				char *buff=(char *)ot;
				//memcpy(buff,ot,sizeof(struct straffinfo)*num);
				ret = send(newId,buff,size,0);
				if(ret<0)
					perror("server send data failed:");
				else
					cout<<"send success!"<<endl;
				free(ot);
			}
			close(newId);
			break;
		}
	case 7://请假查询
		{
			struct vacate *vac=NULL;
			int num=0;
			ret=sqlite(&vac,rec.sql,&num);
			sd.flag=ret;
			sd.num=num;
			memcpy(buf,&sd,sizeof(sd));
			ret = send(newId,buf,sizeof(sd),0);
			if(ret<0)
				perror("server send data failed:");
			else{
				cout<<"send success!"<<endl;
				int size=sizeof(struct vacate)*num;
				char *buff=(char *)vac;
				//memcpy(buff,vac,sizeof(struct straffinfo)*num);
				ret = send(newId,buff,size,0);
				if(ret<0)
					perror("server send data failed:");
				else
					cout<<"send success!"<<endl;
				free(vac);
			}
			close(newId);
			break;
		}
	case 8://出差查询
		{
			struct evection *evec=NULL;
			int num=0;
			ret=sqlite(&evec,rec.sql,&num);
			sd.flag=ret;
			sd.num=num;
			memcpy(buf,&sd,sizeof(sd));
			ret = send(newId,buf,sizeof(sd),0);
			if(ret<0)
				perror("server send data failed:");
			else{
				cout<<"send success!"<<endl;
				int size=sizeof(struct evection)*num;
				char *buff=(char *)evec;
				//memcpy(buff,evec,sizeof(struct straffinfo)*num);
				ret = send(newId,buff,size,0);
				if(ret<0)
					perror("server send data failed:");
				else
					cout<<"send success!"<<endl;
				free(evec);
			}
			close(newId);
			break;
		}
	case 9://考勤统计信息查询
		{
			struct stats *st=NULL;
			int num=0;
			ret=sqlite(&st,rec.sql,&num);
			sd.flag=ret;
			sd.num=num;
			memcpy(buf,&sd,sizeof(sd));
			ret = send(newId,buf,sizeof(sd),0);
			if(ret<0)
				perror("server send data failed:");
			else{
				cout<<"send success!"<<endl;
				int size=sizeof(struct stats)*num;
				char *buff=(char *)st;
				//memcpy(buff,st,sizeof(struct straffinfo)*num);
				ret = send(newId,buff,size,0);
				if(ret<0)
					perror("server send data failed:");
				else
					cout<<"send success!"<<endl;
				free(st);}
			close(newId);
			break;
		}
	case 10://作息时间查询
		{
			struct settime *st=NULL;
			int num=0;
			ret=sqlite(&st,rec.sql,&num);
			sd.flag=ret;
			sd.num=num;
			memcpy(buf,&sd,sizeof(sd));
			ret = send(newId,buf,sizeof(sd),0);
			if(ret<0)
				perror("server send data failed:");
			else{
				cout<<"send success!"<<endl;
				int size=sizeof(struct settime)*num;
				char *buff=(char *)st;
				//memcpy(buff,st,sizeof(struct straffinfo)*num);
				ret = send(newId,buff,size,0);
				if(ret<0)
					perror("server send data failed:");
				else
					cout<<"send success!"<<endl;
				free(st);
			}
			close(newId);
			break;
		}
	case 11://访客查询
		{
			struct caller *call=NULL;
			int num=0;
			ret=sqlite(&call,rec.sql,&num);
			sd.flag=ret;
			sd.num=num;
			memcpy(buf,&sd,sizeof(sd));
			ret = send(newId,buf,sizeof(sd),0);
			if(ret<0)
				perror("server send data failed:");
			else{
				cout<<"send success!"<<endl;
				int size=sizeof(struct caller)*num;
				char *buff=(char *)call;
				//memcpy(buff,call,sizeof(struct straffinfo)*num);
				ret = send(newId,buff,size,0);
				cout<<"ret"<<ret<<endl;
				cout<<"size"<<size<<endl;
				if(ret<0)
					perror("server send data failed:");
				else
					cout<<"send success!"<<endl;
				free(call);
			}
			close(newId);
			break;
		}
	case 12://部门查询
		{
			struct dept *Dept=NULL;
			int num=0;
			ret=sqlite(&Dept,rec.sql,&num);
			cout<<"dept select ret="<<ret<<endl;
			sd.flag=ret;
			sd.num=num;
			memcpy(buf,&sd,sizeof(sd));
			ret = send(newId,buf,sizeof(sd),0);
			if(ret<0)
				perror("server send data failed:");
			else{
				cout<<"send success!"<<endl;
				int size=sizeof(struct dept)*num;
				char *buff=(char *)Dept;
				//memcpy(buff,call,sizeof(struct straffinfo)*num);
				waitKey(30);
				ret = send(newId,buff,size,0);
				cout<<"ret"<<ret<<endl;
				cout<<"size"<<size<<endl;
				if(ret<0)
					perror("server send data failed:");
				else
					cout<<"send success!"<<endl;
				free(Dept);
			}
			close(newId);
			break;
		}
	case 13://职位查询
		{
			struct position *pos=NULL;
			int num=0;
			ret=sqlite(&pos,rec.sql,&num);
			cout<<"position select ret="<<ret<<endl;
			sd.flag=ret;
			sd.num=num;
			memcpy(buf,&sd,sizeof(sd));
			ret = send(newId,buf,sizeof(sd),0);
			if(ret<0)
				perror("server send data failed:");
			else{
				cout<<"send success!"<<endl;
				int size=sizeof(struct position)*num;
				char *buff=(char *)pos;
				//memcpy(buff,call,sizeof(struct straffinfo)*num);
				waitKey(30);
				ret = send(newId,buff,size,0);
				cout<<"ret"<<ret<<endl;
				cout<<"size"<<size<<endl;
				if(ret<0)
					perror("server send data failed:");
				else
					cout<<"send success!"<<endl;
				free(pos);
			}
			close(newId);
			break;
		}
	case 14://接受并保存图片，经图片路径id写进数据库
		{
			Photo photo;
			for(int i=0;i<10;i++)
			{
				char filename[32];
				ret=recv(newId,(char *)&photo,sizeof(photo),0);	
				cout<<ret<<endl;
				ofstream file;
				char mkdir[64];
				sprintf(mkdir,"mkdir ./att_faces/%d",photo.id);
				system(mkdir);
				cout<<photo.size<<endl;

				sprintf(filename,"./att_faces/%d/face%d.jpg",photo.id,i);
				file.open(filename,ios::out|ios::binary);
				file.write(photo.buf,photo.size);
				file.close();

				char sql[64];
				sprintf(sql,"insert into imgpath values(NULL,%d,'%s')",photo.id,filename);
				cout<<sql;
				int ret=sqlite(sql);

				sd.flag=ret;
				memcpy(buf,&sd,sizeof(sd));
				ret=send(newId,buf,sizeof(sd),0);
				if(ret<0)
					perror("server send data failed:");
				else
					perror("send success!");

			}
			int trainRet=train();
			if(trainRet>0)
				cout<<"train fialed"<<endl;
			else
				cout<<"train success"<<endl;
			close(newId);
			break;
		}

	default:
		cout<<"无效选项"<<endl;

	}
	return NULL;
}
static void * handle_accept(void * s)
{
	int socketId=*((int *)s);
	if(socketId<0)
		return NULL;
	while(1)
	{
		cout<<"begin accept connection"<<endl;
		struct sockaddr_in from ;
		socklen_t len = sizeof(from);
		int newId = accept(socketId,(struct sockaddr*)&from,&len);
		if(newId < 0)
		{
			perror("server accept failed:");
			continue;
		}	
		else
			cout<<"new connection:" <<newId<<endl;
		pthread_t newThread;
		pthread_create(&newThread,NULL,handle_request,(void *)&newId);
	}
	return NULL;
}
static void * handle_recognise(void *image)
{
	pthread_detach(pthread_self());
	Mat tmpImage=*(Mat *)image;
	int label;
	double num;
	static int sum=0;
	int recogniseRet=recognise(tmpImage,label,num);
	//detect_face(currentImage,NULL,NULL);
	//imshow("image",currentImage);
	if(recogniseRet<0)
	{
		return NULL;
	}
	if(num>85)
	{
		sum+=1;
		cout<<"努力识别中i....."<<endl;
		if(sum>5)
		{
			cout<<"查无此人"<<endl;
		}
		return NULL;
	}
	sum=0;
	struct staffinfo *staff=NULL;
	int count=0;
	char sql[128]={0};
	sprintf(sql,"select * from staffinfo where id=%d;",label);
	int sqlRet=sqlite(&staff,sql,&count);
	if(sqlRet<0)
		cout<<"查询出错"<<endl;
	else if(count != 1)
		cout<<"查询到"<<count<<"条数据"<<endl;
	else
	{
		cout<<staff->name<<endl;
		time_t t=time(0);
		char tmpBuf[64]={0};
		struct tm *timenow;
		struct tm lastTime;
		time_t last_t;
		time_t now_t;
		timenow=localtime(&t);
		now_t=mktime(timenow);
		strftime(tmpBuf,64,"%Y-%m-%d %H:%M:%S",timenow);
		char now[32]={0};
		strftime(now,32,"%F",timenow);
		char sql[256]={0};
		sprintf(sql,"select * from iochalk where id=%d and number=(select Max(number) from iochalk where id=%d and date like '%s%%');",label,label,now);
		int iostatus=0;
		struct iochalk *io=NULL;
		int sqlRet=sqlite(&io,sql,&count);
		if(sqlRet<0)
			cout<<"select iochalk error"<<endl;
		else
		{
			if(count<=0)
				iostatus=0;
			else
			{
				strptime(io->date,"%Y-%m-%d %H:%M:%S",&lastTime);
				last_t=mktime(&lastTime);
				if(now_t-last_t<60)
				{
					return NULL;
				}
				if(io->iostatus==0)
					iostatus=1;
				else 
					iostatus=0;
			}
			free(io);
			memset(sql,0,sizeof(sql));
			sprintf(sql,"insert into iochalk values(NULL,%d,'%s','%s',%d)",staff->id,staff->name,tmpBuf,iostatus);
			sqlRet=sqlite(sql);
			if(sqlRet>0)
				cout<<"iochalk insert success"<<endl;
			else 
				cout<<"iochalk insert failed"<<endl;
		}
	}
	free(staff);
	return NULL;
}
static void * handle_camera(void *camera)
{
	VideoCapture capture(0);
	while(1)
	{
		//pthread_mutex_lock(&imageLock);
		capture>>currentImage;
		//pthread_mutex_unlock(&imageLock);
		Mat tmpImage=currentImage.clone();
		pthread_t recognisePthread;
		pthread_create(&recognisePthread,NULL,handle_recognise,(void *)&currentImage);
		detect_face(tmpImage,NULL,NULL);
		imshow("image",tmpImage);
		waitKey(100);
	}
}
static void * handle_time(void * t)
{
	time_t now_t;
	struct tm *now;
	bool dayFlag=false;
	bool monFlag=false;
	while(1)
	{
		time(&now_t);
		now=localtime(&now_t);
		//每天23点记录当天考勤信息
		if(now->tm_hour==23 && dayFlag==false )
		{
			dayFlag=true;
			struct staffinfo *staff=NULL;
			int count=0;
			const char sql[]="select * from staffinfo;";

			int sqlRet=sqlite(&staff,sql,&count);
			if(sqlRet<0)
			{
				cout<<"出勤录入查询员工信息失败"<<endl;
				continue;
			}
			struct staffinfo *tmpstaff=staff;
			for(int i=0;i<count;i++)
			{
				char date[16];
				strftime(date,16,"%F",now);
				char sql[512]={0};
				sprintf(sql,"insert into attendence values(NULL,%d,'%s',\
							(select date('now')),\
							(select time(date) from iochalk where number=\
							(select min(number) from iochalk where id=%d and date like '%s%%')),\
							(select time(date) from iochalk where number=\
							(select max(number) from iochalk where id=%d and date like '%s%%')),'正常出勤',0);",
							tmpstaff->id,tmpstaff->name,tmpstaff->id,date,tmpstaff->id,date);
				sqlRet=sqlite(sql);
				if(sqlRet<0)
				{
					cout<<"出勤录入员工信息失败"<<endl;
					continue;
				}
				struct attendence *atten=NULL;
				count=0;
				memset(sql,0,sizeof(sql));
				sprintf(sql,"select * from attendence where id=%d and date=date('now');",tmpstaff->id);

				sqlRet=sqlite(&atten,sql,&count);
				if(sqlRet<0)
				{
					cout<<"出勤录入查询出勤信息失败"<<endl;
					continue;
				}
				struct settime *st=NULL;
				count=0;
				memset(sql,0,sizeof(sql));
				sprintf(sql,"select * from settime;");

				sqlRet=sqlite(&st,sql,&count);
				if(sqlRet<0)
				{
					cout<<"出勤录入查询出勤信息失败"<<endl;
					continue;
				}
				int stat=0;
				struct tm st_t;
				strptime(st->ss,"%H:%M:%S",&st_t);
				time_t stT=mktime(&st_t);

				struct tm atten_t;
				strptime(atten->comeTime,"%H:%M:%S",&atten_t);
				time_t comeT=mktime(&atten_t);
				if(comeT>stT)
					stat=1;
				if(comeT-stT>3600)
					stat+=3;

				strptime(st->xx,"%H:%M:%S",&st_t);
				stT=mktime(&st_t);

				strptime(atten->leaveTime,"%H:%M:%S",&atten_t);
				time_t leaveT=mktime(&atten_t);

				if(leaveT<stT)
					stat+=2;

				char attenStatus[32]={0};
				if(stat==1)
					strcpy(attenStatus,"迟到");
				else if(stat==2)
					strcpy(attenStatus,"早退");
				else if(stat>=3)
					strcpy(attenStatus,"旷工");
				else
					strcpy(attenStatus,"正常出勤");

				memset(sql,0,sizeof(sql));
				sprintf(sql,"update attendence set account='%s',worktime=(julianday(leaveTime)-julianday(comeTime))*24) where id=%d and date=date('now');",attenStatus,tmpstaff->id);
				sqlRet=sqlite(sql);
				if(sqlRet<0)
				{
					cout<<"出勤录入员工信息更新失败"<<endl;
					continue;
				}
				memset(sql,0,sizeof(sql));
				sprintf(sql,"delete from attendence where cometime is NULL; insert into from overtime values(NULL,%d,'%s','date('now'),'%s',%s',0)",tmpstaff->id,tmpstaff->name,st->xx,atten->leaveTime);
				sqlRet=sqlite(sql);
				
				memset(sql,0,sizeof(sql));
				sprintf(sql,"update overtime set interval=(date(end)-date(action)) where date=date('now'); delete from overtime where interval=0");
				sqlRet=sqlite(sql);
				free(st);
				
				free(atten);
				tmpstaff++;


			}
			free(staff);

		}
		else if(now->tm_hour != 23)
			dayFlag=false;
		//每个月的28号记录统计考勤信息;
		if(now->tm_mday==28 && monFlag==false)
		{
			monFlag=true;

			char date[16];
			strftime(date,16,"%Y-%m",now);
			struct staffinfo *staff=NULL;
			int count=0;
			const char sql[]="select * from staffinfo;";

			int sqlRet=sqlite(&staff,sql,&count);
			if(sqlRet<0)
			{
				cout<<"考勤统计查询员工信息失败"<<endl;
				continue;
			}
			struct staffinfo *tmpstaff=staff;
			for(int i=0;i<count;i++)
			{
				 char sql[666]={0};
				 sprintf(sql,"insert into stats values(\
					 NULL,%d,'%s',strftime('%%Y-%%m','now'),\
					(select count(number) from attendence where id=%d and account='迟到'),\
					(select count(number) from attendence where id=%d and account='早退'),\
					(select count(number) from attendence where id=%d and account='旷工'),\
					(select count(number) from vacate where id=%d and action like '%s%%'),\
					(select count(number) from evection where id=%d and action like '%s%%'),\
					(select count(number) from overtime where id=%d and date like '%s%%'),\
					(select sum(interval) from overtime where id=%d and date like '%s%%'));",
					tmpstaff->id,tmpstaff->name,tmpstaff->id,tmpstaff->id,tmpstaff->id,tmpstaff->id,date,tmpstaff->id,date,tmpstaff->id,date,tmpstaff->id,date);
				sqlRet=sqlite(sql);
				if(sqlRet<0)
				{
					cout<<"月统计插入失败"<<endl;
				}
				 tmpstaff++;
			}
		}
		else if(now->tm_mday!=28 )
			monFlag=false;


	}
}
int main()
{
	int socketId=initSocket();
	pthread_t acceptPthread;
	pthread_t cameraPthread;
	pthread_t timePthread;
	pthread_create(&cameraPthread,NULL,handle_camera,NULL);
	pthread_create(&acceptPthread,NULL,handle_accept,(void *)&socketId);
	pthread_create(&timePthread,NULL,handle_time,NULL);
	pthread_join(acceptPthread,NULL);
	pthread_join(cameraPthread,NULL);
	pthread_join(timePthread,NULL);
	close(socketId);
	return 0;
}
