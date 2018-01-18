#include <iostream>
#include <sys/types.h>//socket
#include <sys/socket.h>//socket
#include <netinet/in.h>//sockaddr_in
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "database.h"
using namespace std;
#define SERVER_PORT 8888
#define SIZE 1024
#define LISTEN_SIZE 5

struct receive{

	int flag;
	//	char img[SIZE];
	char sql[512];
};
struct send{
	int flag;
	int num;
};
int main()
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

	while(1){
		//接受连接请求
		int newId=accept(fd,NULL,NULL);
		if(newId < 0)
		{
			perror("server accept failed:");
			close(fd);
			return -1;
		}
		else
			cout<<"server accept a connect success!"<<endl;
		char buf[SIZE]={0};
		ret = recv(newId,buf,sizeof(struct receive),0);
		if(ret<0)
		{
			perror("server recieve data failed:");
			close(fd);
			close(newId);
			return -1;
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
		cout<<rec.sql<<endl;
		switch(rec.flag){
		case 0://传输图片验证登录
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
			//case 1:
			//	break;
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

		default:
			cout<<"无效选项"<<endl;

		}
	}
	close(fd);
	return 0;
}
