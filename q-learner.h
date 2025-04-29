#ifndef Q_LEARNER_HEADER
#define Q_LEARNER_HEADER

#include "car.h"
#include "cell.h"
#include <map>
#include <random>
#include <vector>

class QLearner {
  public:
    QLearner(int stateSpaceWidth, int stateSpaceHeight, std::map<std::pair<int, int>, Cell*> stateSpace, float cellSize, float discountFactor, float learningRate, float initialEpsilon, float finalEpsilon, int numActions, int numEpochs, int maxIterations, std::pair<float, float> agentInitialPos);
    
    void train();

    bool doneLearning();
    int getCurrentEpoch();
    float getCompletionPercentage();

  private:
    // Q Matrix: 
    //  - key is two spatial dimensions to represent state (pair of ints) 
    //  - value is vector of n actions per state (vector of floats)
    std::map<std::pair<int, int>, std::vector<float>> _qMatrix;

    int _numEpochs;
    int _currentEpoch;

    int _maxIterationsPerEpoch;
    int _currentIteration; 

    int _width;
    int _height;
    float _cellSize;

    std::map<std::pair<int, int>, Cell*> _stateSpace;
    
    float _initialEpsilon;
    float _finalEpsilon;

    bool _learningComplete;

    float _discountFactor;
    float _learningRate;

    // CAR AGENT
    Car* _agent;
    std::pair<float, float> _agentInitialPosition;

    int _numActions;
    const float _turnRadius = 8.0;
    const float _driveDist = 15.0;
    const float _initAngleRad = 0.0;
    
    // REWARD FUNCTION
    const float _obstaclePenalty = -50.0;
    const float _goalReward = 100.0;
    const float _outOfStateSpacePenalty = -100.0;

    void update();
    float getReward(std::pair<int, int>);
    float getOptimalFutureReward(std::pair<int, int>);
    float getEpsilon();
    bool exploreOrExploit();
    int getBestAction();
    void newEpoch();

    static std::random_device rd;
    static std::mt19937 gen;

    // TODO: maybe keep track of action history, a vector of vectors of ints 
    //  each vector represents one agents vector of actions in order 
    //  if each agent begins at same position, we can walk over each action and visualize them to see all the different tries 
    //  maybe data structure is linked list as num of actions will be different, go until nullptr 
};


#endif // !Q_LEARNER_HEADER
