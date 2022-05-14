#include <utility>

// #include "ascii_stl_reader.h"
// #include "opengl_viewer.h"
// #include <iostream>
// #include <cstdio>
// #include <iostream>
// #include <fstream>


float grid_cell_size = 0.005; //in m
// typical size of plate is 0.22 m

float half_plane_sign (VERTEX p1, VERTEX p2, VERTEX p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool point_in_triangle (VERTEX pt, VERTEX v1, VERTEX v2, VERTEX v3)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = half_plane_sign(pt, v1, v2);
    d2 = half_plane_sign(pt, v2, v3);
    d3 = half_plane_sign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

std::vector<std::vector<int>> produce_empty_body_grid(Object obj){
	int rows = int((obj.boundaries[1] - obj.boundaries[0] + grid_cell_size) / grid_cell_size); 
	int columns = int((obj.boundaries[3] - obj.boundaries[2] + grid_cell_size) / grid_cell_size);

	std::cout << "grid size" << std::endl;
	std::cout << rows << " " << columns << std::endl;
	
	std::vector<std::vector<int>> grid_body(
    rows,
    std::vector<int>(columns, -1));

	std::cout << sizeof(grid_body[0]) * grid_body.size() << " bytes" << std::endl;

    return grid_body;
}

std::vector<float> find_square_bounds_of_triangle(FACE f){
	std::vector<float> boundaries = {f.vertices[0].x, f.vertices[0].x, f.vertices[0].y, f.vertices[0].y};
	for (int i = 0; i < f.vertices.size(); ++i){
		boundaries[0] = std::min(boundaries[0], f.vertices[i].x);
		boundaries[1] = std::max(boundaries[1], f.vertices[i].x);
		boundaries[2] = std::min(boundaries[2], f.vertices[i].y);
		boundaries[3] = std::max(boundaries[3], f.vertices[i].y);
	}

	return boundaries;
}

std::vector<int> find_int_square_triangle(std::vector<float> float_boundaries){
	int x_min_grid = int(float_boundaries[0]/grid_cell_size);
	int x_max_grid = int((float_boundaries[1] + grid_cell_size)/grid_cell_size);
	int y_min_grid = int(float_boundaries[2]/grid_cell_size);
	int y_max_grid = int((float_boundaries[3] + grid_cell_size)/grid_cell_size);
	return {x_min_grid, x_max_grid, y_min_grid, y_max_grid};
}


std::vector<std::vector<int>> fill_body_grid(Object obj, std::vector<std::vector<int>> empty_grid){
	std::cout << "filling the grid" << std::endl;

	int inside_count = 0;
    std::cout << "empty grid size" << std::endl;
    std::cout << empty_grid.size() << " " << empty_grid[0].size() << std::endl;
	int overall_count = empty_grid.size() * empty_grid[0].size();
	for(size_t i = 0; i < obj.object_faces.size(); i++){
		// for (VERTEX v : obj.object_faces[i].vertices) std::cout << "vertex " << v.x << " " << v.z << 	std::endl; 

		VERTEX v0 = obj.object_faces[i].vertices[0];
		VERTEX v1 = obj.object_faces[i].vertices[1];
		VERTEX v2 = obj.object_faces[i].vertices[2];
		std::vector<int> int_bounds_in_grid = find_int_square_triangle(find_square_bounds_of_triangle(obj.object_faces[i]));

		int x_min_grid = int_bounds_in_grid[0];
		int x_max_grid = int_bounds_in_grid[1];
		int y_min_grid = int_bounds_in_grid[2];
		int y_max_grid = int_bounds_in_grid[3];
		// std::cout << "square bounds of face: ";
		// std::cout << x_min_grid << " " << x_max_grid << " " << y_min_grid << " " << y_max_grid << std::endl;
//        std::cout << i << std::endl;
		for (int j = x_min_grid; j < x_max_grid; ++j)
		{
			for (int k = y_min_grid; k < y_max_grid; ++k)
			{
				VERTEX p = {j*grid_cell_size, k*grid_cell_size, 0};
//				 std::cout << "test point " << p.x << " " << p.y << " " << p.z << std::endl;
//				 std::cout << k << ' ' << j << std::endl;
				if (point_in_triangle(p, v0, v1, v2)){
					if (empty_grid[j][k] != -1) inside_count++;
					empty_grid[j][k] = int(std::min(std::min(v0.z, v1.z), v2.z));
					// empty_grid[j+1][k] = true;
					// empty_grid[j+1][k] = true;
					// empty_grid[j+1][k+1] = true;//fix the indexes here
					// std::cout << p.x << ";" <<  p.z << " is inside" << std::endl;
					// inside_count++;
				}
			}
		}
	}
//	std::cout << inside_count << " inside out of " << overall_count << std::endl;
    return empty_grid;
}

void assign_body_grid(Object &obj, std::vector<std::vector<int>> grid){
	 obj.body = grid;
}
