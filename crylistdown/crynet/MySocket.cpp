#include <process.h>
#include "MySocket.h"

#pragma comment(lib, "Ws2_32.lib")

const int _namelen = 50;  // 50 byte


#define  MY_HTTP_URLLEN  512
#define  MY_HTTP_HOSTLEN  128
//..............................................................

CMySocketObj  _table_obj[MYSOCKET_TABLE_SIZE];
void   http_thread(void *pv);










//=============================================================

CMySocketIp::CMySocketIp(HOSTENT *phe)
{
	m_pname = m_paliases = nullptr;
	m_len =  m_addrtype = 0;

	if( phe != nullptr )sethostinfo(phe);
}

CMySocketIp::~CMySocketIp(void)
{
	clear();
}

void CMySocketIp::clear(void)
{
	if( m_pname != nullptr ) free(m_pname);
	if( m_paliases != nullptr ) free(m_paliases);

	m_pname = m_paliases = nullptr;
	m_len = m_addrtype = 0;
	m_addr.clear();
}

void CMySocketIp::sethostinfo(HOSTENT *phe)
{
	size_t len=0;
	t_mysocketip  add;

	len = strlen(phe->h_name);
	m_pname = (char*)malloc(len+1);
	strcpy_s(m_pname,len+1,phe->h_name);

	if( *phe->h_aliases != nullptr ){
		len = strlen(*phe->h_aliases);
		m_pname = (char*)malloc(len+1);
		strcpy_s(m_pname,len+1, *phe->h_aliases);
	}

	m_len = phe->h_length;
	m_addrtype = phe->h_addrtype;

	int n=0;
	while(phe->h_addr_list[n] != 0 ){
        for(int i=0; i < m_len; i++){
		    switch(i){
				case 0: add.x1 = phe->h_addr_list[n][i]; break;
				case 1: add.x2 = phe->h_addr_list[n][i]; break;
				case 2: add.x3 = phe->h_addr_list[n][i]; break;
				case 3: add.x4 = phe->h_addr_list[n][i]; break;
				case 4: add.x5 = phe->h_addr_list[n][i]; break;
				case 5: add.x6 = phe->h_addr_list[n][i]; break;
			}
		}
		m_addr.push_back(add);
		n++;
	}
/*
	for( n=0; n < m_addr.size() ; n++){
		add = m_addr[n];
		printf("%d.%d.%d.%d", add.x1, add.x2, add.x3, add.x4);
		printf("\n");
	}*/
}


void  CMySocketIp::getip(t_mysocketip &add, int num )
{
	if( num >= m_addr.size() ) return;
	add = m_addr[num];
}

//=============================================================
//=============================================================
CMySocketObj::CMySocketObj(void)
{
	m_begin_thread = false;
	m_url.clear();
	m_host_name.clear();
	m_post_str.clear();

	InitializeCriticalSection(&_mycritical);

	memset(&m_recv_data, 0, sizeof(t_mysocketobj));
	m_recv_data.mem_size = MYSOCKETOBJ_RECV_DEFSIZE;
	m_recv_data.pdata = (char *)malloc(MYSOCKETOBJ_RECV_DEFSIZE);
}

CMySocketObj::~CMySocketObj(void)
{
	clear();
	// 다운 데이터 정보 초기화
	if (m_recv_data.pdata != nullptr) free(m_recv_data.pdata);
	memset(&m_recv_data, 0, sizeof(t_mysocketobj)); 

	DeleteCriticalSection(&_mycritical);
}

void  CMySocketObj::clear(void)
{

//스레드 함수가 동작중이라면 종료신호를 보낸 후 종료될때까지 기다린다.
	if (m_begin_thread == true){
	    set_thread_quit(CMySocketObj::_thread_quit_on);
		while (get_thread_quit() == CMySocketObj::_thread_quit_on){ Sleep(100);  }

	//	m_begin_thread = false;     ==> end_thread()
	}

	m_url.clear();
	m_host_name.clear();
	m_post_str.clear();


	if (m_recv_data.pdata != nullptr) free(m_recv_data.pdata);
	memset(&m_recv_data, 0, sizeof(t_mysocketobj));
	m_recv_data.mem_size = MYSOCKETOBJ_RECV_DEFSIZE;
	m_recv_data.pdata = (char *)malloc(MYSOCKETOBJ_RECV_DEFSIZE);

	m_chunkdata.clear();
}

