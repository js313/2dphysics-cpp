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
    VecN operator*(float n);
    const VecN &operator+=(const VecN &v);
    const VecN &operator-=(const VecN &v);
    const VecN &operator*=(float n);
    float operator[](int index) const;
    float &operator[](int index); // for assignment
};

#endif