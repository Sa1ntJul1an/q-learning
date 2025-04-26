#include "car.h"


Car::Car(float turnRadius, float driveDist, std::pair<float, float> initPosition) {
  _turnRadius = turnRadius;
  _driveDistance = driveDist;
  _position = initPosition;
};


