#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <iostream>
#include "database.h"
using namespace std;

int create_table()
{
	sqlite3 *db;
	char *zErrMsg;
	int rc;
	rc = sqlite3_open("test.db",&db);
	if(rc != 0)
	{
		fprintf(stderr,"Can't open database:%s\r\n",sqlite3_errmsg(db));
		return -1;
	}else{
		fprintf(stderr,"Opened database successfully\r\n");
	}

	const char *sql;
	sql = "create table student("\
	       "stdno int primary key not null,"\
	       "name text not null,"\
	       "age int not null,"\
	       "score real);";
	rc = sqlite3_exec(db,sql,NULL,0,&zErrMsg);
	if(rc != SQLITE_OK)
	{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		fprintf(stdout,"Table created successfully\r\n");
	}
	sqlite3_close(db);
	return 0;
}



int sqlite(const char *sql)
{
	sqlite3 *db;
	char *zErrMsg;
	int rc;
	rc = sqlite3_open("test.db",&db);
	if(rc != 0)
	{
		fprintf(stderr,"Can't open database:%s\r\n",sqlite3_errmsg(db));
		//return -1;
	}else{
		fprintf(stderr,"Opened database successfully\r\n");
	}

	//sql = "update student set score=103 where name='zhang';";
	rc = sqlite3_exec(db,sql,NULL,0,&zErrMsg);
	if(rc != 0)
	{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		sqlite3_close(db);
		return -1;
	}
	else
	{
		fprintf(stdout,"sql successfully\r\n");
		rc=sqlite3_changes(db);
	}
	sqlite3_close(db);
	cout<<rc<<endl;
	return rc;
}

int sqlite(struct staffinfo **data,const char *sql,int *num)
{
	sqlite3 *db;
	char *zErrMsg;
	char ** pResult;
	int row,col;
	int rc;
	rc = sqlite3_open("test.db",&db);
	if(rc != 0)
	{
		fprintf(stderr,"Can't open database:%s\r\n",sqlite3_errmsg(db));
		return -1;
	}else{
		fprintf(stderr,"Opened database successfully\r\n");
	}

	//const char *sql;
	//sql = "select * from student ;";
	rc = sqlite3_get_table(db,sql,&pResult,&row,&col,&zErrMsg);
	if(rc != SQLITE_OK)
	{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return -1;
	}
	else
	{
		fprintf(stdout,"select item successfully\r\n");
	}
	*num=row;
	*data=(struct staffinfo *)malloc(sizeof(struct staffinfo)*(*num));
	struct staffinfo *tmpdata=*data;
	int tmpCol=col;
	for(int i=0;i<row;i++)
	{
		//*data->stdno=pResult[tmpCol++];
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->id));
		strcpy(tmpdata->name,pResult[tmpCol++]);
		strcpy(tmpdata->telephone,pResult[tmpCol++]);
		strcpy(tmpdata->emile,pResult[tmpCol++]);
		strcpy(tmpdata->starttime,pResult[tmpCol++]);
		strcpy(tmpdata->jobtitle,pResult[tmpCol++]);
		strcpy(tmpdata->dept,pResult[tmpCol++]);
		strcpy(tmpdata->passwd,pResult[tmpCol++]);
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->authority));
		//cout<<(*data)->stdno<<(*data)->name<<(*data)->age<<(*data)->score<<endl;
		tmpdata++;
		/*cout<<"第"<<i<<"组数据"<<endl;
		  for(int j=0;j<col;j++)
		  {
		  cout<<pResult[j]<<": "<<pResult[tmpCol]<<endl;
		  tmpCol++;	
		  }*/

	}
	sqlite3_free_table(pResult);
	sqlite3_close(db);
	return 0;
}
int sqlite(struct attendence **data,const char *sql,int *num)
{
	sqlite3 *db;
	char *zErrMsg;
	char ** pResult;
	int row,col;
	int rc;
	rc = sqlite3_open("test.db",&db);
	if(rc != 0)
	{
		fprintf(stderr,"Can't open database:%s\r\n",sqlite3_errmsg(db));
		return -1;
	}else{
		fprintf(stderr,"Opened database successfully\r\n");
	}

	//const char *sql;
	//sql = "select * from student ;";
	rc = sqlite3_get_table(db,sql,&pResult,&row,&col,&zErrMsg);
	if(rc != SQLITE_OK)
	{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return -1;
	}
	else
	{
		fprintf(stdout,"select item successfully\r\n");
	}
	*num=row;
	*data=(struct attendence *)malloc(sizeof(struct attendence)*(*num));
	struct attendence *tmpdata=*data;
	int tmpCol=col;
	for(int i=0;i<row;i++)
	{
		//*data->stdno=pResult[tmpCol++];
		tmpCol++;
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->id));
		strcpy(tmpdata->name,pResult[tmpCol++]);
		strcpy(tmpdata->date,pResult[tmpCol++]);
		strcpy(tmpdata->comeTime,pResult[tmpCol++]);
		strcpy(tmpdata->leaveTime,pResult[tmpCol++]);
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->bookIn));
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->evection));
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->vacate));
		strcpy(tmpdata->account,pResult[tmpCol++]);
		sscanf(pResult[tmpCol++],"%lf",&(tmpdata->workTime));

		//cout<<(*data)->stdno<<(*data)->name<<(*data)->age<<(*data)->score<<endl;
		tmpdata++;
		/*cout<<"第"<<i<<"组数据"<<endl;
		  for(int j=0;j<col;j++)
		  {
		  cout<<pResult[j]<<": "<<pResult[tmpCol]<<endl;
		  tmpCol++;	
		  }*/

	}
	sqlite3_free_table(pResult);
	sqlite3_close(db);
	return 0;
}

