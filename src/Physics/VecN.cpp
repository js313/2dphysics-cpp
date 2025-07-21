#include "VecN.h"

VecN::VecN() : N(0), data(nullptr) {};

VecN::VecN(int n)
{
    N = n;
    data = new float[n];
}

VecN::~VecN()
{
    delete[] data;
}

VecN::VecN(const VecN &v)
{
    N = v.N;
    data = new float[N];

    for (int i = 0; i < N; i++)
        data[i] = v.data[i];
}

void VecN::Zero()
{
    for (int i = 0; i < N; i++)
        data[i] = 0;
}

float VecN::Dot(const VecN &v) const
{
    float sum = 0;
    for (int i = 0; i < N; i++)
        sum += data[i] * v.data[i];
    return sum;
}

VecN VecN::operator=(const VecN &v)
{
    delete[] data;
    N = v.N;
    data = new float[N];
    for (int i = 0; i < N; i++)
        data[i] = v.data[i];
    return *this;
}

VecN VecN::operator+(const VecN &v)
{
    VecN result = *this; // copy constructor called, copies the memory allocated
    for (int i = 0; i < N; i++)
        result.data[i] += v.data[i];
    return result;
}

VecN VecN::operator-(const VecN &v)
{
    VecN result = *this;
    for (int i = 0; i < N; i++)
        result.data[i] -= v.data[i];
    return result;
}

VecN VecN::operator*(float n)
{
    VecN result = *this;
    for (int i = 0; i < N; i++)
        result.data[i] *= n;
    return result;
}

const VecN &VecN::operator+=(const VecN &v)
{
    for (int i = 0; i < N; i++)
        data[i] += v.data[i];
    return *this;
}

const VecN &VecN::operator-=(const VecN &v)
{
    for (int i = 0; i < N; i++)
        data[i] -= v.data[i];
    return *this;
}

const VecN &VecN::operator*=(float n)
{
    return (*this) * n;
}

float VecN::operator[](int index) const
{
    return data[index];
}

float VecN::operator[](int index) const
{
    return data[index];
}