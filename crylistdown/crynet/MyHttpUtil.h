/*
    HTTP 헤더  전송/수신 - 간단한 처리 목적

	수신 데이터는 텍스트와 image만 처리하기로 하자.
	문자집합은 utf-8만 처리

	-절대주소에 대해서 좀더 살펴보자 , 잘못된 url로 뜬다.
	-에러에 대한 처리가 미흡하다. 에러메세지를 남기던가 하자
*/
#pragma once
#include <xstring>
#include  <list>
#include  <vector>
using namespace std;


// HTTP 1.1
typedef struct{
	 int   request_code; //서버 응답코드 , 200 ok, ...
	 std::string   date;
	 std::string   server_name;
	 int   accept_ranges;   //  1:bytes,
	 std::string   file_name;
	 std::string   content_type;    // 
	std::string   content_type_charset; // 데이터파일의 charset, ex) utf-8, ks-2601
	 int   content_length;    //  chunke방식이면 0이다.  
	 std::string   transfer_encoding; // chunked, 

	 unsigned int  header_size; // 수신 헤더 크기,  == 데이터 시작위치값
}t_myhttputil_http_header_recv;

// http  header
typedef  struct {
	std::string Connection;
	std::string User_Agent;
	std::string Accept;
	std::string Origin;
	std::string Accept_Encoding;
	std::string Accept_Language;
	std::string XRW;
	std::string Referer;
	std::string Content_Type;  // POST
}t_mysocket_http_header;



#include <string>
using namespace  std;


class CMyHttpUtil
{
public:
	CMyHttpUtil(void);
	~CMyHttpUtil(void);

	//--- send ---
	//1.
	 int  util_string_hex(const char *pstr, char *pout); // string -> %<2hex>%<2hex>...
	void  set_url_abs(const char *purl,const char *phost); // ex) /search?key=0...
	void  set_url_absolute(const char *purl);              //?? ex) www.naver.com/search?key=0...
	//2.
	bool  make_def_send_header(const char  *psmode, const char  *pdata,const t_mysocket_http_header  *ph );// 전송 HTTP 헤더 만들기
	//3.
	const char *get_send_header_string(void) { return m_str_send_header.data();  /*m_pstr_send_header;*/  } 

	//-- recv --
	void  clear_recv_header(t_myhttputil_http_header_recv &h); // 수신 헤더 초기화
	bool  set_recv_data_parseing(const char *pdata, int size); // 수신 HTTP헤더 파싱 
	void  copy_recv_header(t_myhttputil_http_header_recv  &out) { out = m_recv_info; } // 수신 헤더 복사
	 int  get_recv_request_code(void){ return m_recv_info.request_code; }
	const char *get_recv_request_code_message(void);

	const char  *get_http_ver(void); // HTTP 버전 문자열을 얻는다.    return : HTTP/1.1

protected:
	unsigned int   m_string_buffer_size; 
	string   m_str_send_header;
	char  *m_pstr_send_header;
	char  *m_pstr_url;
	char  *m_pstr_host;
	bool   m_url_type; //  true : abs_path ,   false : absoluteURI

	t_myhttputil_http_header_recv   m_recv_info;


	void  add_send_header(const char *pstr);

	// 수신 HTTP헤더 정보 처리
	int   recv_header_nfield(void);
	void  recv_header_nextfield(void);


	//bool  set_recv_data_file(char *pfname); //테스트용 함수!!!
};

