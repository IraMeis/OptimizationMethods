#pragma once
#include "one_dimensional.h"
#include "vector_utils.h"
#include "matrix_utils.h"

#include <vector>
#include <cassert>
//��������� �� ���������
#define N_DIM_ACCURACY 1e-6
#define N_DIM_ITERS_MAX 1000

// �������� ����������� ��������� ������� � ��������� � ����� {2,2} 
static double test_function(const vec_n& args)
{
	return  (args[0] - 2) * (args[0] - 2) + (args[1] - 2) * (args[1] - 2);
}
// ������ n-������ ���������, �������� ������� � ��������� ���������� ������� ������ ����� ����������� ��  x_0 � x_1
// �.�., ���� �������� ������� ������� �� ���� ����������� �� �����, ����� �� ����� ����� ����������� ��������, �� ��� 
// ����� ���������� �� ��������� ��������
static vec_n dihotomia(func_n f, const vec_n& x_0, const vec_n& x_1, const double eps = N_DIM_ACCURACY, const int max_iters = N_DIM_ITERS_MAX)
{
	vec_n x_0_ = x_0, x_1_ = x_1, x_c, dir;

	dir = direction(x_0, x_1);

	int cntr = 0;

	for (; cntr != max_iters; cntr++)
	{
		if (magnitude(x_1_ - x_0_) < eps)
		{
			break;
		}
		x_c = (x_1_ + x_0_) * 0.5f;

		if (f(x_c + dir * eps) > f(x_c - dir * eps))
		{
			x_1_ = x_c;
			continue;
		}
		x_0_ = x_c;
	}
#ifdef _DEBUG 
	std::cout << "dihotomia iterations number : " << cntr << "\n";
#endif
	return x_c;
}

static vec_n golden_ratio(func_n f, const vec_n& x_0, const vec_n& x_1, const double eps = N_DIM_ACCURACY, const int max_iters = N_DIM_ITERS_MAX)
{
	vec_n  a = x_0, b = x_1;
	vec_n x_0_(a), x_1_(b), dx;

	int cntr = 0;

	double one_div_phi = 1.0 / phi;

	for (; cntr != max_iters; cntr++)
	{
		if (magnitude(x_1_ - x_0_) < eps)
		{
			break;
		}
		dx   = (b - a) * one_div_phi;

		x_0_ =  b - dx;
		x_1_ =  a + dx;

		if (f(x_0_) > f(x_1_))
		{
			b = x_0_;
			continue;
		}
		a = x_1_;
	}
#if _DEBUG
	std::cout <<"golden ratio iterations number : " << cntr << "\n";
#endif
	return (a + b) * 0.5f;
}

static vec_n fibonacci(func_n f, const vec_n& x_0, const vec_n& x_1, const double eps = N_DIM_ACCURACY)
{
	vec_n a(x_0), b(x_1);
	vec_n x_0_(x_0), x_1_(x_1), dx;
	double f_1 = 1.0f, f_2 = 2.0f, f_3 = 3.0f;
	int cntr = 0;

	int max_iters = closest_fibonacci(magnitude(x_1 - x_0) / eps);

	for (; cntr != max_iters; cntr++)
	{
		if (magnitude(x_1_ - x_0_) < eps)
		{
			break;
		}
		dx   = b - a;
		x_0_ = b - dx * (f_1 / f_3);
		x_1_ = b + dx * (f_2 / f_3);

		f_1 = f_2;
		f_2 = f_3;
		f_3 = f_1 + f_2;

		if (f(x_0_) < f(x_1_))
		{
			b = x_0_;
			continue;
		}
		a = x_1_;
	}
#if _DEBUG
	std::cout << "fibonacchi iterations number : " << cntr << "\n";
#endif
	return (x_1_ + x_0_) * 0.5f;
}
// �������������� �����, ����������� ����� � ����� � ������� ���������� ����������, ����������
// ����������� �������� ������� ������ �� ����� ��������� ����� x_start.
// ������� �� ������� �� ������ �����������.
static vec_n per_coord_descend(func_n f, const vec_n& x_start, const double eps = N_DIM_ACCURACY, const int max_iters = N_DIM_ITERS_MAX)
{
	int cntr = 0;
	vec_n x_0(x_start);
	vec_n x_1(x_start);
	double step = 1.0f;
	double y_1, y_0;
	while (true)
	{
		for (int i = 0; i < x_0.size(); i++)
		{
			cntr++;
			if (cntr == max_iters)
			{
#if _DEBUG
				std::cout << "gradient descend iterations number : " << cntr << "\n";
#endif
				return x_0;
			}
		
			x_1[i] -= eps;
			y_0     = f(x_1);
			x_1[i] += 2 *eps;
			y_1     = f(x_1);
			x_1[i] -= eps;

			if (y_0 > y_1)
			{
				x_1[i] += step;
			}
			else
			{
				x_1[i] -= step;
			}
			x_1 = dihotomia(f, x_0, x_1, eps, max_iters);
			if (magnitude(x_1 - x_0) < eps) 
			{
#if _DEBUG
				std::cout << "gradient_descend iterations number : " << cntr << "\n";
#endif
				return x_0;
			}
			x_0 = x_1;
		}
	}
}
//
static vec_n gradient_descend(func_n f, const vec_n& x_start, const double eps = N_DIM_ACCURACY, const int max_iters = N_DIM_ITERS_MAX)
{
	vec_n x_i(x_start);
	vec_n x_i_1; 
	vec_n grad;
	int cntr = 0;
	while (true)
	{
		cntr++;
		if (cntr == max_iters)
		{
			break;
		}
		grad  = gradient(f, x_i, eps);
		x_i_1 = x_i - grad;
		x_i_1 = dihotomia(f, x_i, x_i_1, eps, max_iters);
		
		if (magnitude(x_i_1 - x_i) < eps) 
		{
			break;
		}

		x_i = x_i_1;
	}
#if _DEBUG
	std::cout << "gradient descend iterations number : " << cntr << "\n";
#endif
	return (x_i_1 + x_i) * 0.5f;
}

