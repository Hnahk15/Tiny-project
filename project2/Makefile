CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

SRC = main.cpp \
      Product.cpp \
      Cart.cpp \
      Order.cpp \
      User.cpp \
      Customer.cpp \
      Admin.cpp \
      RecommendationEngine.cpp \
      FileManager.cpp \
      ECommerceSystem.cpp

OBJ = $(SRC:.cpp=.o)
BIN = ecommerce

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all clean
