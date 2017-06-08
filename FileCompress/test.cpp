#include "FileCompress.h"
void Test1()
{
	FileCompress fc;
	fc.CompressFile("1.txt");
	fc.UnCompressFile("1.hzp");
}
void Test2()
{
	FileCompress fc;
	fc.CompressFile("1.jpg");
	fc.UnCompressFile("2.compress");
}
int main()
{
	//Test1();
	Test2();
	system("pause");
	return 0;
}