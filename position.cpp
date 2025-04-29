#include "position.h"

#include <utility>

Position::Position(std::pair<float, float> xy, float thetaRad) {
  _xy = xy;
  _thetaRad = thetaRad;
}

std::pair<float, float> Position::getXY() {
  return _xy;
}

float Position::getThetaRad() {
  return _thetaRad;
}
