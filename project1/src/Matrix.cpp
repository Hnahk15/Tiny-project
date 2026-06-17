#include "Matrix.h"
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <iostream>

// ── Private helpers ──────────────────────────────────────────────────────────

void Matrix::Allocate() {
    mData = new double*[mNumRows];
    for (int i = 0; i < mNumRows; ++i) {
        mData[i] = new double[mNumCols]();  // () zero-initialises
    }
}

void Matrix::Deallocate() {
    for (int i = 0; i < mNumRows; ++i)
        delete[] mData[i];
    delete[] mData;
}

// ── Constructors & Destructor ────────────────────────────────────────────────

Matrix::Matrix(int numRows, int numCols) // cấp phát bộ nhớ cho ma trận có numRows hàng và numCols cột, đồng thời khởi tạo tất cả phần tử về 0
    : mNumRows(numRows), mNumCols(numCols) {
    assert(numRows > 0 && numCols > 0);
    Allocate();
}

// Deep copy constructor
Matrix::Matrix(const Matrix& other)
    : mNumRows(other.mNumRows), mNumCols(other.mNumCols) {
    Allocate();
    for (int i = 0; i < mNumRows; ++i)
        for (int j = 0; j < mNumCols; ++j)
            mData[i][j] = other.mData[i][j];
}

Matrix::~Matrix() {
    Deallocate();
}

// ── Indexing (1-based) ───────────────────────────────────────────────────────

double& Matrix::operator()(int i, int j) {
    assert(i >= 1 && i <= mNumRows);
    assert(j >= 1 && j <= mNumCols);
    return mData[i-1][j-1];
}   

const double& Matrix::operator()(int i, int j) const {
    assert(i >= 1 && i <= mNumRows);
    assert(j >= 1 && j <= mNumCols);
    return mData[i-1][j-1];
}

// ── Assignment ───────────────────────────────────────────────────────────────

Matrix& Matrix::operator=(const Matrix& other) {
    if (this == &other) return *this;
    Deallocate();
    mNumRows = other.mNumRows;
    mNumCols = other.mNumCols;
    Allocate();
    for (int i = 0; i < mNumRows; ++i)
        for (int j = 0; j < mNumCols; ++j)
            mData[i][j] = other.mData[i][j];
    return *this;
}

// ── Unary minus ──────────────────────────────────────────────────────────────

Matrix Matrix::operator-() const {
    Matrix result(mNumRows, mNumCols);
    for (int i = 0; i < mNumRows; ++i)
        for (int j = 0; j < mNumCols; ++j)
            result.mData[i][j] = -mData[i][j];
    return result;
}

// ── Binary arithmetic ────────────────────────────────────────────────────────

