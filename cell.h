#ifndef CELL_HEADER
#define CELL_HEADER

#include <utility>

// GRID CELL CLASS

class Cell{
    public:
        Cell();

        Cell(std::pair<int, int>);

        std::pair<int, int> getPosition();

        void setObstacle(bool);

        bool isObstacle();
    
        bool isGoal();

        void setExplored(bool);

        bool isExplored();

        void setPath(bool);
        
        bool isPath();

        void resetState();

    private:
        bool _isGoal;
        bool _isObstacle;
        bool _isExplored;
        bool _isPath;

        std::pair<int, int> _position;
};

#endif /* !CELL_HEADER */
