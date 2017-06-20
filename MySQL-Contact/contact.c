#include <stdio.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 1024
MYSQL mysql;//mysql句柄
void Connect(const char* host,const char* user,const char* passwd,const char* db)
{
	mysql_init(&mysql);//初始化
	if(!mysql_real_connect(&mysql,host,user,passwd,db,3306,NULL,0))
	{
		printf("mysql_real_connect faile\n");
		exit(0);
	}
	printf("mysql connect success\n");
}
//插入
void Insert(const char* msg)
{
	if(msg)
	{
		char buf[SIZE] = "insert into book(name,tel,mail,addr) values";
		strcat(buf,msg);
		int len = strlen(buf);
		//执行成功返回0，失败返回非0
		if(mysql_real_query(&mysql,buf,len))
		{
			printf("mysql_real_query insert faile\n");
			return ;
		}
		printf("Insert success.\n");
	}
}
//删除
void Delete(const char* condition)
{
	char buf[SIZE] = "delete from book where ";
	strcat(buf,condition);
	int len = strlen(buf);
	//成功返回0，失败返回非0
	if(mysql_real_query(&mysql,buf,len))
	{
		printf("mysql_real_query delete faile\n");
		return ;
	}
	printf("Delete success.\n");
}
//查找
void Select(const char* condition)
{
	char buf[SIZE] = {0};
	//查找某一行信息
	if(condition)
	{
		strcpy(buf,"select * from book where ");
		strcat(buf,condition);
	}
	//打印表中的全部信息
	else 
	{
		strcpy(buf,"select * from book");
	}
	//成功返回0，失败返回非0
	int len = strlen(buf);
	if(mysql_real_query(&mysql,buf,len))
	{
		printf("mysql_read_query select faile\n");
		return ;
	}
	MYSQL_RES* res;//用来指向结果集
	MYSQL_ROW* row;//用来获取某一行的值
	//将结果集存储起来
	if(!(res=mysql_store_result(&mysql)))
	{
		printf("mysql_store_result faile\n");
		mysql_free_result(res);
		return ;
	}
	unsigned int num = mysql_num_fields(res);//获取结果表中的列数
	printf("%s\t %-3s\t %-3s\t %-5s\n","name","tel","mail","addr");
	//获取结果集中的每一行信息
	while((row=mysql_fetch_row(res)))
	{
		unsigned int i = 0;
		//打印同行不同列的信息
		for(i = 0;i < num;i++)
		{
			printf("%s\t",row[i]);
		}
		printf("\n");
	}
	mysql_free_result(res);//释放内存
}
//更新某一列的信息
void Updata(const char* column,const char* condition)
{
	if(condition)
	{
		char buf[SIZE] = "update book set ";
		strcat(buf,column);
		strcat(buf,"where ");
		strcat(buf,condition);
		int len = strlen(buf);
		//成功返回0，失败返回非0
		if(mysql_real_query(&mysql,buf,len))
		{
			printf("mysql_real_query updata faile\n");
			return ;
		}
		printf("Updata success.\n");
	}
}
//清空表中的所有数据
void Clear(const char* condition)
{
	if(condition)
	{
		char buf[SIZE] = "truncate table ";
		strcat(buf,condition);
		int len = strlen(buf);
		if(mysql_real_query(&mysql,buf,len))
		{
			printf("mysql_real_query clear faile\n");
			return ;
		}
		printf("Clear success.\n");
	}
}
//排序
void Sort(const char* condition)
{
	if(condition)
	{
		char buf[SIZE] = "select * from book order by ";
		strcat(buf,condition);
		int len = strlen(buf);
		if(mysql_real_query(&mysql,buf,len))
		{
			printf("mysql_real_query sort faile\n");
			return ;
		}
		printf("Sort success.\n");
	}
	MYSQL_RES* res;//用来指向结果集
	MYSQL_ROW* row;//用来获取某一行的值
	//将结果集存储起来
	if(!(res=mysql_store_result(&mysql)))
	{
		printf("mysql_store_result faile\n");
		mysql_free_result(res);
		return ;
	}
	unsigned int num = mysql_num_fields(res);//获取结果表中的列数
	printf("%s\t %-3s\t %-3s\t %-5s\n","name","tel","mail","addr");
	//获取结果集中的每一行信息
	while((row=mysql_fetch_row(res)))
	{
		unsigned int i = 0;
		//打印同行不同列的信息
		for(i = 0;i < num;i++)
		{
			printf("%s\t",row[i]);
		}
		printf("\n");
	}
	mysql_free_result(res);//释放内存
}
void menu()
{
	int choice = 0;
	while(1)
	{
		printf("************ Contact  ***************\n");
		printf(" ____________________________________ \n");
		printf("|    0.exit  *******  1.insert      |\n");
		printf("|    2.delete *****   3.select      |\n");
		printf("|    4.updata  ***    5.display     |\n");
		printf("|    6.clear    *     7.sort        |\n");
		printf("乛乛乛乛乛乛乛乛乛乛乛乛乛乛乛乛乛乛 \n");
		printf("请选择：");
		scanf("%d",&choice);
		char buf[100] = {0};
		char condition[100] = {0};
		switch(choice)
		{
			case 0:
				exit(1);
				break;
			case 1:	
				printf("请输入要插入的联系人信息(name,tel,mail,addr):\n");
				scanf("%s",buf);
				Insert(buf);
				break;
			case 2:
				printf("请输入要删除的name,tel,mail,addr(其中任意一个:name='Marry'):\n");
				scanf("%s",buf);
				Delete(buf);
				break;
			case 3:
				printf("请输入要查询的name,tel,mail,addr(其中任意一个:name='Marry'):\n");
				scanf("%s",condition);
				Select(condition);
				break;
			case 4:
				printf("请输入要更新的name,tel,mail,addr(任意一个):name='Marry':\n");
				scanf("%s",buf);
				Updata(buf,condition);
				break;
			case 5:
			    Select(NULL);
				break;
			case 6:
				printf("请输入你要删除的表：\n");
				scanf("%s",condition);
				Clear(condition);
				break;
			case 7:
				printf("请输入你要按什么排序(name desc(降序) asc(升序))?\n");
				scanf("%s",condition);
				Sort(condition);
				break;
			default:
				break;
		}
	}
}
int main()
{
	//连接mysql数据库
	Connect("localhost","root","wy19951024","contact");
	menu();
	mysql_close(&mysql);//关闭与mysql的关联
	return 0;
}