bool  CMySocketObj::add_recv_data(const char *pdata, unsigned int size)
{
	if (size == 0 || pdata == nullptr) return false;
	if (m_recv_data.pdata == nullptr || m_recv_data.mem_size == 0) return false; // 메모리 초기화 되지않았다면 그냥 종료한다.

	m_recv_data.data_size += size;
	if ( m_recv_data.data_size >= m_recv_data.mem_size){
		// 할당받은 메모리보다 크다면 재할당받는다.
		char *pt;
		unsigned int  dlen = m_recv_data.data_size - m_recv_data.mem_size;
		dlen += MYSOCKETOBJ_RECV_DEFSIZE;

		m_recv_data.mem_size = m_recv_data.data_size + dlen;
		pt = (char *)malloc(m_recv_data.mem_size);
		memcpy(pt, m_recv_data.pdata, m_recv_data.data_size - size); //일단 기존데이터만큼 복사한다.
		free(m_recv_data.pdata);

		m_recv_data.pdata = pt;
	}
	char *ptemp = m_recv_data.pdata + (m_recv_data.data_size - size);
	memcpy(ptemp, pdata, size);

	return true;
}

unsigned int  CMySocketObj::get_recv_datasize(void)
{
	unsigned int ts = 0;

	EnterCriticalSection(&_mycritical);
		ts = m_recv_data.data_size;
	LeaveCriticalSection(&_mycritical);
	return ts;
}



/*
   다운 받은 데이터를 복사한다.
   len:저장공간의 크기, 
   리턴값은 복사된 데이터 크기
*/
unsigned int   CMySocketObj::copy_recv_data(char *ptemp, unsigned int len)
{
	unsigned int  size=0;

	if (len <= 0 || ptemp == nullptr) return 0;

	EnterCriticalSection(&_mycritical);
		if (len > m_recv_data.data_size) size = m_recv_data.data_size;
		else size = len;

		memcpy(ptemp, m_recv_data.pdata, size);

	LeaveCriticalSection(&_mycritical);

	return size;
}


void   CMySocketObj::set_thread_quit(bool sw)
{
	EnterCriticalSection(&_mycritical);
		m_thread_quit_sw = sw;
	LeaveCriticalSection(&_mycritical);
}

bool   CMySocketObj::get_thread_quit(void)
{
	bool  sw;

	EnterCriticalSection(&_mycritical);
		sw = m_thread_quit_sw;
	LeaveCriticalSection(&_mycritical);

	return sw;
}

bool  CMySocketObj::thread_check(unsigned int *pdownsize)
{
	bool  sw;
	unsigned int ts = 0;

	EnterCriticalSection(&_mycritical);
		sw = m_begin_thread;
		*pdownsize = m_recv_data.data_size;
	LeaveCriticalSection(&_mycritical);

	return sw;
}

void   CMySocketObj::http_header_clear(void)
{
	m_http_header.Accept.clear();
	m_http_header.Accept_Encoding.clear();
	m_http_header.Accept_Language.clear();
	m_http_header.Connection.clear();
	m_http_header.Origin.clear();
	m_http_header.Referer.clear();
	m_http_header.XRW.clear();         // X-Requested-With
	m_http_header.User_Agent.clear();


}

bool   CMySocketObj::setdown_data(const char *pdata, unsigned int size)
{
	if (add_recv_data(pdata, size) == false) return false;

	if (m_chunkdata.proc_data(m_recv_data.pdata, m_recv_data.data_size) == false) return false;
	if (m_chunkdata.end_data() == true){
		return false;
	}

	return true;
}

bool  CMySocketObj::save_recv_chunkdata(const char *pfname)
{
	return  m_chunkdata.save_data(m_recv_data.pdata, m_recv_data.data_size, pfname);
}

//=============================================================
//=============================================================


CMySocket::CMySocket(void)
{

	memset(&m_wsaData, 0, sizeof(m_wsaData) );
	if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0) {
		_msglog("WSAStartup failed");
		return;
	}

	getlocalhostinfo();

}


CMySocket::~CMySocket(void)
{

	// 테이블 초기화
	table_allclear();

    WSACleanup();
}

