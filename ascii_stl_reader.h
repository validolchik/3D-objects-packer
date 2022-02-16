#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>

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

std::vector<std::string> models_filenames;
std::vector<FACE> faces;
std::vector<OBJECT> objects;

bool is_path_exist(const std::string &s)
{
    struct stat buffer;
    return (stat (s.c_str(), &buffer) == 0);
}

void read_filenames(std::string models){
    std::ifstream input( models);

    for( std::string line; getline( input, line ); ){
        models_filenames.push_back(line);
    }
}

std::vector<std::string> parse_string(std::string string, std::string delimiter){
	std::vector<std::string> tokens;

	size_t pos = 0;
	std::string token;
	while ((pos = string.find(delimiter)) != std::string::npos) {
	    token = string.substr(0, pos);
	    tokens.push_back(token);
	    string.erase(0, pos + delimiter.length());
	}
	tokens.push_back(string);

	return tokens;
}

int process_facet_block(std::vector<std::string> facet_block){
	std::vector<std::string> parsed_string = parse_string(facet_block[0].substr(1, facet_block[0].size()), " ");
	// std::cout << parsed_string[2] << parsed_string[3] <<  parsed_string[4] << std::endl;
	float x = std::stof(parsed_string[2]);
	float y = std::stof(parsed_string[3]);
	float z = std::stof(parsed_string[4]);
	
	NORMAL normal = {x, y, z};
	FACE face;

	for(size_t i = 2; i < 5; i++){
		parsed_string = parse_string(facet_block[i].substr(2, facet_block[i].size()), " ");
		// std::cout << parsed_string[1] << parsed_string[2] <<  parsed_string[3] << std::endl;
		VERTEX v = {std::stof(parsed_string[1]), std::stof(parsed_string[2]), std::stof(parsed_string[3])};
		face.vertices.push_back(v);
		face.n = normal;
	}

	faces.push_back(face);

	return 0;
}

std::vector<float> find_boundaries(std::vector<FACE> faces){
	std::vector<float> boundaries = {faces[0].vertices[0].x, faces[0].vertices[0].x, 
		faces[0].vertices[0].y, faces[0].vertices[0].y, 
		faces[0].vertices[0].z, faces[0].vertices[0].z};
	for(size_t i = 0; i < faces.size(); i++){
		for(size_t j = 0; j < faces[i].vertices.size(); j++){
			boundaries[0] = std::min(boundaries[0], faces[i].vertices[j].x);
			boundaries[1] = std::max(boundaries[1], faces[i].vertices[j].x);
			boundaries[2] = std::min(boundaries[2], faces[i].vertices[j].y);
			boundaries[3] = std::max(boundaries[3], faces[i].vertices[j].y);
			boundaries[4] = std::min(boundaries[4], faces[i].vertices[j].z);
			boundaries[5] = std::max(boundaries[5], faces[i].vertices[j].z);	
		}
	}
	return boundaries;
}

int process_stl_file(std::string filename){
	std::string line;
	int facet_count = 0;
	bool reading_facet = false;
	std::vector<std::string> facet_block;

    std::cout << std::endl << "opening the " << filename << std::endl;

	std::ifstream myfile (filename);
	if (myfile.is_open()){
		OBJECT object = OBJECT();
		while ( getline (myfile, line) ){
			// if (line.find("facet") != std::string::npos) {
			//     facet_block.push_back(line);
			// }
			if (!reading_facet && line.find("facet") == 0) {
				facet_block.push_back(line);
				reading_facet = true;
			}
			else if (reading_facet && line.find("endfacet") == 0) {
				facet_block.push_back(line);
				reading_facet = false;
				process_facet_block(facet_block);
				facet_block.clear();
			}
			else if(reading_facet){
				facet_block.push_back(line);
			}
	    }
	    object.object_faces = faces;
	    object.boundaries = find_boundaries(faces);
	    object.filename = filename;
	    objects.push_back(object);
	    myfile.close();
        faces.clear();
	}
	else std::cout << "Unable to open file"; 

	std::cout << "number of faces: " << faces.size() << std::endl;
	std::cout << "boundaries ";
	for(size_t i = 0; i < objects[0].boundaries.size(); i++){
		std::cout << objects[0].boundaries[i] << " ";
	}
	std::cout << std::endl;
	return 0;
}


void shift_object_to_zero(OBJECT &obj){
	std::cout << "shifting the object" << std::endl;
	std::cout << "before: " << obj.object_faces[0].vertices[0].x << std::endl;
	for(int i = 0; i < obj.object_faces.size(); i++){
		for (int y = 0; y < obj.object_faces[i].vertices.size(); y++){
			obj.object_faces[i].vertices[y].x -= obj.boundaries[0];
			obj.object_faces[i].vertices[y].y -= obj.boundaries[2];
			obj.object_faces[i].vertices[y].z -= obj.boundaries[4];
		}
	}
	std::cout << "after: " << obj.object_faces[0].vertices[0].x << std::endl;
}

// int main(int argc, char const *argv[])
// {	
// 	process_stl_file(argv[1]);

// 	for(size_t i = 0; i < faces.size(); i++){
// 		// std::cout << "face " << i << " normal x coord = "<< faces[i].n.x << std::endl;
// 		for(size_t y = 0; y < faces[i].vertices.size(); y++){
// 			// std::cout << "vertex " << y << " x coord = " << faces[i].vertices[y].x << std::endl;
// 			std::cout << faces[i].vertices[y].x << ";" << faces[i].vertices[y].y << ";" << faces[i].vertices[y].z << std::endl;
// 		}
// 		std::cout << std::endl;
// 	}

// 	return 0;
// }