static vec_n conj_gradient_descend(func_n f, const vec_n& x_start, const double eps = N_DIM_ACCURACY, const int max_iters = N_DIM_ITERS_MAX)
{
	vec_n x_i(x_start);
	vec_n x_i_1;
	vec_n s_i = gradient(f, x_start, eps)*(-1.0f), s_i_1;
	double omega;
	int cntr = 0;
	while (true)
	{
		cntr++;
	
		if (cntr == max_iters)
		{
			break;
		}

		x_i_1 = x_i + s_i;
	
		x_i_1 = dihotomia(f, x_i, x_i_1, eps, max_iters);

		if (magnitude(x_i_1 - x_i) < eps)
		{
			break;
		}

		s_i_1 = gradient(f, x_i_1, eps);

		omega = pow(magnitude(s_i_1), 2) / pow(magnitude(s_i), 2);

		s_i = s_i * omega - s_i_1;

		x_i = x_i_1;
	}
#if _DEBUG
	std::cout << "conj gradient descend iterations number : " << cntr << "\n";
#endif
	return (x_i_1 + x_i) * 0.5f;
}

static vec_n newtone_raphson(func_n f, const vec_n& x_start, const double eps = N_DIM_ACCURACY, const int max_iters = N_DIM_ITERS_MAX)
{
	vec_n x_i(x_start);
	vec_n x_i_1;
	vec_n grad;
	mat_mn hess;
	int cntr = 0;
	while (true)
	{
		cntr++;
		if (cntr == max_iters)
		{
			break;
		}
		
		grad = gradient(f, x_i, eps);
	
		hess = invert(hessian(f, x_i, eps));
		
		x_i_1 = x_i - hess * grad;

		if (magnitude(x_i_1 - x_i) < eps)
		{
			break;
		}
		x_i = x_i_1;
	}
#if _DEBUG
	std::cout << "Newtone-Raphson method iterations number : " << cntr << "\n";
#endif
	return (x_i_1 + x_i) * 0.5f;
}
 

/// <summary>
/// ���������� � main
/// </summary>
static void multi_dimensional_methods_test()
{
	vec_n x_0 = { 0,0 };
	vec_n x_1 = { 5,5 };
	
	std::cout << "\n";
	std::cout << "{ x, y } = agrmin((x - 2) * (x - 2) + (y - 2) * (y - 2))\n";
	std::cout << "x_0 = " << x_0 << ", x_1 = " << x_1 << "\n";
	std::cout << "dihotomia             : " << dihotomia   (test_function, x_1, x_0, 1e-3f) << "\n";
	std::cout << "golden_ratio          : " << golden_ratio(test_function, x_1, x_0, 1e-3f) << "\n";
	std::cout << "fibonacci             : " << fibonacci  (test_function,  x_1, x_0, 1e-3f) << "\n";
	std::cout << "\n";
	vec_n x_start = {-14,3.98f};

	std::cout << "x_start = " << x_start << "\n";
	std::cout << "per_coord_descend     : " << per_coord_descend    (test_function,   x_start,     1e-5f) << "\n";
	std::cout << "gradient_descend      : " << gradient_descend     (test_function,   x_start,     1e-5f) << "\n";
	std::cout << "conj_gradient_descend : " << conj_gradient_descend(test_function,   x_start,     1e-5f) << "\n";
	std::cout << "newtone_raphson       : " << newtone_raphson      (test_function_3, {-2, 6, 12}, 1e-5f) << "\n";
}