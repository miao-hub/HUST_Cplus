#define _CRT_SECURE_NO_WARNINGS 
#include <iomanip> 
#include <exception>
#include <typeinfo>
#include <string.h>
#include <iostream>
using namespace std;
template <typename T>
class MAT {
	T* const e;							//指向所有整型矩阵元素的指针
	const int r, c;						//矩阵的行r和列c大小
public:
	MAT(int r, int c);				//矩阵定义
	MAT(const MAT& a);				//深拷贝构造
	MAT(MAT&& a)noexcept;			//移动构造
	virtual ~MAT()noexcept;
	virtual T* const operator[ ](int r);//取矩阵r行的第一个元素地址，r越界抛异常
	virtual MAT operator+(const MAT& a)const;	//矩阵加法，不能加抛异常
	virtual MAT operator-(const MAT& a)const;	//矩阵减法，不能减抛异常
	virtual MAT operator*(const MAT& a)const;	//矩阵乘法，不能乘抛异常
	virtual MAT operator~()const;					//矩阵转置
	virtual MAT& operator=(const MAT& a);		//深拷贝赋值运算
	virtual MAT& operator=(MAT&& a)noexcept;	//移动赋值运算
	virtual MAT& operator+=(const MAT& a);		//“+=”运算
	virtual MAT& operator-=(const MAT& a);		//“-=”运算
	virtual MAT& operator*=(const MAT& a);		//“*=”运算
//print输出至s并返回s：列用空格隔开，行用回车结束
	virtual char* print(char* s)const noexcept;
};

template MAT<int>;			//用于实验四
template MAT<long long>;		//用于实验四
extern const char* TestMAT(int & s);	//用于实验四
int main()
{
	int s1=0;
	const char* c1 = TestMAT(s1);
	cout << c1 << endl;
	cout << s1 << endl;
	return 0;	
}

template <typename T>
MAT<T>::MAT(int r, int c):r(r),c(c),e(new T[r*c])//矩阵定义
{
	for (int m = 0; m < r; m++)
		for (int n = 0; n < c; n++)
			e[m * c + n] = 0;
}

template <typename T>
MAT<T>::MAT(const MAT& a):r(a.r),c(a.c),e(new T[a.r*a.c]) //深拷贝构造
{
	for (int m = 0; m < r; m++)
		for (int n = 0; n < c; n++)
			e[m*c+n] = a.e[m*c+n];
}

template <typename T>
MAT<T >::MAT(MAT&& a)noexcept:r(a.r),c(a.c),e(a.e)//移动构造
{
	*(int *)&a.r = 0;
	*(int *)&a.c = 0;
	*(T **)&a.e = 0;
}

template <typename T>
MAT<T>:: ~MAT()noexcept
{
	if (e)
	{
		delete e;
		*(T **)&e = 0;
	}
	*(int *)&r = 0;
	*(int *)&c = 0;
}

template <typename T>
T* const MAT<T>::operator[ ](int r)//取矩阵r行的第一个元素地址，r越界抛异常
{
	if (r >= (this->r) || r < 0)
		throw "wrong!";
	return &e[r*c];
}

template <typename T>
MAT<T> MAT<T>::operator+(const MAT& a)const	//矩阵加法，不能加抛异常
{
	if (!(r == a.r && c == a.c))
		throw "wrong!";
	MAT<T> *b=new MAT<T>(r, c);
	for (int m=0; m < r; m++)
		for (int n=0; n < c; n++)
			(*b)[m][n] = e[m*c+n] + a.e[m*c+n];
	return *b;
}

template <typename T>
MAT<T> MAT<T>::operator-(const MAT& a)const	//矩阵减法，不能减抛异常
{
	if (!(r == a.r && c == a.c))
		throw "wrong!";
	MAT<T> *b=new MAT<T>(r, c);
	for (int m=0; m < r; m++)
		for (int n=0; n < c; n++)
			(*b)[m][n] = e[m*c+n] - a.e[m*c+n];
	return *b;
}

template <typename T>
MAT<T> MAT<T>::operator*(const MAT& a)const	//矩阵乘法，不能乘抛异常
{
	if (c != a.r)
		throw "wrong!";
	MAT<T> *b=new MAT<T>(r, a.c);
	for (int i = 0; i < r; i++)
		for (int j = 0; j < a.c; j++)
			for (int n = 0; n < c; n++)
				(*b)[i][j] += e[i*c+n] * a.e[n*a.c+j];
	return *b;
}

template <typename T>
MAT<T> MAT<T>:: operator~()const					//矩阵转置
{
	MAT<T> *a=new MAT<T>(c, r);
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			(*a)[j][i] = e[i*c+j];
	return *a;
}

template <typename T>
MAT<T>& MAT<T>::operator=(const MAT& a)		//深拷贝赋值运算
{
	if (this == &a)
		return *this;
	if (!(r == a.r && c == a.c))
		throw "wrong!";
	*(int *)&r = a.r;
	*(int *)&c = a.c;
	if (e)
		delete []e;
	*(T **)&e = new T[r * c];
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			e[i*c+j] = a.e[i*c+j];
	return *this;
}

template <typename T>
MAT<T>& MAT<T>::operator=(MAT&& a)noexcept	//移动赋值运算
{
	if (this == &a)
		return *this;
	if (e)
		delete []e;
	*(int *)&r = a.r;
	*(int *)&a.r = 0;
	*(int *)&c = a.c;
	*(int *)&a.c = 0;
	*(T **)&e = a.e;
	*(T **)&a.e = 0;
	return *this;
}

template <typename T>
MAT<T>& MAT<T>::operator+=(const MAT& a)		//“+=”运算
{
	if (!(r == a.r && c == a.c))
		throw "wrong!";
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			e[i * c + j] += a.e[i * c + j];
	return *this;
}

template <typename T>
MAT<T>& MAT<T>::operator-=(const MAT& a)		//“-=”运算
{
	if (!(r == a.r && c == a.c))
		throw "wrong!";
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			e[i*c+j] -= a.e[i*c+j];
	return *this;
}

template <typename T>
MAT<T>& MAT<T>::operator*=(const MAT& a)		//“*=”运算
{
	if (c!=a.r)
		throw "wrong!";
	*this = *this * a;
	return *this;
}

template <typename T>
char* MAT<T>::print(char* s)const noexcept
{
	int m = 0;
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
			if (!strcmp(typeid(e[0]).name(), "int"))
			{
				sprintf(s + m, "%6d ", e[i * c + j]);
				m += 7;
			}
			else if (!strcmp(typeid(e[0]).name(), "long long"))
			{
				sprintf(s + m, "%6ld ", e[i * c + j]);
				m += 7;
			}
			else if(!strcmp(typeid(e[0]).name(),"double"))
			{
				sprintf(s + m, "%8lf ", e[i * c + j]);
				m += 9;
			}
		s[m-1] = '\n';
	}
	s[m] = '\0';
	return s;
}
