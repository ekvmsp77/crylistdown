/*
    *문자코드 변환 함수*
	- API함수가 있지만, 그냥 내가 직접 만듬
	- Ansi, UTP-8 문자코드값에 따라 길이가 가변적이다. 
	- Ansi코드는 1바이트단위코드이다. 한글일경우 2개의 바이트가 한 문자를 이룬다.
	- shift연산시 부호비트문제로 모두 unsigned형으로 받는다.
	- 한 문자/문자열 두가지 버전의 함수를 제공한다.
	- pout == null일 경우 변환된 문자열의 길이(byte)값이 리턴된다.

	
	= Unicode에서 Ansi로 변환할때에는 어떻게 할까나?  -_-
*/


// Ansi => Unicode
unsigned short  myAnsiToUniCode(unsigned char c1,unsigned char c2); 
int  myAnsiToUniCodeStr(const unsigned char *pansi, unsigned short *pout);

// Unicode => UTF-8
void  myUniCodeToUTF8(unsigned short uni, char *putf);
int  myUniCodeToUTF8Str(const unsigned short *puni, char *pout);

// UTF-8  =>  Unicode
unsigned short  myUTF8ToUniCode(const unsigned char *putf);
int  myUTF8ToUniCodeStr(const unsigned char *putf, unsigned short *pout);
