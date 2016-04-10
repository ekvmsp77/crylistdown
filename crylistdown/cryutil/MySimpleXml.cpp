#include "StdAfx.h"

#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "MySimpleXml.h"

#define  MYSIMPLEXML_ATTNAME   256 // 문자열 최대길이, 이름


char att_name[MYSIMPLEXML_ATTNAME];
char att_attrib[MYSIMPLEXML_ATTNAME];
char att_val[MYSIMPLEXML_ATTNAME];

void  insert_attrib(int dep,CMySimpleXmlElement &ele, unsigned int data);
char *ps_string_value(char *psrc, char *pname, char *pvalue);
///////////////////////////////////////////////////////////////////////

void  insert_attrib(int dep,CMySimpleXmlElement &ele, unsigned int data)
{
	  const string tstr = ele.get_name();
      string  src;
	  char  *psrc;
	  int  len = tstr.length();
	  src.resize( len );
	  tstr._Copy_s( &src[0], len +1,len);

	  psrc = &src[0];
	  int n;
	  for(n=0; n < MYSIMPLEXML_ATTNAME && *psrc != ' ' ; n++ ){ 
		  att_name[n] = *psrc;
		  psrc++;
	  }
	  if (n >= MYSIMPLEXML_ATTNAME) --n;
	  att_name[n] = 0;



	  ele.set_name( att_name );
	  while(1){
		  psrc = ps_string_value(psrc, att_attrib, att_val);
		  if( psrc == 0 ) break;

		  ele.add_attrib(att_attrib,att_val);
	  }
}

/*
  ex)
   input:
      psrc :   ... version = "1.0" encoding...
   output:
	  pname :  "version"
	  pvalue:  "1.0"
	  return :  encoding...
*/
char *ps_string_value(char *psrc, char *pname, char *pvalue)
{
	int n;
	bool  ssw = false; // false : "..." ,  true : '...'

	while( *psrc == ' ') psrc++;
	if( *psrc == 0 ) return 0;


	for(n=0;;n++){
	    if( *psrc == 0 ) return 0;
		if( (*psrc >= 'a' && *psrc <= 'z') ||
			(*psrc >= 'A' && *psrc <= 'Z') ) pname[n] = *psrc++;
		else break;
	}
	pname[n] = 0;

	while( *psrc == ' ') psrc++;
	if( *psrc != '=') return 0;
	psrc++;
	while( *psrc == ' ') psrc++;
	if( *psrc == '"' ) ssw = false; // "..."
	else if( *psrc == 0x27 )  ssw = true; // '...'
	else return 0;
	psrc++;

	for( n=0;;n++){
		if( (ssw == false && *psrc == '"') || 
			(ssw == true  && *psrc == 0x27 ) ){
			psrc++;
			break;
		}
		if( *psrc == 0 ) return 0;
		*(pvalue+n) = *psrc++;
	}
	*(pvalue+n) = 0;

	return psrc;
}


///////////////////////////////////////////////////////////////////////

CMySimpleXmlElement::CMySimpleXmlElement()
{
}


CMySimpleXmlElement::~CMySimpleXmlElement()
{
}


void  CMySimpleXmlElement::add_attrib(const string &name, const string &val)
{
    t_mysimplexml_attrib  att;

	att.name = name;
	att.val = val;

	m_attrib.push_back( att );
}

void  CMySimpleXmlElement::del_attrib(const string &name)
{
	list<t_mysimplexml_attrib>::iterator  itr;

	for( itr = m_attrib.begin(); itr != m_attrib.end(); itr++){
		if(  (*itr).name.compare(name) == 0 ){
			m_attrib.erase(itr);
			break;
		}
	}
}

void  CMySimpleXmlElement::get_attrib(int num, string &name, string &val)
{
	list<t_mysimplexml_attrib>::iterator itr;
	int n=0;

	for( itr = m_attrib.begin(); itr != m_attrib.end(); itr++, n++ ){
		if( n == num ){
			name = (*itr).name;
			val  = (*itr).val;
			break;
		}
	}
}

