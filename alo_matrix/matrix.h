//
// Created by Алоев Артем on 15.10.2024.
//

#ifndef ALO_MATRIX_MATRIX_H
#define ALO_MATRIX_MATRIX_H

#include "iostream"

using namespace std;

class Matrix {
private:

    int **matrix;
    int rows;
    int cols;
    vector<int> an;
    vector<int> d;
    vector<int> nr;
    vector<int> nc;
    vector<int> jr;
    vector<int> jc;

public:

    Matrix(const char *);

    Matrix(int, int);

    Matrix();

    ~Matrix();

    void packMatrix();

    void unpackMatrix();

    int getCols();

    int getRows();

    Matrix sumPackMatrix(Matrix &, Matrix &);

    Matrix &sumMatrix(Matrix &, Matrix &);

    static void compareMatrices(Matrix &, Matrix &);

    void printPackMatrix();

    void printMatrix();

    Matrix sumPackMatrixRm(Matrix &, Matrix &);

    Matrix mulPackMatrixRm(Matrix &, Matrix &);

    Matrix &mulMatrix(Matrix &a, Matrix &b);

    void packMatrixRm();

    void printPackMatrixRm();

    void unpackMatrixRm();
};


#endif //ALO_MATRIX_MATRIX_H
