//
// Created by renat on 10.05.22.
//

#include <cstdlib>
#include <vector>
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
#include <chrono>

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


void shuffle_int_vector(std::vector<int> &vec){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    shuffle (vec.begin(), vec.end(), std::default_random_engine(seed));
}

int save_matrix_to_file(std::string filename, std::vector<std::vector<int>> matrix, bool info = false, int width = 2){
    std::ofstream myfile;
    std::string representations_dir = "plate_matrices";
    if(!is_path_exist(representations_dir)){
        mkdir(&representations_dir[0], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
    std::string output_filename = representations_dir + "/" + filename;
    if (info) std::cout << "saving boundaries to the file " << output_filename << std::endl;
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
    return 0;
}

std::vector<std::vector<int>> generate_2d_matrix(unsigned int x = 5, unsigned int y = 5, bool fill_in_order = false, int def = 0)
{
    std::vector<std::vector<int>> res(
            x,
            std::vector<int>(y, def));
    if (!fill_in_order){
        return res;
    }
    int count = 1;
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y;j++){
            res[i][j] = count++;
        }
    }

    return res;
}

void print_2d_matrix_in_console(std::vector<std::vector<int>> matrix){
    for(auto row : matrix){
        for(auto value : row){
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

// Function to rotate the matrix 90 degree clockwise
std::vector<std::vector<int>> rotate_matrix_clockwise(std::vector<std::vector<int>> matrix)
{
    if(matrix.empty()){
        return matrix;
    }

    int matrix_size_x = matrix.size();
    int matrix_size_y = matrix[0].size();

    std::vector<std::vector<int>> res(
            matrix_size_y,
            std::vector<int>(matrix_size_x, 0));
    for(int i = 0; i < matrix_size_x; i++){
        for(int j = 0; j < matrix_size_y;j++){
            res[j][matrix_size_x-1-i] = matrix[i][j];
        }
    }

    return res;
}