void  CMySimpleXmlElement::add_child(const CMySimpleXmlElement &relement )
{
	m_child.push_back( relement );

}

void  CMySimpleXmlElement::del_child(const string name)
{
	_myxmlitr  itr;

	for( itr = m_child.begin(); itr != m_child.end(); itr++ ){
		if(  (*itr).m_name.compare(name) == 0 ){
			m_child.erase(itr);
			break;
		}
	}
}


///////////////////////////////////////////////////////////////////////

CMySimpleXml::CMySimpleXml()
{
	m_element_root.clear();

	// reset
	clear();

	CMySimpleXmlElement  root;
	root.set_name("root");
	m_element_root.push_back(root);

	m_exp_func = nullptr;
}


CMySimpleXml::~CMySimpleXml(void)
{
	clear();
}

void  CMySimpleXml::clear(void)
{
	m_element_root.clear();

	m_chcode = CHCODE_ASCII;
	m_version[0] = 0;
	m_standalone[0] = 0;
	m_encoding[0] = 0;
}


void  CMySimpleXml::exp_use_element(pf_mysimplexml_use func,  unsigned int  data)
{
	 list<CMySimpleXmlElement>::iterator  itr;

	 m_exp_func = func;

	 for(itr = m_element_root.begin(); itr != m_element_root.end(); itr++){
		 exp_element(0,itr, data);
	 }
}
/*
    element들을 탐색한다. 
	재귀호출, 

	void (*)(int,CMySimpleXmlElement &)
	int : 깊이 번호
	CMySimpleXmlElement & : 데이터
*/
void  CMySimpleXml::exp_element(int num, _myxmlitr  itr, unsigned int  data)
{
	_myxmlitr  r;

	m_exp_func(num, *itr, data);

	for( r = (*itr).child_begin(); r != (*itr).child_end(); r++ ){
		exp_element(num+1,r, data);
	}
}

//===========================================================================

/*
little-endian :FF FE
big-endian    :FE FF
UTF-8         :EF BB BF

BOM 정보가 없다면 기본은 CHCODE_ASCII이다.
*/
char *CMySimpleXml::check_src_bom(char *psrc)
{
	unsigned char c1,c2,c3;

	c1 = (unsigned char)*psrc;
	c2 = (unsigned char)*(psrc+1);
	c3 = (unsigned char)*(psrc+2);

	if( c1 == 0xFF && c2 == 0xFE ){ 
		m_chcode = CHCODE_UNI_L; 
		psrc+=2; 
	}
	else if( c1 == 0xFE && c2 == 0xFF ){ 
		m_chcode = CHCODE_UNI_B; 
		psrc+=2; 
	}
	else if( c1 == 0xEF && c2 == 0xBB && c3 == 0xBF  ){ 
		 m_chcode = CHCODE_UTF_8;
		 psrc+=3;
	}
	else m_chcode = CHCODE_ASCII; 

	return psrc;
}


/*
*/
bool  CMySimpleXml::parsing(char *psrc)
{
	_myxmlitr  itr;

	// xml소스가 없다면 에러
	if( *psrc == 0 )return false;

	// XML 문자코드값 체크
	psrc = check_src_bom(psrc);
	if( m_chcode != CHCODE_ASCII ) return false;

	// XML header
	psrc = ps_xml_header( psrc );
	if( psrc == 0 ) return false;

	// XML 본문
	//if( ps_xml_data(psrc) == 0 ) return false;
	ps_xml_data(m_element_root.begin(), psrc, false);

	// element 속성정보 얻기
	ps_xml_attrib();
	return true;
}

