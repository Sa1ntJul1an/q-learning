#ifndef CAR_HEADER
#define CAR_HEADER

#include <random>
#include <utility>

class Car {
  public:
    Car(float turnRadius, float driveDist, std::pair<float, float> initPosition, float initAngle, int numPossibleActions);

    void takeAction(int);
    int takeRandomAction();

    std::pair<float, float> getPosition();

  private:
    float _turnRadius;
    float _driveDistance;
    std::pair<float, float> _position;
    float _theta;
    int _numPossibleActions;

    static std::random_device rd;
    static std::mt19937 gen;

    void dubinsMove(int action);
};

#endif // !CAR_HEADER
