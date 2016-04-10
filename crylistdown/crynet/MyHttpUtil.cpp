#include "stdlib.h"
#include <string.h>
#include "MyHttpUtil.h"
#include "..\cryutil\Util.h"

//  HTTP  recv  header  FIELD
enum  e_fieldid
{
F_HTTP  ,    //"HTTP/"
F_DATE  ,    //"Date: "
F_SERV  ,    //"Server: "
F_KEEP  ,    //"Keep-Alive: "
F_CONN  ,    //"Connection: "
F_CTYP  ,    //"Content-Type: "
F_ARAN  ,    //"Accept-Ranges: "
F_CLEN  ,    //"Content-Length: "
F_TENC  ,    //"Transfer-Encoding: "
F_LMOD  ,    //"Last-Modified: "
F_HAGE  ,    //"Age: "
F_COOK  ,    //"Set-Cookie: "
F_HEND  ,    // CRLF -- end --
F_ERROR  
};


typedef struct {
	int  id;
	char name[50];
}t_field;
t_field pfields[]={
	{F_HTTP,"HTTP/"},
	{F_DATE,"Date: "},
	{F_SERV,"Server: "},
	{F_KEEP,"Keep-Alive: "},
	{F_CONN,"Connection: "},
	{F_CTYP,"Content-Type: "},
	{F_ARAN,"Accept-Ranges: "},
	{F_CLEN,"Content-Length: "},
	{F_TENC,"Transfer-Encoding: "},
	{F_LMOD,"Last-Modified: "},
	{F_HAGE,"Age: "},
	{F_COOK, "Set-Cookie: " },
	{F_HEND,0x0D,0x0A,0x00}, //이렇게 해도 되네 , 수신 HTTP헤더 끝 : CRLF
	{-1,""}
};




const unsigned int c_add_string_buffer_size = 1024;
const char *phttpver = "HTTP/1.1";
const char *shostkey = "Host: ";
//====================================================================

CMyHttpUtil::CMyHttpUtil(void)
{
	m_string_buffer_size = 0;
	m_pstr_send_header = nullptr;
	m_pstr_url = m_pstr_host = nullptr;
	m_url_type = true;

}


CMyHttpUtil::~CMyHttpUtil(void)
{
	if(m_pstr_send_header != nullptr) free(m_pstr_send_header);
	if(m_pstr_url != nullptr) free(m_pstr_url);
	if(m_pstr_host != nullptr) free(m_pstr_host);
}


const char  *CMyHttpUtil::get_http_ver(void)
{
	return phttpver+1; // ' ' 다음부터
}

/*
  버퍼에 문자열 추가하기
*/
void CMyHttpUtil::add_send_header(const char *pstr)
{
	int slen = strlen(pstr);


	m_str_send_header += pstr;
	/*
	int flen = m_string_buffer_size - ( (m_pstr_send_header == nullptr)? 0: strlen(m_pstr_send_header) );
    // 버퍼증가분이 추가될 문자열 길이보다 작다면, 증가분을 [추가크기+추가될문자열 길이]로 정한다.
	int nsize = (c_add_string_buffer_size > slen)? c_add_string_buffer_size : (c_add_string_buffer_size+slen);

	if( m_pstr_send_header == nullptr ){
		m_pstr_send_header = (char *)malloc( nsize+1 );
		m_string_buffer_size = nsize;
		sprintf_s(m_pstr_send_header,nsize+1,"%s",pstr);
		return;
	}

	if( flen > slen ){ // 버퍼의 남은 공간 > 추가될 문자열 길이
		char *pt = m_pstr_send_header;
		pt += strlen(m_pstr_send_header);
		m_string_buffer_size += slen;
		sprintf_s(pt,strlen(pstr)+1, "%s",pstr);
		return;
	}

	//버퍼의 남은 공간이 부족하다면 공간을 더 늘려준다.
	char *ptemp = (char *)malloc(m_string_buffer_size + nsize+1);
    sprintf_s(ptemp,m_string_buffer_size + nsize+1, "%s%s",m_pstr_send_header, pstr);
	free( m_pstr_send_header );
	m_pstr_send_header = ptemp;
	m_string_buffer_size += nsize;
	*/
}

