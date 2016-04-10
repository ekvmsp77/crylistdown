#include "Util.h"
#include  <time.h>
#include <string>

#include <Windows.h>


CUtil::CUtil(void)
{
}


CUtil::~CUtil(void)
{
}

int  CUtil::get_sc(void)
{
	time_t tt;

	time(&tt);
	return   (int)tt;
}

void  CUtil::getcurtime(char *pout,int len, int opt)
{
	tm pt;
	time_t tt;

	time(&tt);
	gmtime_s( &pt, &tt );

	pt.tm_year +=1900;
	pt.tm_mon++;  // 0-11
	pt.tm_hour +=9;  // UTC +9 ?????
	if( pt.tm_hour >= 24 ){
		pt.tm_hour -= 24; 
		pt.tm_mday++;
	}

	switch(opt)
	{
		case 0:
			sprintf_s(pout,len,"%d_%d_%d ", pt.tm_year, pt.tm_mon, pt.tm_mday);
			break;
		case 1:
            sprintf_s(pout,len,"%02d:%02d:%02d  ",  pt.tm_hour, pt.tm_min, pt.tm_sec);
			break;
		case 2:
		default:
		    sprintf_s(pout,len,"%d_%d_%d %02d:%02d:%02d  ", pt.tm_year, pt.tm_mon, pt.tm_mday, pt.tm_hour, pt.tm_min, pt.tm_sec);
			break;
	}
}

void  CUtil::cnv_outchar(const wchar_t  *pin, char  *pout,int len)
{
	memset(pout, 0, sizeof(char)* len);
	WideCharToMultiByte(CP_ACP, 0, pin, wcslen(pin),pout, len,NULL,NULL );
}

void  CUtil::cnv_outwchar_t(const char  *pin, wchar_t  *pout,int len)
{
	memset(pout, 0, sizeof(wchar_t)* len);
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pin, strlen(pin), pout, len);
}

unsigned int CUtil::strhash(const char *pstr)
{
	hash<string>  hs;

	/*
	unsigned long hash = 5381;
	int c;

	while (c = *pstr++)
		hash = ((hash << 5) + hash) + c; // hash * 33 + c 
 */
	return hs(pstr);
}

const wchar_t  *CUtil::cnv_valuew(const wchar_t  *pin)
{
	static wchar_t  temp[200];

	return  temp;
}

void  CUtil::j8_curtime(char *pbuf,int len)
{
	static char buf[20]; // yyyymmdd
	time_t ti;
	tm   tv;

	time(&ti);
	gmtime_s(&tv,&ti);

	tv.tm_hour += 9;  // UTC +9 ?????
	if (tv.tm_hour >= 24){
		tv.tm_hour -= 24;
		tv.tm_mday++;
	}
	tv.tm_year += 1900;
	tv.tm_mon++;
	sprintf_s(buf, 20, "%d%02d%02d", tv.tm_year, tv.tm_mon, tv.tm_mday);

	if( len >= 8 ) len = 8;

	for(int n=0; n < len; n++){
		pbuf[n] = buf[n];
	}
	pbuf[len] = 0;
}

const char *CUtil::getch_str(char code, const char *psrc)
{
	static  string  temp;
	bool sw = false;

	temp.clear();

	while (*psrc != 0){
		temp += *psrc;
		if (*psrc == code){
			sw = true;
			break;  // code���� 
		}

		psrc++;
	}
	if (!sw) temp.clear();  // code�� ��ġ�ϴ� ���ڰ� ���ٸ�.  x
	return  temp.data();
}

const wchar_t  *CUtil::cnv_value(const int val)
{
	static  wstring  temp;
	int  v = (val<0)? val*-1 : val;  // �����̸� ����� �ٲ��ش�.  
	int  com = 0;
	wchar_t  ts[2] = { 0, 0 };

	temp.clear();
	while (1){
		int  vc = v - (v / 10) * 10;
		v /= 10;
		com++;

		ts[0] = L'0' + vc;
		temp.insert(0, ts);
		if (v == 0) break;

		if (com == 3){
			com = 0;
			temp.insert(0, L",");
		}
	}
	if (val < 0) temp.insert(0, L"-");

	return temp.data();
}

