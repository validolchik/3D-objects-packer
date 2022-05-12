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

    init_random_seq();

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
    Plate plate(objects[0].boundaries[1] * 3, objects[0].boundaries[5] * 3);
    plate.print_plate_info();

    int unplaced_objects = 0;

    for(auto obj : objects){
//        obj.rotate_object_90_degrees_clockwise();
        if (plate.place_new_object(obj) == PLACEMENT_ERROR){
            unplaced_objects++;
        };
    }

    std::cout << "unplaced " << unplaced_objects << " objects" << std::endl;

    plate.remove_object_at_index(plate.objects_on_plate-1);
    plate.save_matrix_to_file("after_deleting_last", plate.plate);
    std::cout << plate.objects_on_plate << std::endl;

    plate.remove_object_with_index(0);
    plate.save_matrix_to_file("after_deleting_with_index0", plate.plate);

    return GENERAL_SUCCESS;
}
