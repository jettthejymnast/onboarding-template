#pragma once

#include <cstddef>
#include <vector>

// Starter Grid for the 2D heat-diffusion problem.
//
// The evaluation harness uses operator() to set initial conditions and to read
// results; it never touches your internal storage. Keep this interface,
// everything else is yours.
class Grid {
private:
  std::size_t rows_;
  std::size_t cols_;

  std::vector <double> g = {}; //1D Vector over 2D for contiguous memory allocation to fully capitalize on future threading and SIMD implementations

public:
  Grid(std::size_t rows, std::size_t cols);

  double& operator()(std::size_t i, std::size_t j);
  double  operator()(std::size_t i, std::size_t j) const;

  std::size_t getrows();
  std::size_t getcols();
};  

void apply_stencil(const Grid& old_grid, Grid& new_grid);


Grid::Grid(std::size_t rows, std::size_t cols) {
    rows_ = rows;
    cols_ = cols;

    for (std::size_t y = 0; y < rows; y++) {
        for (std::size_t x = 0; x < cols; x++) {
            g.push_back(0);
        }
    }
}



double& Grid::operator()(std::size_t i, std::size_t j) {
    return g[(i*cols_)+j];
}

double  Grid::operator()(std::size_t i, std::size_t j) const {
    return g[(i*cols_)+j];
}

std::size_t Grid::getrows(){
    return rows_;
}

std::size_t Grid::getcols(){
    return cols_;
}


void apply_stencil(const Grid& old_grid, Grid& new_grid) {
  std::size_t row = (new_grid.getrows() - 1); 
  std::size_t col = (new_grid.getcols() - 1);

  for (std::size_t x = 0; x <= col; x++) {
    new_grid(0, x) = old_grid(0, x);
    new_grid(row, x) = old_grid(row, x);
  }

  for (std::size_t y = 1; y < row; y++) {
    new_grid(y, 0) = old_grid(y, 0);
    new_grid(y, col) = old_grid(y, col); 
  }

  for (std::size_t y = 1; y < (row); y++) {
      for (std::size_t x = 1; x < col; x++) {
        new_grid(y, x) = (0.5 * old_grid(y, x)) + (0.125 * old_grid((y-1), x)) + (0.125 * old_grid((y), (x-1))) + (0.125 * old_grid((y), (x+1))) + (0.125 * old_grid((y+1), x));
      }
    }
}