int sqlite(struct iochalk **data,const char *sql,int *num)
{
	sqlite3 *db;
	char *zErrMsg;
	char ** pResult;
	int row,col;
	int rc;
	rc = sqlite3_open("test.db",&db);
	if(rc != 0)
	{
		fprintf(stderr,"Can't open database:%s\r\n",sqlite3_errmsg(db));
		return -1;
	}else{
		fprintf(stderr,"Opened database successfully\r\n");
	}

	//const char *sql;
	//sql = "select * from student ;";
	rc = sqlite3_get_table(db,sql,&pResult,&row,&col,&zErrMsg);
	if(rc != SQLITE_OK)
	{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return -1;
	}
	else
	{
		fprintf(stdout,"select item successfully\r\n");
	}
	*num=row;
	*data=(struct iochalk *)malloc(sizeof(struct iochalk)*(*num));
	struct iochalk *tmpdata=*data;
	int tmpCol=col;
	for(int i=0;i<row;i++)
	{
		//*data->stdno=pResult[tmpCol++];
		tmpCol++;
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->id));
		strcpy(tmpdata->name,pResult[tmpCol++]);
		strcpy(tmpdata->date,pResult[tmpCol++]);
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->iostatus));
		//cout<<(*data)->stdno<<(*data)->name<<(*data)->age<<(*data)->score<<endl;
		tmpdata++;
		/*cout<<"第"<<i<<"组数据"<<endl;
		  for(int j=0;j<col;j++)
		  {
		  cout<<pResult[j]<<": "<<pResult[tmpCol]<<endl;
		  tmpCol++;	
		  }*/

	}
	sqlite3_free_table(pResult);
	sqlite3_close(db);
	return 0;
}

int sqlite(struct overtime **data,const char *sql,int *num)
{
	sqlite3 *db;
	char *zErrMsg;
	char ** pResult;
	int row,col;
	int rc;
	rc = sqlite3_open("test.db",&db);
	if(rc != 0)
	{
		fprintf(stderr,"Can't open database:%s\r\n",sqlite3_errmsg(db));
		return -1;
	}else{
		fprintf(stderr,"Opened database successfully\r\n");
	}

	//const char *sql;
	//sql = "select * from student ;";
	rc = sqlite3_get_table(db,sql,&pResult,&row,&col,&zErrMsg);
	if(rc != SQLITE_OK)
	{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return -1;
	}
	else
	{
		fprintf(stdout,"select item successfully\r\n");
	}
	*num=row;
	*data=(struct overtime *)malloc(sizeof(struct overtime)*(*num));
	struct overtime *tmpdata=*data;
	int tmpCol=col;
	for(int i=0;i<row;i++)
	{
		//*data->stdno=pResult[tmpCol++];
		tmpCol++;
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->id));
		strcpy(tmpdata->name,pResult[tmpCol++]);
		strcpy(tmpdata->date,pResult[tmpCol++]);
		strcpy(tmpdata->action,pResult[tmpCol++]);
		strcpy(tmpdata->end,pResult[tmpCol++]);
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->interval));
		//cout<<(*data)->stdno<<(*data)->name<<(*data)->age<<(*data)->score<<endl;
		tmpdata++;
		/*cout<<"第"<<i<<"组数据"<<endl;
		  for(int j=0;j<col;j++)
		  {
		  cout<<pResult[j]<<": "<<pResult[tmpCol]<<endl;
		  tmpCol++;	
		  }*/

	}
	sqlite3_free_table(pResult);
	sqlite3_close(db);
	return 0;
}

