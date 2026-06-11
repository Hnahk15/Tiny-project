#include <iostream>
#include <cassert>
#include <cmath>
#include "Vector.h"
#include "Matrix.h"
#include "LinearSystem.h"
#include "LinearRegression.h"

// ────────────────────────────────────────────────────────────────────────────
// Helper: print a section header
// ────────────────────────────────────────────────────────────────────────────
static void section(const std::string& title) {
    std::cout << "\n========================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "========================================\n";
}

// ────────────────────────────────────────────────────────────────────────────
// Test 1: Vector operations
// ────────────────────────────────────────────────────────────────────────────
static void testVector() {
    section("Vector Class Tests");

    Vector v1(3), v2(3);
    v1(1) = 1.0; v1(2) = 2.0; v1(3) = 3.0;
    v2(1) = 4.0; v2(2) = 5.0; v2(3) = 6.0;

    std::cout << "v1 = " << v1 << "\n";
    std::cout << "v2 = " << v2 << "\n";
    std::cout << "v1 + v2 = " << (v1 + v2) << "\n";      // [5, 7, 9]
    std::cout << "v1 - v2 = " << (v1 - v2) << "\n";      // [-3,-3,-3]
    std::cout << "v1 * 2.0 = " << (v1 * 2.0) << "\n";    // [2, 4, 6]
    std::cout << "3.0 * v2 = " << (3.0 * v2) << "\n";    // [12,15,18]
    std::cout << "dot(v1,v2) = " << (v1 * v2) << "\n";   // 1*4+2*5+3*6=32
    std::cout << "-v1 = " << (-v1) << "\n";               // [-1,-2,-3]

    // Test 0-based [] (bounds-checked)
    assert(v1[0] == 1.0);
    assert(v1[2] == 3.0);

    // Test 1-based ()
    assert(v1(1) == 1.0);
    assert(v1(3) == 3.0);

    std::cout << "Vector tests PASSED.\n";
}

// ────────────────────────────────────────────────────────────────────────────
// Test 2: Matrix operations
// ────────────────────────────────────────────────────────────────────────────
static void testMatrix() {
    section("Matrix Class Tests");

    // 2x2 matrix for easy determinant/inverse verification
    Matrix A(2, 2);
    A(1,1) = 4.0; A(1,2) = 7.0;
    A(2,1) = 2.0; A(2,2) = 6.0;

    std::cout << "A =\n" << A;

    // Determinant: 4*6 - 7*2 = 10
    double det = A.Determinant();
    std::cout << "det(A) = " << det << "   (expected 10)\n";
    assert(std::abs(det - 10.0) < 1e-10);

    // Inverse of [[4,7],[2,6]] = [[0.6,-0.7],[-0.2,0.4]]
    Matrix Ainv = A.Inverse();
    std::cout << "inv(A) =\n" << Ainv;
    assert(std::abs(Ainv(1,1) - 0.6)  < 1e-10);
    assert(std::abs(Ainv(1,2) + 0.7)  < 1e-10);
    assert(std::abs(Ainv(2,1) + 0.2)  < 1e-10);
    assert(std::abs(Ainv(2,2) - 0.4)  < 1e-10);

    // A * inv(A) should be identity
    Matrix I = A * Ainv;
    std::cout << "A * inv(A) =\n" << I;

    // Transpose
    Matrix B(2, 3);
    B(1,1)=1; B(1,2)=2; B(1,3)=3;
    B(2,1)=4; B(2,2)=5; B(2,3)=6;
    std::cout << "B =\n" << B;
    std::cout << "B^T =\n" << B.Transpose();

    // Pseudo-inverse: a 3x2 matrix — pinv should give a 2x3 result
    Matrix C(3, 2);
    C(1,1)=1; C(1,2)=2;
    C(2,1)=3; C(2,2)=4;
    C(3,1)=5; C(3,2)=6;
    std::cout << "C =\n" << C;
    Matrix Cpinv = C.PseudoInverse();
    std::cout << "pinv(C) =\n" << Cpinv;

    std::cout << "Matrix tests PASSED.\n";
}

