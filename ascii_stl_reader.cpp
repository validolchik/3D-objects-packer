#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
std::vector<FACE> faces;

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
	}

	faces.push_back(face);

	return 0;
}


int process_stl_file(std::string filename){
	std::string line;
	int facet_count = 0;
	bool reading_facet = false;
	std::vector<std::string> facet_block;

	// std::ifstream myfile (argv[1]);
	std::ifstream myfile (filename);
	if (myfile.is_open()){
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
	    myfile.close();
	}
	else std::cout << "Unable to open file"; 

	std::cout << faces.size() << std::endl;

	return 0;
}


int main(int argc, char const *argv[])
{	
	process_stl_file(argv[1]);

	for(size_t i = 0; i < faces.size(); i++){
		// std::cout << "face " << i << " normal x coord = "<< faces[i].n.x << std::endl;
		for(size_t y = 0; y < faces[i].vertices.size(); y++){
			// std::cout << "vertex " << y << " x coord = " << faces[i].vertices[y].x << std::endl;
			// std::cout << faces[i].vertices[y].x << ";" << faces[i].vertices[y].y << ";" << faces[i].vertices[y].z << std::endl;
			std::cout << faces[i].n.x << " " << faces[i].n.y << " " << faces[i].n.z << std::endl;
		}
		std::cout << std::endl;
	}

	return 0;
}