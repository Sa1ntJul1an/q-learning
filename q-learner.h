#ifndef Q_LEARNER_HEADER
#define Q_LEARNER_HEADER

#include <map>
#include <random>
#include <vector>

class QLearner {
  public:
    QLearner(int stateSpaceWidth, int stateSpaceHeight, float discountFactor, float initialEpsilon, float finalEpsilon, int numActions, int numEpochs, int maxIterations);
    
    void update();

  private:
    // Q Matrix: 
    //  - key is two spatial dimensions to represent state (pair of ints) 
    //  - value is vector of n actions per state (vector of floats)
    std::map<std::pair<int, int>, std::vector<float>> _qMatrix;

    int _numEpochs;
    int _currentEpoch;

    int _maxIterationsPerEpoch;
    
    int _width;
    int _height;
    int _numActions;

    float _initialEpsilon;
    float _finalEpsilon;

    float _discountFactor;

    std::pair<int, int> currentState;

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
