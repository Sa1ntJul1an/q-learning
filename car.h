#ifndef CAR_HEADER
#define CAR_HEADER

#include "position.h"
#include <random>
#include <utility>

class Car {
  public:
    Car(float turnRadius, float driveDist, std::pair<float, float> initPosition, float initAngle, int numPossibleActions);

    void takeAction(int);

    Position getPosition();

    Position dubinsMove(int action);

  private:
    float _turnRadius;
    float _driveDistance;
    std::pair<float, float> _position;
    float _theta;
    int _numPossibleActions;

    static std::random_device rd;
    static std::mt19937 gen;
};

#endif // !CAR_HEADER
