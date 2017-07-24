# test_BerkeleyDB
最近要使用一个嵌入式key value的数据库，最初选择是level db。不过在windows下编译非常麻烦，所以另外选择了Berkeley DB。使用也非常简单，可以从源码编译，也可以安装官方的安装版，然后copy出.h,.lib和.dll集成到自己程序中使用。本文使用的版本是5.3，使用到的文件如下：
libdb53.lib
libdb_sql53.lib
libdb_stl53.lib
libdb53.dll
还有include目录整个拿来。
目前简单测试了两个功能：key value的读写， 二进制的读写。
其实这两种用法很接近：

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

主要区别应该就是二进制多了个长度吧。
简单做了封装，代码在:https://github.com/sxcong/test_BerkeleyDB
VC2010工程，把代码放到vc2008或以上版本应该都可以。
可能会碰到一个问题，提示某个宏已经定义，我这边的方案是修改系统文件的宏，不要改bdb .h的，除非自己重编译。