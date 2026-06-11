#include "LinearRegression.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <random>

// ── Dataset loading ──────────────────────────────────────────────────────────
// machine.data format (10 comma-separated fields per row):
//   col 0: vendor name  (string, skip)
//   col 1: model name   (string, skip)
//   col 2: MYCT         (int) — machine cycle time in nanoseconds
//   col 3: MMIN         (int) — minimum main memory in KB
//   col 4: MMAX         (int) — maximum main memory in KB
//   col 5: CACH         (int) — cache memory in KB
//   col 6: CHMIN        (int) — minimum channels
//   col 7: CHMAX        (int) — maximum channels
//   col 8: PRP          (int) — published relative performance  ← TARGET
//   col 9: ERP          (int) — estimated (from paper, ignored)

static std::vector<std::vector<double>> LoadDataset(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + filepath);

    std::vector<std::vector<double>> rows;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        // trim carriage return (Windows line endings)
        if (!line.empty() && line.back() == '\r') line.pop_back();

        std::vector<std::string> tokens;
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token, ','))
            tokens.push_back(token);

        if (tokens.size() < 10) continue;

        // Parse numeric columns 2..8 (MYCT, MMIN, MMAX, CACH, CHMIN, CHMAX, PRP)
        std::vector<double> row;
        bool ok = true;
        for (int i = 2; i <= 8; ++i) {
            try { row.push_back(std::stod(tokens[i])); }
            catch (...) { ok = false; break; }
        }
        if (ok && row.size() == 7) rows.push_back(row);
    }
    return rows;
}

// ── Build design matrix ──────────────────────────────────────────────────────
// A is m×7: column 1 is all 1s (intercept/bias), columns 2-7 are features.
// Model: PRP = x0 + x1*MYCT + x2*MMIN + x3*MMAX + x4*CACH + x5*CHMIN + x6*CHMAX
//
// WHY ADD A BIAS COLUMN?
// Without bias, the hyperplane is forced through the origin, causing higher
// RMSE and sometimes negative predictions (since PRP is always > 0).
// Adding a column of 1s lets the pseudo-inverse learn a free intercept x0.

void LinearRegression::BuildSystem(
    const std::vector<std::vector<double>>& data,
    Matrix& A, Vector& b)
{
    int m = (int)data.size();
    A = Matrix(m, 7);   // m rows, 7 cols (1 bias + 6 features)
    b = Vector(m);
    for (int i = 0; i < m; ++i) {
        A(i+1, 1) = 1.0;                  // bias / intercept
        for (int j = 0; j < 6; ++j)
            A(i+1, j+2) = data[i][j];     // MYCT..CHMAX
        b(i+1) = data[i][6];              // PRP (target)
    }
}

// ── RMSE ─────────────────────────────────────────────────────────────────────

double LinearRegression::RMSE(const Vector& predicted, const Vector& actual) {
    assert(predicted.GetSize() == actual.GetSize());
    int n = predicted.GetSize();
    double sum = 0.0;
    for (int i = 1; i <= n; ++i) {
        double diff = predicted(i) - actual(i);
        sum += diff * diff;
    }
    return std::sqrt(sum / n);
}

// ── Main pipeline ─────────────────────────────────────────────────────────────

void LinearRegression::Run(const std::string& filepath) {
    std::cout << "=== Part B: Linear Regression — CPU Performance ===\n\n";

    // 1. Load data
    auto data = LoadDataset(filepath);
    int total = (int)data.size();
    std::cout << "Loaded " << total << " instances.\n";
    if (total == 0) throw std::runtime_error("No valid rows parsed from dataset.");

    // 2. Shuffle with fixed seed (reproducible) then split 80/20
    std::mt19937 rng(42);
    std::shuffle(data.begin(), data.end(), rng);

    int trainSize = (int)(total * 0.8);
    int testSize  = total - trainSize;

    std::vector<std::vector<double>> trainData(data.begin(), data.begin() + trainSize);
    std::vector<std::vector<double>> testData (data.begin() + trainSize, data.end());

    std::cout << "Training set: " << trainSize << " instances\n";
    std::cout << "Test set:     " << testSize  << " instances\n\n";

    // 3. Build training system: A_train (167×7), b_train (167)
    Matrix A_train(1, 1);
    Vector b_train(1);
    BuildSystem(trainData, A_train, b_train);

    // 4. Solve via Moore-Penrose pseudo-inverse (Least Squares)
    //    Problem: A_train * x ≈ b_train  (over-determined: 167 equations, 7 unknowns)
    //    Normal equations: (A^T A) x = A^T b
    //    Solution:         x = (A^T A)^{-1} A^T b  =  pinv(A) * b
    Matrix pinvA = A_train.PseudoInverse();
    Vector x     = pinvA * b_train;   // x = [x0(bias), x1..x6]

    std::cout << "Learned parameters:\n";
    std::cout << "  x0 (bias):  " << x(1) << "\n";
    std::cout << "  x1 (MYCT):  " << x(2) << "\n";
    std::cout << "  x2 (MMIN):  " << x(3) << "\n";
    std::cout << "  x3 (MMAX):  " << x(4) << "\n";
    std::cout << "  x4 (CACH):  " << x(5) << "\n";
    std::cout << "  x5 (CHMIN): " << x(6) << "\n";
    std::cout << "  x6 (CHMAX): " << x(7) << "\n\n";

    // 5. Evaluate
    Matrix A_test(1, 1);
    Vector b_test(1);
    BuildSystem(testData, A_test, b_test);

    Vector trainPred = A_train * x;
    Vector testPred  = A_test  * x;

    double trainRMSE = RMSE(trainPred, b_train);
    double testRMSE  = RMSE(testPred,  b_test);

    // Compute mean of actual PRP for R² calculation
    double meanPRP = 0.0;
    for (int i = 1; i <= b_test.GetSize(); ++i) meanPRP += b_test(i);
    meanPRP /= b_test.GetSize();
    double ssTot = 0.0, ssRes = 0.0;
    for (int i = 1; i <= b_test.GetSize(); ++i) {
        double diff = b_test(i) - meanPRP;
        ssTot += diff * diff;
        double err = testPred(i) - b_test(i);
        ssRes += err * err;
    }
    double R2 = 1.0 - ssRes / ssTot;

    std::cout << "=== Evaluation ===\n";
    std::cout << "Training RMSE : " << trainRMSE << "\n";
    std::cout << "Test RMSE     : " << testRMSE  << "\n";
    std::cout << "Test R²       : " << R2        << "  (1.0 = perfect)\n\n";

    // 6. Print sample predictions
    std::cout << "=== Sample Predictions (test set, first 10) ===\n";
    std::cout << "  Predicted\tActual\tError\n";
    int show = std::min(10, testSize);
    for (int i = 1; i <= show; ++i) {
        std::cout << "  " << testPred(i) << "\t\t"
                  << b_test(i) << "\t"
                  << (testPred(i) - b_test(i)) << "\n";
    }
}
