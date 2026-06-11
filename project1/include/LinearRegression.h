#ifndef LINEAR_REGRESSION_H
#define LINEAR_REGRESSION_H

#include "Matrix.h"
#include "Vector.h"
#include <string>
#include <vector>

/**
 * LinearRegression
 *
 * Loads the UCI Computer Hardware dataset, trains a linear model:
 *   PRP = x1*MYCT + x2*MMIN + x3*MMAX + x4*CACH + x5*CHMIN + x6*CHMAX
 * using Moore-Penrose pseudo-inverse (for over-determined systems),
 * and evaluates on a held-out test set with RMSE.
 */
class LinearRegression {
public:
    /**
     * Run the full pipeline:
     *  1. Load dataset from 'filepath'
     *  2. Split 80/20 train/test
     *  3. Solve for parameters using pseudo-inverse
     *  4. Evaluate RMSE on test set
     *  5. Print results
     */
    static void Run(const std::string& filepath);

private:
    // Build design matrix A (m x 6) and target vector b (m) from data rows
    static void BuildSystem(
        const std::vector<std::vector<double>>& data,
        Matrix& A, Vector& b);

    // Compute Root Mean Square Error
    static double RMSE(const Vector& predicted, const Vector& actual);
};

#endif // LINEAR_REGRESSION_H
