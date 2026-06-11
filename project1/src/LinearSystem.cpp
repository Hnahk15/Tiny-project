#include "LinearSystem.h"
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <iostream>

// ── LinearSystem ─────────────────────────────────────────────────────────────

LinearSystem::LinearSystem(const Matrix& A, const Vector& b) {
    assert(A.GetNumRows() == A.GetNumCols());          // must be square
    assert(A.GetNumRows() == b.GetSize());             // A and b must be compatible

    mSize = b.GetSize();
    mpA   = new Matrix(A);   // deep copy so we own the data
    mpb   = new Vector(b);
}

LinearSystem::~LinearSystem() {
    delete mpA;
    delete mpb;
}

/**
 * Gaussian Elimination with Partial Pivoting
 *
 * Algorithm:
 *  1. Augment [A | b]
 *  2. For each column (pivot column):
 *     a. Find the row with the largest absolute value in that column
 *        (partial pivoting — improves numerical stability)
 *     b. Swap that row to the current pivot position
 *     c. Divide the pivot row by the pivot value (scale to 1)
 *     d. Subtract multiples of the pivot row from all rows below
 *  3. Back-substitution to extract x
 */
Vector LinearSystem::Solve() const {
    int n = mSize;

    // Work on copies so the original A and b are unchanged
    Matrix A(*mpA);
    Vector b(*mpb);

    // ── Forward Elimination ──────────────────────────────────────────────
    for (int col = 0; col < n; ++col) {
        // Step 1: Find pivot (largest |value| in column, from current row down)
        int pivotRow = col;
        double maxVal = std::abs(A(col+1, col+1));  // 1-based access
        for (int row = col + 1; row < n; ++row) {
            if (std::abs(A(row+1, col+1)) > maxVal) {
                maxVal = std::abs(A(row+1, col+1));
                pivotRow = row;
            }
        }
        assert(maxVal > 1e-14);  // if pivot is ~0, matrix is singular

        // Step 2: Swap current row with pivot row (both in A and b)
        if (pivotRow != col) {
            for (int j = 0; j < n; ++j) {
                double tmp = A(col+1, j+1);
                A(col+1, j+1) = A(pivotRow+1, j+1);
                A(pivotRow+1, j+1) = tmp;
            }
            double tmp = b(col+1);
            b(col+1) = b(pivotRow+1);
            b(pivotRow+1) = tmp;
        }

        // Step 3: Eliminate entries below the pivot in this column
        double pivot = A(col+1, col+1);
        for (int row = col + 1; row < n; ++row) {
            double factor = A(row+1, col+1) / pivot;
            for (int j = col; j < n; ++j)
                A(row+1, j+1) -= factor * A(col+1, j+1);
            b(row+1) -= factor * b(col+1);
        }
    }

    // ── Back-substitution ────────────────────────────────────────────────
    // After forward elimination: A is upper-triangular
    // Solve from last equation up
    Vector x(n);
    for (int i = n - 1; i >= 0; --i) {
        double sum = b(i+1);
        for (int j = i + 1; j < n; ++j)
            sum -= A(i+1, j+1) * x(j+1);
        x(i+1) = sum / A(i+1, i+1);
    }
    return x;
}


// ── PosSymLinSystem ──────────────────────────────────────────────────────────

PosSymLinSystem::PosSymLinSystem(const Matrix& A, const Vector& b)
    : LinearSystem(A, b)
{
    // Check symmetry: A(i,j) == A(j,i) within a tolerance
    int n = A.GetNumRows();
    for (int i = 1; i <= n; ++i)
        for (int j = i+1; j <= n; ++j)
            assert(std::abs(A(i,j) - A(j,i)) < 1e-10);
}

/**
 * Conjugate Gradient Method
 *
 * Ideal for large, sparse, symmetric positive definite (SPD) systems.
 * Much faster than Gaussian elimination for such matrices.
 *
 * Algorithm:
 *   x0 = 0 (initial guess)
 *   r0 = b - A*x0 = b
 *   p0 = r0
 *   For k = 0, 1, 2, ...:
 *     alpha_k = (r_k^T r_k) / (p_k^T A p_k)
 *     x_{k+1} = x_k + alpha_k * p_k
 *     r_{k+1} = r_k - alpha_k * A p_k
 *     if ||r_{k+1}|| < tol: break
 *     beta_k  = (r_{k+1}^T r_{k+1}) / (r_k^T r_k)
 *     p_{k+1} = r_{k+1} + beta_k * p_k
 */
Vector PosSymLinSystem::Solve() const {
    int    n   = mSize;
    double tol = 1e-10;
    int    maxIter = 10 * n;

    Vector x(n);           // initial guess: x = 0
    Vector r = *mpb;       // r = b - A*x = b (since x=0)
    Vector p = r;          // search direction p = r

    double rDotR = r * r;  // r^T * r (dot product)

    for (int iter = 0; iter < maxIter; ++iter) {
        if (std::sqrt(rDotR) < tol) break;

        Vector Ap = (*mpA) * p;          // A * p
        double pAp = p * Ap;             // p^T A p
        assert(std::abs(pAp) > 1e-15);  // avoid division by zero

        double alpha = rDotR / pAp;

        // x = x + alpha * p
        x = x + alpha * p;
        // r = r - alpha * A*p
        r = r - alpha * Ap;

        double rDotR_new = r * r;

        double beta = rDotR_new / rDotR;
        // p = r + beta * p
        p = r + beta * p;

        rDotR = rDotR_new;
    }
    return x;
}
