#pragma once
#include  "MyHttpUtil.h"


class CMyChunkData
{
public:
	CMyChunkData();
	~CMyChunkData();

	void clear(void);
	bool proc_data(const char *pdata, unsigned int size);
	bool end_data(void) { return m_end_data;  }

	bool save_data(const char *pdata, unsigned int size, const char *pfname);

	int  get_request_code(void){ return m_recv_info.request_code; }  // 수신 데이터 헤더  응답코드 얻기
	const char *get_request_msg(void) { return m_requset_code_msg.data();  }
protected:

	string  m_requset_code_msg; 
	t_myhttputil_http_header_recv   m_recv_info;
	char  *m_pdata;
	int    m_data_len;
	bool   m_end_data; // 단순히, 소스데이터가 끝까지 받아진상태라면 TRUE, =  청크 길이값이 0

	int   get_hexi(const char *pdata, unsigned int size, unsigned int *pout); 
};

