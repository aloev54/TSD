//
// Created by Алоев Артем on 15.10.2024.
//

#include "matrix.h"

using namespace std;


Matrix::Matrix(const char *filepath) {
//    cout << "constr" << this << endl;
    FILE *matrix_file = fopen(filepath, "r");
    if (matrix_file == NULL) {
        cout << "Ошибка при чтении файла " << filepath << endl;
        exit(1);
    }

    fscanf(matrix_file, "%d %d", &rows, &cols);

    matrix = new int *[rows];

    for (int i = 0; i < rows; ++i) {
        matrix[i] = new int[cols];

        for (int j = 0; j < cols; ++j) {
            fscanf(matrix_file, "%d", &matrix[i][j]); // Считываем элементы матрицы из файла
        }
    }
    fclose(matrix_file);
}

Matrix::Matrix() {}

Matrix::Matrix(int rows, int cols) : rows(rows), cols(cols) {
//    cout << "const" << this << endl;
    matrix = new int *[rows];
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new int[cols]{0};
    }
}


Matrix::~Matrix() {
//    cout << "destr" << this << endl;
    if (matrix != nullptr) {
        for (int i = 0; i < rows; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }
    an.clear();
    d.clear();
    jr.clear();
    jc.clear();
    nr.clear();
    nc.clear();
}


void Matrix::packMatrix() {
    an.push_back(matrix[0][0]);
    d.push_back(0);
    for (int i = 1; i < rows; ++i) {
        bool wasntZeroElem = false;
        for (int j = 0; j <= i; ++j) {
            if (matrix[i][j] != 0 || wasntZeroElem || i == j) {
                wasntZeroElem = true;
                an.push_back(matrix[i][j]);
            }
        }
        d.push_back(an.size() - 1);
    }
}

void Matrix::unpackMatrix() {
    matrix = new int *[rows];
    matrix[0] = new int[cols];
    matrix[0][0] = an[0];

    for (int i = 1; i < rows; ++i) {
        matrix[i] = new int[cols];
        int wasntZeroElem = d[i] - d[i - 1] - 1;
        for (int j = 0; j <= i; ++j) {
            if (wasntZeroElem > 0 && j == i - wasntZeroElem) {
                matrix[i][j] = an[d[i] - wasntZeroElem];
                matrix[j][i] = an[d[i] - wasntZeroElem];
                wasntZeroElem--;
//                continue;
            } else if (i == j) {
                matrix[i][j] = an[d[i]];
                matrix[j][i] = an[d[i]];
//                continue;
            } else {
                matrix[i][j] = 0;
                matrix[j][i] = 0;
            }
        }
    }
}


void Matrix::printPackMatrix() {
    cout << "Запакованная матрица " << rows << "x" << cols << endl;
    cout << "Массив AN: ";
    for (int i = 0; i < an.size(); i++) {
        cout << an[i] << " ";
    }

    cout << endl;

    cout << "Массив D: ";
    for (int i = 0; i < rows; i++) {
        cout << d[i] << " ";
    }

    cout << endl << endl;

}


void Matrix::printMatrix() {
    cout << "Матрица " << rows << "x" << cols << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << " ";
        }

        cout << endl;
    }

    cout << endl;
}


Matrix Matrix::sumPackMatrix(Matrix &a, Matrix &b) {

    this->an.push_back(a.an[0] + b.an[0]);

    this->d.push_back(0);

    for (int i = 1; i < a.rows; ++i) {
        int diff_a_d = a.d[i] - a.d[i - 1];
        int diff_b_d = b.d[i] - b.d[i - 1];

        int diff = abs(diff_a_d - diff_b_d);

        bool flag = false;
        if (diff != 0) {
            if (diff_a_d > diff_b_d) {
                for (int j = 1; j <= (diff); ++j) {
                    int cur_el = a.an[a.d[i - 1] + j];
                    if (cur_el != 0 && !flag) {
                        flag = true;
                    }
                    this->an.push_back(cur_el);
                }

                diff_a_d -= diff;
            } else {
                for (int j = 1; j <= (diff); ++j) {
                    int cur_el = b.an[b.d[i - 1] + j];
                    if (cur_el != 0 && !flag) {
                        flag = true;
                    }
                    this->an.push_back(cur_el);
                }
            }

            bool wasNotZeroElement = flag;
            for (int j = 1; j <= diff_a_d; ++j) {
                int sum = a.an[a.d[i] - diff_a_d + j] + b.an[b.d[i] - diff_a_d + j];

                if (sum == 0 && !wasNotZeroElement && a.d[i] != a.d[i - 1] + j) {
                    continue;
                }

                this->an.push_back(sum);
                wasNotZeroElement = true;
            }

            this->d.push_back(this->an.size() - 1);
        } else {
            bool wasNotZeroElement = false;
            for (int j = 1; j <= (a.d[i] - a.d[i - 1]); ++j) {
                int sum = a.an[a.d[i - 1] + j] + b.an[b.d[i - 1] + j];

                if (sum == 0 && !wasNotZeroElement && a.d[i] != a.d[i - 1] + j) {
                    continue;
                }

                this->an.push_back(sum);
                wasNotZeroElement = true;
            }

            this->d.push_back(this->an.size() - 1);
        }
    }

    this->rows = a.rows;
    this->cols = a.cols;


    return *this;
}

