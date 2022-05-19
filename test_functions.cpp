#include<iostream>
//#include "helpfull_functions.cpp"
#include <map>
#include "classes.cpp"

using std::cout; using std::endl;

int main () {
#include <map>

    std::map<char, char> my_map = {
            { 'A', '1' },
            { 'B', '2' },
            { 'C', '3' }
    };

    char c = 'A';

    if ( my_map.find(c) == my_map.end() ) {
        cout << "no" << endl;
    } else {
        cout << my_map[c] << endl;
    }

    init_random_seq();
    for(int i = 0; i < 0; i++){
        cout << get_random_int(10, 2) << endl;
    }
    POINT p1 = {1, 1};
    POINT p2 = {2, 2};

    std::cout << "distance " << calculate_distance(p1, p2) << std::endl;

	return 0;
}