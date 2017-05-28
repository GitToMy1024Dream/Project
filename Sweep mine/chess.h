#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
#include<time.h>
#define use _CRT_SECURE_NO_WARNINGS
#define rows 10
#define lows 10
#define sweep_num 25
void init(char mine[rows][lows],char show[rows][lows]);
void display(char mine[rows][lows]);
void set_sweep(char show[rows][lows]);
void display_sweep(char show[rows][lows]);
void onclick_sweep_num(char show[rows][lows]);
void init(char mine[rows][lows],char show[rows][lows])
{
     int i=0;
	 int j=0;
	 for(i=0;i<rows;i++)
	 {
		 for(j=0;j<lows;j++)
		 {
			 mine[i][j]=' ';
 
		 }
	 }
 
	 for(i=0;i<rows;i++)
	 {
		 for(j=0;j<lows;j++)
		 {
			 show[i][j]=' ';
		 }
	 }
 
	 display(mine);
     set_sweep(show);
	 onclick_sweep_num(show,mine);
	 //onclick_sweep_num(mine);
	//display_sweep(show);
 
}
 
void display(char mine[rows][lows])
{
    int i=0;
	int j=0;
	printf("  ");
	for(i=0;i<rows;i++)
	{
		printf("%4d",i);
	}
	printf("\n");
	printf("   |---|---|---|---|---|---|---|---|---|---|\n"); 
	for(i=0;i<rows;i++)
	{
	printf("%3d| %c | %c | %c | %c | %c | %c | %c | %c | %c | %c |\n",i,mine[i][0],mine[i][1],mine[i][2],mine[i][3],mine[i][4],mine[i][5],mine[i][6],mine[i][7],mine[i][8],mine[i][9]);
	printf("   |---|---|---|---|---|---|---|---|---|---|\n"); 
	}
}
 
