#include "car.h"
#include "position.h"

#include <cmath>
#include <limits>
#include <random>
#include <utility>


Car::Car(float turnRadius, float driveDist, std::pair<float, float> initPosition, float initAngleRad, int numPossibleActions) {
  _turnRadius = turnRadius;
  _driveDistance = driveDist;
  _position = initPosition;
  _theta = initAngleRad;
  _numPossibleActions = numPossibleActions;
}

void Car::takeAction(int action) {
  Position newPos = dubinsMove(action);
  _position = newPos.getXY();
  _theta = newPos.getThetaRad();
}

Position Car::getPosition() {
  return Position(_position, _theta);
}

Position Car::dubinsMove(int action) {
  float maxCurvature = 1.0 / _turnRadius;
  float curvatureRange = 2.0 * maxCurvature;
  float curvatureStep = curvatureRange / (_numPossibleActions - 1.0);

  float curvature;
  if (_numPossibleActions <= 1) {
    curvature = 0.0;
  } else {
    curvature = -maxCurvature + action * curvatureStep;
  }
  
  float turningRadius;
  if (std::abs(curvature) < 1e-6) {
    turningRadius = std::numeric_limits<float>::infinity();
  } else {
    turningRadius = 1.0 / curvature;
  }

  float signedRadius = turningRadius;
  if (curvature < 0) {
    signedRadius = -turningRadius;
  }

  float x = _position.first;
  float y = _position.second;

  std::pair<float, float> nextPosition;
  float nextTheta;

  if (std::isinf(std::abs(signedRadius))) {
    nextPosition.first = x + _driveDistance * std::cos(_theta);
    nextPosition.second = y + _driveDistance * std::sin(_theta);
    nextTheta = _theta;
  } else {
    float angularChange = _driveDistance / signedRadius;
    float center_x, center_y;
    if (signedRadius > 0) {   // left turn
      center_x = x - signedRadius * std::sin(_theta);
      center_y = y + signedRadius * std::cos(_theta);
      nextPosition.first = center_x + signedRadius * std::sin(_theta + angularChange);
      nextPosition.second = center_y - signedRadius * std::cos(_theta + angularChange);
      nextTheta = _theta + angularChange;
    } else {    // right turn 
      center_x = x - signedRadius * std::sin(_theta);
      center_y = y + signedRadius * std::cos(_theta);
      nextPosition.first = center_x + signedRadius * std::sin(_theta + angularChange);
      nextPosition.second = center_y - signedRadius * std::cos(_theta + angularChange);
      nextTheta = _theta;
    }
  }

  return Position(nextPosition, nextTheta);
}

std::random_device Car::rd;
std::mt19937 Car::gen(Car::rd());