/* 
  XML 본문 처리
  재귀적으로 파싱해 나간다.

  itr : 부모 노드
  psrc: 현재 파싱할 소스 포인터( 위치)
  opt : 바로 전 파싱한 소스에서 
        <body> ... -> false
        <body size="10" ... /> -> true
		element가 <.../>이면 닫는 element ( </...> )가 있을 수도 있고, 없을 수도 있다.

 - 프로그램이 약간 꼬였음 -_-
*/
char *CMySimpleXml::ps_xml_data(list<CMySimpleXmlElement>::iterator itr, char *psrc, bool opt)
{
	static const char  *pcomm_start = "--";
	static const char  *pcomm_end = "-->";
	static const char  *pcdata_start = "[CDATA[";
	static const char  *pcdata_end = "]]>";
	static CMySimpleXmlElement  ele;

	list<CMySimpleXmlElement>::iterator  sub_itr;
	char *pstart,*pend;

	pstart = pend = 0;
	while(1){

		//비정상적인 데이터일경우, ex) <color> ... END 
		if( psrc == 0 ) return 0;

        switch( *psrc ){
			case 0: return 0;
			case ' ':
			case 0x0D: // CRLF
			case 0x0A: //
			case 0x09: // tab
				psrc++;
				break;
			case '<':
				psrc++;
				if( *psrc == '!' ){ 
					psrc++;
					if( strncmp(pcdata_start, psrc, strlen(pcdata_start)) == 0  ){ // <![CDATA[ ... ]]>, 데이터
						psrc += strlen(pcdata_start);

						pstart = psrc; // CDATA 데이터 시작
						while(1){
							if( *psrc == 0 ) return 0;
							if( strncmp(pcdata_end, psrc, strlen(pcdata_end)) == 0 ){
								pend = psrc-1; // 데이터 끝
								psrc += strlen(pcdata_end); 
								break; 
							}
							else psrc++;
						}
					}
					else  if( strncmp(pcomm_start, psrc, strlen(pcomm_start)) == 0  ){// <!-- 주석 -->
						psrc += strlen(pcomm_start);

						while(1){
							if( *psrc == 0 ) return 0;
							if( strncmp(pcomm_end, psrc, strlen(pcomm_end)) == 0 ){ 
								psrc += strlen(pcomm_end); 
								break; 
							}
							else psrc++;
						}
					}
					else { // 기타 데이터,  ~~~ '>'까지
						// <!-- ... --  '>'가 없으면 에러!
						while(1){
							if( psrc == 0 || *psrc == 0 ) return 0; 
							if( *psrc == '>' )break; 
							psrc++;
						}
						psrc++;
					}
				}
				else if( *psrc == '/' ){ //close element,   </color>
					if( opt == false ){
						//data set
						int size = pend - pstart;
						if( size > 0 ){
							string str;

							str.resize(size+1);
							str.insert(0,pstart,size+1);
							(*itr).set_data(str);
							//printf("%s\n", &str[0] ); //TEST , 데이터문자열 출력
						}
					} 

					while(1){
						if( *psrc == '>' ){ 
							psrc++; 
							return psrc; 
						}
						else if( *psrc == 0 ) return 0;
						else psrc++;
					}
					return 0;
				}
				else if( *psrc == '?'){ //  <?...?>, xml의 설정값?
					psrc++;

					pstart = psrc;
					for(int n=0; ; n++ ){
						if( *psrc == '?' && *(psrc+1) == '>' ){ // ... ?>
							psrc+=2;
							break;
						}
						pend = psrc;
						psrc++;
					}

					int size = pend - pstart+1;
					string str;
					str.resize(size);
					str.insert(0,pstart,size);

					ele.set_name( str );
					itr->add_child( ele );
					pstart = pend = 0;
					// 하위로 이동할 필요는 없다.
					break;
				}
				else {//start element,   <color>
					// <A /> <B> 이런 경우, <B>는 같은 부모의 새로운 element이다. 
					if( opt == true )	return psrc-1; // '<' ... 

					int n;
					bool endsw=false;
					bool sw = false;

					pstart = psrc;
					for( n=0; ; n++ ){

						if( *psrc == 0 ) return 0;
						if( *psrc == '>' ){ psrc++; break;}
						if( *psrc == '/' && *(psrc+1) == '>' ){ 
							//   <color  red = "ff0000" ... /> 
							// 데이터는 없고 element만 있는 경우
							psrc +=2; 
							endsw = true;
							break; 
						}

						pend = psrc;
						psrc++;
					}
					if( n == 0 ) return 0; // 이름이 없는 경우,

					// 이름에는 속성정보도 포함되어있다. 이것은 나중에 분리해준다.
					int size = pend - pstart+1;
					string str;
					str.resize(size);
					str.insert(0,pstart,size);

					ele.set_name( str );
					itr->add_child( ele ); //자식 element 추가!!

					if( endsw == true ){ 
						// <... /> 이면, 이어서 다음 element를 파싱한다. 
						 pstart = pend = 0;
						break;
					}

					sub_itr =  (*itr).child_end();
					sub_itr--;

					// <...> 다음에 바로 '<'가 오는지 검사한다.
				    // 그렇다면, 데이터는 없는 것이 된다.
					char *ptsrc = psrc; 
					sw = true;
					while( *ptsrc != '<' ){
						if( *ptsrc == 0x0D || *ptsrc == 0x0A || *ptsrc == ' ' ){ ptsrc++; continue; }
						else { sw = false; break; }
					}
					if( sw == true ) pstart = pend = 0;

					psrc  = ps_xml_data(sub_itr ,psrc, endsw);// 자식 element로 이동!
					if( psrc == 0 ) return 0;
				}
				break;
			default:
				{
					// element 데이터 포함
					if( pstart == 0 ) pstart = psrc;
					pend = psrc;
					psrc++;
				}
		}
	}

	return psrc;
}

