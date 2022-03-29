//
// Created by renat on 26.02.2022.
//

#ifndef PROJECT_CLASSES_H
#define PROJECT_CLASSES_H

#endif //PROJECT_CLASSES_H

#define PLACEMENT_ERROR -1
#define GENERAL_SUCCESS 0
#define RAND_GEN_ERROR -1

#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <cstdlib>
#include <ctime>

int init_random_seq(){
    srand((unsigned) time(0));
    return RAND_GEN_ERROR;
}

int get_random_int(int start = 0, int finish = 10){
    if (start <= finish) {
        return start + rand() % finish;
    }else return RAND_GEN_ERROR;
}

struct POINT{
    int x;
    int y;
};

struct VERTEX{
    float x;
    float y;
    float z;
};

struct NORMAL{
    float x;
    float y;
    float z;
};

struct FACE{
    std::vector<VERTEX> vertices;
    struct NORMAL n;
};

struct OBJECT{
    std::vector<FACE> object_faces;
    // min x max x min y max y min z max z
    std::vector<float> boundaries;
    std::vector<std::vector<int>> body;
    std::string filename = "default";
    int index;
};

class Object_on_plate{
public:
    int object_id;
    POINT ref_point; // upper left point of bounding box
    std::vector<POINT> boundary_points; //actual boundaries of object
};

class Plate{
public:
//    static unsigned int plates_counter;
    unsigned int objects_on_plate;
    unsigned int size_x, size_y;
    std::vector<Object_on_plate> objects;
    std::vector<POINT> occupied;
    std::vector<std::vector<int>> plate;
    int plate_default_placement_value;

    explicit Plate(unsigned int size_x = 100, unsigned int size_y = 100, int default_value = 0){
        this->size_x = size_x;
        this->size_y = size_y;
        this->plate_default_placement_value = default_value;
        gen_plate(default_value);
        this->objects_on_plate = 0;
//        plates_counter ++;
    }

    void print_plate_info(){
        std::cout << this->objects_on_plate << " obj on plate" << std::endl;
        std::cout << "x : " << this->size_x << std::endl;
        std::cout << "y : " << this->size_y << std::endl;
    }

    virtual ~Plate()
    {
        ;
//        plates_counter --;
    }

    void gen_plate(int default_value = 0){
        this->plate = std::vector<std::vector<int>> (
                this->size_x,
                std::vector<int>(this->size_y, default_value));
    }

    int print_plate(unsigned int width = 3){
        if (this->plate.empty()){
            return -1;
        }
        if (this->plate[0].empty()){
            return -2;
        }
        for(size_t i = 0; i < this->plate.size(); i++){
            for(size_t y = 0; y < this->plate[0].size(); y++){
                if (this->plate[i][y] != this->plate_default_placement_value){
                    std::cout << std::setw(width) << this->plate[i][y] << " ";
                } else {
                    std::cout << std::setw(width) << "_" << " ";
                }

            }
            std::cout << std::endl;
        }
        return 0;
    }

    int place_new_object(OBJECT obj){
        if (obj.body.empty()){
            std::cout << "ERROR!! empty body in object received" << std::endl;
            return PLACEMENT_ERROR;
        }
        if(obj.body[0].empty()){
            std::cout << "ERROR!! incorrect body frame received" << std::endl;
            return PLACEMENT_ERROR;
        }

        init_random_seq();
        std::cout << "placing" << std::endl;
        if (objects_on_plate == 0){
            int obj_frame_size_x = obj.body.size();
            int obj_frame_size_y = obj.body[0].size();
            unsigned int x_place = get_random_int(0, this->size_x - obj_frame_size_x);
            unsigned int y_place = get_random_int(0, this->size_y - obj_frame_size_y);
            std::cout << this->size_x - obj_frame_size_x << " " << this->size_y - obj_frame_size_y << std::endl;
            std::cout << obj_frame_size_x << " " << obj_frame_size_y << std::endl;
            std::cout << x_place << " " << y_place << std::endl;
            for(size_t i = 0; i < obj.body.size(); i++){
                for(size_t y = 0; y < obj.body[i].size(); y++){
                    if (obj.body[i][y] != -1){
                        this->plate[i + x_place][y + y_place] = 1;
                    }
                }
            }
        } else {
            ;
        }

        return GENERAL_SUCCESS;
    }


};

