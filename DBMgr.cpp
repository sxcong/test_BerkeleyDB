
#include "DBMgr.h"

#include <sys/types.h>
#include <iostream>
#include <iomanip>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define DB_USE_DLL 1
#include "db_cxx.h"
#pragma comment(lib, "libdb53.lib")


CDBMgr::CDBMgr(void)
{
	m_pDB = NULL;
}

CDBMgr::~CDBMgr(void)
{
}

CDBMgr& CDBMgr::Instance()
{
	static CDBMgr agent;
	return agent;
}

int CDBMgr::openDB(const char* dbFile)
{
	if (m_pDB != NULL)
		return 1;

	if (m_pDB != NULL)
		return -1;

	m_pDB = new Db(0, 0);
	m_pDB->set_error_stream(&cerr);
	m_pDB->set_errpfx("AccessExample");
	m_pDB->set_pagesize(1024);		// Page size: 1K. 
	m_pDB->set_cachesize(0, 32 * 1024, 0);
	m_pDB->open(NULL, dbFile, NULL, DB_BTREE, DB_CREATE, 0664);

	return 0;
}

int CDBMgr::closeDB()
{
	if (m_pDB)
	{
		m_pDB->close(0);
		delete m_pDB;
		m_pDB = NULL;
	}
	return 0;
}

int CDBMgr::putString(const char* key, const char* value)
{
	Dbt dbKey((char*)key, strlen(key)+1);
	Dbt dbValue((char*)value, strlen(value)+1);
	int ret = m_pDB->put(0, &dbKey, &dbValue, DB_NOOVERWRITE);
	if (ret == DB_KEYEXIST)
	{
		return 1;
	}
	return 0;
}

int CDBMgr::getString(const char* key, string& value)
{
	Dbt dbKey((char*)key, strlen(key)+1);
	Dbt data;
	int ret = m_pDB->get(NULL, &dbKey, &data, 0);
	if (ret == 0) 
	{
		char *key_string = (char *)dbKey.get_data();
		//char *data_string = (char *)data.get_data();
		//cout << key_string << " : " << data_string << "\n";
		value = (const char*)data.get_data();
		m_pDB->sync(0);
	}
	else
	{
		char* result = DbEnv::strerror(ret);
		printf(result);
	}
	
	return 0;
}

int CDBMgr::putBlob(const char* key, const unsigned char* data, int len)
{
	
	Dbt dbKey((char*)key, strlen(key)+1);
	Dbt dbValue((unsigned char*)data, len);
	int ret = m_pDB->put(0, &dbKey, &dbValue, DB_NOOVERWRITE);
	if (ret == DB_KEYEXIST)
	{
		return 1;
	}
	m_pDB->sync(0);
	
	return 0;
}

unsigned char* CDBMgr::getBlob(const char* key, int& len)
{
	Dbt dbKey((char*)key, strlen(key)+1);
	Dbt dbValue;
	int ret = m_pDB->get(NULL, &dbKey, &dbValue, 0);
	if (ret == 0) 
	{
		char *key_string = (char *)dbKey.get_data();
		
		len = dbValue.get_size();
		return (unsigned char*)dbValue.get_data();
	}
	else
	{
		char* result = DbEnv::strerror(ret);
		printf(result);
	}
	return NULL;
}