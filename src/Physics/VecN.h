#ifndef VECN_H
#define VECN_H

struct VecN
{
    int N;
    float *data;

    VecN();
    VecN(int n);
    VecN(const VecN &v);
    ~VecN();

    void Zero();
    float Dot(const VecN &v) const;

    VecN operator=(const VecN &v);
    VecN operator+(const VecN &v);
    VecN operator-(const VecN &v);
    VecN operator*(float n) const;
    const VecN &operator+=(const VecN &v);
    const VecN &operator-=(const VecN &v);
    const VecN &operator*=(float n);

    // C++ uses const overloading to control how objects behave when they’re const. Without the const version:
    // 1. You couldn't use [] on const objects.
    // 2. It would prevent passing VecN to const-correct functions
    float operator[](int index) const;
    float &operator[](int index);
};

#endif