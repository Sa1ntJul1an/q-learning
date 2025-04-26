#include "cell.h"
#include <math.h>
#include <vector>

Cell::Cell(){
    _position = std::vector<int> {0, 0};
    _isGoal = false;
    _isObstacle = false;
    _isExplored = false;
    _isPath = false;
}

Cell::Cell (std::vector<int> position){
    _position = position;
    _isGoal = false;
    _isObstacle = false;
    _isExplored = false;
    _isPath = false;
}

std::vector<int> Cell::getPosition(){
    return this->_position;
}

void Cell::setObstacle(bool isObstacle){
    this->_isObstacle = isObstacle;
}

bool Cell::isObstacle(){
    return this->_isObstacle;
}

void Cell::setExplored(bool isExplored){
    this->_isExplored = isExplored;
}

bool Cell::isExplored(){
    return this->_isExplored;
}

void Cell::setPath(bool isPath){
    this->_isPath = isPath;
}

bool Cell::isPath(){
    return this->_isPath;
}

void Cell::resetState(){
    _isObstacle = false;
    _isExplored = false;
    _isPath = false;
}
