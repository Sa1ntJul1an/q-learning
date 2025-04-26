#ifndef CELL_HEADER
#define CELL_HEADER

#include <vector>

// GRID CELL CLASS

class Cell{
    public:
        Cell();

        Cell(std::vector<int>);

        std::vector<int> getPosition();

        void setObstacle(bool);

        bool isObstacle();
    
        void setExplored(bool);

        bool isExplored();

        void setFrontier(bool);

        bool isFrontier();

        void setPath(bool);
        
        bool isPath();

        void setParent(Cell*);

        Cell* getParent();

        void addNeighbor(Cell*);

        std::vector<Cell*> getNeighbors();

        void setFScore(float);

        float getFScore();

        void setGScore(float);

        float getGScore();

        void resetState();

    private:
        std::vector<Cell*> _neighbors;
        Cell* _parent;

        bool _isGoal;
        bool _isObstacle;
        bool _isExplored;
        bool _isFrontier;
        bool _isPath;

        float _gScore;
        float _fScore;
        std::vector<int> _position;
};

#endif /* !CELL_HEADER */