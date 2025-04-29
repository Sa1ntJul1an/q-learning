#ifndef AGENT_HISTORY_NODE_HEADER
#define AGENT_HISTORY_NODE_HEADER

#include <utility>

class AgentHistoryNode {
  public:
    AgentHistoryNode(std::pair<float, float> agentPosition, float agentThetaRad);

    void addNext(AgentHistoryNode*);
    AgentHistoryNode* getNext();
    std::pair<float, float> getPosition();
    float getThetaRad();

  private:
    AgentHistoryNode* _next;
    std::pair<float, float> _agentPosition;
    float _agentThetaRad;
};

#endif // !AGENT_HISTORY_NODE_HEADER
