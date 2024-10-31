//
// Created by Алоев Артем on 15.10.2024.
//

#include "iostream"
#include "matrix.h"


using namespace std;

void firstMode();

void secondMode();


int main() {
    int mode;
    printf("Режим работы программы:\n"
           "1 - сложение сжатых матриц - схема Дженнинга\n"
           "2 - сложение и умножение сжатых матриц - схема Рейнбольдта - Местеньи\n");

    scanf("%d", &mode);

    switch (mode) {
        case 1:
            firstMode();
            break;
        case 2:
            secondMode();
            break;
        default:
            printf("Режима с номером %d не существует", mode);
    }


    return 0;
}

void firstMode() {
    cout << "Считывание матрицы А из файла..." << endl;
    Matrix matrix_a("../matrix_a");
    matrix_a.printMatrix();
    cout << "Упаковка матрицы А..." << endl;
    matrix_a.packMatrix();
    matrix_a.printPackMatrix();

    cout << "Считывание матрицы B из файла..." << endl;
    Matrix matrix_b("../matrix_b");
    matrix_b.printMatrix();
    cout << "Упаковка матрицы B..." << endl;
    matrix_b.packMatrix();
    matrix_b.printPackMatrix();

    Matrix matrix_c(matrix_a.getRows(), matrix_a.getCols());
    matrix_c.sumPackMatrix(matrix_a, matrix_b);
    cout << "Упаковка матрицы C..." << endl;
    matrix_c.printPackMatrix();
    matrix_c.unpackMatrix();
    cout << "Итоговая матрица С:" << endl;
    matrix_c.printMatrix();

    Matrix matrix_d(matrix_a.getRows(), matrix_a.getCols());
    matrix_d.sumMatrix(matrix_a, matrix_b);
    cout << "Итоговая матрица D:" << endl;
    matrix_d.printMatrix();

    Matrix::compareMatrices(matrix_c, matrix_d);
}

void secondMode() {
    // Сложение сжатых матрицы
    cout << "Сложение сжатых матриц";
    Matrix matrix_rm_a("../matrix_rm_a");

    matrix_rm_a.packMatrixRm();

    cout << endl << endl << "Сжатая матрица A";

    matrix_rm_a.printPackMatrixRm();

    Matrix matrix_rm_b("../matrix_rm_b");

    matrix_rm_b.packMatrixRm();

    cout << endl << endl << "Сжатая матрица B";

    matrix_rm_b.printPackMatrixRm();

    Matrix sum_matrix_rm(matrix_rm_a.getRows(), matrix_rm_a.getCols());
//    Matrix sum_matrix_rm;

    sum_matrix_rm.sumPackMatrixRm(matrix_rm_a, matrix_rm_b);

    cout << endl << endl << "Результирующая сжатая матрица sum после сложения";
    sum_matrix_rm.printPackMatrixRm();
    sum_matrix_rm.unpackMatrixRm();
    cout << endl;
    sum_matrix_rm.printMatrix();

    cout << endl << endl << "Обычная матрица sum после сложения" << endl;
    Matrix sum_matrix(matrix_rm_a.getRows(), matrix_rm_a.getCols());
    sum_matrix.sumMatrix(matrix_rm_a, matrix_rm_b);
    sum_matrix.printMatrix();

    Matrix::compareMatrices(sum_matrix_rm, sum_matrix);

    // Умножение сжатых матриц
    cout << "Умножение сжатых матриц" << endl;
    Matrix matrix_rm_c("../matrix_rm_c");
    matrix_rm_c.packMatrixRm();
    cout << endl << "Сжатая матрица C";
    matrix_rm_c.printPackMatrixRm();

    Matrix matrix_rm_d("../matrix_rm_d");
    matrix_rm_d.packMatrixRm();
    cout << endl << "Сжатая матрица D";
    matrix_rm_d.printPackMatrixRm();

    cout << endl << endl << "Результирующая сжатая матрица умножения";
    Matrix mul_matrix_rm(matrix_rm_c.getRows(), matrix_rm_d.getCols());
    mul_matrix_rm.mulPackMatrixRm(matrix_rm_c, matrix_rm_d);
    mul_matrix_rm.printPackMatrixRm();
    mul_matrix_rm.unpackMatrixRm();
    mul_matrix_rm.printMatrix();

    cout << endl << endl << "Обычная матрица после умножения" << endl;
    Matrix mul_matrix(matrix_rm_c.getRows(), matrix_rm_d.getCols());
    mul_matrix.mulMatrix(matrix_rm_c, matrix_rm_d);
    mul_matrix.printMatrix();

    Matrix::compareMatrices(mul_matrix_rm, mul_matrix);
}