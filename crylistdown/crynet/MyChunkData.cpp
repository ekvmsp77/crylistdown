
/*
0x06212828  48 54 54 50 2f 31 2e 31 20 32 30 30 20 4f 4b 0d 0a 44 61 74 65 3a 20 46  HTTP / 1.1 200 OK..Date: F
0x06212840  72 69 2c 20 32 35 20 4a 75 6c 20 32 30 31 34 20 31 34 3a 34 34 3a 31 39  ri, 25 Jul 2014 14 : 44 : 19
0x06212858  20 47 4d 54 0d 0a 53 65 74 2d 43 6f 6f 6b 69 65 3a 20 4a 53 45 53 53 49   GMT..Set - Cookie : JSESSI
0x06212870  4f 4e 49 44 3d 43 32 36 33 34 46 45 39 42 30 41 32 41 43 45 37 36 45 44  ONID = C2634FE9B0A2ACE76ED
0x06212888  32 41 43 46 44 46 42 43 42 36 35 38 34 2e 6e 6f 64 65 5f 74 6f 6d 63 61  2ACFDFBCB6584.node_tomca
0x062128A0  74 31 30 32 5f 38 31 30 39 3b 20 50 61 74 68 3d 2f 70 6f 72 5f 6b 6f 72  t102_8109; Path = / por_kor
0x062128B8  0d 0a 43 6f 6e 74 65 6e 74 2d 44 69 73 70 6f 73 69 74 69 6f 6e 3a 20 61  ..Content - Disposition: a
0x062128D0  74 74 61 63 68 6d 65 6e 74 3b 20 66 69 6c 65 6e 61 6d 65 3d 44 61 74 61  ttachment; filename = Data
0x062128E8  2e 78 6c 73 3b 0d 0a 50 72 61 67 6d 61 3a 20 6e 6f 2d 63 61 63 68 65 3b.xls; ..Pragma: no - cache;
0x06212900  0d 0a 43 6f 6e 6e 65 63 74 69 6f 6e 3a 20 63 6c 6f 73 65 0d 0a 54 72 61  ..Connection: close..Tra
0x06212918  6e 73 66 65 72 2d 45 6e 63 6f 64 69 6e 67 3a 20 63 68 75 6e 6b 65 64 0d  nsfer - Encoding : chunked.
0x06212930  0a 43 6f 6e 74 65 6e 74 2d 54 79 70 65 3a 20 74 65 78 74 2f 78 6c 73 3b.Content - Type : text / xls;
0x06212948  63 68 61 72 73 65 74 3d 55 54 46 2d 38 0d 0a 0d 0a 31 66 66 38 0d 0a d0  charset = UTF - 8....1ff8..

HTTP/1.1 200 OK
Date : Fri, 25 Jul 2014 14 : 44 : 19 GMT
Set - Cookie : JSESSIONID = C2634FE9B0A2ACE76ED2ACFDFBCB6584.node_tomcat102_8109; Path = / por_kor
Content - Disposition: attachment; filename = Data.xls;
Pragma: no - cache;
Connection: close
Transfer - Encoding : chunked
Content - Type : text / xls; charset = UTF - 8
0d0a
0d0a
<십육진수 데이터 크기>  해당 값은 패킷단위를 신경쓰지 않기 때문에 패킷의 처음, 중간, 끝 어디든지 올 수 있다.
......
<십육진수 데이터 크기>
....
<0>   // 수신 데이터 끝  [CRLF]0[CRLF][CRLF]
0d0a
*/

#include "MyChunkData.h"
#include "..\cryutil\Util.h"


CMyChunkData::CMyChunkData()
{
	m_pdata = nullptr;
	m_end_data = false;
	m_requset_code_msg = "?";
}


CMyChunkData::~CMyChunkData()
{
	if (m_pdata != nullptr) free(m_pdata);
	m_pdata = nullptr;
}

void CMyChunkData::clear(void)
{
	CMyHttpUtil  myhttp;

	myhttp.clear_recv_header(m_recv_info);

	if (m_pdata != nullptr) free(m_pdata);
	m_pdata = nullptr;
	m_data_len = 0;
	m_end_data = false;
}

