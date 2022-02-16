#include<iostream>
#include "ascii_stl_reader.h"
#include "transform_to_array.cpp"
//#include <sys/stat.h>
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
    std::string file_with_models(argv[1]);

    read_filenames(file_with_models);

    for (int i = 0; i < models_filenames.size(); i++){
        process_stl_file(models_filenames[i]);
        std::cout << models_filenames[i] << " processed" << std::endl;

        objects[i].index = i;

        shift_object_to_zero(objects[i]);
        objects[i].boundaries = find_boundaries(objects[i].object_faces);

        std::cout << "boundaries after shift: ";
        for (float b : objects[i].boundaries) std::cout << b << " ";
        std::cout << std::endl;

        std::vector<std::vector<int>> grid = produce_empty_body_grid(objects[i]);
        std::cout << "empty_grid produced" << std::endl;

        fill_body_grid(objects[i], grid);
        std::cout << "grid filled" << std::endl;

        assign_body_grid(objects[i], grid);
        std::cout << "assigned grid to the body" << std::endl;

        std::ofstream myfile;
        std::string representations_dir = "object_matrices";
        if(!is_path_exist(representations_dir)) mkdir(&representations_dir[0], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        std::string output_filename = representations_dir + "/" + std::to_string(i) + "representation";
        std::cout << "writing representation to the file " << output_filename << std::endl;
        myfile.open (output_filename);
        for(int ii = 0; ii < grid.size(); ii++){
            for(int y = 0; y < grid[ii].size(); y++){
                if(grid[ii][y] != -1) myfile << objects[i].index;
                else myfile << "_";
            }
            myfile << std::endl;
        }
        myfile.close();
    }


	return 0;
}