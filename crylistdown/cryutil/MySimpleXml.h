/*
<?xml version="1.0" encoding="utf-8" ?>
<wid>
	<header>
		<title>전국 육상주간예보</title>
		<tm>201106051800</tm>
		<wf><![CDATA[북쪽을 지나는 기압골의 영향으로 9일에는 중부지방(강원영동 제외)에 비가 오겠습니다. 제주도는 지형적인 영향으로 7일 오전에 한두차례 비가 오겠며, 10일은 제주도남쪽해상을 지나는 기압골의 영향으로 비가 오겠습니다. 그 밖의 날은 고기압의 가장자리에 들어 가끔 구름이 많겠습니다. <br /><br />기온은 평년(최저기온: 13~18도, 최고기온: 22~28도)과 비슷하겠습니다.<br /><br />강수량은 제주도와 중부지방(강원영동제외)은 평년(강수량 : 1~7mm)과 비슷하거나 조금 많겠고, 그 밖의 지방은 평년보다 적겠습니다.]]></wf>
	</header>

	<body>
		<location city="11B10101" province="11B00000">
				<province code="11B00000">서울ㆍ인천ㆍ경기도</province>
				<city code="11B10101">서울</city>

				<data>
					<numEf>2</numEf>
					<tmEf>2011-06-07</tmEf>
					<wf>구름조금</wf>
					<tmn>18</tmn>
					<tmx>28</tmx>
					<reliability>높음</reliability>
				</data>
		</location>
	</body>
</wid>


요소 (element)
? 요소는 시작 태그와 끝 태그로 묶여진 논리적 문서 단위이다.
? 태그 이름과 요소 타입 이름은 동의어이다.
? 요소의 문법: 시작 태그 요소 내용 끝 태그
? 시작 태그 문법: <태그 이름 속성 이름="값" 속성 이름="값" ...>
예) <termdef id="dt-dog" term="dog">
? 끝 태그 문법: </태그 이름>

빈 태그 문법: <태그 이름     속성 이름="값"      속성 이름="값" ...>
: 내용이 없는 요소를 나타내며, <태그 이름 ...></태그 이름>과 동일한 의미를 갖는다.
? HTML과는 달리 대소문자를 구분한다.
속성 값은 `<'를 제외한 문자로 이루어진다.
요소 내용은 자식 요소, 문자 데이타, CDATA 섹션들로 이루어진다.
유효한 문서는 선언된 문서 타입 정의내의 태그만을 사용해야 하며 그 문법을 따라야한다.

문자 데이타
? `<', `&' 문자는 마크업에 사용되므로 직접 표현될 수 없다.
? 이들 문자는 문자 그 자체를 나타내기 위해서는 문자 참조나 개체 참조 ``<'', ``&''를 사용해야 한다.

주석
? 문법: <!--'' 문자열 ``-->
? `문자열'에는 ``--''가 올 수 없다.
? 마크업 표식이외의 어느 곳에서도 나타날 수 있다.

CDATA 섹션
? 문법: <![CDATA[ 문자열 ]]>
예) <![CDATA[<greeting>Hello, world!</greeting>]]>
? 마크업 표식을 포함하는 문자열을 해석하지 않고 일반 문자 데이타로서 표현하기 위해 사용한다.
? 요소 내용에서만 나타날 수 있다.


BOM:  
이것에 대한 처리는 되어 있지 않다.
다른 문자셋이라면 어떻게 해야 할까나...
(흠, 통째로 변환한뒤에 처리할까?  이게 속편하겠네, 시간이 좀 걸리는게 문제지만)

little-endian :FF FE
big-endian    :FE FF
UTF-8         :EF BB BF
----------------------------------------------------------------

     root -> e1 -> e11
	               e12 -> e121 -> e1211
				   e13
				   e14 -> e141
				          e142	
*/

#pragma once

#include <string>
#include <list>
#include <vector>

using namespace std;


class CMySimpleXmlElement;

typedef  list<CMySimpleXmlElement>::iterator   _myxmlitr;
// ex)  <body  color = "FF00F0" size = 120 > 
//    1.  "color" , "FF00F0"
//    2.  "size"  , "120"
typedef struct {
	string   name;
	string   val;
}t_mysimplexml_attrib;


class  CMySimpleXmlElement
{
	string   m_name;   // ex)  <body> car </body>  : "body"
	string   m_data;   // ex)  <body> car </body>  : "car"

	list<t_mysimplexml_attrib>   m_attrib; 
	list<CMySimpleXmlElement>    m_child;

public:
	CMySimpleXmlElement();
	~CMySimpleXmlElement();

	void  set_name(const string &name) { m_name = name; }
	void  set_data(const string &data) { m_data = data; }
	const  string get_name(void) { return m_name; }
	const  string get_data(void) { return m_data; }
	
	void  add_attrib(const string &name, const string &val);
	void  del_attrib(const string &name);
	void  get_attrib(int num, string &name, string &val); // num번째 데이터 얻기
	size_t  get_attrib_size(void) { return  m_attrib.size(); } // 갯수 리턴

	void  add_child(const CMySimpleXmlElement &relement );
	void  del_child(const string name);
	size_t  get_child_size(void) { return  m_child.size(); }

	list<CMySimpleXmlElement>::iterator  child_begin(void) { return m_child.begin(); }
	list<CMySimpleXmlElement>::iterator  child_end(void) { return m_child.end(); }
};


typedef   void (*pf_mysimplexml_use)(int,CMySimpleXmlElement &, unsigned int );

class CMySimpleXml
{
	// - xml header -
     char  m_version[4]; // "1.0"
	 char  m_encoding[10]; // "utf-8" - element 데이터 문자셋
	 char  m_standalone[4]; // "yes" or "no" 

	 // xml  tree
	 list<CMySimpleXmlElement>    m_element_root;
	 pf_mysimplexml_use   m_exp_func;

	 void  exp_element(int num, _myxmlitr  itr, unsigned int  data);

	 // xml 소스 문자셋
	 enum   e_myxml_chcode{
		 CHCODE_ASCII,//default
		 CHCODE_UNI_L,
		 CHCODE_UNI_B,
		 CHCODE_UTF_8
	 };
	 // BOM check
	 e_myxml_chcode  m_chcode;
	 char *check_src_bom(char *psrc);

	 // parsing
	 char *ps_xml_header(char *psrc);
	 char *ps_xml_data(list<CMySimpleXmlElement>::iterator itr, char *psrc, bool opt);
	 void  ps_xml_attrib(void);
public:
	CMySimpleXml();
	~CMySimpleXml(void);

	// xml 데이터 관리
	list<CMySimpleXmlElement>::iterator get_root_element(void){ return m_element_root.begin();  }
	// xml 노드 탐색
	void  exp_use_element(pf_mysimplexml_use func , unsigned int data);

	// xml string 파싱
	bool  parsing(char *psrc);  //  true이면 정상, return값이 제대로 되어 있지 않다.
	void  clear(void);    

	void  get_encoding(char *pec) { for(int n=0; m_encoding[n] != 0; n++ )*(pec+n) = m_encoding[n]; }
	void  get_ver(char *pvar) { for(int n=0; m_version[n] != 0; n++ )*(pvar+n) = m_version[n]; }

	void  test_print_xml_node(int ntab,int nid);
};

