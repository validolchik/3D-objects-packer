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

struct POINT{
    int x;
    int y;
};

float calculate_distance(POINT p1, POINT p2){

    int x_d = (p1.x - p2.x);
    int y_d = (p1.y - p2.y);

    return sqrt(x_d * x_d + y_d * y_d);
}

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
    POINT weight_center;

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
//        std::cout << "rotating object with index " << index << std::endl;
//
//        std::cout << "point 0 coordinates before" << std::endl;
//        std::cout << points[0].x << " " << points[0].y << std::endl;
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

//        std::cout << "point 0 coordinates after" << std::endl;
//        std::cout << points[0].x << " " << points[0].y << std::endl;
        // rotate edges
        std::set<POINT> temp_set;
        for (auto itr = edges.begin(); itr != edges.end(); itr++){
            unsigned int temp = itr->x;
            POINT p = {itr->y, body[0].size() - 1 - temp};
            temp_set.insert(p);
        }
        edges = temp_set;

        rotated_clockwise_90_counter = (rotated_clockwise_90_counter + 1) % 4;
    }

    Object create_rotated(){
        Object obj;
        //TODO turn faces

        // turn boundaries x->z
        for(float f : boundaries){
            obj.boundaries.push_back(f);
        }
        float temp = obj.boundaries[0];
        obj.boundaries[0] = obj.boundaries[4];
        obj.boundaries[4] = temp;

        temp = obj.boundaries[1];
        obj.boundaries[1] = obj.boundaries[5];
        obj.boundaries[5] = temp;

        //rotate body;
        for(auto row : body){
            std::vector<int> new_row;
            for(int value : row){
                new_row.push_back(value);
            }
            obj.body.push_back(new_row);
        }

        obj.body = rotate_matrix_clockwise(obj.body);

        for(auto &point : boundary_points){
            unsigned int temp = point.x;
            point.x = point.y;
            point.y = body[0].size() - 1 - temp;
            obj.boundary_points.push_back(POINT {point.x, point.y});
        }

        //rotate points
        for(auto &point : points){
            unsigned int temp = point.x;
            point.x = point.y;
            point.y = body[0].size() - 1 - temp;
            obj.points.push_back(POINT{point.x, point.y});
        }

        // rotate edges
        std::set<POINT> temp_set;
        for (auto itr = edges.begin(); itr != edges.end(); itr++){
            unsigned int temp = itr->x;
            POINT p = {itr->y, body[0].size() - 1 - temp};
            temp_set.insert(p);
        }
        obj.edges = temp_set;

        obj.rotated_clockwise_90_counter++;

        obj.index = index;

        obj.weight_center = POINT{weight_center.y, body[0].size() - 1 - weight_center.x};

        return obj;
    }
};

class Object_on_plate{
public:
    int object_id;
    POINT ref_point; // upper left point of bounding box
    std::vector<POINT> boundary_points; //actual boundaries of object on plate
    std::vector<POINT> points;
    std::set<POINT> edges;
    POINT weight_center;
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
    std::vector<Object> objects_itself;

