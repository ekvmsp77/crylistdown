/*
   �ٿ�ε� ���� Ŭ����

*/

#pragma once

#include "MySocket.h"

#include  <list>
#include  <vector>
using namespace std;


#define   DOWNPROC_FILE_CODE_STR   "A0A32C61-D7ED-4339-B294-6C0F0FDF0E44"  // ���ϱ��� ���ڿ�

typedef   void (*_fdownproc)(unsigned int num);

typedef  struct {
	std::string phost;
	std::string purl;
	std::string pstr;
	bool   mode; //  "GET"   "POST"

	char   name[128];  // �׳� �α׿� ��µ� ���ڿ�

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
	list<t_downproc_list>   m_obj; // �ٿ� �Ϸ� �Ǹ� �������� �ϱ⶧���� list ���
	vector<t_downproc>  m_info;    // �ٿ �ʿ��� ���ͳ� ��������
public:
	CDownProc();
	~CDownProc();

	void    timer(void);

	void   new_obj(const t_downproc  *pt);  // �ٷ� �ٿ� �õ�

	bool  open_downinfo(const  char  *pfname, _fdownproc pf); // ���Ϸ� ���� -> ����Ʈ�� �߰��Ѵ�.
	bool  down_info(const char *pname); // ���Ϸ� �о����  �ٿ�������  �ٿ� �����Ѵ�.   pstr  == name
	int  search_data_ctype(const char *pname, const char *ps); //  ����!! �ٿ� ���� data���� �ش� ���ڿ��� ã�´�. return : ��ġ��


	static CDownProc *GetObj(void)
	{
		static CDownProc obj;
		return  &obj;
	}

};



#define DOWNPROC()  CDownProc::GetObj()