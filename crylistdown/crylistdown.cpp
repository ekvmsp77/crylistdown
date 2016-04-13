

#include "stdafx.h"
#include "cryutil\Util.h"
#include "cryutil\Log.h"
#include "crynet\downproc.h"

void  downfunc(unsigned int num);


void  downfunc(unsigned int num)
{
	unsigned int  dsize = CMySocket::GetMySocket()->table_getdownsize(num);
	char *pdata;

	if (dsize == 0) {
		printf("down size == 0\n");
		return;
	}
	printf("down size : %d\n", dsize);

 //	CMySocket::GetMySocket()->table_getdata(num, pdata, dsize);

}


int main()
{
	CDownProc  *pdown;

	pdown = CDownProc::GetObj();
	if (pdown->open_downinfo("uuu1.txt", downfunc) == true) {
		if (pdown->down_info("종목리스트") == true) {
			printf("down start\n");
			do {
				pdown->timer();

				if (pdown->check_objdown() == 0) break;
			} while (1);
		}
		else printf("down error\n");
	}
	else printf("open error\n");



    return 0;
}

