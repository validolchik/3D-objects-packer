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

    std::vector<int> indexes_shuffle;
    for(int i = 0; i < objects.size(); i++){
        indexes_shuffle.push_back(i);
    }
    shuffle_int_vector(indexes_shuffle);

    std::vector<int> chromosome;

    for(int index : indexes_shuffle){
        Object &obj = objects[index];
        int rotate_count = get_random_int(0, 3);
        std::cout << "rotating " << rotate_count << "times" <<std::endl;
//        save_matrix_to_file("obj_" + std::to_string(index) + "before_rotation", obj.body);
        for(int i = 0; i < rotate_count; i++){
            obj.rotate_object_90_degrees_clockwise();
        }
//        save_matrix_to_file("obj_" + std::to_string(index) + "after_rotation", obj.body);

        if (plate.place_new_object(obj) == PLACEMENT_ERROR){
            unplaced_objects++;
        } else{
            chromosome.push_back(index);
        }
    }

    std::cout << "current chromosome" << std::endl;
    for(int ind : chromosome){
        std::cout << ind << " ";
    }
    std::cout << std::endl;

    std::cout << "unplaced " << unplaced_objects << " objects" << std::endl;
    std::cout << "placed " << plate.objects.size() << std::endl;

    Individual ind(0, chromosome, plate);

    save_matrix_to_file("before_mutation", ind.ind_plate.plate);
    ind.mutation(objects);

    save_matrix_to_file("mutation", ind.ind_plate.plate);
    return GENERAL_SUCCESS;
}
