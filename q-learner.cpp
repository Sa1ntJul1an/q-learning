#include "q-learner.h"
#include <random>


QLearner::QLearner(int width, int height, float gamma, float initEpsilon, float finalEpsilon, int numActions, int numEpochs, int maxIterationsPerEpoch) {
  _width = width;
  _height = height;
  _discountFactor = gamma;
  _initialEpsilon = initEpsilon;
  _finalEpsilon = finalEpsilon;
  _numActions = numActions;

  _maxIterationsPerEpoch = maxIterationsPerEpoch;
  _numEpochs = numEpochs;
  _currentEpoch= 0;

  // resize q matrix to proper dimensions
  for (int i = 0; i < _width; i++) {
    for (int j = 0; j < _height; j++) {
      std::pair<int, int> state = {i, j};
      _qMatrix[state].resize(_numActions);
      for (int k = 0; k < _numActions; k++) {
        _qMatrix[state][k] = 0.0;
      }
    }
  }
};

void QLearner::update() {
  _currentEpoch ++;
  
  // decide whether to explore or exploit using epsilon greedy
  bool exploring = exploreOrExploit();

  
}

float QLearner::getEpsilon() {
  // epsilon will linearly decay from initial value to final value 
  if (_currentEpoch >= _numEpochs) {
    return _finalEpsilon;
  }
  return _initialEpsilon - (static_cast<float>(_currentEpoch) * (_initialEpsilon - _finalEpsilon) / static_cast<float>(_numEpochs));
}

bool QLearner::exploreOrExploit() {   // epsilon greedy exploration function 
  // return true if exploring (random action), return false if exploiting (best action)
  float epsilon = getEpsilon();

  std::uniform_real_distribution<> dis(0, 1.0);

  if (dis(gen) < epsilon) {
    // explore (random action)
    return true;
  } else {
    // exploit (best action)
    return false;
  }
}

void QLearner::newEpoch() {
  // restart agent to initial position
}

std::random_device QLearner::rd;
std::mt19937 QLearner::gen(QLearner::rd());
