#pragma once
#include <iostream>
#include <string.h>
#include <assert.h>
#include "heap.h"
#include "huffmanTree.h"
#pragma warning (disable:4996)
using namespace std;
struct FileInfo//文件信息
{
	char _ch;//字符
	long long _apperCount;//字符出现的次数
	string _strCode;//编码
	FileInfo(long long apearCount = 0)
		:_apperCount(apearCount)
	{}
	bool operator!=(const FileInfo& _fileCount)const
	{
		return _apperCount != _fileCount._apperCount;
	}
	bool operator<(const FileInfo& _fileCount)const
	{
		return _apperCount < _fileCount._apperCount;
	}
	size_t operator+(const FileInfo& _fileCount)
	{
		return _apperCount + _fileCount._apperCount;
	}
};
class FileCompress
{
public:
	FileCompress()
	{
		//初始化
		for (size_t idx = 0; idx < 256; idx++)
		{
			_fileInfo[idx]._ch = idx;
		}
	}
	//压缩文件
	void CompressFile(const string _strFilePath)//获取文件路径
	{
		FILE* fIn = fopen(_strFilePath.c_str(), "rb");//fopen是const char*类型，获取一个C类型的字符串string::c_str
		assert(fIn != NULL);
		char* pReadBuff = new char[1024];//定义一个缓冲区来存放每次读取的字符
		//一次读取1024个字符
		while (true)
		{
			size_t readSize = fread(pReadBuff, 1, 1024, fIn);//成功读取的字符
			if (readSize == 0)
				break;
			for (size_t idx = 0; idx < readSize; ++idx)
			{
				//①统计字符出现的次数
				_fileInfo[pReadBuff[idx]]._apperCount++;
			}
		}
		//②通过次数构建huffman树
		FileInfo invaild(0);
		HuffmanTree<FileInfo> _hf(_fileInfo, sizeof(_fileInfo)/sizeof(_fileInfo[0]), invaild);

		//③获取哈夫曼编码
		_GenerateHuffmanCode(_hf.Root());
		FILE* fOut = fopen("1.hzp", "wb");

		//保存压缩信息(方便解压缩)
		 //1.构造压缩信息
		//1)后缀
		string strFileHeadInfo(GetFilePostFix(_strFilePath));//头部信息
		strFileHeadInfo += '\n';
		string strCodeInfo;//字符信息
		size_t lineCount = 0;//行号
		char szappearCount[32];//字符出现的子树;
		//2)保存字符出现的次数
		for (size_t idx = 0; idx < 256; ++idx)
		{
			if (_fileInfo[idx]._apperCount != 0)
			{
				strCodeInfo += _fileInfo[idx]._ch;//保存当前字符
				strCodeInfo += ',';//字符后面用，分隔
				memset(szappearCount, 0, 32);
				itoa(_fileInfo[idx]._apperCount, szappearCount, 10);//字符出现的次数
				strCodeInfo += szappearCount;//每一行的字符个数
				strCodeInfo += '\n';
				lineCount++;
			}
		}
		itoa(lineCount, szappearCount, 10);//转换回去
		strFileHeadInfo += szappearCount;//加载到头部信息的后面
		strFileHeadInfo += '\n';
		strFileHeadInfo += strCodeInfo;//将编码信息加载到后面
		fwrite(strFileHeadInfo.c_str(), 1, strFileHeadInfo.length(), fOut);
		//fwrite(strCodeInfo.c_str(), 1, strCodeInfo.length(), fOut);
		
		fseek(fIn,0,SEEK_SET);//将文件置于起始位置
		char value = 0;
		int pos = 1;
		char *Wbuff = new char[1024];
		size_t index = 0;
		assert(fOut);
		//④重新改写
		while (true)
		{
			size_t readSize = fread(pReadBuff, 1, 1024, fIn);
			if (readSize == 0)
				break;
			for (size_t idx = 0; idx < readSize; ++idx)
			{
				string& strCode = _fileInfo[pReadBuff[idx]]._strCode;
				int i = 0;
				while (i < strCode.size())
				{
					value <<= 1;
					if (strCode[i] == '1')
						value |= 1;
					i++;
					if (8 == pos)
					{
						pos = 1;
						Wbuff[index++] = value;
						if (index == 1024)
						{
							fwrite(Wbuff, 1, 1024, fOut);
							index = 0;
						}
						value = 0;
					}
					else
						pos++;
				}
			}
		}
		//如果不满一个字节，比如0000 0011，只有两位时，将11放在高位
		if (pos < 8)
		{
			value <<= (8 - pos + 1);//最后一个字符移位后未处理
			Wbuff[index++] = value;
			fwrite(Wbuff, 1, index, fOut);
		}
		fclose(fOut);
		fclose(fIn);
		delete[] Wbuff;
		delete[] pReadBuff;
	}
	//解压缩文件
	void UnCompressFile(const string _strFilePath)
	{
		//1.获取压缩信息
		FILE* fIn = fopen(_strFilePath.c_str(), "rb");
		assert(fIn);
		//获取文件的后缀
		string strFilePostFix;
		GetLine(fIn, strFilePostFix);
		//有效字符的行数
		string strLineNode;
		GetLine(fIn, strLineNode);
		size_t lineCount = atoi(strLineNode.c_str());
		string strCodeInfo;
		for (size_t idx = 0; idx < lineCount; ++idx)
		{
			GetLine(fIn, strCodeInfo);
			_fileInfo[strCodeInfo[0]]._apperCount = atoi(strCodeInfo.substr(2,strCodeInfo.length()-2).c_str());
			strCodeInfo = "";
		}
		//构建huffman树
		HuffmanTree<FileInfo> ht(_fileInfo, sizeof(_fileInfo) / sizeof(_fileInfo[0]), FileInfo(0));
		HuffmanTreeNode<FileInfo>* pCur = ht.Root();
		//写入新的文件
		FILE* fOut = fopen("2.txt", "wb");
		assert(fOut);
		int pos = 7;
		size_t writeSize = 0;
		char *RdBuff = new char[1024];//读
		char *WrBuff = new char[1024];//写
		long long fileSize = pCur->_weight._apperCount;//源文件大小
		//读取编码信息
		while (true)
		{
			size_t readSize = fread(RdBuff, 1, 1024, fIn);//一次读取1个字节
			if (0 == readSize)
				break;
			for (size_t idx = 0; idx < 1024;)
			{
				//测试该比特位是0还是1
				if (RdBuff[idx] & (1 << pos))
				{
					pCur = pCur->_pRight;
				}
				else
					pCur = pCur->_pLeft;
				if (pCur->_pLeft == NULL && pCur->_pRight == NULL)//叶子节点
				{
					//保存字符
					WrBuff[writeSize++] = pCur->_weight._ch;
					if (writeSize == 1024)
					{
						fwrite(WrBuff, 1, 1024, fOut);
						writeSize = 0;
					}
					fileSize--;
					if (fileSize == 0)
						break;
					pCur = ht.Root();//回到根的位置
				}
				--pos;
				if (pos < 0)
				{
					pos = 7;
					idx++;
				}
			}
		}
		if (writeSize)
		{
			fwrite(WrBuff, 1, writeSize, fOut);
		}
		delete[] RdBuff;
		delete[] WrBuff;
		fclose(fIn);
		fclose(fOut);
	}
	//获取一行的信息
	void GetLine(FILE* pf, string& strRet)
	{
		//feof判断是否到达文件结尾
		if (feof(pf))
		{
			return;//文件结尾
		}
		char c = fgetc(pf);//读取一个字符
		//看是否是\n
		while (c != '\n')
		{
			strRet += c;
			if (feof(pf))
			{
				break;//文件结尾
			}
			c = fgetc(pf);
		}
	}
private:
	//获取哈夫曼编码,找到叶子节点
	void  _GenerateHuffmanCode(HuffmanTreeNode<FileInfo>* pRoot)
	{
		if (pRoot)
		{
			_GenerateHuffmanCode(pRoot->_pLeft);
			_GenerateHuffmanCode(pRoot->_pRight);
			if (pRoot->_pLeft == NULL && pRoot->_pRight == NULL)
			{
				HuffmanTreeNode<FileInfo>* pCur = pRoot;
				HuffmanTreeNode<FileInfo>* pParent = pCur->_pParent;
				//通过字符ch获取在fileinfo数组中的位置
				string& _strCode = _fileInfo[pRoot->_weight._ch]._strCode;
				//自底向上获取编码(编码是反的)
				while(pParent)
				{
					if (pCur == pParent->_pLeft)
						_strCode += '0';
					else
						_strCode += '1';
					pCur = pParent;
					pParent = pParent->_pParent;
				}
				//逆置编码
				reverse(_strCode.begin(), _strCode.end());
			}
		}
	}
	string GetFilePostFix(const string& FilePath)
	{
		size_t pos = FilePath.find_last_of('.');//寻找后缀的·
		return FilePath.substr(pos, FilePath.length() - pos);//截取字符串的后缀
	}
public:
	FileInfo _fileInfo[256];//因为字符有256种状态
};