int getCurrentIdx(int index, int firstElementsArrLen, vector<int> firstElementsArr, vector<int> nextElementsArr) {
    int idx = -1;
    while (idx == -1) {
        for (int i = 0; i < firstElementsArrLen; ++i) {
            if (firstElementsArr[i] == index) {
                idx = i;
                break;
            }
        }

        index = nextElementsArr[index];
    }

    return idx;
}

void setFirstIdxAndNextIdx(int idx, vector<int> &first_el_arr, vector<int> &next_el_arr, int an_size) {
    if (first_el_arr[idx] == -1) {
        first_el_arr[idx] = an_size;
        next_el_arr.push_back(an_size);
    } else {
        next_el_arr.push_back(first_el_arr[idx]);

        int cur_idx = first_el_arr[idx];

        while (next_el_arr[cur_idx] != first_el_arr[idx]) {
            cur_idx++;
        }

        next_el_arr[cur_idx] = an_size;
    }
}

Matrix Matrix::mulPackMatrixRm(Matrix &a, Matrix &b) {
    if (a.cols != b.rows) {
        cout << "Количество столбцов первой матрицы не равно количеству строк второй матрицы, умножение невозможно"
             << endl;
        exit(0);
    }

    int indexRow; // индекс строки
    int indexCol; // индекс столбца

    for (int i = 0; i < a.rows; ++i) {
        this->jr.push_back(-1);
    }

    for (int i = 0; i < b.cols; ++i) {
        this->jc.push_back(-1);
    }

    for (int i = 0; i < a.rows; ++i) { // внешний цикл по строкам первой матрицы
        indexRow = a.jr[i];
        // Если строка пуста, то переходим к следующей строке
        if (indexRow == -1) {
            continue;
        }

        for (int j = 0; j < b.cols; ++j) { // цикл по столбцам второй матрицы
            indexCol = b.jc[j];
            // Если столбец пуст, то переходим к следующему столбцу
            if (indexCol == -1) {
                continue;
            }

            int sum = 0;

            bool rowEnd = false;
            while (!rowEnd) { // цикл прохода по текущей строке
                bool colEnd = false;
                int curRowElemIdx = getCurrentIdx(indexRow, a.cols, a.jc, a.nc);

                while (!colEnd) { // цикл прохода по текущему столбцу
                    int curColElemIdx = getCurrentIdx(indexCol, b.rows, b.jr, b.nr);
                    if (curColElemIdx >
                        curRowElemIdx) { // если индекс элемента в колонке больше индекса элемента в строке - продолжатть неправильно
                        break;
                    }

                    //проверка совпадения индекса элемента строки и стобца
                    if (curColElemIdx == curRowElemIdx) {
                        sum += a.an[indexRow] * b.an[indexCol];
                    }

                    indexCol = b.nc[indexCol]; // следующий индекс столбца

                    if (indexCol == b.jc[j]) { //если прошли круг - выход
                        colEnd = true;
                    }
                }

                indexRow = a.nr[indexRow]; // следующий индекс строки

                if (indexRow == a.jr[i]) { //если прошли круг - выход
                    rowEnd = true;
                }
            }

            if (sum != 0) {
                setFirstIdxAndNextIdx(i, jr, nr, an.size()); // заполнение массивов jr и nr
                setFirstIdxAndNextIdx(j, jc, nc, an.size()); // заполнение массивов jc и nc
                an.push_back(sum); // заполнение AN
            }
        }
    }

    this->rows = a.rows;
    this->cols = b.cols;

    return *this;

}

