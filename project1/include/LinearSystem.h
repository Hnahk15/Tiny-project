#ifndef LINEAR_SYSTEM_H
#define LINEAR_SYSTEM_H

#include "Matrix.h"
#include "Vector.h"

/**
 * LinearSystem — solves Ax = b via Gaussian Elimination with partial pivoting.
 *
 * Design decisions (as required by spec):
 *  - No default constructor (force user to provide A and b)
 *  - Copy constructor made private to prevent accidental copying
 *  - mSize is derived from A and b, not set by user
 *  - Solve() is virtual so derived classes can override
 */
class LinearSystem {
private:
    // Prevent copy construction (spec requirement)
    LinearSystem(const LinearSystem&);
    LinearSystem& operator=(const LinearSystem&);

protected:
    // Protected so derived class PosSymLinSystem can access them
    int     mSize;   // n (the system dimension)
    Matrix* mpA;     // pointer to the coefficient matrix (n x n)
    Vector* mpb;     // pointer to the right-hand side vector (n)

public:
    // Constructor: takes A and b, derives mSize, validates dimensions
    LinearSystem(const Matrix& A, const Vector& b);

    // Virtual destructor — important when using polymorphism
    virtual ~LinearSystem();

    // Solve Ax = b; returns solution vector x
    // virtual so PosSymLinSystem can override with Conjugate Gradient
    virtual Vector Solve() const;
};


/**
 * PosSymLinSystem — derived from LinearSystem.
 * Solves Ax = b using the Conjugate Gradient (CG) method.
 * Requires A to be symmetric positive definite (SPD).
 * Performs a symmetry check on construction.
 */
class PosSymLinSystem : public LinearSystem {
public:
    PosSymLinSystem(const Matrix& A, const Vector& b);

    // Override Solve with the Conjugate Gradient method
    Vector Solve() const override;
};

#endif // LINEAR_SYSTEM_H
