#include "Vector.h"
#include <cassert>
#include <cmath>
#include <iostream>

// ── Constructors & Destructor ────────────────────────────────────────────────

// Parameterised constructor: allocate mSize doubles, initialise to 0
Vector::Vector(int size) : mSize(size) {
    assert(size > 0); // size must be positive
    mData = new double[mSize]();  // () zero-initialises
}

// Copy constructor: deep copy — allocates new memory and copies values (nếu ko có copy constructor, compiler sẽ tạo ra một bản sao nông, dẫn đến double free khi cả hai đối tượng bị hủy)
Vector::Vector(const Vector& other) : mSize(other.mSize) {
    mData = new double[mSize];
    for (int i = 0; i < mSize; ++i)
        mData[i] = other.mData[i];
}

// Destructor: free heap memory to avoid memory leaks
Vector::~Vector() {
    delete[] mData;
}

// ── Indexing operators ───────────────────────────────────────────────────────

// operator[] — 0-based, bounds-checked with assert
double& Vector::operator[](int i) { // sửa (đọc và ghi)
    assert(i >= 0 && i < mSize); // kiểm tra i có nằm trong phạm vi hợp lệ không (0 đến mSize-1)
    return mData[i];
}
const double& Vector::operator[](int i) const { // chỉ đọc (const)
    assert(i >= 0 && i < mSize);
    return mData[i];
}

// operator() — 1-based (spec requirement: index from 1, not 0)
double& Vector::operator()(int i) {
    assert(i >= 1 && i <= mSize);
    return mData[i - 1];   // translate to 0-based internally
}
const double& Vector::operator()(int i) const {
    assert(i >= 1 && i <= mSize);
    return mData[i - 1];
}

// ── Assignment ───────────────────────────────────────────────────────────────

/*a = b
 │
 ├─ a và b cùng là 1 object? → return ngay, không làm gì
 │
 ├─ Kích thước khác nhau?
 │   ├─ Có → xóa bộ nhớ cũ, cấp phát mới
 │   └─ Không → dùng bộ nhớ cũ luôn
 │
 ├─ Copy từng phần tử từ b sang a
 │
 └─ Return *this (để hỗ trợ a = b = c)
 Khác với copy constructor, operator= phải xử lý trường hợp tự gán (a = a) và có thể tái sử dụng bộ nhớ nếu kích thước không đổi, trong khi copy constructor luôn tạo một đối tượng mới với bộ nhớ mới
 Tức a = b với constructor thì b chưa tồn tại, còn với operator= thì b đã tồn tại rồi, nên phải xử lý trường hợp tự gán và có thể tái sử dụng bộ nhớ nếu kích thước không đổi
 */

Vector& Vector::operator=(const Vector& other) {
    if (this == &other) return *this;  // self-assignment guard (nếu a và b cùng là 1 object, return ngay, không làm gì)
    // If sizes differ, reallocate
    if (mSize != other.mSize) { // khác size
        delete[] mData; // giải phóng bộ nhớ cũ
        mSize = other.mSize; // cập nhật kích thước mới
        mData = new double[mSize]; // cấp phát bộ nhớ mới
    }
    for (int i = 0; i < mSize; ++i) // copy dữ liệu từ other sang this
        mData[i] = other.mData[i];
    return *this; // hỗ trợ đọc và ghi a = b = c; đọc từ phải sang: b=c trước, rồi a=(kết quả của b=c)
}

// ── Unary minus ──────────────────────────────────────────────────────────────

Vector Vector::operator-() const { // đổi dấu tất cả phần tử của vector (có const để đảm bảo không thay đổi đối tượng hiện tại)
    Vector result(mSize); // tạo vector mới có cùng kích thước
    for (int i = 0; i < mSize; ++i)
        result.mData[i] = -mData[i]; // đổi dấu từng phần
    return result;
}

// ── Binary arithmetic ────────────────────────────────────────────────────────

Vector Vector::operator+(const Vector& other) const {
    assert(mSize == other.mSize); // bắt buộc cùng kích thước để cộng
    Vector result(mSize); // tạo vector mới để lưu kết quả
    for (int i = 0; i < mSize; ++i)
        result.mData[i] = mData[i] + other.mData[i]; // cộng từng phần tử
    return result;
}
// operator- tương tự như operator+
Vector Vector::operator-(const Vector& other) const {
    assert(mSize == other.mSize);
    Vector result(mSize);
    for (int i = 0; i < mSize; ++i)
        result.mData[i] = mData[i] - other.mData[i];
    return result;
}

// Vector * scalar (tương tự như operator+ nhưng nhân với scalar)
Vector Vector::operator*(double scalar) const {
    Vector result(mSize);
    for (int i = 0; i < mSize; ++i)
        result.mData[i] = mData[i] * scalar; // nhân từng phần tử với scalar
    return result;
}

// Dot product: v1 * v2 returns a scalar (tương tự như operator+ nhưng thay vì trả về vector mới, trả về một giá trị double là tổng của tích từng phần tử)
double Vector::operator*(const Vector& other) const {
    assert(mSize == other.mSize);
    double sum = 0.0;
    for (int i = 0; i < mSize; ++i)
        sum += mData[i] * other.mData[i]; // tích từng phần tử và cộng vào sum
    return sum;
// Vector v1 = [1, 2, 3];
// Vector v2 = [4, 5, 6];
// double result = v1 * v2;    result = 1*4 + 2*5 + 3*6 = 32
}

// ── Friend functions ─────────────────────────────────────────────────────────

// scalar * Vector (commutative)
Vector operator*(double scalar, const Vector& v) {
    return v * scalar; // mượn operator* đã định nghĩa trong class để thực hiện phép nhân (v * scalar) thay vì viết lại logic nhân trong hàm này, giúp tránh trùng lặp mã và đảm bảo tính nhất quán
}

// Stream output: [v1, v2, v3]
std::ostream& operator<<(std::ostream& os, const Vector& v) { // hàm này không phải là thành viên của class Vector, nên không có quyền truy cập trực tiếp vào mData và mSize, nhưng vì nó được khai báo là friend nên có thể truy cập được
    os << "["; // hàm in ra vector theo định dạng [v1, v2, v3]
    for (int i = 0; i < v.mSize; ++i) {
        os << v.mData[i];
        if (i < v.mSize - 1) os << ", ";
    }
    os << "]";
    return os;
}
