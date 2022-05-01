//
// Created by renat on 20.03.2022.
//

//#include "classes.h"
//#include <iostream>
#include "ascii_stl_reader.cpp"
#include "transform_to_array.cpp"

int main(int argc, char const *argv[]){
    int default_plate_value = 0;
    int plate_size_x = 100;
    int plate_size_y = 100;

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

        std::vector<POINT> points;
        std::vector<POINT> boundary_points;
        for(size_t x = 0; x < objects[i].body.size(); x++){
            for(size_t y = 0; y < objects[i].body[x].size(); y++){
                if (objects[i].body[x][y] != -1){
                    points.push_back(POINT{int(x), int(y)});
                    if(objects[i].boundary_point(int(x), int(y), objects[i].body)){
                        boundary_points.push_back(POINT{int(x), int(y)});
                    }
                }
            }
        }
        objects[i].points = points;
        objects[i].boundary_points = boundary_points;
        std::cout << objects[i].points.size() << std::endl;
        std::cout << objects[i].boundary_points.size() << std::endl;

        objects[i].find_edges();
    }
    std::cout << "creating plate" << std::endl;
    Plate plate(objects[0].boundaries[1] + 90, objects[0].boundaries[5] + 90);
    plate.print_plate_info();

    std::cout << "empty grid ? " << objects[0].body.empty() << std::endl;

    plate.place_new_object(objects[0]);

    plate.save_plate_matrix_to_file("matrix");
    plate.print_boundaries_for_obj(0);

    plate.place_new_object(objects[1]);
    return GENERAL_SUCCESS;
}
