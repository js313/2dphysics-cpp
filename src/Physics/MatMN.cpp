#include "MatMN.h"

MatMN::MatMN() : M(0), N(0), rows(nullptr) {}

MatMN::MatMN(int M, int N) : M(M), N(N)
{
    rows = new VecN[M];
    for (int i = 0; i < M; i++)
        rows[i] = VecN(N);
}

MatMN::MatMN(const MatMN &m)
{
    *this = m; // overloaded '=' called
}

MatMN::~MatMN()
{
    delete[] rows;
}

void MatMN::Zero()
{
    for (int i = 0; i < M; i++)
        rows[i].Zero();
}

MatMN MatMN::Transpose() const
{
    MatMN t = MatMN(*this);
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            t.rows[j][i] = (*this).rows[i][j];

    return t;
}

const MatMN &MatMN::operator=(const MatMN &m)
{
    M = m.M;
    N = m.N;
    rows = new VecN(M);
    for (int i = 0; i < M; i++)
        rows[i] = m.rows[i];

    return *this;
}

VecN MatMN::operator*(const VecN &v) const
{
    if (N != v.N)
        return v;
    VecN res = VecN(M);

    for (int i = 0; i < M; i++)
        res[i] = rows[i].Dot(v);

    return res;
}

MatMN MatMN::operator*(const MatMN &m) const
{
    if (N != m.M)
        return m;

    MatMN t = m.Transpose();
    MatMN res = MatMN(M, m.N);

    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            res.rows[i][j] = rows[i].Dot(t.rows[j]);

    return res;
}