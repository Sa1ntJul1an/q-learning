#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "cell.h"


using namespace sf;
using namespace std;

// width in height, in grid cells
const int WIDTH = 30;
const int HEIGHT = 30;

// size of cell, pixels
const int CELL_SIZE = 30;

const vector<int> start = {0, HEIGHT - 1};
const vector<int> goal = {WIDTH - 1, 0};

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


int main(){

  vector<float> mousePosition;

  vector<vector<Cell*>> cells;

  // fill state space 
  for (int row = 0; row < WIDTH; row ++){
    vector<Cell*> column;
    for (int col = 0; col < HEIGHT; col ++){
      vector<int> position = {row, col};
      if (position == start){
        column.push_back(startCell);
      } else if (position == goal){
        column.push_back(goalCell);
      } else{
        Cell* cell = new Cell({row, col});
        column.push_back(cell);
      }
    }
    cells.push_back(column);
  }
  
  // SEARCH RENDER WINDOW
  // =======================================================================
  RenderWindow renderWindow(VideoMode(WIDTH * CELL_SIZE + 1, HEIGHT * CELL_SIZE + 2), "Search Algorithm");
  // =======================================================================

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

  bool learning = false;
  int iteration = 0;

  const int ITERATIONS = 10000;

  while(renderWindow.isOpen()){

    mousePosition = {float(Mouse::getPosition(renderWindow).x), float(Mouse::getPosition(renderWindow).y)};

    if (!learning && !(iteration >= ITERATIONS)){
      if (Mouse::isButtonPressed(Mouse::Left)){       // set as obstacle
        int row = mousePosition[0] / CELL_SIZE;
        int col = mousePosition[1] / CELL_SIZE;

        if (row < WIDTH && row >= 0 && col < HEIGHT && col >= 0) {
          cells[row][col]->setObstacle(true);
        }
      } else if (Mouse::isButtonPressed(Mouse::Right)) {      // remove obstacle
        int row = mousePosition[0] / CELL_SIZE;
        int col = mousePosition[1] / CELL_SIZE;

        if (row < WIDTH && row > 0 && col < HEIGHT && col > 0) {
          cells[row][col]->setObstacle(false);
        }
      }
    }

    if (learning) {
      // update sim here
      iteration++;
    }

    // KEYBOARD EVENTS =========================================
    if (!learning && Keyboard::isKeyPressed(Keyboard::Space)){   // space to start search
      learning = true;
    }
    if (Keyboard::isKeyPressed(Keyboard::R)){       // R to reset
      cout << "Restarting search, clearing state space." << endl;
      for (int row = 0; row < WIDTH; row++){
        for (int col = 0; col < HEIGHT; col++){
          cells[row][col]->resetState();
        }
      }
      renderWindow.setFramerateLimit(60);
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

    Color color;
    for (int row = 0; row < WIDTH; row ++){
      for (int col = 0; col < WIDTH; col ++){
        Cell* currentCell = cells[row][col];
        vector<int> cellPosition = cells[row][col]->getPosition();
        cellRect.setPosition(Vector2f(double(cellPosition[0] * CELL_SIZE), double(cellPosition[1] * CELL_SIZE)));
        if (currentCell->isPath()){
          color = path_color;
        }else if (currentCell->isFrontier()){
          color = frontier_color;
        }else if (currentCell->isExplored()){
          color = explored_color;
        } else if (currentCell->isObstacle()){
          color = obstacle_color;
        } else {
          color = default_color;
        }
        cellRect.setFillColor(color);
        renderWindow.draw(cellRect);
      }
    }

    // DRAW START
    cellRect.setFillColor(startColor);
    cellRect.setPosition(Vector2f(double(start.at(0) * CELL_SIZE), double(start.at(1) * CELL_SIZE)));
    renderWindow.draw(cellRect);

    // DRAW GOAL
    cellRect.setFillColor(goalColor);
    cellRect.setPosition(Vector2f(double(goal.at(0) * CELL_SIZE), double(goal.at(1) * CELL_SIZE)));
    renderWindow.draw(cellRect);

    // DRAW VERTICAL GRID LINES
    int greyValue = 50;
    Color gridColor(greyValue, greyValue, greyValue);
    for (int i = 0; i <= WIDTH; i++){
      RectangleShape verticalLine(Vector2f(2, CELL_SIZE * HEIGHT));
      Color color(greyValue, greyValue, greyValue);
      verticalLine.setFillColor(gridColor);
      verticalLine.setPosition(Vector2f(i * CELL_SIZE, 0));
      renderWindow.draw(verticalLine);
    }

    // DRAW HORIZONTAL GRID LINES
    for (int i = 0; i <= HEIGHT; i++){
      RectangleShape horizontalLine(Vector2f(CELL_SIZE * HEIGHT, 2));
      horizontalLine.setFillColor(gridColor);
      horizontalLine.setPosition(Vector2f(0, i * CELL_SIZE));
      renderWindow.draw(horizontalLine);
    }
    // ==========================================================
    iterationText.setString("Iteration: " + to_string(iteration));
    renderWindow.draw(iterationText);
    renderWindow.display();
  }

  delete startCell;
  delete goalCell;

  for (int row = 0; row < WIDTH; row ++){
    for (int col = 0; col < HEIGHT; col ++){
      delete cells.at(row).at(col);
    }
  }

  return 0;
}