    explicit Plate(unsigned int size_x = 170, unsigned int size_y = 160, int default_value = -1){
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

    void print_plate_info(){
        std::cout << "----------info about plate----------" << std::endl;
        std::cout << this->objects_on_plate << " obj on plate" << std::endl;
        std::cout << "size_x : " << this->size_x << std::endl;
        std::cout << "size_y : " << this->size_y << std::endl;
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
        new_object.rotated = obj.rotated_clockwise_90_counter;

        new_object.set_ref_point(ref_point);

        std::vector<POINT> points;

        for(auto p : obj.points){
            unsigned int x = p.x + ref_point.x;
            unsigned int y = p.y + ref_point.y;
            if (plate[x][y] != EMPTY_SPACE){
                return PLACEMENT_ERROR;
            }
        }

        for(auto p : obj.points){
            unsigned int x = p.x + ref_point.x;
            unsigned int y = p.y + ref_point.y;
            points.push_back(POINT{x, y});
            if (plate[x][y] != EMPTY_SPACE){
                return PLACEMENT_ERROR;
            } else{
                plate[x][y] = obj.index;
            }
        }
        new_object.points = points;

        for(auto p : obj.boundary_points){
            new_object.insert_new_boundary_point(p.x+ref_point.x, p.y+ref_point.y);
        }

        for(auto p : obj.edges){
            new_object.edges.insert(POINT{p.x + ref_point.x, p.y + ref_point.y});
        }

        new_object.weight_center = POINT{obj.weight_center.x + ref_point.x, obj.weight_center.y + ref_point.y};

        this->objects.push_back(new_object);
        objects_itself.push_back(obj);
        return GENERAL_SUCCESS;
    }

    int place_new_object(Object obj, bool info = false){
        if (obj.body.empty()){
            std::cout << "ERROR!! empty body in object received while placing" << std::endl;
            return PLACEMENT_ERROR;
        }
        if(obj.body[0].empty()){
            std::cout << "ERROR!! incorrect body frame received while placing" << std::endl;
            return PLACEMENT_ERROR;
        }
        if (info){
            std::cout << "placing obj with index " << obj.index  << std::endl;
        }

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

            if (place_object_at_ref_point(obj, POINT{x_place, y_place}) == PLACEMENT_ERROR){
                std::cout << "ERROR! Placement first model is broken" << std::endl;
            }

            if (info){
                std::cout << "Placed on empty plate new object with sizes: " << obj_frame_size_x << " " << obj_frame_size_y << std::endl;
                std::cout << "Selected place: (" << x_place << "; " << y_place << ")" << std::endl;
            }


        } else {
            //std::vector<std::vector<int>> unavailable_placement = this->plate;
            std::vector<std::vector<int>> unavailable_placement (size_x,
                                                                 std::vector<int>(size_y, EMPTY_SPACE));

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
                        if (new_obj_ref_point.x < 0 or new_obj_ref_point.y < 0) continue;
//                        if (unavailable_placement[x][y] != EMPTY_SPACE){
                            unavailable_placement[new_obj_ref_point.x][new_obj_ref_point.y] = OCCUPIED_PLACE;
//                        }
                    }
                    std::string step = std::to_string(c);
                    c++;
                }
            }
//            save_matrix_to_file("occupied_obj_ind_" + std::to_string(obj.index) + "_" + std::to_string(objects_on_plate), unavailable_placement);
            std::vector<POINT> free_places;
//            std::set<POINT> free_places;

            int x_place = -1;
            int y_place = size_y;

            for(int i = 0; i < size_x; i++){
                for(int j = 0; j < size_y; j++){
                    if (unavailable_placement[i][j] == EMPTY_SPACE){
                        if(j <= y_place){
                            x_place = i;
                            y_place = j;
                        }
                        free_places.push_back(POINT{i, j});
//                        free_places.insert(POINT{i, j});
                    }
                }
            }

            if (x_place == -1 or y_place == size_y){
                if (info){
                    std::cout << "no more space for new obj" << std::endl;
                }
                return PLACEMENT_ERROR;
            }

            if(free_places.empty()){
                std::cout << "no more space for new obj" << std::endl;
                return PLACEMENT_ERROR;
            }

//            int random_free_point_index = get_random_int(0, free_places.size());

            POINT placement_point = {x_place, y_place};

            if(info){
                std::cout << "placing not first object" << std::endl;
                std::cout << "ref point calculated... placing" << std::endl;
            }

            if (place_object_at_ref_point(obj, placement_point) == PLACEMENT_ERROR){
                std::cout << "ERROR! ref point calculated wrong, objects collide\n Object unplaced" << std::endl;
            };
        }

//        save_matrix_to_file("plate_after_placing_ind" + std::to_string(obj.index) + "_" + std::to_string(objects_on_plate), plate);
        objects_on_plate++;
        this->obj_index_to_list_index[obj.index] = this->objects.size()-1;
        return GENERAL_SUCCESS;
    }

    //TODO fix remove unplaced objects
    int remove_object_with_index(int index){
        remove_object_at_index(this->obj_index_to_list_index[index]);

        return GENERAL_SUCCESS;
    }

    //TODO fix remove unplaced objects
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
};

class Individual{
public:
    static int ind_count;
    int ind_index;
    Plate ind_plate;
    std::vector<int> ind_chromosome;

    Individual(std::vector<int> chromo, Plate plate){
        ind_count++;
        ind_index = ind_count;
        ind_chromosome = chromo;
        ind_plate = plate;
    }

