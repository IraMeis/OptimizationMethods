#pragma once
#include <vector>
#include <cassert>
#include <iostream>

typedef  std::vector<double> vec_n;
typedef double(*func_n)(const vec_n&);

template<typename T>
static bool operator == (const std::vector<T> & a, const std::vector<T> & b)
{
	if (a.size() != b.size())
	{
		return false;
	}
	for (int i = 0;i < a.size();i++) 
	{
		if (a[i] != b[i])
		{
			return false;
		}
	}
	return true;
}

static vec_n operator+(const vec_n& a, const vec_n& b)
{
	if (a.size() != b.size())
	{
		throw std::runtime_error("error :: operator+ :: vectors of different dimensions");
	}
	vec_n res(a);
	for (int i = 0; i < a.size(); i++)
	{
		res[i] += b[i];
	}
	return res;
}

static vec_n operator+(const vec_n& a, const double x)
{
	vec_n res(a);
	for (int i = 0; i < a.size(); i++)
	{
		res[i] += x;
	}
	return res;
}

static vec_n operator+(const double x, const vec_n& a)
{
	return a + x;
}

static vec_n operator-(const vec_n& a, const vec_n& b)
{
	if (a.size() != b.size())
	{
		throw std::runtime_error("error :: operator- :: vectors of different dimensions");
	}
	vec_n res(a);
	for (int i = 0; i < a.size(); i++)
	{
		res[i] -= b[i];
	}
	return res;
}

static vec_n operator-(const vec_n& a, const double b)
{
	vec_n res(a);
	for (int i = 0; i < a.size(); i++)
	{
		res[i] -= b;
	}
	return res;
}

static vec_n operator-(const double b, const vec_n& a)
{
	vec_n res; res.resize(a.size());
	for (int i = 0; i < a.size(); i++)
	{
		res[i] = b - a[i];
	}
	return res;
}

static vec_n operator*(const vec_n& a, const double  b)
{
	vec_n res(a);
	for (int i = 0; i < a.size(); i++)
	{
		res[i] *= b;
	}
	return res;
}

static vec_n operator*(const double  b, const vec_n& a)
{
	return a * b;
}

static double dot(const vec_n& a, const vec_n& b) 
{
	if (a.size()!=b.size())
	{
		throw std::runtime_error("error :: dot product :: vectors of different dimensions");
	}
	double res = 0.0;

	for (int i = 0; i < a.size(); i++)
	{
		res += a[i] * b[i];
	}
	return res;
}
// ����� ���������� ��� ������ vec_n � ����� ostream
static std::ostream& operator<<(std::ostream& steram, const vec_n& v)
{
	steram << "{ ";
	for (int i = 0; i < v.size() - 1; i++)
	{
		steram << v[i] << ", ";
	}
	steram << v[v.size() - 1];

	steram << " }";

	return steram;
}

static bool operator>=(const vec_n& a, const double x)
{
	for (int i = 0; i < a.size(); i++)
	{
		if (a[i] < x)
		{
			return false;
		}
	}
	return true;
}

static bool operator<=(const vec_n& a, const double x)
{
	for (int i = 0; i < a.size(); i++)
	{
		if (a[i] > x)
		{
			return false;
		}
	}
	return true;
}

// ������ ����� ������� vec_n
static double magnitude(const  vec_n& a)
{
	double mag = 0;

	for (int i = 0; i < a.size(); i++)
	{
		mag += a[i] * a[i];
	}
	return sqrt(mag);
}
// ������������ ������� vec_n
static vec_n& normalize(vec_n& vec)
{
	double mag = 1.0 / magnitude(vec);
	for (int i = 0; i < vec.size(); i++)
	{
		vec[i] *= mag;
	}
	return vec;
}
// ����������� ���������� ������� ����������� �� ����� a � ����� b
static vec_n direction(const vec_n& a, const vec_n& b)
{
	vec_n res = b - a;
	return normalize(res);
}
// ����������� (����������� ���������� ������) n-������ ������� � ����� x � ����� eps
static vec_n gradient(func_n fun, const vec_n& x, const double eps = 1e-6)
{
	vec_n df, x_l(x), x_r(x);

	df.resize(x.size());
	for (int i = 0; i < x.size(); i++)
	{
		x_l[i] -= eps;
		x_r[i] += eps;

		df[i] = (fun(x_r) - fun(x_l)) * (0.5 / eps);

		x_l[i] += eps;
		x_r[i] -= eps;
	}
	return df;
}
// ������� ����������� �� i-�� ����������
static double partial(func_n func, vec_n& x, const int coord_index, const double eps = 1e-6)
{
	if (x.size() <= coord_index)
	{
		throw std::runtime_error("Partial derivative index out of bounds!");
	}
	x[coord_index] += eps;
	double f_r = func(x);
	x[coord_index] -= (2.0 * eps);
	double f_l = func(x);
	x[coord_index] += eps;
	return (f_r - f_l) / eps * 0.5;
}
//������ ������� ����������� �� i-�� j-�� �����������
static double partial2(func_n func, vec_n& x, const int coord_index_1, const int coord_index_2, const double eps = 1e-6)
{
	if (x.size() <= coord_index_2)
	{
		throw std::runtime_error("Partial derivative index out of bounds!");
	}
	x[coord_index_2] -= eps;
	double f_l = partial(func, x, coord_index_1, eps);
	x[coord_index_2] += (2.0 * eps);
	double f_r = partial(func, x, coord_index_1, eps);
	x[coord_index_2] -= eps;
	return (f_r - f_l) / eps * 0.5;
}

static double max(const vec_n& x, int& index, const int range = 0)
{
	double max_ = -1e32;
	int index_max = range == 0 ? x.size() : range;
	for (int i = 0; i < index_max; i++)
	{
		if (x[i] > max_)
		{
			max_ = x[i];
			index = i;
		}
	}
	return max_;
}

static double min(const vec_n& x, int& index, const int range = 0)
{
	double min_ = 1e32;

	int index_max = range == 0 ? x.size() : range;

	for (int i = 0; i < index_max; i++)
	{
		if (x[i] < min_)
		{
			min_ = x[i];
			index = i;
		}
	}
	return min_;
}

static double abs_max(const vec_n& x, int& index, const int range = 0)
{
	double max_ =  -1e32;
	int i = 0;
	for (auto& const x_i : x)
	{
		if (abs(x_i) > max_)
		{
			max_ = abs(x_i);
			index = i;
		}
		i++;
	}
	return max_;
}

static double abs_min(const vec_n& x, int& index)
{
	double min_ = 1e32;
	int i = 0;
	for (auto& const x_i : x)
	{
		if (abs(x_i) < min_)
		{
			min_ = abs(x_i);
			index = i;
		}
		i++;
	}
	return min_;
}