Matrix Matrix::operator+(const Matrix& other) const { // công 2 ma trận cùng kích thước, trả về ma trận mới là tổng của chúng
    assert(mNumRows == other.mNumRows && mNumCols == other.mNumCols);
    Matrix result(mNumRows, mNumCols);
    for (int i = 0; i < mNumRows; ++i)
        for (int j = 0; j < mNumCols; ++j)
            result.mData[i][j] = mData[i][j] + other.mData[i][j]; // cộng từng phần tử tương ứng của hai ma trận và lưu vào result
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const { // trừ 2 ma trận cùng logic với operator+ nhưng là trừ
    assert(mNumRows == other.mNumRows && mNumCols == other.mNumCols);
    Matrix result(mNumRows, mNumCols);
    for (int i = 0; i < mNumRows; ++i)
        for (int j = 0; j < mNumCols; ++j)
            result.mData[i][j] = mData[i][j] - other.mData[i][j];
    return result;
}

// Matrix multiplication: (m x k) * (k x n) = (m x n)
Matrix Matrix::operator*(const Matrix& other) const {
    assert(mNumCols == other.mNumRows); // điều kiện cần để nhân ma trận
    Matrix result(mNumRows, other.mNumCols);
    for (int i = 0; i < mNumRows; ++i) // duyệt từng hàng của ma trận đầu tiên
        for (int j = 0; j < other.mNumCols; ++j) // duyệt từng cột của ma trận thứ hai
            for (int k = 0; k < mNumCols; ++k) // duyệt từng phần tử trong hàng i của ma trận đầu tiên và cột j của ma trận thứ hai để tính tích vô hướng
                result.mData[i][j] += mData[i][k] * other.mData[k][j]; // tích vô hướng
    return result;
}

// Matrix * scalar
Matrix Matrix::operator*(double scalar) const {
    Matrix result(mNumRows, mNumCols);
    for (int i = 0; i < mNumRows; ++i)
        for (int j = 0; j < mNumCols; ++j)
            result.mData[i][j] = mData[i][j] * scalar; // nhân từng phần tử của ma trận với scalar và lưu vào result
    return result;
}

// Matrix * Vector: A is (m x n), v must have size n, returns Vector of size m
Vector Matrix::operator*(const Vector& v) const {
    assert(mNumCols == v.GetSize());
    Vector result(mNumRows);
    for (int i = 0; i < mNumRows; ++i) { // duyệt từng row
        double sum = 0.0;
        for (int j = 0; j < mNumCols; ++j)
            sum += mData[i][j] * v[j]; // tích vô hướng giữa row i của ma trận và vector v
        result[i] = sum;
    }
    return result;
}

// ── Transpose ────────────────────────────────────────────────────────────────

Matrix Matrix::Transpose() const {
    Matrix result(mNumCols, mNumRows);
    for (int i = 0; i < mNumRows; ++i)
        for (int j = 0; j < mNumCols; ++j)
            result.mData[j][i] = mData[i][j];
    return result;
}

// ── Determinant (recursive cofactor expansion) ───────────────────────────────
// For large matrices a LU approach is better, but this is correct and clear.

double Matrix::Determinant() const {
    assert(mNumRows == mNumCols);  // must be square
    int n = mNumRows;

    if (n == 1) return mData[0][0]; // nếu là matrix 1x1 thì det là chính nó
    if (n == 2) return mData[0][0]*mData[1][1] - mData[0][1]*mData[1][0]; // nếu là 2x2 thì det = ad - bc

    // General: expand along first row
    double det = 0.0;
    for (int col = 0; col < n; ++col) {
        // Build (n-1) x (n-1) minor — skip row 0, skip column col
        Matrix minor(n-1, n-1); // tạo ma trận con (minor) có kích thước (n-1) x (n-1)
        for (int r = 1; r < n; ++r) { // bỏ hàng 0 → bắt đầu từ r=1
            int mc = 0; // mc = cột trong minor, bắt đầu từ 0
            for (int c = 0; c < n; ++c) {
                if (c == col) continue; // bỏ cột đang xét
                minor.mData[r-1][mc++] = mData[r][c];
            }
        }
        double sign = (col % 2 == 0) ? 1.0 : -1.0; // dấu luân phiên: + - + - ...
        det += sign * mData[0][col] * minor.Determinant();
    }
    return det;
}

// ── Inverse via Gauss-Jordan elimination ─────────────────────────────────────
// Augment [A | I], then reduce to [I | A^{-1}]

Matrix Matrix::Inverse() const {
    assert(mNumRows == mNumCols);
    int n = mNumRows;

    // Build augmented matrix [A | I]
    Matrix aug(n, 2*n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            aug.mData[i][j] = mData[i][j];
        aug.mData[i][n + i] = 1.0;  // identity block
    }

    // Forward elimination with partial pivoting
    for (int col = 0; col < n; ++col) {
        // Find pivot row
        int pivot = col;
        for (int row = col+1; row < n; ++row)
            if (std::abs(aug.mData[row][col]) > std::abs(aug.mData[pivot][col])) // tìm hàng có phần tử trị tuyệt đối lớn nhất ở cột hiện tại để làm pivot
                pivot = row;

        // Swap rows
        std::swap(aug.mData[col], aug.mData[pivot]); // đổi chỗ hàng col với hàng pivot để đưa phần tử lớn nhất lên vị trí pivot
        // Mục tiêu làm sao là tìm và swap để phần tử ở vị trí (col, col) aka (đường chéo chính) có trị tuyệt đối lớn nhất có thể, giúp giảm thiểu sai số trong quá trình khử Gauss-Jordan

        double pivotVal = aug.mData[col][col];
        assert(std::abs(pivotVal) > 1e-14); // singular check (giá trị pivot quá nhỏ có thể dẫn đến sai số lớn, coi như ma trận không khả nghịch : 10^-14)

        // Scale pivot row so diagonal becomes 1
        for (int j = 0; j < 2*n; ++j)
            aug.mData[col][j] /= pivotVal; // chia toàn bộ hàng pivot cho giá trị pivot để đưa phần tử trên đường chéo về 1

        // Eliminate all other rows in this column
        for (int row = 0; row < n; ++row) {
            if (row == col) continue; // bỏ qua đường chéo chính (hiện có giá trị là 1)
            double factor = aug.mData[row][col];
            for (int j = 0; j < 2*n; ++j)
                aug.mData[row][j] -= factor * aug.mData[col][j]; // trừ factor * hàng pivot từ hàng hiện tại để đưa phần tử ở cột col về 0, tiến hành khử Gauss-Jordan để biến phần bên trái (A) thành ma trận đơn vị, đồng thời phần bên phải sẽ trở thành A^{-1}
        }
    }

    // Extract right half: A^{-1}
    Matrix inv(n, n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            inv.mData[i][j] = aug.mData[i][n + j];
    return inv;
}

// ── Moore-Penrose Pseudo-Inverse ─────────────────────────────────────────────
// For full column rank (m >= n): A^+ = (A^T A)^{-1} A^T
// For full row rank (m < n):     A^+ = A^T (A A^T)^{-1}
// Note: for singular or rank-deficient matrices an SVD-based approach
// is needed; this implementation assumes full rank.

Matrix Matrix::PseudoInverse() const {
    Matrix At = Transpose();
    if (mNumRows >= mNumCols) {
        // Tall/square matrix: (A^T A)^{-1} A^T
        Matrix AtA = At * (*this);
        return AtA.Inverse() * At;
    } else {
        // Wide matrix: A^T (A A^T)^{-1}
        Matrix AAt = (*this) * At;
        return At * AAt.Inverse();
    }
}

// ── Friends ───────────────────────────────────────────────────────────────────

Matrix operator*(double scalar, const Matrix& m) {
    return m * scalar; // mượn operator* đã định nghĩa trong class để thực hiện phép nhân (m * scalar) thay vì viết lại logic nhân trong hàm này, giúp tránh trùng lặp mã và đảm bảo tính nhất quán
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) { // in matrix
    for (int i = 0; i < m.mNumRows; ++i) {
        os << "[ ";
        for (int j = 0; j < m.mNumCols; ++j) {
            os << m.mData[i][j];
            if (j < m.mNumCols - 1) os << "\t";
        }
        os << " ]\n";
    }
    return os;
}
