//
// Created by renat on 15.05.22.
//

#include "ascii_stl_reader.cpp"
#include "transform_to_array.cpp"


int read_transform_files(const char* filename_models){
    std::string file_with_models(filename_models);

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
        std::cout << "body size: " << objects[i].body.size() << std::endl;

        std::vector<POINT> points;
        std::vector<POINT> boundary_points;
        int weight_center_x = 0;
        int weight_center_y = 0;
        for(size_t x = 0; x < objects[i].body.size(); x++){
            for(size_t y = 0; y < objects[i].body[x].size(); y++){
                if (objects[i].body[x][y] != -1){
                    points.push_back(POINT{int(x), int(y)});
                    weight_center_x += x;
                    weight_center_y += y;
                    if(objects[i].boundary_point(int(x), int(y), objects[i].body)){
                        boundary_points.push_back(POINT{int(x), int(y)});
                    }
                }
            }
        }
        weight_center_x /= points.size();
        weight_center_y /= points.size();

//        print_2d_matrix_in_console(objects[i].body);
        objects[i].weight_center = {weight_center_x, weight_center_y};
        std::cout << "point array created" << std::endl;
        objects[i].points = points;
        std::cout << "assigned" << std::endl;
        objects[i].boundary_points = boundary_points;
        std::cout << "object body size: " << objects[i].points.size() << std::endl;
        std::cout << "object boundary points size: " << objects[i].boundary_points.size() << std::endl;

        objects[i].find_edges();
    }

    return GENERAL_SUCCESS; //result is filled array of Objects;
}