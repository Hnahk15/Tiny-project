# Tiny Projects - OOP C++

## Project 1 - Linear System

1. Mở terminal trong VS Code (Ctrl + `)
2. cd vào thư mục project1
3. make (nếu ko có make nhập: g++ -std=c++17 -Wall -O2 -o project1 src/Vector.cpp src/Matrix.cpp src/LinearSystem.cpp src/LinearRegression.cpp src/main.cpp)
4. ./project1 machine.data

---

## Project 2 - Smart E-Commerce System

1. Mở terminal trong VS Code (Ctrl + `)
2. cd vào thư mục project2
3. Compile:

**Linux / macOS:**
'''bash'''

make

./ecommerce


**Windows (CMD):**
cmd
g++ -std=c++17 -o ecommerce.exe main.cpp Product.cpp Cart.cpp Order.cpp User.cpp Customer.cpp Admin.cpp RecommendationEngine.cpp FileManager.cpp ECommerceSystem.cpp

Then:
ecommerce.exe


> Nếu lỗi `g++ not recognized`: tìm đường dẫn g++.exe trên máy rồi thay vào.

4. Đăng nhập mặc định — Admin: `admin` / `admin123`  
   Hoặc chọn Register để tạo tài khoản mới.
