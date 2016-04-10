/*
   인터넷 송수신의 경우
   쓰레드함수안에서 모든 송수신처리를 한다.
   -쓰레드 하나당 인터넷접속 하나.




WSAEBADF 10009         잘못된 기술자(소켓 핸들)이다
WSAEACCES 10013        브로드캐스트 어드레스를 위한 데이터그램 소켓의 접속시도가 setsockopt 함수로 SO_BROADCAST가 설정되어있지 않은 상태에서 실패 했습니다.
WSAEFAULT 10014        name 또는 namelen 매개변수가 올바른 형태가 아닙니다.
WSAEINVAL 10022        accept 하기 전에 listen 함수가 불려지지 않았습니다.
WSAEMFILE 10024        새로운 소켓에 할당하기 위한 소켓 기술자가 더 이상 남아있지 않습니다
WSAEWOULDBLOCK 10035   소켓 함수가 비블럭킹 모드로 동작중이다
WSAEINPROGRESS 10036   블록화 함수가 호출 되는 동안 부적절한 소켓 함수가 호출되었다
WSAEALREADY 10037      이미 완료된 비동기 명령에 대한 취소가 시도됨
WSAENOTSOCK 10038      지정한 기술자가 소켓 기술자가 아닙니다
WSAEDESTADDRREQ 10039  해당 함수에 목적지 어드레스가 필요하지만 제공되지 않았음
WSAEMSGSIZE 10040      수신된 메시지가 지정된 버퍼에 저장하기에 너무 커서 손실 되었습니다
WSAEPROTOTYPE 10041    지정된 프로토콜이 잘못되었거나 이 소켓에 대해서 잘못된 형식입니다
WSAENOPROTOOPT 10042   알 수 없는 옵션이거나, 지원지지 않는 옵션을 사용했습니다.
WSAEPROTONOSUPPORT 10043  지정된 프로토콜이 지원되지 않는 형식입니다
WSAESOCKTNOSUPPORT 10044  지정된 소켓 타입이 지정한 어드레스 체계에서 지원되지 않는 형식입니다
WSAEOPNOTSUPP 10045       socket이 연결지향형 서비스(SOCK_STREAM)형태가 아닙니다. ex) listen이 UDP socket에서 호출
WSAEPFNOSUPPORT 10046     지정된 프로토콜 체계가(PF_*) 지원되지 않습니다
WSAEAFNOSUPPORT 10047     지정된 어드레스 체계가(AF_*) 지원되지 않습니다
WSAEADDRINUSE 10048       지정한 어드레스(IP)가 이미 사용중이다
WSAEADDRNOTAVAIL 10049    지정된 어드레스는 로컬 머신에서 사용할 수가 없다
WSAENETDOWN 10050         네트웍 서브 시스템에 에러가 발생했습니다
WSAENETUNREACH 10051      원격 시스템까지 네트웍이 도달할 수 없습니다
WSAENETRESET 10052        연산이 진행되고 있는 도중 접속이 끊겨버렸습니다.
WSAECONNABORTED 10053     연결이 out-of-band나 다른 실패 때문에 끊어져 버렸습니다.
WSAECONNRESET 10054       원격 연결지에서 "hard"나 "abortive" 종료를 수행해서 리셋되었습니다.
WSAENOBUFS 10055          윈도우 소켓 시스템의 버퍼 공간이 모자라거나, 애플리케이션에 의해 API에게 제공된 공간이 너무 작아서 요청된 정보를 저장 할 수가 없음
WSAEISCONN 10056         *지정된 소켓이 이미 연결 되어 있음
WSAENOTCONN 10057         지정된 소켓이 이미 연결 되어 있지 않음
WSASHUTDOWN 10058         소켓이 셧다운(shutdown()) 되었습니다.
WSAETOOMANYREFS 10059     지정한 함수에 대한 인자가 너무 많음
WSAETIMEDOUT 10060        접속 시도가 시간초과 되었습니다.
WSAECONNREFUSED 10061     접속시도가 강제로 종료되었습니다
WSAELOOP 10062            WSAENAMETOOLONG 10063
WSAEHOSTDOWN 10064        원격 호스트가 다운 되었음
WSAHOSTUNREACH 10065      네트웍 시스템 장애 등에 의해서 원격호스트까지도 달 할 수 없습니다.
WSASYSNOTREADY 10091      네트워크 서브 시스템이 아직 통신할 준비가 되어 있지 않음(WSAStartup()이 반환)
WSAVERNOTSUPPORTED 10092  요청한 윈도우즈 소켓 버전이 현재 윈도우즈 소켓 시스템에서 지원하지 않습니다.
WSANOTINITIALISED 10093   이 함수를 사용하기 전에 성공적인 WSAStartup 함수의 호출이 없었습니다.
WSAHOST_NOT_FOUND 11001   호스트를 찾아낼 수 없습니다.
WSATRY_AGAIN 11002        요청된 정보가 발견 되지 않음
WSANO_RECOVERY  11003     회복할 수 없는 에러발생
WSANO_DATA 11004          잘못된 이름(name)으로 아무런 데이터가 기록되지 않았습니다. 


*/

#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
using namespace std;

#include  ".\MyHttpUtil.h"
#include  ".\MyChunkData.h"


//  x1.x2.x3.x4 
typedef struct{
	unsigned char x1;
	unsigned char x2;
	unsigned char x3;
	unsigned char x4;
	unsigned char x5;
	unsigned char x6;
}t_mysocketip;

typedef  struct{


}t_mysocketpeer;


//====================================================================
/*
  IP 번호를 관리한다.
  여러개의 ip번호를 가질 수 있다.
*/
class CMySocketIp
{
public:
	CMySocketIp(HOSTENT *phe = nullptr );
	~CMySocketIp(void);

