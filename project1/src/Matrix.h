#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"
#include <cassert>
#include <iostream>

/**
 * Matrix class
 * - Private members: mNumRows, mNumCols (int), mData (double**)
 * - 1-based indexing via operator()(i, j)
 * - Supports +, -, *, scalar ops, assignment
 * - Determinant, Inverse, Pseudo-inverse (Moore-Penrose)
 */
class Matrix {
private:
    int     mNumRows;
    int     mNumCols;
    double** mData;   // mData[row][col], 0-based internally

    // Helper: allocate mData for current mNumRows x mNumCols
    void Allocate();
    // Helper: free mData
    void Deallocate();

public:
    // ── Constructors & Destructor ──────────────────────────────────────────
    Matrix(int numRows, int numCols);   // Allocates & zero-fills
    Matrix(const Matrix& other);        // Copy constructor (deep copy)
    ~Matrix();                          // Destructor

    // ── Accessors ─────────────────────────────────────────────────────────
    int GetNumRows() const { return mNumRows; }
    int GetNumCols() const { return mNumCols; }

    // ── Indexing (1-based) ─────────────────────────────────────────────────
    double& operator()(int i, int j);
    const double& operator()(int i, int j) const;

    // ── Assignment ────────────────────────────────────────────────────────
    Matrix& operator=(const Matrix& other);

    // ── Unary minus ───────────────────────────────────────────────────────
    Matrix operator-() const;

    // ── Binary arithmetic ─────────────────────────────────────────────────
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;    // Matrix * Matrix
    Matrix operator*(double scalar) const;           // Matrix * scalar
    Vector operator*(const Vector& v) const;         // Matrix * Vector

    // ── Mathematical methods ───────────────────────────────────────────────
    double  Determinant() const;          // Only valid for square matrices
    Matrix  Inverse() const;              // Only valid for square, nonsingular
    Matrix  Transpose() const;
    Matrix  PseudoInverse() const;        // Moore-Penrose: (A^T A)^{-1} A^T

    // ── Friends ───────────────────────────────────────────────────────────
    friend Matrix operator*(double scalar, const Matrix& m);
    friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
};

#endif // MATRIX_H
