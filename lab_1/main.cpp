#include <iostream>
#include "cstdlib"
#include "time.h"
#include <fstream>

using namespace std;

int **create_matrix(int, int);

void print_matrix(int **, int, int);

int **std_matrix_mul(int **, int **, int, int, int);

int **vngrd_matrix_mul(int **, int **, int, int, int);

void compare_matrix(int **, int **, int, int);

int **random_matrix(int, int);

void first_mode();

void second_mode();

int main() {
    int choice;
    cout << "Выберите режим работы программы:" << endl;
    cout << "1) Ввод с клавиатуры 2-х матриц и сравнение результата их перемножения 2-мя разными алгоритмами" << endl;
    cout << "2) Замер процессорного времени при перемножении 2-х матриц 2-мя разными алгоритмами" << endl;
    cin >> choice;

    switch (choice) {
        case 1:
            first_mode();
            break;
        case 2:
            second_mode();
            break;
        default:
            cout << "Ошибка при выборе режима!" << endl;
    }
    return 0;
}

int **create_matrix(int rows, int cols) {
    int **matrix = new int *[rows];
    for (int i = 0; i < rows; i++) matrix[i] = new int[cols];
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) cin >> matrix[i][j];
    return matrix;
}

void print_matrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            cout << matrix[i][j] << " ";
        cout << endl;
    }
}

int **std_matrix_mul(int **matrix_a, int **matrix_b, int a_rows, int a_cols, int b_cols) {
    int **matrix_c = new int *[a_rows];
    for (int i = 0; i < a_rows; i++)
        matrix_c[i] = new int[b_cols];
    for (int i = 0; i < a_rows; i++)
        for (int j = 0; j < b_cols; j++) {
            matrix_c[i][j] = 0;
            for (int k = 0; k < a_cols; k++)
                matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
        }
    return matrix_c;
}

void compare_matrix(int **matrix_a, int **matrix_b, int cols, int rows) {
    int counter = 0;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (matrix_a[i][j] == matrix_b[i][j])
                counter++;
    if (counter != 0)
        cout << "Матрицы совпадают" << endl;
    else
        cout << "Матрицы не совпадают" << endl;
}

int **random_matrix(int rows, int cols) {
    int **random_matrix = new int *[rows];
    for (int i = 0; i < rows; i++)
        random_matrix[i] = new int[cols];

    srand(time(NULL));

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            random_matrix[i][j] = rand();
    return random_matrix;
}

int **vngrd_matrix_mul(int **matrix_a, int **matrix_b, int a_rows, int a_cols, int b_cols) {
    int **matrix_c = new int *[a_rows];
    for (int i = 0; i < a_rows; i++)
        matrix_c[i] = new int[b_cols];

    int *mul_h = new int[a_rows];

    int *mul_v = new int[b_cols];

    for (int i = 0; i < a_rows; i++) {
        mul_h[i] = 0;
        for (int k = 1; k < a_cols; k += 2)
            mul_h[i] += matrix_a[i][k - 1] * matrix_a[i][k];
    }

    for (int j = 0; j < b_cols; j++) {
        mul_v[j] = 0;
        for (int k = 1; k < a_cols; k += 2)
            mul_v[j] += matrix_b[k - 1][j] * matrix_b[k][j];
    }

    for (int i = 0; i < a_rows; i++)
        for (int j = 0; j < b_cols; j++) {
            matrix_c[i][j] = -mul_h[i] - mul_v[j];
            for (int k = 1; k < a_cols; k += 2)
                matrix_c[i][j] +=
                        (matrix_a[i][k - 1] + matrix_b[k][j]) *
                        (matrix_a[i][k] + matrix_b[k - 1][j]);
        }

    if (a_cols % 2 == 1) {
        for (int i = 0; i < a_rows; i++)
            for (int j = 0; j < b_cols; j++)
                matrix_c[i][j] += matrix_a[i][a_cols - 1] * matrix_b[a_cols - 1][j];
    }

    delete[] mul_h;
    delete[] mul_v;
    return matrix_c;
}

void first_mode() {
    int a_rows, a_cols, b_rows, b_cols;

    cout << "Введите количество строк и столбцов матрицы А:" << endl;
    cin >> a_rows >> a_cols;
    cout << "Введите количество строк и столбцов матрицы B:" << endl;
    cin >> b_rows >> b_cols;

    if (a_rows != b_cols)
        cout << "Недопустимые размеры матриц!" << endl;
    else {
        cout << "Введите элементы матрицы А:" << endl;
        int **matrix_a = create_matrix(a_rows, a_cols);
        cout << "Введите элементы матрицы B:" << endl;
        int **matrix_b = create_matrix(b_rows, b_cols);

        int **matrix_c_std = std_matrix_mul(matrix_a, matrix_b, a_rows, a_cols, b_cols);
        int **matrix_c_vngrd = vngrd_matrix_mul(matrix_a, matrix_b, a_rows, a_cols, b_cols);

        cout << "Результат перемножения матриц стандартным методом:" << endl;
        print_matrix(matrix_c_std, a_rows, b_cols);
        cout << "Результат перемножения матриц методом Винограда:" << endl;
        print_matrix(matrix_c_vngrd, a_rows, b_cols);

        compare_matrix(matrix_c_std, matrix_c_vngrd, a_rows, b_cols);

        delete[] matrix_a;
        delete[] matrix_b;
        delete[] matrix_c_std;
        delete[] matrix_c_vngrd;
    }
}

void second_mode() {
    int cls = 10;
    clock_t t;

    ofstream std_alg;
    std_alg.open("std_alg.txt");
    if (!std_alg.is_open()) {
        perror("Ошибка открытия файла std_alg.txt!");
        exit(1);
    }

    ofstream vngrd_alg;
    vngrd_alg.open("vngrd_alg.txt");
    if (!std_alg.is_open()) {
        perror("Ошибка открытия файла vngrd_alg.txt!");
        exit(1);
    }

    for (int i = 100; i < 501; i += 100) {
        double std_mul_time = 0.0, vngrd_mul_time = 0.0;
        for (int j = 0; j < cls; j++) {
            int **rand_matrix = random_matrix(i, i);
            // ссылка на доку https://cplusplus.com/reference/ctime/clock/
            t = clock();
            int **std_matrix = std_matrix_mul(rand_matrix, rand_matrix, i, i, i);
            t = clock() - t;
            std_mul_time += (double) t;

            t = clock();
            int **vngrd_matrix = vngrd_matrix_mul(rand_matrix, rand_matrix, i, i, i);
            t = clock() - t;
            vngrd_mul_time += (double) t;

            delete[] rand_matrix;
            delete[] std_matrix;
            delete[] vngrd_matrix;
        }
        if (std_alg.is_open())
            std_alg << std_mul_time / CLOCKS_PER_SEC / cls * 1000 << endl;
        if (vngrd_alg.is_open())
            vngrd_alg << vngrd_mul_time / CLOCKS_PER_SEC / cls * 1000 << endl;
    }
    std_alg.close();
    vngrd_alg.close();
    cout << "Файл std_alg.txt успешно записан!" << endl;
    cout << "Файл vngrd_alg.txt успешно записан!" << endl;
}