/*
   element의 속성정보를 얻어낸다.
   element의 이름에서 얻어낸다.
*/
void  CMySimpleXml::ps_xml_attrib(void)
{
	exp_use_element( insert_attrib ,0);
}

/*
   ex) <?xml version="1.0" encoding="utf-8” standalone="yes" ?>
*/
char   *CMySimpleXml::ps_xml_header(char *psrc)
{
	const char *pxml = "xml ";
	const char *pver = "version";
	const char *penc = "encoding";
	const char *pstd = "standalone";
	char  name[MYSIMPLEXML_ATTNAME+1];
	char  val[MYSIMPLEXML_ATTNAME+1];
	int n;
    char *ptemp = psrc;

	while(1){
		if( *psrc == 0 ){
			// 헤더가 없는 경우는 그냥 넘어간다.
			return ptemp;
		}
		if( *psrc == ' ' || *psrc == 0x0D || *psrc == 0x0A || *psrc == 0x09 ){ psrc++; continue; }
		else if( *psrc == '<'  && *(psrc+1) == '?' ){ psrc+=2; break; }
		else psrc++;
	}
	
	if(  strncmp(psrc, pxml, strlen(pxml)) != 0 ) return 0;
	psrc += strlen(pxml);

	while( psrc != 0 ){
		while( *psrc == ' ') psrc++;
		if( psrc == 0 ||  *psrc == 0 ) return 0;
		if( *psrc == '?' && *(psrc+1) == '>' ) return psrc+2; // header end
 
		psrc = ps_string_value(psrc, name, val);

		if(  strncmp(name, pver, strlen(pver)) == 0 ){
			for( n=0; n < 4 && val[n] != 0 ; n++ ) m_version[n] = val[n];
			m_version[n] = 0;
		}
		else if(  strncmp(name, penc, strlen(penc)) == 0 ){
			for( n=0; n < 10 && val[n] != 0 ; n++ ) m_encoding[n] = val[n];
			m_encoding[n] = 0;
		}
		else if(  strncmp(name, pstd, strlen(pstd)) == 0 ){
			for( n=0; n < 4 && val[n] != 0 ; n++ ) m_standalone[n] = val[n];
			m_standalone[n] = 0;
		}
		else return false;
	}

	return  psrc;
}
//===========================================================================

