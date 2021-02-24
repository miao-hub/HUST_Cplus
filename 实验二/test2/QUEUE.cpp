#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <malloc.h>
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
extern const char* TestQUEUE(int& s);
int main()
{
	int s = 0;
	const char *c = TestQUEUE(s);
	cout << s << endl;
	cout << c << endl;
	return 0;
}

QUEUE::QUEUE(int m):max(m),elems(new int[m])//初始化队列
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
	*(int **)&q.elems = 0;
	*(int *)&q.max = 0;
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
	*(int **)&elems = (int *)malloc(sizeof(int)*q.max);
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
		delete []elems;
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
	int i, k, m, data;
	char c[1000];
	m = 0;
	for (i = head; i != tail; i++, i = i % max)
	{
		data = elems[i];
		for (k = 0;; k++)
		{
			c[k] = data % 10 + '0';
			data = data / 10;
			if (data == 0)
				break;
		}
		for (; k >= 0; k--, m++)
			s[m] = c[k];
		s[m++] = ' ';
	}
	s[m] = '\0';
	return s;
}

QUEUE::~QUEUE()//析构函数
{
	if (elems)
		delete elems;//先调用析构函数再释放空间
	*(int **)&elems = 0;
	*(int*)&max = 0;
	head = tail = 0;
}