/*
   다운로드 관리 클래스

*/

#pragma once

#include "MySocket.h"

#include  <list>
#include  <vector>
using namespace std;


#define   DOWNPROC_FILE_CODE_STR   "A0A32C61-D7ED-4339-B294-6C0F0FDF0E44"  // 파일구분 문자열

typedef   void (*_fdownproc)(unsigned int num);

typedef  struct {
	std::string phost;
	std::string purl;
	std::string pstr;
	bool   mode; //  "GET"   "POST"

	char   name[128];  // 그냥 로그에 출력될 문자열

	t_mysocket_http_header  header;
	_fdownproc  func;
}t_downproc;


typedef  struct {
	_fdownproc  func;
	char   name[128];
	int    objnum;
}t_downproc_list;




class CDownProc
{
	list<t_downproc_list>   m_obj; // 다운 완료 되면 지워져야 하기때문에 list 사용
	vector<t_downproc>  m_info;    // 다운에 필요한 인터넷 연결정보
public:
	CDownProc();
	~CDownProc();

	void    timer(void);

	void   new_obj(const t_downproc  *pt);  // 바로 다운 시도

	bool  open_downinfo(const  char  *pfname, _fdownproc pf); // 파일로 열기 -> 리스트에 추가한다.
	bool  down_info(const char *pname); // 파일로 읽어들인  다운정보로  다운 시작한다.   pstr  == name
	int  search_data_ctype(const char *pname, const char *ps); //  보류!! 다운 정보 data에서 해당 문자열을 찾는다. return : 위치값


	static CDownProc *GetObj(void)
	{
		static CDownProc obj;
		return  &obj;
	}

};



#define DOWNPROC()  CDownProc::GetObj()