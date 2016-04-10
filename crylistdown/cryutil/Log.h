#pragma once
#include  <vector>
using namespace std;



class CLog
{
protected:
	CLog(void);
	~CLog(void);

	unsigned int  m_read_cnt;
	vector<string>  m_strlist;
public:
	void  addstr(string str);
	void  addstr(wstring str);
	void  save(void);
	const char *getlog(int  num) const { return m_strlist[num].data();  }
	int   getlogsize(void) { return m_strlist.size(); }

	const char *getread(void); // 로그 기록 읽기

	static CLog *LogObj(void)
	{
		 static CLog obj;
		 return  &obj;
	}
};

#define  MYLOG(s)    CLog::LogObj()->addstr(s)