	void clear(void);
	void sethostinfo(HOSTENT *phe);

	char *gethostname(void){ return m_pname; }
	short getipnum(void){ return m_addr.size(); } // ip 갯수
	short getiplen(void){ return m_len; } // ip 길이, 4 or 6
	void  getip(t_mysocketip &add, int num ); // num번째 아이피번호를 얻어온다.

	void  addip(t_mysocketip add){ m_addr.push_back(add); }// ip주소 추가
protected:
	char   *m_pname;
	char   *m_paliases;
	short   m_addrtype;
	short   m_len;   // 4 : xxx.xxx.xxx.xxx
	vector<t_mysocketip>  m_addr;
};

//====================================================================
#define   MYSOCKETOBJ_RECV_DEFSIZE   (1024 * 30)
typedef  struct {
	unsigned int  mem_size; // 할당받은 메모리 전체 사이즈
	unsigned int  data_size; // 실제 데이터 사이즈
	unsigned int  header_size; // 수신헤더 사이즈

	char  *pdata;

}t_mysocketobj;



class  CMySocketObj
{
public:
	 CMySocketObj(void);
	~CMySocketObj(void);


	void  clear(void);
	void  set_url(const char *purl) { m_url = purl;  }
	void  set_hostname(const char *phost) { m_host_name = phost;  }
	void  set_post_string(const char *pstr){ m_post_str = pstr;   }
	const char *get_post_string(void)const { return m_post_str.data();  }
	const char *get_url(void)const { return m_url.data();  }
	const char *get_hostname(void)const { return m_host_name.data();  }
	void  set_mode_get(void){ m_mode = true;  }
	void  set_mode_post(void){ m_mode = false; }
	bool  get_mode(void){ return m_mode;  }

	//다운받은 데이터 처리
	bool  setdown_data(const char *pdata, unsigned int size); 
	bool  add_recv_data(const char *pdata, unsigned int size);
	unsigned int  get_recv_datasize(void);
	unsigned int  copy_recv_data(char *ptemp, int len);
	const char *get_recv_dataptr(void) const { return m_recv_data.pdata;  }

	bool  save_recv_chunkdata(const char *pfname);
	int  get_request_code(void){ return m_chunkdata.get_request_code();  }  // 수신 응답코드
	const char *get_request_msg(void) { return m_chunkdata.get_request_msg(); }    // 수신 응답코드에 대한 메세지

	bool  isuse(void){ return  (m_host_name.size() != 0)?true: false; } // 호스트의 이름이 있다면 사용중으로 간주한다


	void  begin_thread(void) { m_begin_thread = true;  }
	void  end_thread(void) { m_begin_thread = false; }
	bool  thread_check(unsigned int *pdownsize);  //스레드 함수가 종료되었는지 체크


	void   set_thread_quit(bool sw); // 스레드 함수에 종료 요청
	bool   get_thread_quit(void); // 스레드 함수에서 종료요청이 있는지 확인

	void  http_header_clear(void);
	void  set_http_header(const t_mysocket_http_header  *pheader) { m_http_header = *pheader; }
	void  get_http_header(t_mysocket_http_header  *pheader){ *pheader = m_http_header; }


	static const bool _thread_quit_on = true;
	static const bool _thread_quit_off = false;
protected:
	string   m_url;
	string   m_host_name;
	string   m_post_str;
	bool    m_mode; // true : GET,  false: POST

	int   m_down_state; 
	bool  m_thread_quit_sw;
	bool  m_begin_thread;

	CRITICAL_SECTION  _mycritical;
	t_mysocket_http_header   m_http_header;

	t_mysocketobj  m_recv_data; // 수신 패킷 데이터 정보

	CMyChunkData  m_chunkdata; // HTTP  수신 데이터 관리 클래스
};
//====================================================================





#define MYSOCKET_MODE_GET   true
#define MYSOCKET_MODE_POST  false
#define MYSOCKET_TABLE_SIZE  50      // thread 최대 갯수이기도 하다
#define  _msglog(s)  printf("%s\n", s);
//====================================================================
class CMySocket
{
	    WSADATA  m_wsaData;
	CMySocketIp  m_localip;

	  CMySocket(void);
	  void  getlocalhostinfo(void);


public:
	~CMySocket(void);
	  SOCKET  new_socket_http(void);
	  void  del_socket_http(SOCKET sc);

	 
	  bool  connect_http(SOCKET sc,const char *phost_name);
	  bool  send_http(SOCKET sc,const char *pdata, int data_len);


	   int  thread_down_http(const char *phost_name, const char *purl, const char *pstr, bool mode, t_mysocket_http_header  *ph = nullptr);


	   int  table_check(void); // 쓰지 않는 테이블의 번호를 얻는다.
	  void  table_allclear(void); // 모든 테이블을 초기화 한다.,  다운중인 데이터는 모두 사라진다.
	  void  table_clear(int num); 

	  int  table_request_code(int num);  // 응답코드 얻기
	  const char *table_request_msg(int num);

	  unsigned int table_getdownsize(int num); // 다운받은 데이터 크기 얻기
	  bool  table_downcheck(int num, unsigned int  *pdownsize); // 다운이 다 되었는지 체크
	  unsigned int  table_getdata(int num,char *pdata, unsigned int len); // 다운받은 데이터 복사,  리턴값은 복사된 데이터크기

	  const char *table_getptr(int num);
	  bool  table_save_data(int num,const char *pfname); 


	static CMySocket *GetMySocket(void)
	{
		static CMySocket  msc;
		return  &msc;
	}
};

