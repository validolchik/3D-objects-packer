#include<iostream>
//#include "ascii_stl_reader.cpp"
//#include "transform_to_array.cpp"
#include <sys/stat.h>
#include <fstream>
#include "helpfull_functions.cpp"

int main(int argc, char const *argv[])
{
    auto matrix = generate_2d_matrix(5, 5,true);

    print_2d_matrix_in_console(matrix);

    auto res_matrix = rotate_matrix_clockwise(matrix);
    res_matrix = rotate_matrix_clockwise(res_matrix);
    res_matrix = rotate_matrix_clockwise(res_matrix);
    res_matrix = rotate_matrix_clockwise(res_matrix);

    print_2d_matrix_in_console(res_matrix);

	return 0;
}