int sqlite(struct vacate **data,const char *sql,int *num)
{
	sqlite3 *db;
	char *zErrMsg;
	char ** pResult;
	int row,col;
	int rc;
	rc = sqlite3_open("test.db",&db);
	if(rc != 0)
	{
		fprintf(stderr,"Can't open database:%s\r\n",sqlite3_errmsg(db));
		return -1;
	}else{
		fprintf(stderr,"Opened database successfully\r\n");
	}

	//const char *sql;
	//sql = "select * from student ;";
	rc = sqlite3_get_table(db,sql,&pResult,&row,&col,&zErrMsg);
	if(rc != SQLITE_OK)
	{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return -1;
	}
	else
	{
		fprintf(stdout,"select item successfully\r\n");
	}
	*num=row;
	*data=(struct vacate *)malloc(sizeof(struct vacate)*(*num));
	struct vacate *tmpdata=*data;
	int tmpCol=col;
	for(int i=0;i<row;i++)
	{
		//*data->stdno=pResult[tmpCol++];
		tmpCol++;
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->id));
		strcpy(tmpdata->name,pResult[tmpCol++]);
		strcpy(tmpdata->action,pResult[tmpCol++]);
		strcpy(tmpdata->end,pResult[tmpCol++]);
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->intervar));
		strcpy(tmpdata->cause,pResult[tmpCol++]);
		//cout<<(*data)->stdno<<(*data)->name<<(*data)->age<<(*data)->score<<endl;
		tmpdata++;
		/*cout<<"第"<<i<<"组数据"<<endl;
		  for(int j=0;j<col;j++)
		  {
		  cout<<pResult[j]<<": "<<pResult[tmpCol]<<endl;
		  tmpCol++;	
		  }*/

	}
	sqlite3_free_table(pResult);
	sqlite3_close(db);
	return 0;
}

int sqlite(struct evection **data,const char *sql,int *num)
{
	sqlite3 *db;
	char *zErrMsg;
	char ** pResult;
	int row,col;
	int rc;
	rc = sqlite3_open("test.db",&db);
	if(rc != 0)
	{
		fprintf(stderr,"Can't open database:%s\r\n",sqlite3_errmsg(db));
		return -1;
	}else{
		fprintf(stderr,"Opened database successfully\r\n");
	}

	//const char *sql;
	//sql = "select * from student ;";
	rc = sqlite3_get_table(db,sql,&pResult,&row,&col,&zErrMsg);
	if(rc != SQLITE_OK)
	{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return -1;
	}
	else
	{
		fprintf(stdout,"select item successfully\r\n");
	}
	*num=row;
	*data=(struct evection *)malloc(sizeof(struct evection)*(*num));
	struct evection *tmpdata=*data;
	int tmpCol=col;
	for(int i=0;i<row;i++)
	{
		//*data->stdno=pResult[tmpCol++];
		tmpCol++;
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->id));
		strcpy(tmpdata->name,pResult[tmpCol++]);
		strcpy(tmpdata->action,pResult[tmpCol++]);
		strcpy(tmpdata->end,pResult[tmpCol++]);
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->interval));
		strcpy(tmpdata->loction,pResult[tmpCol++]);
		strcpy(tmpdata->cause,pResult[tmpCol++]);
		//cout<<(*data)->stdno<<(*data)->name<<(*data)->age<<(*data)->score<<endl;
		tmpdata++;
		/*cout<<"第"<<i<<"组数据"<<endl;
		  for(int j=0;j<col;j++)
		  {
		  cout<<pResult[j]<<": "<<pResult[tmpCol]<<endl;
		  tmpCol++;	
		  }*/

	}
	sqlite3_free_table(pResult);
	sqlite3_close(db);
	return 0;
}