// ────────────────────────────────────────────────────────────────────────────
// Test 3: LinearSystem — Gaussian Elimination
// ────────────────────────────────────────────────────────────────────────────
static void testGaussian() {
    section("LinearSystem (Gaussian Elimination) Tests");

    // Solve:  2x + y - z = 8
    //        -3x - y + 2z = -11
    //        -2x + y + 2z = -3
    // Exact solution: x=2, y=3, z=-1
    Matrix A(3, 3);
    A(1,1)= 2; A(1,2)= 1; A(1,3)=-1;
    A(2,1)=-3; A(2,2)=-1; A(2,3)= 2;
    A(3,1)=-2; A(3,2)= 1; A(3,3)= 2;

    Vector b(3);
    b(1)= 8; b(2)=-11; b(3)=-3;

    LinearSystem sys(A, b);
    Vector x = sys.Solve();

    std::cout << "Ax = b, where:\n";
    std::cout << "A =\n" << A;
    std::cout << "b = " << b << "\n";
    std::cout << "x = " << x << "   (expected [2, 3, -1])\n";

    assert(std::abs(x(1) - 2.0) < 1e-10);
    assert(std::abs(x(2) - 3.0) < 1e-10);
    assert(std::abs(x(3) + 1.0) < 1e-10);
    std::cout << "Gaussian elimination PASSED.\n";
}

// ────────────────────────────────────────────────────────────────────────────
// Test 4: PosSymLinSystem — Conjugate Gradient
// ────────────────────────────────────────────────────────────────────────────
static void testCG() {
    section("PosSymLinSystem (Conjugate Gradient) Tests");

    // SPD matrix: [[4,1],[1,3]], b = [1, 2]
    // Exact solution: x ≈ [1/11, 7/11]
    Matrix A(2, 2);
    A(1,1) = 4.0; A(1,2) = 1.0;
    A(2,1) = 1.0; A(2,2) = 3.0;

    Vector b(2);
    b(1) = 1.0; b(2) = 2.0;

    PosSymLinSystem sys(A, b);
    Vector x = sys.Solve();

    std::cout << "SPD system Ax = b:\n";
    std::cout << "A =\n" << A;
    std::cout << "b = " << b << "\n";
    std::cout << "x = " << x << "\n";

    // Expected: x1 = 1/11 ≈ 0.0909, x2 = 7/11 ≈ 0.6364
    assert(std::abs(x(1) - 1.0/11.0) < 1e-8);
    assert(std::abs(x(2) - 7.0/11.0) < 1e-8);
    std::cout << "Conjugate Gradient PASSED.\n";
}

// ────────────────────────────────────────────────────────────────────────────
// Test 5: Under/Over-determined systems via Pseudo-Inverse
// ────────────────────────────────────────────────────────────────────────────
static void testUnderdetermined() {
    section("Over/Under-determined Systems via Pseudo-Inverse");

    // Over-determined (3 equations, 2 unknowns): least-squares solution
    // [1 1][x1]   [2]
    // [1 2][x2] = [3]   -> exact: x1=1, x2=1; here the system is consistent
    // [1 3]         [4]
    Matrix A(3, 2);
    A(1,1)=1; A(1,2)=1;
    A(2,1)=1; A(2,2)=2;
    A(3,1)=1; A(3,2)=3;

    Vector b(3);
    b(1)=2; b(2)=3; b(3)=4;

    Matrix pinvA = A.PseudoInverse();  // (A^T A)^{-1} A^T
    Vector x = pinvA * b;

    std::cout << "Over-determined system (3x2):\n";
    std::cout << "A =\n" << A << "b = " << b << "\n";
    std::cout << "x (least-squares) = " << x << "   (expected [1, 1])\n\n";

    assert(std::abs(x(1) - 1.0) < 1e-8);
    assert(std::abs(x(2) - 1.0) < 1e-8);

    std::cout << "Pseudo-inverse test PASSED.\n";
}

// ────────────────────────────────────────────────────────────────────────────
// Main
// ────────────────────────────────────────────────────────────────────────────
int main(int argc, char* argv[]) {
    // Part A tests
    testVector();
    testMatrix();
    testGaussian();
    testCG();
    testUnderdetermined();

    // Part B: Linear Regression
    // Pass dataset path as argument, or default to "machine.data"
    std::string dataPath = "machine.data";
    if (argc > 1) dataPath = argv[1];
    try {
        LinearRegression::Run(dataPath);
    } catch (const std::exception& e) {
        std::cerr << "[Part B] " << e.what() << "\n";
        std::cerr << "  -> Download dataset from:\n";
        std::cerr << "     https://archive.ics.uci.edu/ml/machine-learning-databases/cpu-performance/machine.data\n";
        std::cerr << "  -> Then run: ./project1 machine.data\n";
    }

    std::cout << "\n=== All tests completed! ===\n";
    return 0;
}
