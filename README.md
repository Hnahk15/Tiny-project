# How to run Projects
Clone the repo using this command:

```bash
git clone https://github.com/Hnahk15/Tiny-project.git
cd Tiny-project
```

### Project 1 
To run project 1, type in these commands:

Change directory to project1 folder:
`
cd project1
`

Run this command line:

```bash
g++ -std=c++17 -Wall -Wextra -O2 -I include -o project1 src/Vector.cpp src/Matrix.cpp src/LinearSystem.cpp src/LinearRegression.cpp src/main.cpp

./project1 data/machine.data
```

### Project 2
To run project 2, type in these commands:

Change directory to project2 folder:

`
cd project2
`

Run this command line:

```bash
g++ -std=c++17 -I include -o ecommerce.exe src/main.cpp src/Product.cpp src/Cart.cpp src/Order.cpp src/User.cpp src/Customer.cpp src/Admin.cpp src/RecommendationEngine.cpp src/FileManager.cpp src/ECommerceSystem.cpp

./ecommerce.exe
```