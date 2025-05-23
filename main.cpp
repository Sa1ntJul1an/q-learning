#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <cmath>
#include <string>
#include <utility>
#include <vector>

#include "agent-history-node.h"
#include "cell.h"
#include "q-learner.h"


using namespace sf;
using namespace std;

// width in height, in grid cells
const int WIDTH = 60;
const int HEIGHT = 30;

// size of cell, pixels
const int CELL_SIZE = 30;

const int RENDER_WINDOW_WIDTH = WIDTH * CELL_SIZE + 1;
const int RENDER_WINDOW_HEIGHT = HEIGHT * CELL_SIZE + 2;

const pair<int, int> start = {0, HEIGHT - 1};
const pair<int, int> goal = {WIDTH - 1, 0};

Cell * startCell = new Cell(start);
Cell * goalCell = new Cell(goal);

Cell * currentPathCell;

Color default_color = Color(0, 0, 0);
Color explored_color = Color(110, 110, 110);
Color obstacle_color = Color(255, 255, 0);
Color frontier_color = Color(255, 0, 255);
Color path_color = Color(0, 255, 255);
Color startColor(0, 255, 0);
Color goalColor(255, 0, 0);
int greyValue = 50;
Color gridColor(greyValue, greyValue, greyValue);

RectangleShape verticalLine(Vector2f(2, CELL_SIZE * HEIGHT));
RectangleShape horizontalLine(Vector2f(CELL_SIZE * WIDTH, 2));


Vector2f convertCoords(pair<float, float> coords) {
  return Vector2f(coords.first, RENDER_WINDOW_HEIGHT - coords.second);
}

// TODO: create more complex reward function that uses euclidean distance to the goal as reward for each path point
// TODO: if I change the drive distance to 25 instead of 15, there is sometimes a segfault... need to figure out why

