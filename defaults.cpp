//
// Created by renat on 26.05.22.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

float grid_cell_size = 0.01; //in m
float plate_size = 0.8; //in m
int percentage_mutation_inds = 10;
int indivduals_in_population = 30;

unsigned int size = int(plate_size / grid_cell_size);

void read_defaults(){
    std::ifstream input("defaults");

    std::string line;
    //grid cell size input
    std::getline( input, line );
    grid_cell_size = std::stof(line.substr(0, line.size()));

    //plate size input
    std::getline( input, line );
    plate_size = std::stof(line.substr(0, line.size()));

    //mutation percentage input
    std::getline( input, line );
    percentage_mutation_inds = std::stoi(line.substr(0, line.size()));

    //inds count input
    std::getline( input, line );
    indivduals_in_population = std::stoi(line.substr(0, line.size()));

    std::cout << grid_cell_size << " " << plate_size << " "
    << percentage_mutation_inds << " " << indivduals_in_population << std::endl;

    size = int(plate_size / grid_cell_size);

    input.close();
}