#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

class QUEUE
{
	int* const elems;//elems申请内存用于存放队列的元素
	const int max;//elems申请的最大元素个数为max
	int head, tail;//队列头head和尾tail，队空tail=head；初始head=tail=0；
public:
	QUEUE(int m);//初始化队列，最多申请m个元素
	QUEUE(const QUEUE& q);//用q深拷贝初始化队列
	QUEUE(QUEUE&& q)noexcept;//用q移动初始化队列
	virtual operator int()const noexcept;//返回队列的实际元素个数
	virtual int size()const noexcept;//返回队列申请的最大元素个数max
	virtual QUEUE& operator<<(int e);//将e入队列尾部，并返回当前队列
	virtual QUEUE& operator>>(int& e);//从队首出元素到e，并返回当前队列
	virtual QUEUE& operator=(const QUEUE& q);//深拷贝赋值并返回被赋值队列
	virtual QUEUE& operator=(QUEUE&& q)noexcept;//移动赋值并返回被赋值队列
	virtual char* print(char* s)const noexcept;//打印队列至s并返回s
	virtual ~QUEUE();//销毁当前队列
};
class STACK :public QUEUE
{
	QUEUE q;
public:
	STACK(int m);//初始化栈：最多存放2m-2个元素
	STACK(const STACK& s);//用栈s深拷贝初始化栈
	STACK(STACK&& s)noexcept;//用栈s移动拷贝初始化栈
	int size()const noexcept;//返回栈的容量即2m
	operator int()const noexcept;//返回栈的实际元素个数
	STACK& operator<<(int e);//将e入栈，并返回当前栈
	STACK& operator>>(int& e);//出栈到e，并返回当前栈
	STACK& operator=(const STACK& s);//深拷贝赋值并返回被赋值栈
	STACK& operator=(STACK&& s)noexcept;//移动赋值并返回被赋值栈
	char* print(char* b)const noexcept;//从栈底到栈顶打印栈元素
	~STACK()noexcept;//销毁栈
};
extern const char* TestSTACK(int& s);
int main()
{
	int s;
	const char* c = TestSTACK(s);
	cout << c << endl;
	cout << s << endl;
	return 0;
}
QUEUE::QUEUE(int m) :max(m), elems(new int[m])//初始化队列
{
	head = tail = 0;
}

QUEUE::QUEUE(const QUEUE& q) : max(q.max), elems(new int[q.max])//深拷贝初始化队列
{
	for (int i = q.head; i != q.tail; i++, i = i % q.max)
		*(int*)&elems[i] = q.elems[i];
	head = q.head;
	tail = q.tail;
}

QUEUE::QUEUE(QUEUE&& q)noexcept :max(q.max), elems(q.elems)//移动赋值初始化队列
{
	*(int**)&q.elems = 0;
	*(int*)&q.max = 0;
	head = q.head;
	q.head = 0;
	tail = q.tail;
	q.tail = 0;
}

QUEUE::operator int()const noexcept//返回队列中的元素个数
{
	if (head == tail)
		return 0;
	return (tail + max - head) % max;
}

int QUEUE::size()const noexcept//返回队列中的最大申请元素个数
{
	return max;
}

QUEUE& QUEUE::operator<<(int e)//入队操作
{
	if ((tail + 1) % max == head)
		throw "QUEUE is full!";
	elems[tail] = e;
	tail = (tail + 1) % max;
	return *this;
}

QUEUE& QUEUE::operator>>(int& e)//出队操作
{
	if (head == tail)
		throw "QUEUE is empty!";
	e = elems[head];
	head = (head + 1) % max;
	return *this;
}

QUEUE& QUEUE::operator=(const QUEUE& q)//深拷贝赋值
{
	if (this == &q)
		return *this;
	*(int**)&elems = (int*)malloc(sizeof(int) * q.max);
	for (int i = q.head; i != q.tail; i++, i = i % q.max)
		elems[i] = q.elems[i];
	*(int*)&max = q.max;
	head = q.head;
	tail = q.tail;
	return *this;
}

QUEUE& QUEUE::operator=(QUEUE&& q)noexcept//移动赋值
{
	if (this == &q)
		return *this;
	if (elems)
		delete[]elems;
	*(int**)&elems = q.elems;
	*(int**)&q.elems = 0;
	*(int*)&max = q.max;
	*(int*)&q.max = 0;
	head = q.head;
	q.head = 0;
	tail = q.tail;
	q.tail = 0;
	return *this;
}

char* QUEUE::print(char* s)const noexcept//打印队列中的元素到字符串中
{
	int i, j = 0, elem;
	for (i = head; i != tail; i++, i = i % max)
	{
		sprintf(s + j, "%d ", elems[i]);
		elem = elems[i];
		while (elem)
		{
			j++;
			elem = elem / 10;
		};
		j++;
	}
	s[j] = 0;
	return s;
}

QUEUE::~QUEUE()//析构函数
{
	if (elems)
		delete elems;//先调用析构函数再释放空间
	*(int**)&elems = 0;
	*(int*)&max = 0;
	head = tail = 0;
}
STACK::STACK(int m):q(m),QUEUE(m)//初始化栈：最多存放2m-2个元素
{

}

STACK::STACK(const STACK& s):q(s.q),QUEUE(s)//用栈s深拷贝初始化栈
{

}

STACK::STACK(STACK&& s)noexcept :q(move(s.q)), QUEUE(move(s))//用栈s移动拷贝初始化栈
{

}

int STACK::size()const noexcept//返回栈的容量即2m
{
	return (q.size() + QUEUE::size());
}

STACK::operator int()const noexcept//返回栈的实际元素个数
{
	return (int(q) + QUEUE::operator int());
}

STACK& STACK::operator<<(int e)//将e入栈，并返回当前栈
{
	if (QUEUE::operator int() < QUEUE::size() - 1)
		QUEUE::operator<<(e);
	else if (int(q) == q.size() - 1)
		throw "STACK is full!";
	else
		q << e;
	return *this;
}

STACK& STACK::operator>>(int& e)//出栈到e，并返回当前栈
{
	int i;
	if (int(q))
	{
		for (i = 0; i<int(q) - 1; i++)
		{
			q >> e;
			q << e;
		}
		q >> e;
	}
	else if (QUEUE::operator int())
	{
		for (i = 0; i < QUEUE::operator int() - 1; i++)
		{
			QUEUE::operator>>(e);
			QUEUE::operator<<(e);
		}
		QUEUE::operator>>(e);
	}
	else
		throw "STACK is empty!";
	return *this;
}

STACK& STACK::operator=(const STACK& s)//深拷贝赋值并返回被赋值栈
{
	if (this == &s)
		return *this;
	q = s.q;
	*(QUEUE*)this = s;
	return *this;
}

STACK& STACK::operator=(STACK&& s)noexcept//移动赋值并返回被赋值栈
{
	if (this == &s)
		return *this;
	q = (QUEUE&&)s.q;
	*(QUEUE*)this = (QUEUE&&)s;
	return *this;
}
char* STACK::print(char* b)const noexcept//从栈底到栈顶打印栈元素
{
	char* b1=new char[1000];
	q.print(b1);
	QUEUE::print(b);
	if (b1)
	{
		strcat(b, " \0");
		strcat(b, b1);
	}
	return b;
}

STACK::~STACK()noexcept//销毁栈
{

}