Matrix &Matrix::mulMatrix(Matrix &a, Matrix &b) {
    if (a.cols != b.rows) {
        cout << "Количество столбцов первой матрицы не равно количеству строк второй матрицы, умножение невозможно"
             << endl;
        exit(0);
    }

    matrix = new int *[a.rows];

    for (int i = 0; i < a.rows; ++i) {
        matrix[i] = new int[b.cols];
        for (int j = 0; j < b.cols; ++j) {
            for (int k = 0; k < a.cols; k++) {
                matrix[i][j] += a.matrix[i][k] * b.matrix[k][j];
            }
        }
    }

    return *this;
}

void Matrix::compareMatrices(Matrix &a, Matrix &b) {
    if (a.rows != b.rows || a.cols != b.cols) {
        printf("Матрицы имеют разный размер: A[%dx%d]!=B[%dx%d]\n",
               a.rows, a.cols, b.rows, b.cols);

        return;
    }

    int counter = 0;

    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            if (a.matrix[i][j] != b.matrix[i][j]) {
                counter++;
                printf("A[%d][%d]!=B[%d][%d] (%d!=%d)\n",
                       i, j, i, j, a.matrix[i][j], b.matrix[i][j]);
            }
        }
    }

    if (counter == 0) {
        printf("Результирующие матрицы совпадают\n\n");
    } else {
        printf("Результирующие матрицы имеют %d отличий\n\n", counter);
    }
}

Matrix &Matrix::sumMatrix(Matrix &a, Matrix &b) {
//    this->matrix = new int *[a.rows];
//    this->rows = a.rows;
//    this->cols = a.cols;

    for (int i = 0; i < a.rows; ++i) {
//        this->matrix[i] = new int[a.cols];
        for (int j = 0; j < a.cols; ++j) {
            this->matrix[i][j] = a.matrix[i][j] + b.matrix[i][j];
        }
    }

    return *this;
}

int Matrix::getCols() {
    return this->cols;
}

int Matrix::getRows() {
    return this->rows;
}


Matrix Matrix::sumPackMatrixRm(Matrix &a, Matrix &b) {

    for (int i = 0; i < a.rows; ++i) {
        this->jr.push_back(-1);
    }

    for (int i = 0; i < a.cols; ++i) {
        this->jc.push_back(-1);
    }


    int cursor_a = 0;
    int cursor_b = 0;

    while (cursor_a < a.an.size() && cursor_b < b.an.size()) {
        // Ищем у элементов текущие индексы строки и столбца в матрице
        int col_a_idx = getCurrentIdx(a.nc[cursor_a], a.cols, a.jc, a.nc);
        int col_b_idx = getCurrentIdx(b.nc[cursor_b], b.cols, b.jc, b.nc);
        int row_a_idx = getCurrentIdx(a.nr[cursor_a], a.rows, a.jr, a.nr);
        int row_b_idx = getCurrentIdx(b.nr[cursor_b], b.rows, b.jr, b.nr);

        if ((row_a_idx < row_b_idx) || ((row_a_idx == row_b_idx) && (col_a_idx < col_b_idx))) {
            setFirstIdxAndNextIdx(row_a_idx, this->jr, this->nr, this->an.size());
            setFirstIdxAndNextIdx(col_a_idx, this->jc, this->nc, this->an.size());

            an.push_back(a.an[cursor_a]);
            cursor_a++;
        } else if (row_a_idx > row_b_idx || ((row_a_idx == row_b_idx) && (col_a_idx > col_b_idx))) {
            setFirstIdxAndNextIdx(row_b_idx, this->jr, this->nr, this->an.size());
            setFirstIdxAndNextIdx(col_b_idx, this->jc, this->nc, this->an.size());

            this->an.push_back(b.an[cursor_b]);
            cursor_b++;
        } else {
            if (a.an[cursor_a] + b.an[cursor_b] != 0) {
                setFirstIdxAndNextIdx(row_a_idx, this->jr, this->nr, this->an.size());
                setFirstIdxAndNextIdx(col_a_idx, this->jc, this->nc, this->an.size());

                this->an.push_back(a.an[cursor_a] + b.an[cursor_b]);
            }

            cursor_a++;
            cursor_b++;
        }
    }

    while (cursor_a < a.an.size()) {
        int row_a_idx = getCurrentIdx(a.nr[cursor_a], a.rows, a.jr, a.nr);
        int col_a_idx = getCurrentIdx(a.nc[cursor_a], a.cols, a.jc, a.nc);

        setFirstIdxAndNextIdx(row_a_idx, this->jr, this->nr, this->an.size());
        setFirstIdxAndNextIdx(col_a_idx, this->jc, this->nc, this->an.size());

        this->an.push_back(a.an[cursor_a]);
        cursor_a++;
    }

    while (cursor_b < b.an.size()) {
        int row_b_idx = getCurrentIdx(b.nr[cursor_b], b.rows, b.jr, b.nr);
        int col_b_idx = getCurrentIdx(b.nc[cursor_b], b.cols, b.jc, b.nc);

        setFirstIdxAndNextIdx(row_b_idx, this->jr, this->nr, this->an.size());
        setFirstIdxAndNextIdx(col_b_idx, this->jc, this->nc, this->an.size());

        this->an.push_back(b.an[cursor_b]);
        cursor_b++;
    }
    rows = a.rows;
    cols = a.cols;
    return *this;
}


