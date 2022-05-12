//
// Created by renat on 10.05.22.
//

#include <cstdlib>
#include <vector>

std::vector<std::vector<int>> generate_2d_matrix(unsigned int x = 5, unsigned int y = 5, bool fill_in_order = false, int def = 0)
{
    std::vector<std::vector<int>> res(
            x,
            std::vector<int>(y, def));
    if (!fill_in_order){
        return res;
    }
    int count = 1;
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y;j++){
            res[i][j] = count++;
        }
    }

    return res;
}

void print_2d_matrix_in_console(std::vector<std::vector<int>> matrix){
    for(auto row : matrix){
        for(auto value : row){
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

// Function to rotate the matrix 90 degree clockwise
std::vector<std::vector<int>> rotate_matrix_clockwise(std::vector<std::vector<int>> matrix)
{
    if(matrix.empty()){
        return matrix;
    }

    int matrix_size_x = matrix.size();
    int matrix_size_y = matrix[0].size();

    std::vector<std::vector<int>> res(
            matrix_size_y,
            std::vector<int>(matrix_size_x, 0));
    for(int i = 0; i < matrix_size_x; i++){
        for(int j = 0; j < matrix_size_y;j++){
            res[j][matrix_size_x-1-i] = matrix[i][j];
        }
    }

    return res;
}