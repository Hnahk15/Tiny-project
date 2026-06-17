* File Structure:

File	                            Role
Vector.h / Vector.cpp:	            1D vector class with full operator support
Matrix.h / Matrix.cpp:	            2D matrix class with determinant, inverse, pseudo-inverse
LinearSystem.h / LinearSystem.cpp:	Gaussian elimination + Conjugate Gradient solver
LinearRegression.h / .cpp:	        Part B: dataset loader, least-squares regression, RMSE/R2
main.cpp:	                        Test driver — runs all unit tests then Part B pipeline
Makefile:   	                    Build system (Windows-compatible via MSYS2)
