// testbdb.cpp : Defines the entry point for the console application.
//


#include <string>
using namespace std;

#include "DBMgr.h"
int main(int argc, char* argv[])
{
	CDBMgr::Instance().openDB("d:\\1.db");
	CDBMgr::Instance().putString("key1", "value1");
	string str;
	CDBMgr::Instance().getString("key1", str);
	

	unsigned char buff[256] = {'a'};

	CDBMgr::Instance().putBlob("image", buff, 256);//�ڴ��ļ����浽���ݿ�
	int len = 0;
	unsigned char* pOut = CDBMgr::Instance().getBlob("image", len);//�����ݿ���ļ�����

	CDBMgr::Instance().closeDB();
	getchar();
	return 0;
}

