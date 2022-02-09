#include<iostream>
#include "ascii_stl_reader.h"
#include "transform_to_array.cpp"
#include <fstream>

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
	std::cout << argv[1] << " processed" << std::endl;
	
	shift_object_to_zero(objects[0]);
	objects[0].boundaries = find_boundaries(objects[0].object_faces);

	std::cout << "boundaries after shift: ";
	for (float b : objects[0].boundaries) std::cout << b << " ";
	std::cout << std::endl; 

	std::vector<std::vector<bool>> empty_grid = produce_empty_body_grid(objects[0]);
	std::cout << "empty_grid produced" << std::endl;
	
	fill_body_grid(objects[0], empty_grid);
	std::cout << "grid filled" << std::endl;
	
	assign_body_grid(objects[0], empty_grid);
	std::cout << "assigned grid to the body" << std::endl;

	std::ofstream myfile;
	myfile.open ("square_representation_of_model");
	for(int i = 0; i < empty_grid.size(); i++){
		for(int y = 0; y < empty_grid[i].size(); y++){
			if(empty_grid[i][y]) myfile << 0;
			else myfile << "_";
		}
		myfile << std::endl;
	}
	myfile.close();

	return 0;
}