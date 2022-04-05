#include<iostream>
#include "ascii_stl_reader.cpp"
#include "transform_to_array.cpp"
#include <sys/stat.h>
#include <fstream>

int main(int argc, char const *argv[])
{
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
        std::cout << "empty ?"<< grid.empty() << std::endl;

        grid = fill_body_grid(objects[i], grid);
        std::cout << "grid filled" << std::endl;

        assign_body_grid(objects[i], grid);
        std::cout << "assigned grid to the body" << std::endl;
        std::cout << objects[i].body.empty() << " empty?" << std::endl;

        std::ofstream myfile;
        std::string representations_dir = "object_matrices";
        if(!is_path_exist(representations_dir)) mkdir(&representations_dir[0], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        std::string output_filename = representations_dir + "/" + std::to_string(i) + "representation";
        std::cout << "writing representation to the file " << output_filename << std::endl;
        myfile.open (output_filename);
        for(int ii = 0; ii < objects[i].body.size(); ii++){
            for(int y = 0; y < objects[i].body[ii].size(); y++){
                if(objects[i].body[ii][y] != -1) myfile << objects[i].index;
//                if(grid[ii][y] != -1) myfile << grid[ii][y];
                else myfile << "_";
            }
            myfile << std::endl;
        }
        myfile.close();
    }
	return 0;
}