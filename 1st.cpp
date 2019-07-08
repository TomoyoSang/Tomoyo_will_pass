#include "inst_class.h"

InterMem im;

int main()
{
	freopen("pi.data", "r", stdin);
	im.readin();

	int cnt = 1;
	while (1)
	{
		Fet.Fecth(im);
		inst* p = my_dec.Decode();
		if (flag)
		{
			p->EX();
			p->MA(im);
			p->WB();
		}
		else
		{
			cout << (reg.Register[10] & 255u);
			break;
		}
		
	
		/*cout << cnt << " ";
		cout<< PC-4 << " ";
		for (int i = 1; i <= 31; ++i)
			cout << reg.Register[i]<<" ";
		cout <<p->elem<< endl;
		++cnt;*/
		delete p;
	}
	return 0;
}
