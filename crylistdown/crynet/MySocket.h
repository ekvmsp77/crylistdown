/*
   ���ͳ� �ۼ����� ���
   �������Լ��ȿ��� ��� �ۼ���ó���� �Ѵ�.
   -������ �ϳ��� ���ͳ����� �ϳ�.




WSAEBADF 10009         �߸��� �����(���� �ڵ�)�̴�
WSAEACCES 10013        ��ε�ĳ��Ʈ ��巹���� ���� �����ͱ׷� ������ ���ӽõ��� setsockopt �Լ��� SO_BROADCAST�� �����Ǿ����� ���� ���¿��� ���� �߽��ϴ�.
WSAEFAULT 10014        name �Ǵ� namelen �Ű������� �ùٸ� ���°� �ƴմϴ�.
WSAEINVAL 10022        accept �ϱ� ���� listen �Լ��� �ҷ����� �ʾҽ��ϴ�.
WSAEMFILE 10024        ���ο� ���Ͽ� �Ҵ��ϱ� ���� ���� ����ڰ� �� �̻� �������� �ʽ��ϴ�
WSAEWOULDBLOCK 10035   ���� �Լ��� ���ŷ ���� �������̴�
WSAEINPROGRESS 10036   ���ȭ �Լ��� ȣ�� �Ǵ� ���� �������� ���� �Լ��� ȣ��Ǿ���
WSAEALREADY 10037      �̹� �Ϸ�� �񵿱� ��ɿ� ���� ��Ұ� �õ���
WSAENOTSOCK 10038      ������ ����ڰ� ���� ����ڰ� �ƴմϴ�
WSAEDESTADDRREQ 10039  �ش� �Լ��� ������ ��巹���� �ʿ������� �������� �ʾ���
WSAEMSGSIZE 10040      ���ŵ� �޽����� ������ ���ۿ� �����ϱ⿡ �ʹ� Ŀ�� �ս� �Ǿ����ϴ�
WSAEPROTOTYPE 10041    ������ ���������� �߸��Ǿ��ų� �� ���Ͽ� ���ؼ� �߸��� �����Դϴ�
WSAENOPROTOOPT 10042   �� �� ���� �ɼ��̰ų�, �������� �ʴ� �ɼ��� ����߽��ϴ�.
WSAEPROTONOSUPPORT 10043  ������ ���������� �������� �ʴ� �����Դϴ�
WSAESOCKTNOSUPPORT 10044  ������ ���� Ÿ���� ������ ��巹�� ü�迡�� �������� �ʴ� �����Դϴ�
WSAEOPNOTSUPP 10045       socket�� ���������� ����(SOCK_STREAM)���°� �ƴմϴ�. ex) listen�� UDP socket���� ȣ��
WSAEPFNOSUPPORT 10046     ������ �������� ü�谡(PF_*) �������� �ʽ��ϴ�
WSAEAFNOSUPPORT 10047     ������ ��巹�� ü�谡(AF_*) �������� �ʽ��ϴ�
WSAEADDRINUSE 10048       ������ ��巹��(IP)�� �̹� ������̴�
WSAEADDRNOTAVAIL 10049    ������ ��巹���� ���� �ӽſ��� ����� ���� ����
WSAENETDOWN 10050         ��Ʈ�� ���� �ý��ۿ� ������ �߻��߽��ϴ�
WSAENETUNREACH 10051      ���� �ý��۱��� ��Ʈ���� ������ �� �����ϴ�
WSAENETRESET 10052        ������ ����ǰ� �ִ� ���� ������ ���ܹ��Ƚ��ϴ�.
WSAECONNABORTED 10053     ������ out-of-band�� �ٸ� ���� ������ ������ ���Ƚ��ϴ�.
WSAECONNRESET 10054       ���� ���������� "hard"�� "abortive" ���Ḧ �����ؼ� ���µǾ����ϴ�.
WSAENOBUFS 10055          ������ ���� �ý����� ���� ������ ���ڶ�ų�, ���ø����̼ǿ� ���� API���� ������ ������ �ʹ� �۾Ƽ� ��û�� ������ ���� �� ���� ����
WSAEISCONN 10056         *������ ������ �̹� ���� �Ǿ� ����
WSAENOTCONN 10057         ������ ������ �̹� ���� �Ǿ� ���� ����
WSASHUTDOWN 10058         ������ �˴ٿ�(shutdown()) �Ǿ����ϴ�.
WSAETOOMANYREFS 10059     ������ �Լ��� ���� ���ڰ� �ʹ� ����
WSAETIMEDOUT 10060        ���� �õ��� �ð��ʰ� �Ǿ����ϴ�.
WSAECONNREFUSED 10061     ���ӽõ��� ������ ����Ǿ����ϴ�
WSAELOOP 10062            WSAENAMETOOLONG 10063
WSAEHOSTDOWN 10064        ���� ȣ��Ʈ�� �ٿ� �Ǿ���
WSAHOSTUNREACH 10065      ��Ʈ�� �ý��� ��� � ���ؼ� ����ȣ��Ʈ������ �� �� �� �����ϴ�.
WSASYSNOTREADY 10091      ��Ʈ��ũ ���� �ý����� ���� ����� �غ� �Ǿ� ���� ����(WSAStartup()�� ��ȯ)
WSAVERNOTSUPPORTED 10092  ��û�� �������� ���� ������ ���� �������� ���� �ý��ۿ��� �������� �ʽ��ϴ�.
WSANOTINITIALISED 10093   �� �Լ��� ����ϱ� ���� �������� WSAStartup �Լ��� ȣ���� �������ϴ�.
WSAHOST_NOT_FOUND 11001   ȣ��Ʈ�� ã�Ƴ� �� �����ϴ�.
WSATRY_AGAIN 11002        ��û�� ������ �߰� ���� ����
WSANO_RECOVERY  11003     ȸ���� �� ���� �����߻�
WSANO_DATA 11004          �߸��� �̸�(name)���� �ƹ��� �����Ͱ� ��ϵ��� �ʾҽ��ϴ�. 


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
  IP ��ȣ�� �����Ѵ�.
  �������� ip��ȣ�� ���� �� �ִ�.
*/
class CMySocketIp
{
public:
	CMySocketIp(HOSTENT *phe = nullptr );
	~CMySocketIp(void);