void Matrix::packMatrixRm() {
    if (rows < 1 || cols < 1) {
        cout << "[Упаковка матрицы] матрица не должна быть нулевой" << endl;
        exit(1);
    }

    for (int i = 0; i < rows; ++i)
        jr.push_back(-1);

    for (int i = 0; i < cols; ++i)
        jc.push_back(-1);


    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (matrix[i][j] != 0) {
                an.push_back(matrix[i][j]);

                if (jr[i] == -1) {
                    jr[i] = an.size() - 1;
                    nr.push_back(an.size() - 1);
                } else {
                    nr.push_back(jr[i]);

                    int cur_idx = jr[i];

                    while (nr[cur_idx] != jr[i]) {
                        cur_idx++;
                    }

                    nr[cur_idx] = an.size() - 1;
                }

                if (jc[j] == -1) {
                    jc[j] = an.size() - 1;
                    nc.push_back(an.size() - 1);
                } else {
                    nc.push_back(jc[j]);

                    int cur_idx = jc[j];

                    while (nc[cur_idx] != jc[j]) {
                        cur_idx++;
                    }

                    nc[cur_idx] = an.size() - 1;
                }
            }
        }
    }
}

void Matrix::unpackMatrixRm() {
    if (rows < 1 || cols < 1) {
        cout << endl;
        cout << "Распаковка матрицы матрица не должна быть нулевой" << endl;
        exit(1);
    }

    matrix = new int *[rows];

    for (int i = 0; i < rows; ++i) {
        matrix[i] = new int[cols];
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = 0;
        }
    }

    for (int i = 0; i < an.size(); ++i) {
        int col_idx = -1;
        int row_idx = -1;
        int cur_nc = nc[i];
        int cur_nr = nr[i];

        while (col_idx == -1) {
            for (int j = 0; j < cols; ++j) {
                if (jc[j] == cur_nc) {
                    col_idx = j;
                    break;
                }
            }

            cur_nc = nc[cur_nc];
        }


        while (row_idx == -1) {
            for (int j = 0; j < rows; ++j) {
                if (jr[j] == cur_nr) {
                    row_idx = j;
                    break;
                }
            }

            cur_nr = nr[cur_nr];
        }

        matrix[row_idx][col_idx] = an[i];
    }
}


void Matrix::printPackMatrixRm() {
    cout << endl;
    cout << "[Массив AN]: ";

    for (int i = 0; i < an.size(); ++i) {
        cout << an[i] << " ";
    }

    cout << endl;
    cout << "[Массив NR]: ";

    for (int i = 0; i < nr.size(); ++i) {
        cout << nr[i] << " ";
    }

    cout << endl;
    cout << "[Массив NC]: ";

    for (int i = 0; i < nc.size(); ++i) {
        cout << nc[i] << " ";
    }

    cout << endl;
    cout << "[Массив JR]: ";

    for (int i = 0; i < rows; ++i) {
        cout << jr[i] << " ";
    }

    cout << endl;
    cout << "[Массив JC]: ";

    for (int i = 0; i < cols; ++i) {
        cout << jc[i] << " ";
    }
    cout << endl;
}