int sqlite(struct stats **data,const char *sql,int *num)
{
	sqlite3 *db;
	char *zErrMsg;
	char ** pResult;
	int row,col;
	int rc;
	rc = sqlite3_open("test.db",&db);
	if(rc != 0)
	{
		fprintf(stderr,"Can't open database:%s\r\n",sqlite3_errmsg(db));
		return -1;
	}else{
		fprintf(stderr,"Opened database successfully\r\n");
	}

	//const char *sql;
	//sql = "select * from student ;";
	rc = sqlite3_get_table(db,sql,&pResult,&row,&col,&zErrMsg);
	if(rc != SQLITE_OK)
	{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return -1;
	}
	else
	{
		fprintf(stdout,"select item successfully\r\n");
	}
	*num=row;
	*data=(struct stats *)malloc(sizeof(struct stats)*(*num));
	struct stats *tmpdata=*data;
	int tmpCol=col;
	for(int i=0;i<row;i++)
	{
		//*data->stdno=pResult[tmpCol++];
		tmpCol++;
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->id));
		strcpy(tmpdata->name,pResult[tmpCol++]);
		strcpy(tmpdata->date,pResult[tmpCol++]);
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->late));
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->leave));
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->truant));
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->vacate));
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->evection));
		//cout<<(*data)->stdno<<(*data)->name<<(*data)->age<<(*data)->score<<endl;
		tmpdata++;
		/*cout<<"第"<<i<<"组数据"<<endl;
		  for(int j=0;j<col;j++)
		  {
		  cout<<pResult[j]<<": "<<pResult[tmpCol]<<endl;
		  tmpCol++;	
		  }*/

	}
	sqlite3_free_table(pResult);
	sqlite3_close(db);
	return 0;
}

int sqlite(struct settime **data,const char *sql,int *num)
{
	sqlite3 *db;
	char *zErrMsg;
	char ** pResult;
	int row,col;
	int rc;
	rc = sqlite3_open("test.db",&db);
	if(rc != 0)
	{
		fprintf(stderr,"Can't open database:%s\r\n",sqlite3_errmsg(db));
		return -1;
	}else{
		fprintf(stderr,"Opened database successfully\r\n");
	}

	//const char *sql;
	//sql = "select * from student ;";
	rc = sqlite3_get_table(db,sql,&pResult,&row,&col,&zErrMsg);
	if(rc != SQLITE_OK)
	{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return -1;
	}
	else
	{
		fprintf(stdout,"select item successfully\r\n");
	}
	*num=row;
	*data=(struct settime *)malloc(sizeof(struct settime)*(*num));
	struct settime *tmpdata=*data;
	int tmpCol=col;
	for(int i=0;i<row;i++)
	{
		//*data->stdno=pResult[tmpCol++];
		strcpy(tmpdata->ss,pResult[tmpCol++]);
		strcpy(tmpdata->ss,pResult[tmpCol++]);
		strcpy(tmpdata->xs,pResult[tmpCol++]);
		strcpy(tmpdata->xx,pResult[tmpCol++]);
		//cout<<(*data)->stdno<<(*data)->name<<(*data)->age<<(*data)->score<<endl;
		tmpdata++;
		/*cout<<"第"<<i<<"组数据"<<endl;
		  for(int j=0;j<col;j++)
		  {
		  cout<<pResult[j]<<": "<<pResult[tmpCol]<<endl;
		  tmpCol++;	
		  }*/

	}
	sqlite3_free_table(pResult);
	sqlite3_close(db);
	return 0;
}

