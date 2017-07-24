#pragma once
#include <string>
using namespace std;

class Db;
class CDBMgr
{
public:
	static CDBMgr& Instance();
	CDBMgr(void);
	~CDBMgr(void);

	int openDB(const char* dbFile);
	int closeDB();

	int putString(const char* key, const char* value);
	int getString(const char* key, string& value);

	int putBlob(const char* key, const unsigned char* data, int len);
	unsigned char* getBlob(const char* key, int& len);
private:
	Db* m_pDB;
};
