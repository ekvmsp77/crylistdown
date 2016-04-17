/*
<?xml version="1.0" encoding="utf-8" ?>
<wid>
	<header>
		<title>���� �����ְ�����</title>
		<tm>201106051800</tm>
		<wf><![CDATA[������ ������ ��а��� �������� 9�Ͽ��� �ߺ�����(�������� ����)�� �� ���ڽ��ϴ�. ���ֵ��� �������� �������� 7�� ������ �ѵ����� �� ���ڸ�, 10���� ���ֵ������ػ��� ������ ��а��� �������� �� ���ڽ��ϴ�. �� ���� ���� ������ �����ڸ��� ��� ���� ������ ���ڽ��ϴ�. <br /><br />����� ���(�������: 13~18��, �ְ���: 22~28��)�� ����ϰڽ��ϴ�.<br /><br />�������� ���ֵ��� �ߺ�����(������������)�� ���(������ : 1~7mm)�� ����ϰų� ���� ���ڰ�, �� ���� ������ ��⺸�� ���ڽ��ϴ�.]]></wf>
	</header>

	<body>
		<location city="11B10101" province="11B00000">
				<province code="11B00000">�������õ����⵵</province>
				<city code="11B10101">����</city>

				<data>
					<numEf>2</numEf>
					<tmEf>2011-06-07</tmEf>
					<wf>��������</wf>
					<tmn>18</tmn>
					<tmx>28</tmx>
					<reliability>����</reliability>
				</data>
		</location>
	</body>
</wid>


��� (element)
? ��Ҵ� ���� �±׿� �� �±׷� ������ ���� ���� �����̴�.
? �±� �̸��� ��� Ÿ�� �̸��� ���Ǿ��̴�.
? ����� ����: ���� �±� ��� ���� �� �±�
? ���� �±� ����: <�±� �̸� �Ӽ� �̸�="��" �Ӽ� �̸�="��" ...>
��) <termdef id="dt-dog" term="dog">
? �� �±� ����: </�±� �̸�>

�� �±� ����: <�±� �̸�     �Ӽ� �̸�="��"      �Ӽ� �̸�="��" ...>
: ������ ���� ��Ҹ� ��Ÿ����, <�±� �̸� ...></�±� �̸�>�� ������ �ǹ̸� ���´�.
? HTML���� �޸� ��ҹ��ڸ� �����Ѵ�.
�Ӽ� ���� `<'�� ������ ���ڷ� �̷������.
��� ������ �ڽ� ���, ���� ����Ÿ, CDATA ���ǵ�� �̷������.
��ȿ�� ������ ����� ���� Ÿ�� ���ǳ��� �±׸��� ����ؾ� �ϸ� �� ������ ������Ѵ�.

���� ����Ÿ
? `<', `&' ���ڴ� ��ũ���� ���ǹǷ� ���� ǥ���� �� ����.
? �̵� ���ڴ� ���� �� ��ü�� ��Ÿ���� ���ؼ��� ���� ������ ��ü ���� ``<'', ``&''�� ����ؾ� �Ѵ�.

�ּ�
? ����: <!--'' ���ڿ� ``-->
? `���ڿ�'���� ``--''�� �� �� ����.
? ��ũ�� ǥ���̿��� ��� �������� ��Ÿ�� �� �ִ�.

CDATA ����
? ����: <![CDATA[ ���ڿ� ]]>
��) <![CDATA[<greeting>Hello, world!</greeting>]]>
? ��ũ�� ǥ���� �����ϴ� ���ڿ��� �ؼ����� �ʰ� �Ϲ� ���� ����Ÿ�μ� ǥ���ϱ� ���� ����Ѵ�.
? ��� ���뿡���� ��Ÿ�� �� �ִ�.


BOM:  
�̰Ϳ� ���� ó���� �Ǿ� ���� �ʴ�.
�ٸ� ���ڼ��̶�� ��� �ؾ� �ұ...
(��, ��°�� ��ȯ�ѵڿ� ó���ұ�?  �̰� �����ϰڳ�, �ð��� �� �ɸ��°� ��������)

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
	void  get_attrib(int num, string &name, string &val); // num��° ������ ���
	size_t  get_attrib_size(void) { return  m_attrib.size(); } // ���� ����

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
	 char  m_encoding[10]; // "utf-8" - element ������ ���ڼ�
	 char  m_standalone[4]; // "yes" or "no" 

	 // xml  tree
	 list<CMySimpleXmlElement>    m_element_root;
	 pf_mysimplexml_use   m_exp_func;

	 void  exp_element(int num, _myxmlitr  itr, unsigned int  data);

	 // xml �ҽ� ���ڼ�
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

	// xml ������ ����
	list<CMySimpleXmlElement>::iterator get_root_element(void){ return m_element_root.begin();  }
	// xml ��� Ž��
	void  exp_use_element(pf_mysimplexml_use func , unsigned int data);

	// xml string �Ľ�
	bool  parsing(char *psrc);  //  true�̸� ����, return���� ����� �Ǿ� ���� �ʴ�.
	void  clear(void);    

	void  get_encoding(char *pec) { for(int n=0; m_encoding[n] != 0; n++ )*(pec+n) = m_encoding[n]; }
	void  get_ver(char *pvar) { for(int n=0; m_version[n] != 0; n++ )*(pvar+n) = m_version[n]; }

	void  test_print_xml_node(int ntab,int nid);
};

