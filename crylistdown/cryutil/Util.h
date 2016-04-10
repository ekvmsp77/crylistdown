#pragma once
#include <vector>
using namespace std;


class CUtil
{
protected:
	CUtil(void);
	~CUtil(void);

	bool  comp_ch(const char sc, const char *ptc,int ntc);  // ptc에서 sc와 일치하는 문자가 있는지 검사한다.
public:
	void  j8_curtime(char *pbuf,int len);
    void  getcurtime(char *pout,int len, int opt); // opt: 0:날짜,  1:시간, 2: 모두
	void  cnv_outchar(const wchar_t  *pin, char  *pout,int len);
	void  cnv_outwchar_t(const char  *pin, wchar_t  *pout,int len);
	unsigned int strhash(const char *pstr);  // 문자열에 대한 해쉬코드만들기

	const char *getch_str(char code, const char *psrc); // 문자열src에서 문자 "code까지" 의 문자열만 따로 얻는다. 

	const wchar_t  *cnv_valuew(const wchar_t  *pin); // "00012345" ->  "12,345"
	const wchar_t  *cnv_value(const int val); //    -12345 ->  "-12,345"
	int  get_sc(void); // 현재 시간값을 얻는다.

	bool  get_string_word(const char *pin, vector<string> *pout, const char *ptc, int ntc ); // 문자열에서  단어문자열을 얻는다. 

	bool  comp_str(const char *psrc, const char *pds, int len); // 문자열 비교, 알파벳의 경우 대소문자 구분없이 비교한다. 
	char  *cnv_http_hexch(char ch); // char code -> "%xx"

	static  CUtil *UtilObj(void)
	{
		static  CUtil  obj;
		return &obj;
	}

};

#define   MYUTIL()   CUtil::UtilObj()