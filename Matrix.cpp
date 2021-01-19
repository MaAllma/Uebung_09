#include "Matrix.h"
#include "ParsingException.h"
#include <string>
#include <iostream>



Matrix::Matrix(size_t rows, size_t cols)
    : rows_(rows),
      cols_(cols),
      data_(rows * cols) {
}


 std::string Matrix::toString(){
  std::string result = "Matrix: ";
  for (size_t i = 0; i < rows_ * cols_; i++){
    if(i%cols_ == 0){
      result = result + "\n" + std::to_string(data_[i]) + " ";
    } else result = result + std::to_string(data_[i]) + " ";
  }
  result = result + "\n";
  return result;
}


Matrix::Element& Matrix::operator() (size_t row, size_t column){
    if (row < rows_ && column < cols_){
    //im Vektor stehen quasi alle Zeilen hintereinander -> i mal die Zeilen durchgehen, dort dann die j_te Spalte
    return data_[row * cols_ + column];
    } else {
      if (row >= rows_){
        throw ParsingException("Error in Matrix with the rowNr");
      }
      if (column >= cols_){
        throw ParsingException("Error in Matrix with the colNr");
      }
      else throw ParsingException("Error in Matrix");	
		
	}
}




	
	