    Individual(std::vector<Object> &objects){
        std::cout << "creating new individ" << std::endl;
        Plate plate(objects[0].body.size() * 2 + 20, objects[0].body[0].size() * 2 + 20);

        int unplaced_objects = 0;

        std::vector<int> indexes_shuffle;
        for(int i = 0; i < objects.size(); i++){
            indexes_shuffle.push_back(i);
        }
        shuffle_int_vector(indexes_shuffle);

        std::vector<int> chromosome;

//        std::cout << "adding objects" << std::endl;
        for(int index : indexes_shuffle){
            Object obj = objects[index];

            int rotate_count = get_random_int(0, 4);
//            std::cout << "rotating " << rotate_count << "times" <<std::endl;
            for(int i = 0; i < rotate_count; i++){
                obj.rotate_object_90_degrees_clockwise();
            }
//            std::cout << index << " " << &obj << " " << &objects[index] << std::endl;
            if (plate.place_new_object(obj) == PLACEMENT_ERROR){
                unplaced_objects++;
            } else{
                chromosome.push_back(index);
            }
//            std::cout << "done with " << index << std::endl;
        }
//        std::cout << "added" << std::endl;

        ind_count++;
        ind_index = ind_count;
        ind_plate = plate;
        ind_chromosome = chromosome;
//        std::cout << "created" << std::endl;
    }

    // only for crossover
    Individual(std::vector<int> chromo, std::vector<Object> &objects_){
        // get objects on plate, not objects
        std::cout << "creating new individ" << std::endl;
        Plate plate;

        int unplaced_objects = 0;

        std::vector<int> chromosome;

        int in = 0;

        for(int index : chromo){
            Object obj = objects_[in];
            in++;

            if (plate.place_new_object(obj) == PLACEMENT_ERROR){
                unplaced_objects++;
            }else{
                chromosome.push_back(index);
            }
        }

        ind_count++;
        ind_index = ind_count;
        ind_plate = plate;
        ind_chromosome = chromosome;
    }

    int mutation(std::vector<Object> &objects){
        int first_gen = get_random_int(0, ind_chromosome.size());
        int second_gen = get_random_int(0, ind_chromosome.size());
        while(second_gen == first_gen){
            second_gen = get_random_int(0, ind_chromosome.size());
        }

        //swap
        int temp = ind_chromosome[first_gen];
        ind_chromosome[first_gen] = ind_chromosome[second_gen];
        ind_chromosome[second_gen] = temp;
        //delete objects from plate
        int x = ind_plate.size_x;
        int y = ind_plate.size_y;
        Plate new_plate(x, y);
        ind_plate = new_plate;

        // fill plate again
        for(int index : ind_chromosome){

            Object obj = objects[index];
            // take object from chromosomes, before save them somewhere in intermediate storage
            if (ind_plate.place_new_object(obj) == PLACEMENT_ERROR){
                std::cout << "genes not placed" << std::endl;
            }
        }

    }

