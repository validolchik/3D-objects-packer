//
// Created by renat on 26.02.2022.
//

#ifndef PROJECT_CLASSES_H
#define PROJECT_CLASSES_H

#endif //PROJECT_CLASSES_H

#define PLACEMENT_ERROR -1
#define GENERAL_SUCCESS 0
#define RAND_GEN_ERROR -1
#define OBJECT_NOT_FIT -2

#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <sys/stat.h>
#include <set>

int init_random_seq(){
    srand((unsigned) time(0));
    return RAND_GEN_ERROR;
}

int get_random_int(int start = 0, int finish = 10){
    if (start <= finish) {
        return start + rand() % finish;
    }else return RAND_GEN_ERROR;
}

bool is_path_exist(const std::string &s)
{
    struct stat buffer;
    return (stat (s.c_str(), &buffer) == 0);
}

struct POINT{
    int x;
    int y;
};

inline bool operator<(const POINT& lhs, const POINT& rhs)
{
    return lhs.x < rhs.x;
}

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
    std::set<POINT> boundary_points; //actual boundaries of object

    int set_ref_point(int x, int y){
        POINT p = POINT();
        p.x = x;
        p.y = y;
        this->ref_point = p;
        return GENERAL_SUCCESS;
    }

    int set_ref_point(POINT p){
        this->ref_point = p;
        return GENERAL_SUCCESS;
    }

    void insert_new_boundary_point(POINT p){
        this->boundary_points.insert(p);
    }

    void insert_new_boundary_point(int x, int y){
        POINT p = POINT();
        p.x = x;
        p.y = y;
        this->boundary_points.insert(p);
    }
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

    explicit Plate(unsigned int size_x = 100, unsigned int size_y = 100, int default_value = -1){
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

    void gen_plate(int default_value = -1){
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

    int save_plate_matrix_to_file(std::string filename, int width = 2){
        save_matrix_to_file(filename, this->plate, width);
        return 0;
    }

    bool boundary_point(int x, int y, std::vector<std::vector<int>> matrix){
        if (x == 0 or y == 0) return true;
        if (x == matrix.size()-1 or y == matrix[0].size() - 1) return true;

        int lower = x - 1;
        int upper = x + 1;
        int left = y - 1;
        int right = y + 1;
        if (matrix[lower][y] == -1
        or matrix[upper][y] == -1
        or matrix[x][left] == -1
        or matrix[x][right] == -1){
            return true;
        }

        return false;
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

        Object_on_plate new_object;
        new_object.object_id = obj.index;

        if (objects_on_plate == 0){
            if (obj.boundaries[0] > this->size_x or obj.boundaries[5] > this->size_y){
                std::cout << "object " << obj.index << " will not fit into plate" << std::endl;
                return OBJECT_NOT_FIT;
            }
            int obj_frame_size_x = obj.body.size();
            int obj_frame_size_y = obj.body[0].size();
            unsigned int x_place = get_random_int(0, this->size_x - obj_frame_size_x);
            unsigned int y_place = get_random_int(0, this->size_y - obj_frame_size_y);
            std::cout << this->size_x - obj_frame_size_x << " " << this->size_y - obj_frame_size_y << std::endl;
            std::cout << obj_frame_size_x << " " << obj_frame_size_y << std::endl;
            std::cout << x_place << " " << y_place << std::endl;

            new_object.set_ref_point(x_place, y_place);

            for(size_t i = 0; i < obj.body.size(); i++){
                for(size_t y = 0; y < obj.body[i].size(); y++){
                    if (obj.body[i][y] != -1){
                        this->plate[i + x_place][y + y_place] = obj.index;
                    }
                }
            }
            objects.push_back(new_object);
        } else {
            /*TODO
             *
            idea: take new object and check inside each placed obj

             */
            ;
        }

        return GENERAL_SUCCESS;
    }

    void print_boundaries_for_obj(unsigned int index, int width = 2){
        std::vector<std::vector<int>> print_plate(
                this->size_x,
                        std::vector<int>(this->size_y, -1));
        for(auto p : objects[index].boundary_points) {
            print_plate[p.x][p.y] = 1;
        }
        std::cout << "boundary points " << objects[index].boundary_points.size() << std::endl;
        save_matrix_to_file("boundaries", print_plate);
    }

    int save_matrix_to_file(std::string filename, std::vector<std::vector<int>> matrix, int width = 2){
        std::ofstream myfile;
        std::string representations_dir = "plate_matrices";
        if(!is_path_exist(representations_dir)) mkdir(&representations_dir[0], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        std::string output_filename = representations_dir + "/" + filename;
        std::cout << "saving boundaries to the file " << output_filename << std::endl;
        myfile.open (output_filename);
        for(auto row : matrix){
            for(auto value : row){
                if(value != -1){
                    myfile<< std::setw(width) << value << " ";
                }else{
                    myfile << std::setw(width) << "_" << " ";
                }
            }
            myfile << std::endl;
        }
        myfile.close();
        return 0;
    }
};

