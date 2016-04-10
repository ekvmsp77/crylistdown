/*
    *�����ڵ� ��ȯ �Լ�*
	- API�Լ��� ������, �׳� ���� ���� ����
	- Ansi, UTP-8 �����ڵ尪�� ���� ���̰� �������̴�. 
	- Ansi�ڵ�� 1����Ʈ�����ڵ��̴�. �ѱ��ϰ�� 2���� ����Ʈ�� �� ���ڸ� �̷��.
	- shift����� ��ȣ��Ʈ������ ��� unsigned������ �޴´�.
	- �� ����/���ڿ� �ΰ��� ������ �Լ��� �����Ѵ�.
	- pout == null�� ��� ��ȯ�� ���ڿ��� ����(byte)���� ���ϵȴ�.

	
	= Unicode���� Ansi�� ��ȯ�Ҷ����� ��� �ұ?  -_-
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
