#ifndef VECTOR_H // define nếu chưa được định nghĩa, tránh include nhiều lần
#define VECTOR_H

#include <cassert>
#include <iostream>

/**
 * Vector class
 * - Private members: mSize (int), mData (double*)
 * - 1-based indexing via operator()
 * - Bounds-checked [] operator
 * - Supports +, -, *, unary -, assignment
 */
class Vector {
private:
    int     mSize;   // number of elements
    double* mData;   // pointer to heap-allocated array

public:
    // ── Constructors & Destructor ──────────────────────────────────────────
    explicit Vector(int size);          // Allocate and zero-fill
    Vector(const Vector& other);        // Copy constructor (deep copy)
    ~Vector();                          // Destructor — frees mData

    // ── Accessors ─────────────────────────────────────────────────────────
    int GetSize() const { return mSize; }

    // ── Indexing operators ────────────────────────────────────────────────
    // operator[] : 0-based, bounds-checked via assert
    double& operator[](int i);
    const double& operator[](int i) const;

    // operator() : 1-based (as required by the project spec)
    double& operator()(int i);
    const double& operator()(int i) const;

    // ── Assignment ────────────────────────────────────────────────────────
    Vector& operator=(const Vector& other);

    // ── Unary minus ───────────────────────────────────────────────────────
    Vector operator-() const;

    // ── Binary arithmetic ─────────────────────────────────────────────────
    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    Vector operator*(double scalar) const;          // Vector * scalar
    double operator*(const Vector& other) const;    // dot product

    // ── Friends ───────────────────────────────────────────────────────────
    friend Vector operator*(double scalar, const Vector& v); // scalar * Vector
    friend std::ostream& operator<<(std::ostream& os, const Vector& v);
};

#endif // VECTOR_H
