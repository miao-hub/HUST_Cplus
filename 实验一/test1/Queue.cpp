#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <iostream>
using namespace std;
struct Queue {
	int* const elems;
	const int max;
	int head, tail;
};
void initQueue(Queue* const p, int m);//初始化p指队列：最多申请m个元素
void initQueue(Queue* const p, const Queue& s);//用s深拷贝初始化p指队列
void initQueue(Queue* const p, Queue&& s);//用s移动初始化p指队列
int number(const Queue* const p);//返回p指队列的实际元素个数
int size(const Queue* const p);//返回p指队列申请的最大元素个数
Queue* const enter(Queue* const p, int e);//将e入队列尾部，并且返回p
Queue* const leave(Queue* const p, int& e);//从队首出元素到e，并且返回p
Queue* const assign(Queue* const p, const Queue &q);//深拷贝赋s给队列并返回p
Queue* const assign(Queue* const p, Queue&& q);//移动赋s给队列并返回p
char* print(const Queue* const p, char* s);//打印p指队列至s的尾部，并返回s
void destroyQueue(Queue* const p);//销毁p指向的队列
extern const char* TestQueue(int& s);
int main()
{
	int s = 0;
	const char* c = TestQueue(s);
	cout << c<<endl;
	cout << s;
	return 0;
}
void initQueue(Queue* const p, int m)//初始化p指队列：最多申请m个元素
{
	*(int *)&(p->max) = m;
	*(int**)&(p->elems) = (int*)malloc(sizeof(int) * m);
	p->head = 0;
	p->tail = 0;
}

void initQueue(Queue* const p, const Queue& s)//用s深拷贝初始化p指队列
{
	if (p != &s)
	{
		if (p->elems != NULL)
			free(p->elems);
		*(int*)&(p->max) = s.max;
		p->head = s.head;
		p->tail = s.tail;
		*(int**)&(p->elems) = (int*)malloc(sizeof(int) * (p->max));
		for (int i = s.head; (i % s.max) != s.tail; i++)
			*(int*)&(p->elems[i % (p->max)]) = s.elems[i % (p->max)];
	}
}

void initQueue(Queue* const p, Queue&& s)//用s移动初始化p指队列
{
	if (p->elems != NULL)
		free(p->elems);
	p->head = s.head;
	s.head = 0;
	*(int *)&(p->max) = s.max;
	*(int *)&s.max = 0;
	p->tail = s.tail;
	s.tail = 0;
	*(int *)&(p->elems) = *(int *)&s.elems;
	*(int*)&s.elems = NULL;
}

int number(const Queue* const p)//返回p指队列的实际元素个数
{
	if (!p->max) return 0;
	return (p->tail + p->max - p->head) % (p->max);
}

int size(const Queue* const p)//返回p指队列申请的最大元素个数
{
	return p->max;
}

Queue* const enter(Queue* const p, int e)//将e入队列尾部，并且返回p
{
	if ((p->tail + 1) % p->max == p->head)
		throw "Queue is full!";
	p->elems[p->tail] = e;
	p->tail = (p->tail + 1) % p->max;
	return p;
}

Queue* const leave(Queue* const p, int& e)//从队首出元素到e，并且返回p
{
	if (p->head == p->tail)
		throw "Queue is empty!";
	e = p->elems[p->head];
	p->head = (p->head + 1) % p->max;
	return p;
}

Queue* const assign(Queue* const p,const Queue&q)//深拷贝赋q给队列并返回p
{
	if (p == &q)
		return p;
	if (p->elems != NULL)
		free(p->elems);
	*(int*)&(p->max) = q.max;
	p->head = q.head;
	p->tail = q.tail;
	*(int**)&(p->elems) = (int*)malloc(sizeof(int) * (p->max));
	for (int i = p->head; i % (p->max) != p->tail; i++)
		p->elems[i%(p->max)] = q.elems[i % (p->max)];
	return p;
}

Queue* const assign(Queue* const p, Queue&& q)//移动赋s给队列并返回p
{
	if (p == &q)
		return p;
	if (p->elems != NULL)
		free(p->elems);
	p->head = q.head;
	q.head = 0;
	p->tail = q.tail;
	q.tail = 0;
	*(int*)&(p->max) = *(int*)&q.max;
	*(int*)&(p->elems) = *(int*)&q.elems;
	*(int*)&q.max = 0;
	*(int**)&q.elems = 0;
	return p;
}

char* print(const Queue* const p, char* s)//打印p指队列至s的尾部，并返回s;
{
	int i,j = 0,elems;
	for (i = p->head; i != p->tail; i++,i=i%p->max)
	{
		sprintf(s+j, "%d ", p->elems[i]);
		elems = p->elems[i];
		while (elems)
		{
			j++;
			elems = elems / 10;
		};
		j++;
	}
	s[j] = 0;
	return s;
}

/*
char* print(const Queue* const p, char* s)//打印p指队列至s的尾部，并返回s;
{
	int i, k, m, elem;
	char c[1000];
	m = 0;
	for (i = p->head; i % (p->max) != p->tail; i++,i=i%(p->max))
	{
		elem = p->elems[i];
		for (k = 0;; k++)
		{
			c[k] = elem % 10 + '0';
			elem = elem / 10;
			if (elem == 0)
				break;
		}
		for (; k >= 0; k--, m++)
			s[m] = c[k];
		s[m++] = ' ';

	}
	s[m] = '\0';
	return s;
}
*/
void destroyQueue(Queue* const p)//销毁p指向的队列
{
	if (p->elems != NULL)
		free(p->elems);
	*(int**)&p->elems = 0;
	*(int*)&p->max = 0;
	p->head = 0;
	p->tail = 0;
}