void display_sweep(char show[rows][lows])
{
    int i=0;
	int j=0;
	printf("  ");
	for(i=0;i<rows;i++)
	{
		printf("%4d",i);
	}
	printf("\n");
	printf("   |---|---|---|---|---|---|---|---|---|---|\n"); 
	for(i=0;i<rows;i++)
	{
	printf("%3d| %c | %c | %c | %c | %c | %c | %c | %c | %c | %c |\n",i+1,show[i][0],show[i][1],show[i][2],show[i][3],show[i][4],show[i][5],show[i][6],show[i][7],show[i][8],show[i][9]);
	printf("   |---|---|---|---|---|---|---|---|---|---|\n"); 
	}
	
}
void set_sweep(char show[rows][lows])  
{  
    int i=0;  
    int x=0,y=0;  
    for(i=0;i<sweep_num;)  
    {  
	    x=rand()%rows;  
        y=rand()%lows;
		if(show[x][y]==' ')
		{
		show[x][y]='*';
		i++;
		}
    } 
 
}
void onclick_sweep_num(char show[rows][lows],char mine[rows][lows])
{
	int i=0;
	int j=0;
	int count=0;
	int m=rows*lows;	
	while(m-sweep_num>0)
	{
	 
		printf("ÇëÊäÈë×ø±ê-->:");
	    scanf("%d,%d",&i,&j);	
		m--;
		system("cls");
		
		if(show[i][j]=='*')
		{
			printf("ÄãÕæ±¿£¬²ÈÀ×ÁË£¡\n");
			display_sweep(show);
			return;
		}
 
     if(show[i][j]!='*')
	 {
		 if(((i>=1)&&(i<=(rows-2)))&&((j>=1)&&j<=(lows-2)))
		 {
			// char arr[]={'0','1','2','3','4','5','6','7'};
			 char count='0';
			 if(show[i-1][j-1]=='*')
			 {
				 count++;
			 }
			 if(show[i-1][j]=='*')
			 {
				 count++;
			 }
			 if(show[i-1][j+1]=='*')
			 {
				 count++;
			 }
			 if(show[i][j+1]=='*')
			 {
				 count++;
			 }
			 if(show[i+1][j+1])
			 {
				 count++;
			 }
			 if(show[i+1][j]=='*')
			 {
				 count++;
			 }
			 if(show[i+1][j-1]=='*')
			 {
				 count++;
			 }
			 if(show[i][j-1]=='*')
			 {
				 count++;
			 }
			
		 mine[i][j]=count;
		 display(mine);
		 }
		 //2
		 if(((i>=1)&&(i<=(rows-2)))&&(j==0))
		 {
			 
			 //char arr[]={'0','1','2','3','4','5','6','7'};
			 char count='0';
			 if(show[i-1][j]=='*')
			 {
				 count++;
			 }
			 if(show[i-1][j+1]=='*')
			 {
				 count++;
			 }
			 if(show[i][j+1]=='*')
			 {
				 count++;
			 }
			 if(show[i+1][j+1]=='*')
			 {
				 count++;
			 }
			 if(show[i+1][j]=='*')
			 {
				 count++;
			 }
			 
		 mine[i][j]=count;
		 display(mine);
		 }
		 //3
		 if((i==0)&&((j>=1)&&(j<=(rows-2))))
		 {
			// char arr[]={'0','1','2','3','4','5','6','7'};
			 char count='0';
			 if(show[i][j-1]=='*')
			 {
				 count++;
			 }
			 if(show[i][j+1]=='*')
			 {
				 count++;
			 }
			 if(show[i+1][j-1]=='*')
			 {
				 count++;
			 }
			 if(show[i+1][j]=='*')
			 {
				 count++;
			 }
			 if(show[i+1][j+1]=='*')
			 {
				 count++;
			 }
			 mine[i][j]=count;
			 display(mine);
		 }
			 //4
			 if((i>=1)&&(i<=(rows-2))&&(j==(rows-1)))
			 {
				// char arr[]={'0','1','2','3','4','5','6','7'};
			 char count='0';
				 if(show[i-1][j]=='*')
				 {
					 count++;
				 }
				 if(show[i-1][j-1]=='*')
				 {
					 count++;
				 }
				 if(show[i][j-1]=='*')
				 {
					 count++;
				 }
				 if(show[i+1][j-1]=='*')
				 {
					 count++;
				 }
				 if(show[i+1][j]=='*')
				 {
					 count++;
				 }
				 mine[i][j]=count;
				 display(mine);
			 }
			 //5
			 if((i==(rows-1))&&((j>=1)&&(j<=(rows-2))))
			 {
				 //char arr[]={'0','1','2','3','4','5','6','7'};
			 char count='0';
				 if(show[i][j-1]=='*')
				 {
					 count++;
				 }
				 if(show[i-1][j-1]=='*')
				 {
					 count++;
				 }
				 if(show[i-1][j]=='*')
				 {
					 count++;
				 }
				 if(show[i-1][j+1]=='*')
				 {
					 count++;
				 }
				 if(show[i][j+1]=='*')
				 {
					 count++;
				 }
				 mine[i][j]=count;
				 display(mine);
			 }
			 //6
			 if((i==0)&&(j==0))
			 {
				// char arr[]={'0','1','2','3','4','5','6','7'};
			  char count='0';
				 if(show[i][j+1]=='*')
				 {
					 count++;
				 }
				 if(show[i+1][j+1]=='*')
				 {
					 count++;
				 }
				 if(show[i+1][j]=='*')
				 {
					 count++;
				 }
				 mine[i][j]=count;
				 display(mine);
			 }
			 //7
			 if((i==(rows-1))&&(j==0))
			 {
				 //char arr[]={'0','1','2','3','4','5','6','7'};
			 char count='0';
				 if(show[i-1][j]=='*')
				 {
					 count++;
				 }
				 if(show[i-1][j+1]=='*')
				 {
					 count++;
				 }
				 if(show[i][j+1]=='*')
				 {
					 count++;
				 }
				 mine[i][j]=count;
				 display(mine);
			 }
			 //8
			 if((i==0)&&(j==(lows-1)))
			 {
				 //char arr[]={'0','1','2','3','4','5','6','7'};
			  char count='0';
				 if(show[i][j-1]=='*')
				 {
					 count++;
				 }
				 if(show[i+1][j-1]=='*')
				 {
					 count++;
				 }
				 if(show[i+1][j]=='*')
				 {
					 count++;
				 }
				 mine[i][j]=count;
				 display(mine);
			 }
			 //9
			 if((i==(rows-1))&&(j==(lows-1)))
			 {
				// char arr[]={'0','1','2','3','4','5','6','7'};
			  char count='0';
				 if(show[i-1][j]=='*')
				 {
					 count++;
				 }
				 if(show[i-1][j-1]=='*')
				 {
					 count++;
				 }
				 if(show[i][j-1]=='*')
				 {
					 count++;
				 }
				 mine[i][j]=count;
				 display(mine);
			 }
 
	 }
	 }
	   
	 if(m-sweep_num==0)
	 {
      printf("¹§Ï²Äã£¬ÅÅÀ×³É¹¦£¡\n");
	 }	
	 }