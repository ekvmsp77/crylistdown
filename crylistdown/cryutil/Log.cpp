
#include "Log.h"
#include "Util.h"


CLog::CLog(void)
{
	m_read_cnt = 0;
	addstr("log start");
}


CLog::~CLog(void)
{
	addstr("log end");
	save();
	m_strlist.clear();
}

void  CLog::addstr(string str) 
{  
	char buf[100];
	string  temp;

	MYUTIL()->getcurtime(buf,100,1);
	temp =buf;
	temp += str;

	m_strlist.push_back(temp); 
}

void  CLog::addstr(wstring str)
{
	string  temp;
	char buf[100];
	char  out[200];
	memset(out, 0, sizeof(out));
	MYUTIL()->cnv_outchar(str.data(), out, 200);


	MYUTIL()->getcurtime(buf, 100, 1);
	temp = buf;
	temp += out;

	m_strlist.push_back(temp);
}

void  CLog::save(void)
{
	FILE  *pf;

	if (fopen_s(&pf, "cry5log.txt", "wt") != 0 ) return;

	int cnt = m_strlist.size();
	for( int n=0; n < cnt; n++ ){
		fprintf(pf, "%s\n", m_strlist[n].data() );
	}

	fclose(pf);

}


const char *CLog::getread(void)
{
	if (m_strlist.size() == 0) return nullptr;
	if (m_strlist.size() <= m_read_cnt) return nullptr;

	return m_strlist[m_read_cnt++].data();
}