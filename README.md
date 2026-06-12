# Tiny Projects - OOP C++

Open terminal in VS Code (`Ctrl + ~`) and navigate to the project directory:
```bash
cd "Tiny-project"
Project 1 - Linear System
Open terminal in VS Code (Ctrl + ~) and ensure you are in the Tiny-project directory.

Navigate to the project1 directory:

Bash
cd "project1"
Compile the project:

If make is available, run:

Bash
make
If make is not available, run:

Bash
g++ -std=c++17 -Wall -O2 -o project1 src/Vector.cpp src/Matrix.cpp src/LinearSystem.cpp src/LinearRegression.cpp src/main.cpp
Run the program with the data file:

Bash
./project1 data/machine.data
Project 2 - Smart E-Commerce System
Open terminal in VS Code (Ctrl + ~) and ensure you are in the Tiny-project directory.

Navigate to the project2 directory:

Bash
cd "project 2"
Compile and run the program:

Linux / macOS:

Bash
make
./ecommerce
Windows (CMD):

Bash
g++ -std=c++17 -o ecommerce.exe main.cpp Product.cpp Cart.cpp Order.cpp User.cpp Customer.cpp Admin.cpp RecommendationEngine.cpp FileManager.cpp ECommerceSystem.cpp
./ecommerce.exe
💡 If g++ not recognized error occurs: find the path to g++.exe on your system and replace it.

Default login — Admin: admin / admin123

Or select Register to create a new account.