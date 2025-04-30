#include "agent-history-node.h"

#include <utility>


AgentHistoryNode::AgentHistoryNode(std::pair<float, float> position, float thetaRad) {
  _agentPosition = position;
  _agentThetaRad = thetaRad;
  _next = nullptr;
}

void AgentHistoryNode::addNext(AgentHistoryNode* next) {
  _next = next;
}

AgentHistoryNode* AgentHistoryNode::getNext() {
  return _next;
}

std::pair<float, float> AgentHistoryNode::getPosition() {
  return _agentPosition;
}

float AgentHistoryNode::getThetaRad() {
  return _agentThetaRad;
}