    std::vector<Individual> crossover(Individual &second){
        std::vector<Individual> result;
        //TODO remember the rotations of the objects
        int min_length = std::min(ind_chromosome.size(), second.ind_chromosome.size());

        int swap_start = get_random_int(0, min_length/2);
        int swap_finish = get_random_int(swap_start, min_length-swap_start) + 1;
        
        std::cout << "min len " << min_length << " swap start " << swap_start << " swap finish " << swap_finish << std::endl;
        std::cout << "lenghts " << ind_chromosome.size() << " " << second.ind_chromosome.size() << std::endl;

        // copy objects selected for placement in local array and only then rotate them
        // after GA is done, rotate the STL, and combine the whole file
        std::vector<int> first_child;

        std::map<int, int> first_child_map;

        std::vector<Object> first_child_objects;
        std::vector<Object> second_child_objects;

        for(int i = swap_start; i < swap_finish; i++){
            first_child.push_back(ind_chromosome[i]);
//            std::cout << "rotated on plate " << i << " " << ind_plate.objects[i].rotated << " " << std::endl;
            first_child_map[ind_chromosome[i]] = 1;

//            std::cout << "checking whether map works" << std::endl;
            int index_of_object = ind_plate.obj_index_to_list_index[ind_chromosome[i]];
//            int index_of_object = get_index_of_object_with_index(ind_chromosome[i], ind_plate.objects_itself);
//            std::cout << index_of_object << " " << ind_plate.obj_index_to_list_index[ind_chromosome[i]] << std::endl;

            if(index_of_object != -1){
                Object obj = ind_plate.objects_itself[index_of_object];
                first_child_objects.push_back(obj);
            } else{
                std::cout << "something went wrong in composing first child" << std::endl;
                return result;
            }
        }

        std::cout << "first child pre gen " << first_child.size() << std::endl;
        for(Object &obj : first_child_objects){
            std::cout << obj.index << " " << obj.rotated_clockwise_90_counter << "; ";
        }
        std::cout << std::endl;

        for(int value : second.ind_chromosome){
            if( first_child_map.find(value) == first_child_map.end()){
                first_child_map[value] = 1;
                first_child.push_back(value);
                std::cout << "!" << value << " ";
                int index_of_object = second.ind_plate.obj_index_to_list_index[value];
                if(index_of_object != -1){
                    Object obj = second.ind_plate.objects_itself[index_of_object];
                    first_child_objects.push_back(obj);
                } else{
                    std::cout << "something went wrong in composing first child from second" << std::endl;
                    return result;
                }
            }
        }

        std::cout << "first child chromosome unplaced yet ";
        for(int value : first_child){
            std::cout << value << " ";
        }
        std::cout << std::endl;

        result.push_back(Individual(first_child, first_child_objects));

        std::vector<int> second_child;
        std::map<int, int> second_child_map;

        for(int i = 0; i < ind_chromosome.size(); i++){
            if(i < swap_start or i >= swap_finish){
                second_child.push_back(ind_chromosome[i]);
                second_child_map[ind_chromosome[i]] = 1;
                Object obj;
                int index_of_obj = second.ind_plate.obj_index_to_list_index[ind_chromosome[i]];
                obj = second.ind_plate.objects_itself[index_of_obj];
                second_child_objects.push_back(obj);
            }
        }

        for(int value : second.ind_chromosome){
            if( second_child_map.find(value) == second_child_map.end()){
                second_child_map[value] = 1;
                second_child.push_back(value);
                std::cout << "!" << value << " ";
                int index_of_object = ind_plate.obj_index_to_list_index[value];
                if(index_of_object != -1){
                    Object obj = ind_plate.objects_itself[index_of_object];
                    second_child_objects.push_back(obj);
                } else{
                    std::cout << "something went wrong in composing second child from first" << std::endl;
                    return result;
                }
            }
        }

        std::cout << "second child chromosome unplaced yet ";
        for(int value : second_child){
            std::cout << value << " ";
        }
        std::cout << std::endl;

        result.push_back(Individual(second_child, second_child_objects));

        return result;
    }

    void print_individual_info(){
        std::cout << "--------------Individ-----------" << std::endl;
        std::cout << "index " << ind_index << std::endl;
        std::cout << "ind plate info" << std::endl;
        ind_plate.print_plate_info();
        std::cout << "chromosome size " << ind_chromosome.size() << std::endl;
        std::cout << "chromosome\n";
        for(auto c : ind_chromosome) std::cout << c << " ";
        std::cout << std::endl;
        for(Object &obj : ind_plate.objects_itself) std::cout << obj.rotated_clockwise_90_counter << " ";
        std::cout << std::endl;
        std::cout << std::endl;
    }

    float get_fitness(){
        float fitness = 0;

        for(Object_on_plate &obj_on_plate : ind_plate.objects){
            fitness += obj_on_plate.points.size();
        }

        float dist = 0;
        for(int i = 0; i < ind_plate.objects.size() - 1; i++){
            for(int j = i + 1; j < ind_plate.objects.size(); j++){
                float cur_dist = calculate_distance(ind_plate.objects[i].weight_center, ind_plate.objects[j].weight_center);
                dist += cur_dist;
            }
        }
        int n = ind_plate.objects.size();
        dist /= (n*n / 2);

        std::cout << "average distance = " << dist << std::endl;

        return fitness;
    }

    int get_index_of_object_with_index(int index, std::vector<Object> objects){

        if(objects.empty()){
            std::cout << "empty objects on plate received at get object with index" << std::endl;
            return -1;
        }

        for(int i = 0; i < objects.size(); i++){
            if(objects[i].index == index){
                return i;
            }
        }
        std::cout << "error in finding index " << index << std::endl;
        for(Object &obj : objects){
            std::cout << obj.index << " ";
        }
        std::cout << std::endl;

        return -1;
    }
};