/*
   url에 추가될 문자코드변환 함수

   ex)
   in)  pstr = "삼국지"
   out) pout = "%EC%82%BC%EA%B5%AD%EC%A7%80"
        만약, pout이 null이면 변환될 문자열의 길이값이 리턴된다. x3
*/
int  CMyHttpUtil::util_string_hex(const char *pstr, char *pout)
{
	const char hext[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	int n=0,i=0;

	if( pout == nullptr ) return strlen(pstr)*3; // :)
	while(pstr[n] != 0 ){
		pout[i++] = '%';
		pout[i++] = hext[((unsigned char)pstr[n])>>4];
		pout[i++] = hext[((unsigned char)pstr[n])&0x0F];
		n++;
	}
	pout[i]=0;

	return 0;
}



void  CMyHttpUtil::set_url_abs(const char *purl, const char *phost)
{
	int len = strlen(purl) + strlen(phttpver);
	int hlen = strlen(phost)+strlen(shostkey);

	m_url_type = true;

	if( m_pstr_url != nullptr ) free( m_pstr_url );
	m_pstr_url = (char *)malloc( len +5);
	sprintf_s(m_pstr_url,len+5,"%s %s", purl,phttpver );
	
	if( m_pstr_host != nullptr ) free( m_pstr_host );
	m_pstr_host = (char *)malloc( hlen +2);
	sprintf_s(m_pstr_host,hlen+2,"%s%s", shostkey,phost );
}


void  CMyHttpUtil::set_url_absolute(const char *purl)
{
	int len = strlen(purl) + strlen(phttpver);
	m_url_type = false;

	if( m_pstr_url != nullptr ) free( m_pstr_url );
	m_pstr_url = (char *)malloc( len +1);
	sprintf_s(m_pstr_url,len+1,"%s%s", purl,phttpver );
}


bool  CMyHttpUtil::make_def_send_header(const char  *psmode, const char  *pdata,const  t_mysocket_http_header  *ph)
{
	if( m_pstr_url == nullptr ) return false;

	if( strcmp(psmode,"GET") == 0 )	add_send_header("GET ");
	else if(  strcmp(psmode,"POST") == 0  )	add_send_header("POST ");
	else return false;


	add_send_header(m_pstr_url);
	add_send_header("\r\n");
	if( m_url_type ){
		if( m_pstr_host == nullptr ) return false;
		add_send_header(m_pstr_host); 
		add_send_header("\r\n"); 
	}
	if (ph->Connection.empty() == true ) add_send_header("Connection: keep-alive\r\n");
	else add_send_header( ((std::string)("Connection: ") + ph->Connection.data()).data() );

	if (ph->User_Agent.empty() == true) add_send_header("User-Agent: Mozilla/5.0 (Windows NT 6.1) AppleWebKit/534.24 (KHTML, like Gecko) Chrome/11.0.696.65 Safari/534.24\r\n");
	else add_send_header(((std::string)("User-Agent: ") + ph->User_Agent.data()).data());
	
	if (ph->Accept.empty() == true) add_send_header("Accept: text/html,*/*\r\n");
	else add_send_header(((std::string)("Accept: ") + ph->Accept.data()).data());

	if (ph->Origin.empty() == true) add_send_header("Origin: http://dart.fss.or.kr\r\n");
	else add_send_header(((std::string)("Origin: ") + ph->Origin.data()).data());

	if (ph->Accept_Encoding.empty() == true) add_send_header("Accept-Encoding: deflate,sdch\r\n");
	else add_send_header(((std::string)("Accept-Encoding: ") + ph->Accept_Encoding.data()).data());

	if (ph->Accept_Language.empty() == true) add_send_header("Accept-Language: ko-KR,ko;q=0.8,en-US;q=0.6,en;q=0.4\r\n");
	else add_send_header(((std::string)("Accept-Language: ") + ph->Accept_Language.data()).data());

	if (ph->XRW.empty() != true) add_send_header(((std::string)("XRW: ") + ph->XRW.data()).data());// add_send_header("X-Requested-With: XMLHttpRequest\r\n");

	if (ph->Referer.empty() == true) add_send_header("Referer: http://dart.fss.or.kr/dsap001/guide.do\r\n");
	else add_send_header(((std::string)("Referer: ") + ph->Referer.data()).data());


	//add_send_header("Cookie: JSESSIONID=OGguaPvV2A5cibabROiwNzBADb7R33QqCyVEjalbOJUVFISPSc1iuZTcA5dFXbzk\r\n");

	if(  strcmp(psmode,"POST") == 0  ){
		int  len = strlen(pdata);
		char  buf[50];
		if( *pdata != 0 ){

			if( ph->Content_Type.empty() == true ) add_send_header("Content-Type: text/xls;charset=UTF-8\r\n");
			else add_send_header(((std::string)("Content-Type: ") + ph->Content_Type.data()).data());

			sprintf_s(buf,49,"Content-Length: %d\r\n", len);
			add_send_header(buf);
			add_send_header("\r\n");
			add_send_header(pdata);
		}
		else { 
			sprintf_s(buf,49,"Content-Length: 0\r\n");
			add_send_header(buf);
	        add_send_header("\r\n"); //  end !!!!!
		}
	}

	return true;
}

//==============================================================================================
//==============================================================================================
//==============================================================================================

void  CMyHttpUtil::clear_recv_header(t_myhttputil_http_header_recv &h)
{
	h.accept_ranges = 0;
	h.content_length = 0;
	h.content_type.clear();
	h.content_type_charset.clear();
	h.date.clear();
	h.file_name.clear();
	h.request_code = 0;
	h.server_name.clear();
	h.transfer_encoding.clear();
	h.header_size = 0;

}
/*
  수신된 데이터의 시작포인터를 받는다.
  - http헤더가 있다면 true를 리턴한다.
  - http헤더를 분석한다.

  HTTP/1.1 200 OK
  Date: Sun, 09 Nov 2014 06:00:25 GMT
  Server: Apache
  Content-Disposition: attachment; filename=Data.xls;
  Pragma: no-cache;
  Content-Type: text/xls;charset=UTF-8
  Transfer-Encoding: chunked
  0d0a
  0d0a

*/
bool  CMyHttpUtil::set_recv_data_parseing(const char *pdata, int size)
{
	if (pdata == nullptr || size <= 0) return false;
	vector<string>  swords;

	//수신 응답 헤더부분을 단어단위로 분리한다.
	const char stc[] = { ':', ';', '=', '\t', 0x0D, 0x0A, 0 };
	MYUTIL()->get_string_word(pdata, &swords, stc, _countof(stc));

	int  cnt = swords.size();
	bool  swhttp = false;
	int  n = 0;

	if (cnt == 0) return false;
	clear_recv_header(m_recv_info);

	while (n < cnt){
		if (MYUTIL()->comp_str(swords[n].data(), "HTTP/1.1", strlen("HTTP/1.1")) == true){
			// "HTTP/1.1 200 OK"  문자열을 다시 분해한다.
			vector<string> wht;
			const char htc[] = { ' ', 0 };
			MYUTIL()->get_string_word(swords[n].data(), &wht, htc, 1);
			if (strncmp(wht[0].data(), phttpver, strlen(phttpver)) != 0) return false;  // HTTP/1.1   ??
			m_recv_info.request_code  = atoi(wht[1].data()); // 상태코드

			swhttp = true;
		}
		else if (MYUTIL()->comp_str(swords[n].data(), "Date", strlen("Date")) == true){
			m_recv_info.date=swords[n + 1].data();
		}
		else if (MYUTIL()->comp_str(swords[n].data(), "Server", strlen("Server")) == true){
			m_recv_info.server_name = swords[n + 1].data();
		}
		else if (MYUTIL()->comp_str(swords[n].data(), "filename", strlen("filename")) == true){
			m_recv_info.file_name = swords[n + 1].data();
		}
		else if (MYUTIL()->comp_str(swords[n].data(), "Content-Type", strlen("Content-Type")) == true){
			m_recv_info.content_type = swords[n + 1].data();
		}
		else if (MYUTIL()->comp_str(swords[n].data(), "Transfer-Encoding", strlen("Transfer-Encoding")) == true){
			m_recv_info.transfer_encoding = swords[n + 1].data();
		}
		else if (MYUTIL()->comp_str(swords[n].data(), "charset", strlen("charset")) == true){
			m_recv_info.content_type_charset = swords[n + 1].data();
		}
		else if (MYUTIL()->comp_str(swords[n].data(), "Content-Length", strlen("Content-Length")) == true){
			m_recv_info.content_length = atoi(swords[n + 1].data());
		}

		n++;
	}

	if (swhttp == false) return false; // "HTTP1.1"이 없었다면 HTTP헤더가 아니다

	// 헤더의 끝을 알아온다.
	char  crlf[4];
	cnt = 0;
	do{
		for (n = 0; n < 4; n++)crlf[n] = *(pdata + cnt + n);
		cnt++;
		if (crlf[0] == 0x0d && crlf[1] == 0x0a && crlf[2] == 0x0d && crlf[3] == 0x0a){
			m_recv_info.header_size = cnt + 3;
			return true;
		}
	} while (cnt < size);
	return false;
}

//==============================================================================================
//==============================================================================================


const char *CMyHttpUtil::get_recv_request_code_message(void)
{
	struct t_request_code{
		int code;
		char msg[64];
	} rlist[]={
		{200 ,"OK"},
		{201 ,"Created"},
		{202 , "Accepted"},
		{203 , "Non-Authoritative Information"},
		{204 , "No Content"},
		{205 , "Reset Content"},
		{206 , "Partial Content"},
		{300 , "Multiple Choices"},
		{301 , "Moved Permanently"},
		{302 , "Found"},
		{303 , "See Other"},
		{304 , "Not Modified"},
		{305 , "Use Proxy"},
		{306 , "(Unused)"},
		{307 , "Temporary Redirect"},
		{400 , "Bad Request"},
		{401 , "Unauthorized"},
		{402 , "Payment Required"},
		{403 , "Forbidden"},
		{404 , "Not Found"},
		{405 , "Method Not Allowed"},
		{406 , "Not Acceptable"},
		{407 , "Proxy Authentication Required"},
		{408 , "Request Timeout"},
		{409 , "Conflict"},
		{410 , "Gone"},
		{411 , "Length Required"},
		{412 , "Precondition Failed"},
		{413 , "Request Entity Too Large"},
		{414 , "Request-URI Too Long"},
		{415 , "Unsupported Media Type"},
		{416 , "Requested Range Not Satisfiable"},
		{417 , "Expectation Failed"},
		//Server Error 5xx
		{500 , "Internal Server Error"},
		{501 , "Not Implemented"},
		{502 , "Bad Gateway"},
		{503 , "Service Unavailable"},
		{504 , "Gateway Timeout"},
		{505 , "HTTP Version Not Supported"},
		{-1,""}
	};
	const int len = 200;
	static  char  msg[len]; 

	sprintf_s(msg, len, "-1: ");
	for(int n=0; rlist[n].code != -1; n++){
		if( rlist[n].code == m_recv_info.request_code ){
			sprintf_s(msg, len, "%d:%s", rlist[n].code, rlist[n].msg);
			break;
		}
	}
	
	return msg;
}