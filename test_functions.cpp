#include<iostream>
#include "ascii_stl_reader.h"
#include "transform_to_array.cpp"


struct fPoint{
	float x;
	float y;
};


int main(int argc, char const *argv[])
{
	// fPoint v1 = {0, 0};
	// fPoint v2 = {1, 0};
	// fPoint v3 = {0, 1};
	// fPoint p = {0.5, 0.5};

	// VERTEX vv1 = {10.3, 3.3, 3.4};
	// VERTEX vv2 = {3.2, 2.1, 6.1};
	// VERTEX vv3 = {9.6, 2.34, 5.4};

	// FACE f;
	// f.vertices = {vv1, vv2, vv3};

	// // std::vector<float> boundaries = find_square_bounds_of_triangle(f);
	// std::vector<int> int_bounds = find_int_square_triangle(find_square_bounds_of_triangle(f));

	// for(float a : int_bounds) std::cout << a << " ";
	// std::cout << std::endl;

	// VERTEX p1 = {1, 1, 1};

	process_stl_file(argv[1]);
	// produce_empty_body_grid(objects[0]);
	std::cout << argv[1] << " processed" << std::endl;
	std::cout << objects[0].object_faces[0].vertices[0].x << std::endl;
	std::vector<std::vector<bool>> empty_grid = produce_empty_body_grid(objects[0]);
	std::cout << "empty_grid produced" << std::endl;
	fill_body_grid(objects[0], empty_grid);
	std::cout << "grid filled" << std::endl;
	assign_body_grid(objects[0], empty_grid);
	std::cout << "assigned grid to the body" << std::endl;

	for(int i = 0; i < empty_grid.size(); i++){
		for(int y = 0; y < empty_grid[i].size(); y++){
			if(empty_grid[i][y]) std::cout << 0;
			else std::cout << "_";
		}
		std::cout << std::endl;
	}

	return 0;
}