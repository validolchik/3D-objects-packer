//
// Created by renat on 26.02.2022.
//

#ifndef PROJECT_CLASSES_H
#define PROJECT_CLASSES_H

#endif //PROJECT_CLASSES_H

struct POINT{
    int x;
    int y;
};

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

class Object_on_plate{
public:
    int id;
    OBJECT obj;
    POINT ref_point;
};

class Plate{
public:
    int id, size_x, size_y;
    std::vector<Object_on_plate> objects;
};