/*
  ���ڰ��� �������� ���ڿ��� ��ȯ�Ѵ�.
  HTML���� �� �������� ����
*/
char  *CUtil::cnv_http_hexch(char ch)
{
	static char temp[4];

	temp[0] = (char)'%%';
	temp[3] = 0;

	temp[1] = 10 > (ch >> 4) ? (ch >> 4) + '0' : (ch >> 4) + 'A';
	temp[2] = 10 > (ch & 0x0F) ? (ch & 0x0F) + '0' : (ch & 0x0F) + 'A';

	return temp;
}

/*  
   �ϳ��� ���ڿ��� Ư������ �������� �и��Ѵ�.
   ex)
     *ptc    =>    �и��� ���ڳ���
	 ntc   => �и� ���� ����
     pin   => "HTTP/1.1 200 OK"
	 pout  => [0]  "HTTP/1.1"
	          [1]  "200"
			  [2]  "OK"
	return => ���ڿ��� ���ٸ� false
	�и��� �ܾ��� �յ� ������ �����Ѵ�.
*/
bool  CUtil::get_string_word(const char *pin, vector<string> *pout,const char  *ptc, int ntc)
{
	string  temp;
	const char *pt = pin;
	const char ce = 0;

	if (pin == nullptr)return false;
	if (pout == nullptr) return false;
	if (ntc <= 0 || ptc == nullptr || *ptc == 0 )return false; // �и� ���ڰ� ���ٸ� �׳� ������.
	temp.clear();
	pout->clear();

	while (*pt != 0){
		if (comp_ch(*pt, ptc, ntc) == true){
			if (temp.size() > 0){
				// �ܾ��� �յ� ������ ���ְ� �����Ѵ�.
				string  ts;
				unsigned int sn = 0, en=temp.size()-1;
				while (sn < (temp.size()-1) && temp[sn] == ' ') sn++;
				while (en >= 0 && temp[en] == ' ') en--;
				if (sn < en){
					ts.clear();
					for (; sn <= en; sn++) ts += temp[sn];
					ts += ce;
					pout->push_back(ts);
				}
				else pout->push_back(temp); // " " �ΰ��
				temp.clear();
			}
		}
		else temp += *pt;
		pt++;
	}

	if (temp.size() > 0){
		string  ts;
		unsigned int sn = 0, en = temp.size() - 1;
		while (sn < (temp.size() - 1) && temp[sn] == ' ') sn++;
		while (en >= 0 && temp[en] == ' ') en--;
		if (sn < en){
			ts.clear();
			for (; sn <= en; sn++) ts += temp[sn];
			pout->push_back(ts);
		}
		else pout->push_back(temp); // " " �ΰ��	
	}

	return true;
}

/*
   ���� ���ڰ� �ִ��� �˻��Ѵ�.
   sc: �ҽ�����
   ptc : ���� ���ڳ���
*/
bool  CUtil::comp_ch(const char sc, const char *ptc,int ntc)
{
	if (ntc <= 0) return false;
	int n = 0;
	while (*ptc != 0){
		if (sc == *ptc) return true;
		ptc++;
		n++;
		if (n >= ntc) break;
	}
	return  false;
}

/*
 ���ڿ� �� �Լ�, ���ĺ��� ��� ��ҹ��ڸ� �������� �ʴ´�.
 ex)  "abc123" == "ABC123"
*/
bool  CUtil::comp_str(const char *psrc, const char *pds, int len)
{	

	for (int n = 0; n < len; n++){
		char  cs, cd;
		char  xs, xd;

		cs = *(psrc + n);
		cd = *(pds + n);
		if ( cs == 0) break;
		if ( cd == 0) break;

		if ('A' <= cs && cs <= 'Z') xs = (cs - 'A') + 'a';
		else xs = cs;

		if ('A' <= cd && cd <= 'Z') xd = (cd - 'A') + 'a';
		else xd = cd;

		if (xs != xd) return false;
	}

	return true;
}