	void clear(void);
	void sethostinfo(HOSTENT *phe);

	char *gethostname(void){ return m_pname; }
	short getipnum(void){ return m_addr.size(); } // ip ����
	short getiplen(void){ return m_len; } // ip ����, 4 or 6
	void  getip(t_mysocketip &add, int num ); // num��° �����ǹ�ȣ�� ���´�.

	void  addip(t_mysocketip add){ m_addr.push_back(add); }// ip�ּ� �߰�
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
	unsigned int  mem_size; // �Ҵ���� �޸� ��ü ������
	unsigned int  data_size; // ���� ������ ������
	unsigned int  header_size; // ������� ������

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

	//�ٿ���� ������ ó��
	bool  setdown_data(const char *pdata, unsigned int size); 
	bool  add_recv_data(const char *pdata, unsigned int size);
	unsigned int  get_recv_datasize(void);
	unsigned int  copy_recv_data(char *ptemp, int len);
	const char *get_recv_dataptr(void) const { return m_recv_data.pdata;  }

	bool  save_recv_chunkdata(const char *pfname);
	int  get_request_code(void){ return m_chunkdata.get_request_code();  }  // ���� �����ڵ�
	const char *get_request_msg(void) { return m_chunkdata.get_request_msg(); }    // ���� �����ڵ忡 ���� �޼���

	bool  isuse(void){ return  (m_host_name.size() != 0)?true: false; } // ȣ��Ʈ�� �̸��� �ִٸ� ��������� �����Ѵ�


	void  begin_thread(void) { m_begin_thread = true;  }
	void  end_thread(void) { m_begin_thread = false; }
	bool  thread_check(unsigned int *pdownsize);  //������ �Լ��� ����Ǿ����� üũ


	void   set_thread_quit(bool sw); // ������ �Լ��� ���� ��û
	bool   get_thread_quit(void); // ������ �Լ����� �����û�� �ִ��� Ȯ��

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

	t_mysocketobj  m_recv_data; // ���� ��Ŷ ������ ����

	CMyChunkData  m_chunkdata; // HTTP  ���� ������ ���� Ŭ����
};
//====================================================================





#define MYSOCKET_MODE_GET   true
#define MYSOCKET_MODE_POST  false
#define MYSOCKET_TABLE_SIZE  50      // thread �ִ� �����̱⵵ �ϴ�
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


	   int  table_check(void); // ���� �ʴ� ���̺��� ��ȣ�� ��´�.
	  void  table_allclear(void); // ��� ���̺��� �ʱ�ȭ �Ѵ�.,  �ٿ����� �����ʹ� ��� �������.
	  void  table_clear(int num); 

	  int  table_request_code(int num);  // �����ڵ� ���
	  const char *table_request_msg(int num);

	  unsigned int table_getdownsize(int num); // �ٿ���� ������ ũ�� ���
	  bool  table_downcheck(int num, unsigned int  *pdownsize); // �ٿ��� �� �Ǿ����� üũ
	  unsigned int  table_getdata(int num,char *pdata, unsigned int len); // �ٿ���� ������ ����,  ���ϰ��� ����� ������ũ��

	  const char *table_getptr(int num);
	  bool  table_save_data(int num,const char *pfname); 


	static CMySocket *GetMySocket(void)
	{
		static CMySocket  msc;
		return  &msc;
	}
};

