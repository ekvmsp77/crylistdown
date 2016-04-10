#include "DownProc.h"
#include "..\cryutil\Log.h"
#include "..\cryutil\Util.h"

CDownProc::CDownProc()
{
}


CDownProc::~CDownProc()
{

	CMySocket::GetMySocket()->table_allclear();


	m_obj.clear();
}


void   CDownProc::new_obj(const t_downproc  *pt)
{
	t_downproc_list  temp;
	t_mysocket_http_header   header;

	for (int n = 0; n < _countof(temp.name); n++)  temp.name[n] = pt->name[n];
	temp.func = pt->func;

	
	if (pt->header.Content_Type.length() != 0 ){
		header = pt->header;
		temp.objnum = CMySocket::GetMySocket()->thread_down_http(pt->phost.data(), pt->purl.data(), pt->pstr.data(), pt->mode, &header);
	}
	else  temp.objnum = CMySocket::GetMySocket()->thread_down_http(pt->phost.data(), pt->purl.data(), pt->pstr.data(), pt->mode);


	m_obj.push_back(temp);
}


void    CDownProc::timer(void)
{
	char  msg[128];

	list<t_downproc_list>::iterator itor;

	itor = m_obj.begin();

	unsigned int  downsize;

	while (itor != m_obj.end())
	{
		if (itor->objnum >= 0){
			bool  dsw = CMySocket::GetMySocket()->table_downcheck(itor->objnum, &downsize);
			if (dsw == false){ 
				// 다운 완료
				// 유저 함수를 호출 한 다음  연결 정보를 삭제한다.
				// 따라서, 유저함수에서 다운 받은 정보를 저장하던가 해야 한다.
				unsigned int  dsize;
				dsize = CMySocket::GetMySocket()->table_getdownsize(itor->objnum);

				itor->func(itor->objnum);  // 유저 함수 호출

				sprintf_s(msg, "%s : %dbytes 다운 완료", itor->name, dsize);
				MYLOG(msg);

				// 연결 정보 삭제
				CMySocket::GetMySocket()->table_clear(itor->objnum);
				itor = m_obj.erase(itor);
			}
		}
		else itor++;
	}

}


bool  CDownProc::open_downinfo(const  char  *pfname, _fdownproc pf)
{
	t_downproc  info;
	string  temp;

	FILE  *ps;
	char  *buffer;

	if (pfname == nullptr) return false;
	if (fopen_s(&ps, pfname, "rt") != 0) return false;
	
	fseek(ps, 0L, SEEK_END);
	int flen = ftell(ps);
	fseek(ps, 0L, SEEK_SET);

	flen++;
	buffer = (char *)malloc(flen);

	memset(buffer, 0, flen);
	fgets(buffer, flen, ps);
	buffer[strlen(buffer) - 1] = 0;

	if (strcmp(buffer, DOWNPROC_FILE_CODE_STR) != 0){
		fclose(ps);
		return false;
	}


	while (!feof(ps)){
		memset(buffer, 0, flen);
		if (fgets(buffer, flen, ps) == NULL) break;
		buffer[strlen(buffer) - 1] = 0;

		temp = MYUTIL()->getch_str(':', buffer);
		int  len = temp.length();
		
		if (strncmp(buffer, "--", 2) == 0) continue; // 주석
		if (strcmp(buffer, "end") == 0) break;
		if (temp.compare("name:") == 0) sprintf_s(info.name, _countof(info.name) - 1, "%s", buffer + len);
		if (temp.compare("host:") == 0){ info.phost = (buffer + len);  }
		if (temp.compare("url:") == 0){ info.purl = (buffer + len);  }
		if (temp.compare("mode:") == 0){
			if (strcmp(buffer + len, "post") == 0 || strcmp(buffer + len, "POST") == 0) info.mode = MYSOCKET_MODE_POST;
			else info.mode = MYSOCKET_MODE_GET;
		}
		if (temp.compare("Accept:") == 0){ info.header.Accept = (buffer + len); info.header.Accept += "\r\n"; }
		if (temp.compare("Accept_Encoding:") == 0){ info.header.Accept_Encoding = (buffer + len); info.header.Accept_Encoding += "\r\n"; }
		if (temp.compare("Accept_Language:") == 0){ info.header.Accept_Language = (buffer + len); info.header.Accept_Language += "\r\n"; }
		if (temp.compare("Connection:") == 0){ info.header.Connection = (buffer + len); info.header.Connection += "\r\n"; }
		if (temp.compare("Origin:") == 0){ info.header.Origin = (buffer + len); info.header.Origin += "\r\n"; }
		if (temp.compare("Referer:") == 0){ info.header.Referer = (buffer + len); info.header.Referer += "\r\n"; }
		if (temp.compare("User_Agent:") == 0){ info.header.User_Agent = (buffer + len); info.header.User_Agent += "\r\n"; }
		if (temp.compare("Content_Type:") == 0){ info.header.Content_Type = (buffer + len); info.header.Content_Type += "\r\n"; }
		if (temp.compare("data:") == 0){ info.pstr = (buffer + len); }
		if (temp.compare("XRW:") == 0) { info.header.XRW = (buffer + len); info.header.XRW += "\r\n"; }

	}
	fclose(ps);

	info.func = pf;

	m_info.push_back(info);
	free(buffer);
	return true;
}

bool  CDownProc::down_info(const char *pname)
{
	int size = m_info.size();

	for (int n = 0; n < size; n++){
		if (strcmp(pname, m_info[n].name) != 0) continue;
		new_obj(&m_info[n]);
		return true;
	}

	return false;
}


/*   보류
   다운정보 data에서 특정 문자열을 찾는다. 
   리턴값은  위치값이다. 없으면 -1

   pname의 문자값을 십육진수형태로 변환해서 찾는다.

	//2015%2F03%2F20%3C%2Ftd%3E%0D%0A%3Ctd%3E2015%2F03%2F23
	-> 2015/03/20</ftd>\n<ctd>2015/03/23 

*/
int  CDownProc::search_data_ctype(const char *pname, const char *ps)
{
	int  pos = -1;

	return pos;
}