void  CMySocket::getlocalhostinfo(void)
{
	HOSTENT  *phe = nullptr;
	char name[_namelen];

	if(  gethostname(name,_namelen) != NULL ){
		_msglog("host name = 0 ");
		return;
	}


	phe = gethostbyname( name );
	if( phe == nullptr ){ 
		_msglog("host information = 0"); 	
		return;	
	}

	m_localip.sethostinfo(phe);
}



//=====================  http  =======================
SOCKET CMySocket::new_socket_http(void)
{
	SOCKET sc  = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if( sc == INVALID_SOCKET )	_msglog("http socket error!");

	return sc;
}

void  CMySocket::del_socket_http(SOCKET sc)
{
	_msglog("http socket - close");
    closesocket(sc);
}


bool  CMySocket::connect_http(SOCKET sc,const char *phost_name)
{
	struct hostent *remoteHost;
	remoteHost = gethostbyname(phost_name);

	if( remoteHost == 0 ){
		_msglog("http - connect() hoset name error!");
		return false;
	}
	sockaddr_in name;

	memset(&name,0,sizeof(sockaddr_in));
	name.sin_addr.S_un.S_addr = *(u_long *) remoteHost->h_addr_list[0];
	name.sin_family = AF_INET;
	name.sin_port = htons(80);
	int ret = connect(sc,(SOCKADDR *)&name, sizeof(name));
	if(ret == SOCKET_ERROR){
		int r = WSAGetLastError();
		char  str[50];
		sprintf_s(str,50, "http - connect(),error code=%d",r);
		_msglog(str);
		return false;
	}
	return true;
}


bool  CMySocket::send_http(SOCKET sc,const char *pdata, size_t data_len)
{
	int rs = send(sc,pdata,(int)data_len,0);
	if( rs ==  SOCKET_ERROR ) {
		char  str[50];
		int r = WSAGetLastError();
		sprintf_s(str,50, "http - recv(),error code=%d",r);
		_msglog(str);
		return false;
	}

	return true;
}



int  CMySocket::thread_down_http(const char *phost_name, const char *purl, const char *pstr, bool mode, t_mysocket_http_header  *ph)
{
	static int num ;

	num = -1;
	//사용되지 않는 테이블을 찾는다.
	for (int n = 0; MYSOCKET_TABLE_SIZE; n++){
		if (_table_obj[n].isuse() == 0){ num = n; break; }
	}

	// 모두 사용중이라면 그냥 끝낸다.
	if (num == -1) return num;


	_table_obj[num].clear();  // 초기화  , 다운 정보도 초기화된다.


	_table_obj[num].set_url(purl);
	_table_obj[num].set_hostname(phost_name);
	_table_obj[num].set_post_string(pstr);

	if (ph != nullptr) _table_obj[num].set_http_header(ph);
	else _table_obj[num].http_header_clear();



	if (mode == MYSOCKET_MODE_GET) _table_obj[num].set_mode_get();
	else _table_obj[num].set_mode_post();

	_beginthread(http_thread, 0, &num);


	return num;
}


void  CMySocket::table_allclear(void)
{
	for (int n = 0; n < MYSOCKET_TABLE_SIZE; n++){
		_table_obj[n].clear();
	}

}

int  CMySocket::table_check(void)
{
	int num = -1;


	return num;
}

void   CMySocket::table_clear(int num)
{
	if (0 <= num &&  num < MYSOCKET_TABLE_SIZE){
		_table_obj[num].clear();
	}
}

int  CMySocket::table_request_code(int num)
{
	if (0 > num || num >= MYSOCKET_TABLE_SIZE) return 0;

	return _table_obj[num].get_request_code();
}

const char *CMySocket::table_request_msg(int num)
{
	if (0 > num || num >= MYSOCKET_TABLE_SIZE) return 0;

	return _table_obj[num].get_request_msg();
}

unsigned int CMySocket::table_getdownsize(int num)
{
	if (0 > num || num >= MYSOCKET_TABLE_SIZE) return 0;
	unsigned int size;

	size = _table_obj[num].get_recv_datasize();

	return size;
}

bool  CMySocket::table_downcheck(int num, unsigned int  *pdownsize)
{
	if (0 > num || num >= MYSOCKET_TABLE_SIZE) return false;

	return  _table_obj[num].thread_check(pdownsize);
}

unsigned int  CMySocket::table_getdata(int num,char *pdata, unsigned int len)
{
	if (0 > num || num >= MYSOCKET_TABLE_SIZE) return 0 ;
    
	return _table_obj[num].copy_recv_data(pdata, len);
}

