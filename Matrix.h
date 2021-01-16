#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <string>
#include <iostream>

class Matrix{
    public:

    
    //using Element = std::pair<int, std::pair<size_t, size_t>>;
    using Element = float;

    /**
     * Konstruktor der Matrix
     */
    Matrix(size_t rows, size_t cols);

    /**
     * Zugriff auf einzelne Elemente mit a(i,j)
     */
    Element& operator() (size_t i, size_t j);

    /**
     * to String Method zum testen
     */
    std::string toString();

	
	
    private:
    size_t rows_;
    size_t cols_;

    
    std::vector<Element> data_;
};

#endif