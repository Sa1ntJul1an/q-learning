#include "q-learner.h"
#include "car.h"
#include "cell.h"
#include "position.h"
#include <iostream>
#include <limits>
#include <random>
#include <utility>


QLearner::QLearner(int width, int height, std::map<std::pair<int, int>, Cell*> stateSpace, float cellSize, float gamma, float alpha, float initEpsilon, float finalEpsilon, int numActions, int numEpochs, int maxIterationsPerEpoch, std::pair<float, float> agentInitialPos) {
  _width = width;
  _height = height;
  _cellSize = cellSize;
  _stateSpace = stateSpace;

  _learningRate = alpha;
  _discountFactor = gamma;
  _initialEpsilon = initEpsilon;
  _finalEpsilon = finalEpsilon;
  _numActions = numActions;

  _agentInitialPosition = agentInitialPos;

  _maxIterationsPerEpoch = maxIterationsPerEpoch;
  _numEpochs = numEpochs;
  _currentEpoch= 0;

  _learningComplete = false;

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
  
  // begin a new epoch (initialize agent to initial position)
  newEpoch();
};

void QLearner::train() {
  // train until all epochs complete 
  std::cout << "Training...\n";
  int lastEpochLogged = getCurrentEpoch();
  while (!doneLearning()) {
    if (getCurrentEpoch() % 100 == 0 && getCurrentEpoch() != lastEpochLogged) {
      std::cout << getCompletionPercentage() << "\% complete.\n";
      lastEpochLogged = getCurrentEpoch();
    }
    update();
  }
  std::cout << "Training complete.\n";
}

void QLearner::update() {
  if (_learningComplete) {
    return;
  }

  _currentIteration ++;

  std::pair<float, float> currentAgentPos = _agent->getPosition().getXY();
  std::pair<int, int> state = {currentAgentPos.first / _cellSize, currentAgentPos.second / _cellSize};

  // decide whether to explore or exploit using epsilon greedy
  bool exploring = exploreOrExploit();
  int action;
  if (exploring) {    // take random action
    std::uniform_int_distribution<> dis(0, _numActions - 1);
    action = dis(gen);
  } else {            // take best action
    action = getBestAction();
  }

  // take action
  _agent->takeAction(action);
  
  // get next state based on action taken
  currentAgentPos = _agent->getPosition().getXY();
  std::pair<int, int> newState = {currentAgentPos.first / _cellSize, currentAgentPos.second / _cellSize};

  float newStateReward = getReward(newState);

  float Q_new = (1 - _learningRate) * _qMatrix[state][action] + _learningRate * (newStateReward + _discountFactor * getOptimalFutureReward(newState));

  _qMatrix[state][action] = Q_new;

  if (_currentIteration > _maxIterationsPerEpoch) {
    newEpoch();
  }

  if (_currentEpoch >= _numEpochs) {
    // done learning 
    _learningComplete = true;
  }
}

bool QLearner::doneLearning() {
  return _learningComplete;
}

int QLearner::getCurrentEpoch() {
  return _currentEpoch;
}

float QLearner::getCompletionPercentage() {
  return 100.0 * static_cast<float>(_currentEpoch) / static_cast<float>(_numEpochs);
}

float QLearner::getReward(std::pair<int, int> state) {
  if (state.first > _width || state.first < 0 || state.second > _height || state.second < 0) {      // agent has left state space
    newEpoch();
    return _outOfStateSpacePenalty;
  } else if (_stateSpace[state]->isGoal()) {      // goal reached
    return _goalReward;
    newEpoch();
  } else if (_stateSpace[state]->isObstacle()) {    // obstacle hit
    return _obstaclePenalty;
  } else {          // no obstacle (on path)
    return 0.0;
  }
}

float QLearner::getOptimalFutureReward(std::pair<int, int> state) {
  // get max Q value for all possible actions that can be taken from current state
  // - this function calculate the maximum reward that could be obtained from the next possible state 
  // - to get this, we must simulate all moves the car could take from the current state, and get all the potential new states that it could reach 
  // - then, for each of these new states, we must find the highest Q value given all possible actions that could be taken at each of these new states 
  float maxReward = -std::numeric_limits<float>::infinity();
  
  for (int action = 0; action < _numActions; action++) {     // for all actions that could be taken at this current state, calculate the new state that would be reached
    Position newAgentPos = _agent->dubinsMove(action);
    std::pair<int, int> newState = {newAgentPos.getXY().first / _cellSize, newAgentPos.getXY().second / _cellSize};
    if (newState.first < 0 || newState.first > _width || newState.second < 0 || newState.second > _height) {
      continue;   // cannot go outside of state space
    }
    float bestFutureReward = -std::numeric_limits<float>::infinity();
    for (int nextAction = 0; nextAction < _numActions; nextAction++) {        // at this new state that could be reached, find the max reward value across all actions that could be taken
      if (_qMatrix[newState][nextAction] > bestFutureReward) {
        bestFutureReward = _qMatrix[newState][nextAction];
      }
    }
    // discount future reward
    bestFutureReward *= _discountFactor;
    if (bestFutureReward > maxReward) {
      maxReward = bestFutureReward;
    }
  }
  if (maxReward == -std::numeric_limits<float>::infinity()) {   // if no states are reachable, we should restart this epoch as the agent is leaving the state space 
    newEpoch();
    return _outOfStateSpacePenalty;
  }
  return maxReward;
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

int QLearner::getBestAction() {
  std::pair<float, float> agentPos = _agent->getPosition().getXY();
  std::pair<int, int> state = {agentPos.first / _cellSize, agentPos.second / _cellSize};

  int bestAction = 0;
  float highestReward = _qMatrix[state][bestAction];
  for (int action = 1; action < _numActions; action++) {
     if (_qMatrix[state][action] > highestReward) {
       highestReward = _qMatrix[state][action];
       bestAction = action;
     }
  }
  return bestAction;
}

void QLearner::newEpoch() {
  // restart agent to initial position
  if (_currentEpoch > 0) {
    delete _agent;
  }
  _agent = new Car(_turnRadius, _driveDist, _agentInitialPosition, _initAngleRad, _numActions);
  _currentIteration = 0;
  _currentEpoch ++;
}

std::random_device QLearner::rd;
std::mt19937 QLearner::gen(QLearner::rd());
