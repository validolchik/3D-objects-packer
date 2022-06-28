//
// Created by renat on 20.03.2022.
//

//#include "classes.h"
//#include <iostream>
//#include "ascii_stl_reader.cpp"
//#include "transform_to_array.cpp"
#include <chrono>
#include <unistd.h>
#include "reading_transformation.cpp"

int Individual::ind_count = 0;

int test_mutation(const char *objects_file){
    auto start = std::chrono::steady_clock::now();

    init_random_seq();
    read_defaults();

    int reading_processing_status = read_transform_files(objects_file);
    std::cout << "Reading and processing files status is " << reading_processing_status << std::endl;
    auto reading_end = std::chrono::steady_clock::now();

    if(objects.empty()){
        std::cout << "empty objects array received at test_mutation" << std::endl;
        return -1;
    }
    Individual ind(objects);
    std::cout << std::endl << "ind0 before mutation " << ind.get_fitness() << std::endl;
    save_matrix_to_file("before", ind.ind_plate.plate);

    ind.print_individual_info();

    int fitness = ind.fitness;
    float dist = ind.average_dist;
    int count = 0;
    // fitness < ind.fitness or (dist > ind.average_dist and ind.fitness >= fitness)
    Individual new_ind(ind.ind_chromosome, ind.ind_plate.objects_itself);
    new_ind.get_fitness();
    bool better = false;
    while(true){
        new_ind.mutation_swap_two_genes();
        count++;
        new_ind.get_fitness();
        if(fitness <= new_ind.fitness and new_ind.average_dist < dist){
            better = true;
            break;
        }
        if(count > 500){
            std::cout << "out of attempts" << std::endl;
            break;
        }
        std::cout << fitness << " " << new_ind.fitness << " ";
        std::cout << dist << " " << new_ind.average_dist << std::endl;
        new_ind = Individual(ind.ind_chromosome, ind.ind_plate.objects_itself);
    }

    ind.print_individual_info();
    new_ind.print_individual_info();
    save_matrix_to_file("after", new_ind.ind_plate.plate);

    bool placed_new = false;
    if(better){
        std::vector<Object> unplaced_obj;
        for(int index : find_unplaced(objects.size(), new_ind.ind_chromosome)){
            unplaced_obj.push_back(objects[index]);
        }

        placed_new = new_ind.try_to_fill(unplaced_obj) == 0;
    }

    if(placed_new){
        std::cout << "placed new obj" << std::endl;
    }else{
        std::cout << "no more space for any left objects" << std::endl;
    }

    return GENERAL_SUCCESS;
}

int run_genetics(const char* objects_file){
    auto start = std::chrono::steady_clock::now();

    init_random_seq();
    read_defaults();

    int reading_processing_status = read_transform_files(objects_file);
    std::cout << "Reading and processing files status is " << reading_processing_status << std::endl;
    auto reading_end = std::chrono::steady_clock::now();
//    test_mutation();

    std::cout << "Elapsed time for reading in milliseconds: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(reading_end - start).count()
              << " ms" << std::endl;

    GeneticAlgorithm ga(objects, indivduals_in_population);
    ga.print_fitness_inds();

    std::cout << ga.ga_individuals[0].ind_plate.size_x << "size of plate" << std::endl;

    for(auto &ind : ga.ga_individuals){
        save_matrix_to_file("ind_start_"+std::to_string(ind.ind_index), ind.ind_plate.plate);
    }

    int rounds_count = 200;
    int current_best = ga.get_best_ind().fitness;
    int current_best_count = 0;
    int rounds_without_upgrade = 7;
    for(int i = 0; i < rounds_count; i++){
        auto round_start = std::chrono::steady_clock::now();
        ga.run_one_round(objects, percentage_mutation_inds);
        std::cout << "round " << i << std::endl;
        ga.print_best_info();
        ga.save_best_to_file("round_" + std::to_string(i));
        if(ga.get_best_ind().fitness > current_best){
            current_best = ga.get_best_ind().fitness;
            current_best_count = 0;
        } else{
            std::cout << "increase current_best_count -> ";
            current_best_count++;
            std::cout << current_best_count << std::endl;
        }
        auto round_end = std::chrono::steady_clock::now();
        std::cout << "Elapsed time for round in milliseconds: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(round_end - round_start).count()
                  << std::endl;
        if(current_best_count > rounds_without_upgrade){
            break;
        }
    }
    ga.print_fitness_inds();
    ga.save_best_to_file();

    auto end = std::chrono::steady_clock::now();

    std::cout << "Elapsed time in seconds: "
              << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
              << " s - " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
    std::cout << "Elapsed time for genetics in seconds: "
              << std::chrono::duration_cast<std::chrono::seconds>(end - reading_end).count()
              << " s - " << std::chrono::duration_cast<std::chrono::milliseconds>(end - reading_end).count()
              << std::endl;

    return GENERAL_SUCCESS;
}

int main(int argc, char const *argv[]){

    run_genetics(argv[1]);

//    test_mutation(argv[1]);
    return GENERAL_SUCCESS;
}
