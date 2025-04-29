#ifndef POSITION_HEADER
#define POSITION_HEADER

#include <utility>

class Position {
  public:
    Position(std::pair<float, float> xy, float thetaRad);

    std::pair<float, float> getXY();
    float getThetaRad();

  private:
    std::pair<float, float> _xy; 
    float _thetaRad;
};

#endif // !POSITION_HEADER
