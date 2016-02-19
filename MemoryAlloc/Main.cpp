//#include "MemoryPool.h"
#include <vector>
class AAA
{
public:
	AAA(int i)
	{
		m = i;
		b = 'a';
	}
private:
	int m;
	char b;
};

int main()
{
	AAA* p1 = new AAA(1);
	AAA* p2 = new AAA(2);
	AAA* p3 = new AAA(3);
	delete p3;
	delete p2;
	delete p1;
	return 0;
}