/*
  수신 데이터의 상태를 파악한다.
  chunke방식만 처리한다.
  실제 데이터는 복사하지 않는다.
*/
bool CMyChunkData::proc_data(const char *pdata, unsigned int size)
{
	CMyHttpUtil  myhttp;

	if (m_recv_info.transfer_encoding.size() == 0){
		bool sw = myhttp.set_recv_data_parseing(pdata, size);
		if (sw == false) return false;

		myhttp.copy_recv_header(m_recv_info);

		m_requset_code_msg = myhttp.get_recv_request_code_message();
	}

	if (m_recv_info.header_size == 0) return false;
	if (size <= m_recv_info.header_size) return true; // 헤더만 있는 상태라면 그냥 넘어간다.
	const char  *ptemp = pdata + m_recv_info.header_size;
	int  offset = (int)size - m_recv_info.header_size;
	unsigned int  csize = 0;

	m_data_len = 0;

	if (MYUTIL()->comp_str(m_recv_info.transfer_encoding.data(), "chunked", strlen("chunked")) == false){
		if (m_recv_info.content_length > 0){
			if (offset < m_recv_info.content_length) return true;
			m_end_data = true;
		}
		return false;
	}

	do{
		int r = get_hexi(ptemp, offset, &csize);
		if (r == 0) return false; // 오류

		m_data_len += csize;

		if (csize == 0){
			m_end_data = true;
			break;
		}
		
		r = r + csize + 3; // "청크 길이값" <CR><LF> + 청크 데이터 +  <CR><LF>
		ptemp += r;
		offset -= r;
	} while ( offset > 0 );

	return true;
}

/*
  반드시 proc_data()먼저 실행시키고, m_end_data 가 true이여야만 한다.
*/
bool CMyChunkData::save_data(const char *pdata, unsigned int size,const char *pfname)
{
	if (m_end_data == false) return false;

	FILE  *ps;

	if (pfname == nullptr){
		if (fopen_s(&ps, m_recv_info.file_name.data(), "wb") != 0) return false;
	}
	else {
		if (fopen_s(&ps, pfname, "wb") != 0) return false;
	}
	const char  *ptemp = pdata + m_recv_info.header_size;
	int  offset = (int)size - m_recv_info.header_size;
	unsigned int  csize = 0;

	if (m_recv_info.content_length > 0){
		fwrite(ptemp, m_recv_info.content_length, 1, ps);

		fclose(ps);
		return true;
	}


	do{
		int r = get_hexi(ptemp, offset, &csize);
		if (r == 0){
			fclose(ps);
			return false; // 오류
		}

		m_data_len += csize;

		if (csize == 0){
			break;
		}

		ptemp += r + 1;
		offset -= r+1;

		fwrite(ptemp, csize, 1, ps);

		ptemp += csize + 2;
		offset -= csize + 2;

	} while (offset > 0);

	fclose(ps);
	return true;
}

/*
   십육진수 문자열을  정수값으로 얻는다.

   pdata : 소스 데이터 포인터
   size : 소스 데이터 길이
   pout : 정수값
   return : 옵셋값
            값을 얻을 수 없다면 0이다

   ex)   A7<CR><LF>
   =>  pout : 167
       return : 4
*/
int   CMyChunkData::get_hexi(const char *pdata, unsigned int size, unsigned int *pout)
{
	string so;
	unsigned int  cnt = 0;
	int  value = 0;

	if (pdata == nullptr) return 0;
	if (size == 0) return 0;

	do{
		so += *pdata++;
		cnt++;
		if (*pdata == 0x0D && *(pdata + 1) == 0x0A){
			cnt++;
			size_t cs = so.size();

			for (int n = 0; n < cs; n++){
				char c = so[n];
				int  i;

				if ('A' <= c && c <= 'F') i = (c - 'A')+0x0a;
				else if ('a' <= c && c <= 'f') i = (c - 'a')+0x0a;
				else if ('0' <= c && c <= '9') i = c - '0';
				else  return 0; // 그외 다른 문자가 있다면 뭔가 잘못되었다.
				value <<= 4;
				value |= i;
			}
			*pout = value;
			return cnt;
		}
	} while (cnt < size);

	return 0;
}