int main(){

  vector<float> mousePosition;

  std::map<std::pair<int, int>, Cell*> cells;

  // fill state space 
  for (int row = 0; row < HEIGHT; row ++){
    for (int col = 0; col < WIDTH; col ++){
      pair<int, int> position = {col, row}; 
      if (position == goal) {
        cells[position] = goalCell;
      } else{
        Cell* cell = new Cell(position);
        cells[position] = cell;
      }
    }
  }
  
  Font font;
  FileInputStream fontIn;
  fontIn.open("slkscr.ttf");
  font.loadFromStream(fontIn);

  Text iterationText;
  iterationText.setFillColor(Color::Red);
  iterationText.setPosition(10, 10);
  iterationText.setFont(font);
  iterationText.setCharacterSize(30);

  RectangleShape cellRect(Vector2f(CELL_SIZE, CELL_SIZE));

  RectangleShape agentRectangle(Vector2f(10.0, 5.0));
  agentRectangle.setFillColor(Color::Cyan);

  bool learning = false;
  int iteration = 0;

  // Q learning algorithm 
  // =========================================================================================================================================================
  const int EPOCHS = 10000;
  const float GAMMA = 0.7;            // discount factor
  const float ALPHA = 0.8;            // learning rate
  const float initEpsilon = 0.9;      // initial chance of exploration rather than exploitation
  const float finalEpsilon = 0.2;     // final chance of exploration rather than exploitation
  const int numActions = 5;
  const int maxIterationsPerEpoch = 2000;
  const int epochsToStore = 50;

  QLearner qLearner = QLearner(WIDTH, HEIGHT, cells, CELL_SIZE, GAMMA, ALPHA, initEpsilon, finalEpsilon, numActions, EPOCHS, maxIterationsPerEpoch, {CELL_SIZE / 2.0, CELL_SIZE / 2.0}, epochsToStore);
  vector<AgentHistoryNode*> agentHistories = qLearner.train();
  vector<AgentHistoryNode*> currentNode = agentHistories;
  // =========================================================================================================================================================

  // RENDER WINDOW
  // =========================================================================================================================================================
  RenderWindow renderWindow(VideoMode(RENDER_WINDOW_WIDTH, RENDER_WINDOW_HEIGHT), "Q Learning");
  renderWindow.setFramerateLimit(60);
  while(renderWindow.isOpen()){

    mousePosition = {float(Mouse::getPosition(renderWindow).x), float(Mouse::getPosition(renderWindow).y)};

    /*if (!learning && !(iteration >= EPOCHS)){*/
    /*  if (Mouse::isButtonPressed(Mouse::Left)){       // set as obstacle*/
    /*    int col = mousePosition[0] / CELL_SIZE;*/
    /*    int row = mousePosition[1] / CELL_SIZE;*/
    /*    pair<int, int> position = {col, row};*/
    /**/
    /*    if (row < HEIGHT && row >= 0 && col < WIDTH && col >= 0) {*/
    /*      cells[position]->setObstacle(true);*/
    /*    }*/
    /*  } else if (Mouse::isButtonPressed(Mouse::Right)) {      // remove obstacle*/
    /*    int row = mousePosition[0] / CELL_SIZE;*/
    /*    int col = mousePosition[1] / CELL_SIZE;*/
    /*    pair<int, int> position = {row, col};*/
    /**/
    /*    if (row < WIDTH && row > 0 && col < HEIGHT && col > 0) {*/
    /*      cells[position]->setObstacle(false);*/
    /*    }*/
    /*  }*/
    /*}*/

    // KEYBOARD EVENTS =========================================
    if (!learning && Keyboard::isKeyPressed(Keyboard::Space)){   // space to start search
      learning = true;
    }
    if (Keyboard::isKeyPressed(Keyboard::R)){       // R to reset
      cout << "Restarting search, clearing state space." << endl;
      for (int row = 0; row < HEIGHT; row++){
        for (int col = 0; col < WIDTH; col++){
          pair<int, int> position = {col, row};
          cells[position]->resetState();
        }
      }
      iteration = 0;
      learning = false;
    }
    // ==========================================================


    // CLOSE WINDOWS IF X PRESSED
    // ==========================================================
    Event renderWindowEvent;

    while(renderWindow.pollEvent(renderWindowEvent)){
      if(renderWindowEvent.type == Event::Closed){
        renderWindow.close();
      }
    }
    // ==========================================================


    // DRAW CELLS
    // ==========================================================
    renderWindow.clear();

    for (int row = 0; row < HEIGHT; row ++){
      for (int col = 0; col < WIDTH; col ++){
        pair<int, int> position = {col, row};
        Cell* currentCell = cells[position];
        pair<int, int> cellPosition = currentCell->getPosition();
        cellRect.setPosition(Vector2f(double(cellPosition.first * CELL_SIZE), double(cellPosition.second * CELL_SIZE)));
        if (currentCell->isPath()){
          cellRect.setFillColor(path_color);
        }else if (currentCell->isExplored()){
          cellRect.setFillColor(explored_color);
        } else if (currentCell->isObstacle()){
          cellRect.setFillColor(obstacle_color);
        } else {
          cellRect.setFillColor(default_color);
        }
        renderWindow.draw(cellRect);
      }
    }

    // DRAW GOAL
    cellRect.setFillColor(goalColor);
    cellRect.setPosition(Vector2f(double(goal.first * CELL_SIZE), double(goal.second * CELL_SIZE)));
    renderWindow.draw(cellRect);

    // DRAW VERTICAL GRID LINES
    for (int i = 0; i <= WIDTH; i++){
      verticalLine.setFillColor(gridColor);
      verticalLine.setPosition(Vector2f(i * CELL_SIZE, 0));
      renderWindow.draw(verticalLine);
    }

    // DRAW HORIZONTAL GRID LINES
    for (int i = 0; i <= HEIGHT; i++){
      horizontalLine.setFillColor(gridColor);
      horizontalLine.setPosition(Vector2f(0, i * CELL_SIZE));
      renderWindow.draw(horizontalLine);
    }
    // ==========================================================
    
    for (int i = 0; i < agentHistories.size(); i++) {
      agentRectangle.setPosition(convertCoords(agentHistories[i]->getPosition()));
      agentRectangle.setRotation((180.0 / M_PI) * agentHistories[i]->getThetaRad());
      if (agentHistories[i]->getNext() != nullptr) {
        agentHistories[i] = agentHistories[i]->getNext();
      }
      renderWindow.draw(agentRectangle);
    }

    iterationText.setString("Iteration: " + to_string(iteration));
    renderWindow.draw(iterationText);
    renderWindow.display();
  }

  for (int row = 0; row < HEIGHT; row ++){
    for (int col = 0; col < WIDTH; col ++){
      pair<int, int> position = {col, row};
      delete cells[position];
    }
  }

  return 0;
}
