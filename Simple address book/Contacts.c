#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#pragma warning (disable:4996)
enum Contact   //枚举变量
{
	Quit,      //默认为0，下面依次递增+1
	Add,       //1
	Delect,    //2
	Select,    //3
	Alter,     //4
 	Show,      //5
	Empty,     //6
	Sort	      //7
};
//打印菜单
void menu()            
{
	printf("#########################################################\n");
	printf("##############      C语言实现简易通讯录     #############\n");
	printf("##############         1.添加联系人         #############\n");
	printf("##############         2.删除联系人         #############\n");
	printf("##############         3.查找联系人         #############\n");
	printf("##############         4.修改联系人         #############\n");
	printf("##############         5.显示所有联系人     #############\n");
	printf("##############         6.清空联系人         #############\n");
	printf("##############         7.按联系人名字排序   #############\n");
	printf("##############         0.退出               #############");
}
//定义一个结构体
typedef struct Peo
{
	char name[10];
	char sex[5];
	char Old[10];
	char Phone_num[20];
	char adrress[30];
}people;
people Peo[1000]; //定义一个可以存储1000用户的结构图数组
//添加联系人
int Add_Peo(people Peo[], int n)   
{
	int i = 0;
	char select = 0;
	char Rem[20];
	while (select != 'N')   //判断是否继续添加联系人
	{
		printf("姓名:");
		scanf("%s", &Peo[n + i].name);
		printf("性别:");
		scanf("%s", &Peo[n + i].sex);
		printf("年龄:");
		scanf("%s", &Peo[n + i].Old);
		printf("电话:");
		scanf("%s", &Peo[n + i].Phone_num);
		printf("地址:");
		scanf("%s", &Peo[n + i].adrress);
		gets(Rem);           //读取字符串
		printf("\n是否继续添加?(Y/N):");
		scanf("%c", &select);
		i++;
	}
	return (n + i);    
}
//删除联系人
int Del_Peo(people Peo[], int n)
{
	int i = 0;
	int j = 0;
	char Name[15];
	printf("请输入你要删除联系人的姓名：");
	scanf("%s", Name);
	while ((strcmp(Peo[i].name, Name) != 0) && (i < n))
	{       //判断输入的与结构体数组中存在的是否匹配，此处为不匹配，且未到达联系人上限
		i++;
	}
	if (i == n)  //结构体的出口，说明到达联系人上限
	{
		printf("没有此联系人！\n\n");
		return n;
	}
	for (j = i; j < n - 1; j++) //找到匹配的联系人，删除后，将删除处之后的数据前移一位
	{
		strcpy(Peo[j].name, Peo[j + 1].name);
		strcpy(Peo[j].sex, Peo[j + 1].sex);
		strcpy(Peo[j].Old, Peo[j + 1].Old);
		strcpy(Peo[j].Phone_num, Peo[j + 1].Phone_num);
		strcpy(Peo[j].adrress, Peo[j + 1].adrress);
	}
	printf("删除成功！\n");
	return (n - 1);
}
//查找联系人
void Sel_Peo(people Peo[], int n)
{
	char Name[15];
	int i = 0;
	printf("请输入要查看联系人的姓名：");
	scanf("%s", Name);
	while ((strcmp(Peo[i].name, Name) != 0) && (i < n))
	{
		i++;
	}
	if (i == n)
	{
		printf("查找的联系人不存在！\n\n");
	}
	else
	{
		printf("姓名:%s\n", Peo[i].name);
		printf("性别:%s\n", Peo[i].sex);
		printf("年龄:%s\n", Peo[i].Old);
		printf("电话:%s\n", Peo[i].Phone_num);
		printf("地址:%s\n", Peo[i].adrress);
	}
	printf("\n");
}
//修改联系人信息
void Alter_Peo(people Peo[], int n)
{
	int i = 0;
	char Name[15];
	printf("请输入你要修改的联系人姓名:");
	scanf("%s", Name);
	while ((strcmp(Peo[i].name, Name) != 0) && (i < n))
	{
		i++;
	}
	if (i == n)
	{
		printf("未查到此联系人！\n\n");
	}
	else
	{
		printf("姓名:");
		scanf("%s", &Peo[i].name);
		printf("性别:");
		scanf("%s", &Peo[i].sex);
		printf("年龄:");
		scanf("%s", &Peo[i].Old);
		printf("电话:");
		scanf("%s", &Peo[i].Phone_num);
		printf("地址:");
		scanf("%s", &Peo[i].adrress);
	}
}
//显示所有联系人信息
void Show_Peo(people Peo[], int n)
{
	int i = 0;
	printf("   姓名    性别    年龄    电话           地址\n");
	for (i = 0; i < n; i++)
	{
		printf("%7s%6s%8s%14s%15s\n", Peo[i].name, Peo[i].sex, Peo[i].Old, Peo[i].Phone_num, Peo[i].adrress);
	}
	printf("\n");
}
//清空联系人信息
void Empty_Peo(people Peo[], int n)
{
	int i = 0;
	for (i = 0; i < n; i++)
	{
		*Peo[i].name = '\0';
		*Peo[i].sex = '\0';
		*Peo[i].Old = '\0';
		*Peo[i].Phone_num = '\0';
		*Peo[i].adrress = '\0';
	}
	printf("\n");
}
//按联系人名字排序
void Sort_Peo(people Peo[], int n)
{
	int i = 0;
	int j = 0;
	struct Peo tmp;
	for (i = 0; i < n - 1; i++)
	{
		for (j = i; j < n; j++)
		{
			if ((strcmp(Peo[i].name, Peo[j].name)) > 0)
			{
				tmp = Peo[i];
				Peo[i] = Peo[j];
				Peo[j] = tmp;
			}
		}
	}
}
int main()
{
	int input = 1;
	int ret = 0;
	while (input)
	{
		menu();
		printf("\n请选择-->");
		scanf("%d", &input);
		switch (input)
		{
		case Add:
			ret = Add_Peo(Peo, ret);
			break;
		case Delect:
			ret = Del_Peo(Peo, ret);
			break;
		case Select:
			Sel_Peo(Peo, ret);
			break;
		case Alter:
			Alter_Peo(Peo, ret);
			break;
		case Show:
			Show_Peo(Peo, ret);
			break;
		case Empty:
			Empty_Peo(Peo, ret);
			break;
		case Sort:
			Sort_Peo(Peo, ret);
			break;
		case Quit:
			exit(0);
			break;
		default:
			break;
		}
	}
	system("pause");
	return 0;
}