const char *CMySocket::table_getptr(int num)
{
	if (0 > num || num >= MYSOCKET_TABLE_SIZE) return 0;

	return  _table_obj[num].get_recv_dataptr();
}

bool  CMySocket::table_save_data(int num, const char *pfname)
{
	if (0 > num || num >= MYSOCKET_TABLE_SIZE) return 0;

	return _table_obj[num].save_recv_chunkdata(pfname);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////


void   http_thread(void *pv)
{
	int   table_num = *(int *)pv;

	if (table_num < 0 || table_num >= MYSOCKET_TABLE_SIZE) return;
	_table_obj[table_num].begin_thread();

	CMySocket  *pobj = CMySocket::GetMySocket();

	//소켓 생성
	SOCKET  sc = pobj->new_socket_http();
	if (sc == SOCKET_ERROR) {
		_table_obj[table_num].set_thread_quit(CMySocketObj::_thread_quit_off);
		_table_obj[table_num].end_thread();
		return;
	}

	// 연결할 주소를 얻는다
	CMyHttpUtil  myhttp;
	string  surl = _table_obj[table_num].get_url();
	string  shost = _table_obj[table_num].get_hostname();
	string  str = _table_obj[table_num].get_post_string();
	string  smode;

	if (_table_obj[table_num].get_mode() == MYSOCKET_MODE_GET) smode = "GET";
	else smode = "POST";

	//HTP 송신 헤더를 만들고, 헤더 문자열을 얻는다.
	myhttp.set_url_abs(surl.data(), shost.data() );

	t_mysocket_http_header  header;
	_table_obj[table_num].get_http_header(&header);
	myhttp.make_def_send_header(smode.data(), str.data() , &header);

	const char *psheader = myhttp.get_send_header_string();
	size_t len = strlen(psheader);

	// 접속
	if (pobj->connect_http(sc, shost.data()) == false){
		_table_obj[table_num].set_thread_quit(CMySocketObj::_thread_quit_off);
		_table_obj[table_num].end_thread();
		pobj->del_socket_http(sc);
		return;
	}

	//HTTP헤더 전송
	pobj->send_http(sc, psheader,len );

	char *ptemp;
	bool  rcv_header_sw=false; // 수신 헤더 체크
	unsigned long   rdsize=0;
	bool  rendsw = true;

	unsigned long  rmaxsize=0;//
	int  types = sizeof(long);
	if (getsockopt(sc, SOL_SOCKET, SO_RCVBUF, (char*)rmaxsize, &types) == SOCKET_ERROR){//수신버퍼의 크기를 얻는다.
		rmaxsize = 1024 * 100; // 만약,버퍼의 크기를 얻지못한다면  임의 크기로 지정한다.
	}

	ptemp = (char *)malloc(rmaxsize+5);
	memset(ptemp, 0, rmaxsize);

	while (1){
		// 스레드 함수 종료요청이 오면 바로 종료한다.
		if (_table_obj[table_num].get_thread_quit() == CMySocketObj::_thread_quit_on) break;

		// 데이터 수신
		rendsw = true;
		rdsize = rmaxsize-1;
	/*	while (1){
			if (ioctlsocket(sc, FIONREAD, &rdsize) != 0){ rendsw = false;  break; }
			if (rdsize > 0) break;
		}
		if (rendsw == false) break;
		*/
		int r = recv(sc, ptemp, rdsize, 0); // !!
		//*(ptemp + r) = 0;  수신데이터의끝에 0을 넣는다.

		// 수신 에러이면 끝
		if (r == SOCKET_ERROR){
			char  str[50];
			int r = WSAGetLastError();
			sprintf_s(str, 50, "http - recv_http(),error code=%d", r);
			_msglog(str);
			break;
		}
		else if (r == 0) break; // 수신 데이터가 없다면 종료
		else if (_table_obj[table_num].setdown_data(ptemp, r) == false) break;   // 수신 데이터 넣기 

	}
	free(ptemp);
	
	//소켓 닫기
	pobj->del_socket_http(sc);

	//종료되었음을 알린다. - 스레드 종료를 알린다.ㅣ
	_table_obj[table_num].set_thread_quit(CMySocketObj::_thread_quit_off);
	_table_obj[table_num].end_thread();
}