int sqlite(struct caller **data,const char *sql,int *num)
{
	sqlite3 *db;
	char *zErrMsg;
	char ** pResult;
	int row,col;
	int rc;
	rc = sqlite3_open("test.db",&db);
	if(rc != 0)
	{
		fprintf(stderr,"Can't open database:%s\r\n",sqlite3_errmsg(db));
		return -1;
	}else{
		fprintf(stderr,"Opened database successfully\r\n");
	}

	//const char *sql;
	//sql = "select * from student ;";
	rc = sqlite3_get_table(db,sql,&pResult,&row,&col,&zErrMsg);
	if(rc != SQLITE_OK)
	{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return -1;
	}
	else
	{
		fprintf(stdout,"select item successfully\r\n");
	}
	*num=row;
	*data=(struct caller *)malloc(sizeof(struct caller)*(*num));
	struct caller *tmpdata=*data;
	int tmpCol=col;
	for(int i=0;i<row;i++)
	{
		//*data->stdno=pResult[tmpCol++];
		tmpCol++;
		strcpy(tmpdata->name,pResult[tmpCol++]);
		strcpy(tmpdata->who,pResult[tmpCol++]);
		strcpy(tmpdata->time,pResult[tmpCol++]);
		strcpy(tmpdata->cause,pResult[tmpCol++]);
		//cout<<(*data)->stdno<<(*data)->name<<(*data)->age<<(*data)->score<<endl;
		tmpdata++;
		/*cout<<"第"<<i<<"组数据"<<endl;
		  for(int j=0;j<col;j++)
		  {
		  cout<<pResult[j]<<": "<<pResult[tmpCol]<<endl;
		  tmpCol++;	
		  }*/

	}
	sqlite3_free_table(pResult);
	sqlite3_close(db);
	return 0;
}

int sqlite(struct imgpath **data,const char *sql,int *num)
{
	sqlite3 *db;
	char *zErrMsg;
	char ** pResult;
	int row,col;
	int rc;
	rc = sqlite3_open("test.db",&db);
	if(rc != 0)
	{
		fprintf(stderr,"Can't open database:%s\r\n",sqlite3_errmsg(db));
		return -1;
	}else{
		fprintf(stderr,"Opened database successfully\r\n");
	}

	//const char *sql;
	//sql = "select * from student ;";
	rc = sqlite3_get_table(db,sql,&pResult,&row,&col,&zErrMsg);
	if(rc != SQLITE_OK)
	{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return -1;
	}
	else
	{
		fprintf(stdout,"select item successfully\r\n");
	}
	*num=row;
	*data=(struct imgpath *)malloc(sizeof(struct imgpath)*(*num));
	struct imgpath *tmpdata=*data;
	int tmpCol=col;
	for(int i=0;i<row;i++)
	{
		//*data->stdno=pResult[tmpCol++];
		sscanf(pResult[tmpCol++],"%d",&(tmpdata->id));
		strcpy(tmpdata->path,pResult[tmpCol++]);
		//cout<<(*data)->stdno<<(*data)->name<<(*data)->age<<(*data)->score<<endl;
		tmpdata++;
		/*cout<<"第"<<i<<"组数据"<<endl;
		  for(int j=0;j<col;j++)
		  {
		  cout<<pResult[j]<<": "<<pResult[tmpCol]<<endl;
		  tmpCol++;	
		  }*/

	}
	sqlite3_free_table(pResult);
	sqlite3_close(db);
	return 0;
}
#if 0
int main()
{
	struct student *stu;
	int num;
	const char *sql="select * from student;";
	const char *s="insert into student values(201511,'zzd',21,23.2);";
	const char *q="delete from student where stdno=20000;";
	const char *l= "update student set age=12 where stdno=201512;";
	const char *l1= "update student set age=12 where stdno=201511;";
	//create_table();
	int ret=sqlite(s);
	cout<<"insert:"<<ret<<endl;
	ret=sqlite(q);
	cout<<"delete:"<<ret<<endl;
	ret=sqlite(l);
	cout<<"update:"<<ret<<endl;
	ret=sqlite(l1);
	cout<<"update:"<<ret<<endl;
	sqlite(&stu,sql,&num);
	struct student *tmp=stu;
	//sqlite();
	cout<<"num="<<num<<endl;
	for(int i=0;i<num;i++)
	{
		cout<<tmp->stdno<<" ";
		cout<<tmp->name<<" ";
		cout<<tmp->age<<" ";
		cout<<tmp->score<<" ";
		tmp++;
	}
	//updata_item();

}
#endif
