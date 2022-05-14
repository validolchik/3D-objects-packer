//
// Created by renat on 26.02.2022.
//

#ifndef PROJECT_CLASSES_H
#define PROJECT_CLASSES_H

#endif //PROJECT_CLASSES_H

#define PLACEMENT_ERROR 1234
#define GENERAL_SUCCESS 0
#define RAND_GEN_ERROR 2345
#define OBJECT_NOT_FIT 3456
#define OCCUPIED_PLACE 1
#define EMPTY_SPACE -1


#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <cstdlib>
#include <ctime>
#include <sys/stat.h>
#include <set>
#include <map>

#include "helpfull_functions.cpp"

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
    return lhs.y < rhs.y or lhs.x > rhs.x;
}

inline bool operator==(const POINT& lhs, const POINT& rhs)
{
    return lhs.x == rhs.x and lhs.y == rhs.y;
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

class Object{
public:
    std::vector<FACE> object_faces;
    // min x max x min y max y min z max z
    std::vector<float> boundaries;
    std::vector<std::vector<int>> body;
    std::string filename = "default";
    int index;
    std::vector<POINT> boundary_points; //actual boundaries of object
    std::vector<POINT> points;
    std::set<POINT> edges; //four points in the edges
    int rotated_clockwise_90_counter = 0;

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

    void find_edges(){
        std::set<POINT> possible_candidate;
        bool finding_edge = true;
        int column = 0;
        while(finding_edge) {
            for (int i = 0; i < this->body.size(); i++) {
                if (this->body[i][column] != -1) {
                    possible_candidate.insert(POINT{i, column});
                    finding_edge = false;
//                    break;
                }
            }
            column++;
        }

        column = this->body[0].size()-1;
        finding_edge = true;
        while(finding_edge) {
            for (int i = 0; i < this->body.size(); i++) {
                if (this->body[i][column] != -1) {
                    possible_candidate.insert(POINT{i, column});
                    finding_edge = false;
//                    break;
                }
            }
            column--;
        }


        int row = 0;
        finding_edge = true;
        while(finding_edge) {
            for (int i = 0; i < this->body[0].size(); i++) {
                if (this->body[row][i] != -1) {
                    possible_candidate.insert(POINT{row, i});
                    finding_edge = false;
//                    break;
                }
            }
            row++;
        }

        row = this->body.size() - 1;
        finding_edge = true;
        while(finding_edge) {
            for (int i = 0; i < this->body[0].size(); i++) {
                if (this->body[row][i] != -1) {
                    possible_candidate.insert(POINT{row, i});
                    finding_edge = false;
//                    break;
                }
            }
            row--;
        }

        POINT p1 = {0, 0};
        POINT p2 = {body.size()-1, body[0].size()-1};
        POINT p3 = {body.size()-1, 0};
        POINT p4 = {0, body[0].size()-1};

        possible_candidate.insert(p1);
        possible_candidate.insert(p2);
        possible_candidate.insert(p3);
        possible_candidate.insert(p4);
//        std::cout << "possible points" << std::endl;
//        for(auto p : possible_candidate){
//            std::cout << p.x << " " << p.y << std::endl;
//        }

        this->edges = possible_candidate;
    }

    int rotate_object_90_degrees_clockwise(){
        //TODO turn faces

        // turn boundaries x->z
        float temp = boundaries[0];
        boundaries[0] = boundaries[4];
        boundaries[4] = temp;

        temp = boundaries[1];
        boundaries[1] = boundaries[5];
        boundaries[5] = temp;

        //rotate body;
        body = rotate_matrix_clockwise(body);
        std::cout << "rotating object with index " << index << std::endl;

        std::cout << "point 0 coordinates before" << std::endl;
        std::cout << points[0].x << " " << points[0].y << std::endl;
        //rotate boundary points
        for(auto &point : boundary_points){
            unsigned int temp = point.x;
            point.x = point.y;
            point.y = body[0].size() - 1 - temp;
        }

        //rotate points
        for(auto &point : points){
            unsigned int temp = point.x;
            point.x = point.y;
            point.y = body[0].size() - 1 - temp;
        }

        std::cout << "point 0 coordinates after" << std::endl;
        std::cout << points[0].x << " " << points[0].y << std::endl;
        // rotate edges
        std::set<POINT> temp_set;
        for (auto itr = edges.begin(); itr != edges.end(); itr++){
            unsigned int temp = itr->x;
            POINT p = {itr->y, body[0].size() - 1 - temp};
            temp_set.insert(p);
        }
        edges = temp_set;

        rotated_clockwise_90_counter++;
    }
};

class Object_on_plate{
public:
    int object_id;
    POINT ref_point; // upper left point of bounding box
    std::vector<POINT> boundary_points; //actual boundaries of object on plate
    std::vector<POINT> points;
    std::set<POINT> edges;
    int rotated = 0; // how many times rotated

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
        this->boundary_points.push_back(p);
    }

    void insert_new_boundary_point(int x, int y){
        POINT p = POINT();
        p.x = x;
        p.y = y;
        this->boundary_points.push_back(p);
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
    std::map<int, int> obj_index_to_list_index;

    explicit Plate(unsigned int size_x = 100, unsigned int size_y = 100, int default_value = -1){
        this->size_x = size_x;
        this->size_y = size_y;
        this->plate_default_placement_value = default_value;
        gen_plate(default_value);
        this->objects_on_plate = 0;
//        plates_counter ++;
    }

    void print_plate_info(float cell_size){
        std::cout << "----------info about plate----------" << std::endl;
        std::cout << this->objects_on_plate << " obj on plate" << std::endl;
        std::cout << "size_x : " << this->size_x << std::endl;
        std::cout << "size_y : " << this->size_y << std::endl;
        std::cout << "sizes in cm: " << size_x * cell_size << " " << size_y * cell_size << std::endl;
        std::cout << "------------------------------------" << std::endl;
    }

    virtual ~Plate()
    {
        ;
//        plates_counter --;
    }

    void gen_plate(int default_value = EMPTY_SPACE){
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

    int place_object_at_ref_point(Object obj, POINT ref_point){

        Object_on_plate new_object;
        new_object.object_id = obj.index;

        new_object.set_ref_point(ref_point);

        std::vector<POINT> points;

        for(auto p : obj.points){
            points.push_back(POINT{p.x + ref_point.x, p.y + ref_point.y});
            this->plate[p.x + ref_point.x][p.y + ref_point.y] = obj.index;
        }
        new_object.points = points;

        for(auto p : obj.boundary_points){
            new_object.insert_new_boundary_point(p.x+ref_point.x, p.y+ref_point.y);
        }

        for(auto p : obj.edges){
            new_object.edges.insert(POINT{p.x + ref_point.x, p.y + ref_point.y});
        }

        this->objects.push_back(new_object);

        return GENERAL_SUCCESS;
    }

    int place_new_object(Object obj){
        if (obj.body.empty()){
            std::cout << "ERROR!! empty body in object received" << std::endl;
            return PLACEMENT_ERROR;
        }
        if(obj.body[0].empty()){
            std::cout << "ERROR!! incorrect body frame received" << std::endl;
            return PLACEMENT_ERROR;
        }
        std::cout << "placing obj with index " << obj.index  << std::endl;

        if (objects_on_plate == 0){
            if (obj.body.size() > this->size_x or obj.body[0].size() > this->size_y){
                std::cout << "object " << obj.index << " will not fit into plate" << std::endl;
                return PLACEMENT_ERROR;
            }
            int obj_frame_size_x = obj.body.size();
            int obj_frame_size_y = obj.body[0].size();
            unsigned int x_place = get_random_int(0, this->size_x - obj_frame_size_x);
            unsigned int y_place = get_random_int(0, this->size_y - obj_frame_size_y);
//            std::cout << this->size_x - obj_frame_size_x << " " << this->size_y - obj_frame_size_y << std::endl;

            x_place = size_x - obj_frame_size_x;
            y_place = 0;
            std::cout << "Placing on empty plate new object with sizes: " << obj_frame_size_x << " " << obj_frame_size_y << std::endl;
            std::cout << "Selected place: (" << x_place << "; " << y_place << ")" << std::endl;

            place_object_at_ref_point(obj, POINT{x_place, y_place});
        } else {
            std::vector<std::vector<int>> unavailable_placement = this->plate;
            // take 4 ref points and follow them through boundaries of each object
            // find 4 (or less) point on the boundaries
            // find boundaries of object

            //TODO debug segfault appearing somewhere here
            if (obj.body.empty() or obj.body[0].empty()){
                std::cout << "Empty object received at placement" << std::endl;
                return PLACEMENT_ERROR;
            }
            int obj_frame_size_x = obj.body.size();
            int obj_frame_size_y = obj.body[0].size();

            for(int i = 0; i < size_x; i++){
                for (int j = size_y - obj_frame_size_y; j < size_y; j++) {
                    unavailable_placement[i][j] = OCCUPIED_PLACE;
                }
            }

            for(int i = size_x - obj_frame_size_x + 1; i < size_x; i++){
                for (int j = 0; j < size_y- obj_frame_size_y; j++) {
                    unavailable_placement[i][j] = OCCUPIED_PLACE;
                }
            }

            int c = 0;

            for(auto &object_on_plate : this->objects){
                for(auto edge_point : obj.boundary_points){
                    for (auto &boundary_point : object_on_plate.points) {
                        // object - one of the objects on plate
                        // edge point - one of the edge points of new object
                        // boundary_point - one of the boundary points of <object>
                        POINT new_obj_ref_point = {boundary_point.x - edge_point.x, boundary_point.y - edge_point.y};

                        POINT bp_new_obj_absolute = {
                                new_obj_ref_point.x + edge_point.x,
                                new_obj_ref_point.y + edge_point.y
                        };
                        int x = bp_new_obj_absolute.x;
                        int y = bp_new_obj_absolute.y;
                        if (x < 0 or y < 0 or new_obj_ref_point.x < 0 or new_obj_ref_point.y < 0) continue;
                        if (unavailable_placement[x][y] != -1 and unavailable_placement[x][y] != 0){
                            unavailable_placement[new_obj_ref_point.x][new_obj_ref_point.y] = OCCUPIED_PLACE;
                        }
                    }
                    std::string step = std::to_string(c);
                    c++;
                }
            }
            save_matrix_to_file("not_available_places_for_placing_" + std::to_string(obj.index), unavailable_placement);
            std::vector<POINT> free_places;
//            std::set<POINT> free_places;

            int x_place = 0;
            int y_place = size_y-1;

            for(int i = 0; i < size_x; i++){
                for(int j = 0; j < size_y; j++){
                    if (unavailable_placement[i][j] == EMPTY_SPACE){
                        if(i > x_place){
                            x_place = i;
                            y_place = j;
                        }
                        free_places.push_back(POINT{i, j});
//                        free_places.insert(POINT{i, j});
                    }
                }
            }

            if(free_places.empty()){
                std::cout << "no more space for new obj" << std::endl;
                return PLACEMENT_ERROR;
            }

//            int random_free_point_index = get_random_int(0, free_places.size());

            std::cout << "placing not first object" << std::endl;

            POINT placement_point = {x_place, y_place};
            std::cout << "ref point calculated... placing" << std::endl;
//            place_object_at_ref_point(obj, free_places[random_free_point_index]);
            place_object_at_ref_point(obj, placement_point);
//            place_object_at_ref_point(obj, *(free_places.begin()));
        }

        save_matrix_to_file("plate_after_placing_" + std::to_string(obj.index), plate);
        objects_on_plate++;
        this->obj_index_to_list_index[obj.index] = this->objects.size()-1;
        return GENERAL_SUCCESS;
    }

    int remove_object_with_index(int index){
        remove_object_at_index(this->obj_index_to_list_index[index]);

        return GENERAL_SUCCESS;
    }

    int remove_object_at_index(int index){

        for(POINT &p : this->objects[index].points){
            this->plate[p.x][p.y] = EMPTY_SPACE;
        }

        objects.erase(objects.begin()+index);
        this->objects_on_plate--;

        return GENERAL_SUCCESS;
    }

    std::set<POINT> find_edges(Object obj){
        std::set<POINT> possible_candidate;
        bool finding_edge = true;
        int column = 0;
        while(finding_edge) {
            for (int i = 0; i < obj.body.size(); i++) {
                if (obj.body[i][column] != -1) {
                    possible_candidate.insert(POINT{i, column});
                    finding_edge = false;
                    break;
                }
            }
            column++;
        }

        column = obj.body[0].size()-1;
        finding_edge = true;
        while(finding_edge) {
            for (int i = 0; i < obj.body.size(); i++) {
                if (obj.body[i][column] != -1) {
                    possible_candidate.insert(POINT{i, column});
                    finding_edge = false;
                    break;
                }
            }
            column--;
        }


        int row = 0;
        finding_edge = true;
        while(finding_edge) {
            for (int i = 0; i < obj.body[0].size(); i++) {
                if (obj.body[row][i] != -1) {
                    possible_candidate.insert(POINT{row, i});
                    finding_edge = false;
                    break;
                }
            }
            row++;
        }

        row = obj.body.size() - 1;
        finding_edge = true;
        while(finding_edge) {
            for (int i = 0; i < obj.body[0].size(); i++) {
                if (obj.body[row][i] != -1) {
                    possible_candidate.insert(POINT{row, i});
                    finding_edge = false;
                    break;
                }
            }
            row--;
        }

        std::cout << "possible points" << std::endl;

        for(auto p : possible_candidate){
            std::cout << p.x << " " << p.y << std::endl;
        }

        return possible_candidate;
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
        if(!is_path_exist(representations_dir)){
            mkdir(&representations_dir[0], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }
        std::string output_filename = representations_dir + "/" + filename;
        std::cout << "saving boundaries to the file " << output_filename << std::endl;
        myfile.open (output_filename);
        int count = 0;
        for(auto &row : matrix){
            for(auto &value : row){
                if(value != -1){
                    count++;
                    myfile<< std::setw(width) << value << " ";
                }else{
                    myfile << std::setw(width) << "_" << " ";
                }
            }
            myfile << std::endl;
        }
        myfile.close();
//        std::cout << count << " values != -1" << std::endl;
        return 0;
    }
};

