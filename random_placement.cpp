//
// Created by renat on 20.03.2022.
//

#include "classes.h"
#include <iostream>
//#include "ascii_stl_reader.h"
//#include "transform_to_array.cpp"

int plate_size_x = 20;
int plate_size_y = 20;


int main(int argc, char const *argv[]){
    int default_plate_value = 0;
    Plate plate(100, plate_size_x, plate_size_y);
    plate.print_plate(2);

}
