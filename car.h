#ifndef CAR_HEADER
#define CAR_HEADER

#include <utility>

class Car {
  public:
    Car(float, float, std::pair<float, float>);

  private:
    float _turnRadius;
    float _driveDistance;
    std::pair<float, float> _position;
    std::pair<int, int> _state;
};

#endif // !CAR_HEADER
