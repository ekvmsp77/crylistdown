#pragma once
#include <vector>
using namespace std;


class CUtil
{
protected:
	CUtil(void);
	~CUtil(void);

	bool  comp_ch(const char sc, const char *ptc,int ntc);  // ptc���� sc�� ��ġ�ϴ� ���ڰ� �ִ��� �˻��Ѵ�.
public:
	void  j8_curtime(char *pbuf,int len);
    void  getcurtime(char *pout,int len, int opt); // opt: 0:��¥,  1:�ð�, 2: ���
	void  cnv_outchar(const wchar_t  *pin, char  *pout,int len);
	void  cnv_outwchar_t(const char  *pin, wchar_t  *pout,int len);
	unsigned int strhash(const char *pstr);  // ���ڿ��� ���� �ؽ��ڵ常���

	const char *getch_str(char code, const char *psrc); // ���ڿ�src���� ���� "code����" �� ���ڿ��� ���� ��´�. 

	const wchar_t  *cnv_valuew(const wchar_t  *pin); // "00012345" ->  "12,345"
	const wchar_t  *cnv_value(const int val); //    -12345 ->  "-12,345"
	int  get_sc(void); // ���� �ð����� ��´�.

	bool  get_string_word(const char *pin, vector<string> *pout, const char *ptc, int ntc ); // ���ڿ�����  �ܾ�ڿ��� ��´�. 

	bool  comp_str(const char *psrc, const char *pds, int len); // ���ڿ� ��, ���ĺ��� ��� ��ҹ��� ���о��� ���Ѵ�. 
	char  *cnv_http_hexch(char ch); // char code -> "%xx"

	static  CUtil *UtilObj(void)
	{
		static  CUtil  obj;
		return &obj;
	}

};

#define   MYUTIL()   CUtil::UtilObj()