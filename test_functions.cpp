#include<iostream>
#include "helpfull_functions.cpp"

int main () {
    std::vector<int> vec = {1, 2, 3, 4, 5};

    shuffle_int_vector(vec);

    std::cout << "shuffled elements:";
    for (int& x: vec) std::cout << ' ' << x;
    std::cout << '\n';

	return 0;
}