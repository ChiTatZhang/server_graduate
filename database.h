#ifndef _DATABASE_H
#define _DATABASE_H

struct imgpath{
	int id;
	char path[32];
};
struct  staffinfo
{
	int id;
	char name[32];
	char telephone[32];
	char emile[32];
	char starttime[32];
	char jobtitle[32];
	char dept[32];
	char passwd[32];
	int authority;
};
struct attendence{
	int id;
	char name[16];
	char date[16];
	char comeTime[16];
	char leaveTime[16];
	double workTime;
	int bookIn;
	int evection;
	int vacate;
	char account[128];
};
struct iochalk{
	int id;
	char name[15];
	char date[32];
	int iostatus;
};
struct overtime{
	int id; 
	char name[15];
	char date[15];
	char action[15];
	char end[15];
	int interval;
};
struct vacate
{
	int id; 
	char name[15];
	char action[15];
	char end[15];
	int intervar;
	char cause[127];
};
struct evection{
	int id;
	char name[15];
	char action[15];
	char end[15];
	int interval;
	char loction[31];
	char cause[127];
}  ;
struct stats{
	int id;
	char name[15];
	char date[15];
	int late;
	int leave;
	int truant;
	int vacate;
	int evection;
};
struct settime{
	char ss[15];
	char sx[15];
	char xs[15];
	char xx[15];
};
struct caller{
	char name[16];
	char who[16];
	char time[32];
	char cause[128];
};

int create_table();
int sqlite(const char *sql);
int sqlite(struct staffinfo **data,const char *sql,int *num);
int sqlite(struct attendence **data,const char *sql,int *num);
int sqlite(struct iochalk **data,const char *sql,int *num);
int sqlite(struct overtime **data,const char *sql,int *num);
int sqlite(struct vacate **data,const char *sql,int *num);
int sqlite(struct evection **data,const char *sql,int *num);
int sqlite(struct stats **data,const char *sql,int *num);
int sqlite(struct settime **data,const char *sql,int *num);
int sqlite(struct caller **data,const char *sql,int *num);
int sqlite(struct imgpath **data,const char *sql,int *num);
#endif //_DATABASE_H
