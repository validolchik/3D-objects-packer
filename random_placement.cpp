//
// Created by renat on 20.03.2022.
//

//#include "classes.h"
//#include <iostream>
//#include "ascii_stl_reader.cpp"
//#include "transform_to_array.cpp"
#include "reading_transformation.cpp"


int main(int argc, char const *argv[]){
    int default_plate_value = 0;
    int plate_size_x = 100;
    int plate_size_y = 100;

    init_random_seq();

    int reading_processing_status = read_transform_files(argv[1]);
    std::cout << "Reading and processing files status is " << reading_processing_status << std::endl;

    std::cout << "creating plate" << std::endl;
    Plate plate(objects[0].body.size() * 2 + 20, objects[0].body[0].size() * 2 + 20);
    plate.print_plate_info(grid_cell_size);

    int unplaced_objects = 0;

    for(auto obj : objects){
        if(get_random_int(0, 10) < 5){
            obj.rotate_object_90_degrees_clockwise();
        }
//        obj.rotate_object_90_degrees_clockwise();
        if (plate.place_new_object(obj) == PLACEMENT_ERROR){
            unplaced_objects++;
        };
    }

    std::cout << "unplaced " << unplaced_objects << " objects" << std::endl;
    std::cout << "placed " << plate.objects.size() << std::endl;
    if (plate.remove_object_at_index(plate.objects_on_plate-1)){
        std::cout << "removed object" << std::endl;
    }
    plate.save_matrix_to_file("after_deleting_last", plate.plate);
    std::cout << plate.objects_on_plate << std::endl;

    plate.remove_object_with_index(0);
    plate.save_matrix_to_file("after_deleting_with_index0", plate.plate);

    return GENERAL_SUCCESS;
}
