CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SRCDIR   = src
SRCS     = $(SRCDIR)/Vector.cpp \
           $(SRCDIR)/Matrix.cpp \
           $(SRCDIR)/LinearSystem.cpp \
           $(SRCDIR)/LinearRegression.cpp \
           $(SRCDIR)/main.cpp
TARGET   = project1

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	del /f /q $(TARGET).exe 2>nul || rm -f $(TARGET)