// #include "ascii_stl_reader.h"
// #include "opengl_viewer.h"
// #include <iostream>
// #include <cstdio>
// #include <iostream>
// #include <fstream>


/* i have array of faces where face is
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
};

i need to create class or struct for representing the whole 3d object
as 2d array

desirable field:
-array itself
-area
-boundaries

TODO:
object as projection into grid
write obejct to stl (or find one)
move figure
collision detection
several figures

*/
float grid_cell_size = 0.1;

float half_plane_sign (VERTEX p1, VERTEX p2, VERTEX p3)
{
    return (p1.x - p3.x) * (p2.z - p3.z) - (p2.x - p3.x) * (p1.z - p3.z);
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

std::vector<std::vector<bool>> produce_empty_body_grid(OBJECT obj){
	int rows = int((obj.boundaries[1] - obj.boundaries[0] + grid_cell_size) / grid_cell_size); 
	int columns = int((obj.boundaries[5] - obj.boundaries[4] + grid_cell_size) / grid_cell_size);

	std::cout << "grid size" << std::endl;
	std::cout << rows << " " << columns << std::endl;
	
	std::vector<std::vector<bool>> grid_body(
    rows,
    std::vector<bool>(columns));

	std::cout << sizeof(grid_body[0]) * grid_body.size() << " bytes" << std::endl;

    return grid_body;
}

std::vector<float> find_square_bounds_of_triangle(FACE f){
	std::vector<float> boundaries = {f.vertices[0].x, f.vertices[0].x, f.vertices[0].z, f.vertices[0].z};
	for (int i = 0; i < f.vertices.size(); ++i){
		boundaries[0] = std::min(boundaries[0], f.vertices[i].x);
		boundaries[1] = std::max(boundaries[1], f.vertices[i].x);
		boundaries[2] = std::min(boundaries[2], f.vertices[i].z);
		boundaries[3] = std::max(boundaries[3], f.vertices[i].z);
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


void fill_body_grid(OBJECT obj, std::vector<std::vector<bool>>& empty_grid){
	std::cout << "filling the grid" << std::endl;

	int inside_count = 0;
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

		for (int j = x_min_grid; j < x_max_grid; ++j)
		{
			for (int k = y_min_grid; k < y_max_grid; ++k)
			{
				VERTEX p = {j*grid_cell_size, 0, k*grid_cell_size}; // in absolute
				// std::cout << "test point " << p.x << " " << p.y << " " << p.z << std::endl;
				// std::cout << k << ' ' << j << std::endl;
				if (point_in_triangle(p, v0, v1, v2)){
					if (!empty_grid[j][k]) inside_count++;
					empty_grid[j][k] = true;
					// empty_grid[j+1][k] = true;
					// empty_grid[j+1][k] = true;
					// empty_grid[j+1][k+1] = true;//fix the indexes here
					// std::cout << p.x << ";" <<  p.z << " is inside" << std::endl;
					// inside_count++;
				}
			}
		}
	}
	std::cout << inside_count << " inside out of " << overall_count << std::endl;
}

void assign_body_grid(OBJECT obj, std::vector<std::vector<bool>> grid){
	obj.body = grid;
}

// main in the test functionss