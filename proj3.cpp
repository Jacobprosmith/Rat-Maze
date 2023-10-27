#include <iostream>
#include <cstdlib>
#include "Stack.h"
#include <iostream>
#include <string>
#include <fstream>
#include <istream>


using namespace std;
using namespace cop4530;
const int NORTH_WALL =           0b0001;
const int EAST_WALL =            0b0010;
const int SOUTH_WALL =           0b0100;
const int WEST_WALL =            0b1000;
const int START =               0b10000;
const int FINISH =             0b100000;
const int MOVE_UP =           0b1000000;
const int MOVE_RIGHT =       0b10000000;
const int MOVE_DOWN =       0b100000000;
const int MOVE_LEFT =      0b1000000000;


int getNextNum(std::istream& is) {
    char buffer[16] = {0};
    int bufferIndex = 0;
    char c;
    while (!(is.peek() >= 48 && is.peek() <= 57)) {
        is.read(&c, 1);
    }
    while (is.peek() >= 48 && is.peek() <= 57) {
        is.read(&c, 1);

        if (c >= 48 && c <= 57) {
            buffer[bufferIndex] = c;
            bufferIndex++;
        }
    }
    return atoi(buffer);
}

class Rat {

private:
    bool **visited;
    int numRows;
    int numCols;
    Stack<int> pathR;
    Stack<int> pathC;
    int **maze;
    bool startPoint = true;
    int lastDirection;

public:
    Rat(int numR_in, int numC_in) {
        numRows = numR_in;
        numCols = numC_in;
        visited = new bool*[numRows];
        for (int i = 0; i < numRows; i++) {
            visited[i] = new bool[numCols];
        }
        for (int r = 0; r < numRows; r++) {
            for (int c = 0; c < numCols; c++) {
                visited[r][c] = false;
            }
        }
    }

    ~Rat() {
        for (int i = 0; i < numRows; i++) {
            delete[] visited[i];
        }
        delete[] visited;
    }

    string getPath(int r, int c, bool type, bool wall) {
        if (!wall) {
            if (type) {
                if (maze[r][c] & START) {
                    lastDirection = START;
                    return " S";
                } else if (maze[r][c] & FINISH) {
                    lastDirection = FINISH;
                    return " F";
                } else {
                    lastDirection = 0;
                    return "  ";
                }
            } else {
                if (maze[r][c] & START) {
                    lastDirection = START;
                    return " S";
                } else if (maze[r][c] & FINISH) {
                    lastDirection = FINISH;
                    return " F";
                } else if (maze[r][c] & MOVE_UP) {
                    if (lastDirection == MOVE_RIGHT) {
                        lastDirection = MOVE_UP;
                        return ">^";
                    } else if (lastDirection == MOVE_LEFT){
                        lastDirection = MOVE_UP;
                        return "<^";
                    } else {
                        lastDirection = MOVE_UP;
                        return " ^";
                    }
                } else if (maze[r][c] & MOVE_DOWN) {
                    if (lastDirection == MOVE_RIGHT) {
                        lastDirection = MOVE_DOWN;
                        return ">v";
                    } else if (lastDirection == MOVE_LEFT) {
                        lastDirection = MOVE_DOWN;
                        return "<v";
                    } else {
                        lastDirection = MOVE_DOWN;
                        return " v";
                    }
                } else if (maze[r][c] & MOVE_RIGHT) {
                    lastDirection = MOVE_RIGHT;
                    return ">>";
                } else if (maze[r][c] & MOVE_LEFT) {
                    lastDirection = MOVE_LEFT;
                    return "<<";
                } else {
                    return "  ";
                }
            }
            // wall
        } else {
            if (type) {
                if (maze[r][c] & START) {
                    lastDirection = START;
                    return "S";
                } else if (maze[r][c] & FINISH) {
                    lastDirection = FINISH;
                    return "F";
                } else {
                    lastDirection = 0;
                    return " ";
                }
            } else {
                if (maze[r][c] & START) {
                    lastDirection = START;
                    return "S";
                } else if (maze[r][c] & FINISH) {
                    lastDirection = FINISH;
                    return "F";
                } else if (maze[r][c] & MOVE_UP) {
                    lastDirection = MOVE_UP;
                    return "^";
                } else if (maze[r][c] & MOVE_DOWN) {
                    lastDirection = MOVE_DOWN;
                    return "v";
                } else if (maze[r][c] & MOVE_RIGHT) {
                    lastDirection = MOVE_RIGHT;
                    return ">";
                } else if (maze[r][c] & MOVE_LEFT) {
                    lastDirection = MOVE_LEFT;
                    return "<";
                } else {
                    lastDirection = 0;
                    return " ";
                }
            }
        }
    }

    void setMaze(int **maze_in){
        maze = maze_in;
    }

    bool getVisited(int r, int c) {
        return visited[r][c];
    }

    void setVisited(int r, int c) {
        visited[r][c] = true;
    }

    void pathPop(int r, int c) {
        pathR.pop();
        pathC.pop();
    }

    void pathPush(int r, int c) {
        pathR.push(r);
        pathC.push(c);
    }

    bool findFood(int r, int c, int finalR, int finalC) {
        // create neighbor list
        if (startPoint) {
            startPoint = false;
            pathPush(r, c);
        }
        visited[r][c] = true;
        if (r == finalR && c == finalC) {
            return true;
        }
        Stack<int> neighborR;
        Stack<int> neighborC;
        // Search order = W S E N
        if (!(maze[r][c] & WEST_WALL)) {
            neighborR.push(r);
            neighborC.push(c-1);
        }
        if (!(maze[r][c] & SOUTH_WALL)) {
            neighborR.push(r+1);
            neighborC.push(c);
        }
        if (!(maze[r][c] & EAST_WALL)) {
            neighborR.push(r);
            neighborC.push(c+1);
        }
        if (!(maze[r][c] & NORTH_WALL)) {
            neighborR.push(r-1);
            neighborC.push(c);
        }
        int size = neighborR.size();
        // for each neighbor in the list
        for (int i = 0; i < size; i++) {
            if (!visited[neighborR.top()][neighborC.top()]) {
                pathR.push(neighborR.top());
                pathC.push(neighborC.top());
                bool foundFood = false;
                foundFood = findFood(neighborR.top(), neighborC.top(), finalR, finalC);
                if (foundFood) {
                    return true;
                } else {
                    pathR.pop();
                    pathC.pop();
                }
            }
            neighborR.pop();
            neighborC.pop();
        }
        return false;
    }

    void pathPrint() {
        Stack<int> pathFinalR;
        Stack<int> pathFinalC;
        if (pathR.size() == 1) {
            cout << "Man, no way to the food" << endl;
            exit(0);
        }
        cout << "Maze solution: " << endl;
        int size = pathR.size();
        for (int i = 0; i < size; i++) {
            if (!(i == 0 || i == size-1)) {
                if (pathFinalR.top() == pathR.top()) {
                    if (pathFinalC.top() < pathC.top()) {
                        maze[pathR.top()][pathC.top()] |= MOVE_LEFT;
                    } else {
                        maze[pathR.top()][pathC.top()] |= MOVE_RIGHT;
                    }
                } else {
                    if (pathFinalR.top() < pathR.top()) {
                        maze[pathR.top()][pathC.top()] |= MOVE_UP;
                    } else {
                        maze[pathR.top()][pathC.top()] |= MOVE_DOWN;
                    }
                }
            }
            pathFinalR.push(pathR.top());
            pathFinalC.push(pathC.top());

            pathC.pop();
            pathR.pop();

        }
        for (int r = 0; r < numRows; r++) {
            for (int c = 0; c < numCols; c++) {
                //cout << maze[r][c];
                //cout << " ";
            }
            //cout << endl;
        }
        for (int i = 0; i < size; i++) {
            cout << "(" << pathFinalR.top() << "," << pathFinalC.top() << ")";
            if (i == size-1) {
                cout << endl;
            } else {
                cout << "->";
            }
            pathFinalR.pop();
            pathFinalC.pop();
        }
    }

    void drawMap(bool type) {
        for (int c = 0; c < numCols; c++) {
            if (maze[0][c] & WEST_WALL) {
                cout << "+-";
            } else {
                cout << "--";
            }
            if (c == numCols - 1) {
                cout << "+" << endl;
            }
        }
        for (int r = 0; r < numRows; r++) {
            for (int c = 0; c < numCols; c++) {
                // Draw West Walls
                if (maze[r][c] & WEST_WALL) {
                    cout << "|" << getPath(r, c, type, true);
                } else {
                    cout << getPath(r, c, type, false);
                }
                if (c == numCols - 1) {
                    // Draw West Wall
                    cout << "|" << endl;

                    // Draw South Walls
                    for (int c_tmp = 0; c_tmp < numCols; c_tmp++) {
                        if ((maze[r][c_tmp] & WEST_WALL) && (maze[r+1][c_tmp] & WEST_WALL) && !(maze[r][c_tmp] & SOUTH_WALL)) {
                            if (c_tmp >= 1) {
                                if (!(maze[r][c_tmp - 1] & SOUTH_WALL)) {
                                    cout << "| ";
                                } else {
                                    cout << "+ ";
                                }
                            } else {
                                cout << "| ";
                            }
                        } else if (!(maze[r][c_tmp] & WEST_WALL) && !(maze[r+1][c_tmp] & WEST_WALL) && !(maze[r][c_tmp] & SOUTH_WALL) && !(maze[r][c_tmp - 1] & SOUTH_WALL)) {
                            cout << "  ";
                        } else if (!(maze[r][c_tmp] & WEST_WALL) && (maze[r+1][c_tmp] & WEST_WALL) && !(maze[r][c_tmp] & SOUTH_WALL) && !(maze[r][c_tmp - 1] & SOUTH_WALL)) {
                            cout << "  ";
                        } else if ((maze[r][c_tmp] & WEST_WALL) && !(maze[r+1][c_tmp] & WEST_WALL) && !(maze[r][c_tmp] & SOUTH_WALL) && !(maze[r][c_tmp - 1] & SOUTH_WALL)) {
                            cout << "  ";
                        } else if (!(maze[r][c_tmp] & WEST_WALL) && !(maze[r+1][c_tmp] & WEST_WALL) && !(maze[r][c_tmp] & SOUTH_WALL) && (maze[r][c_tmp - 1] & SOUTH_WALL)) {
                            cout << "  ";
                        } else if (!(maze[r][c_tmp] & WEST_WALL) && !(maze[r+1][c_tmp] & WEST_WALL) &&  (maze[r][c_tmp] & SOUTH_WALL) && !(maze[r][c_tmp - 1] & SOUTH_WALL)) {
                            cout << " -";
                        } else if (((maze[r][c_tmp] & WEST_WALL) ||  (maze[r+1][c_tmp] & WEST_WALL)) &&  (maze[r][c_tmp] & SOUTH_WALL)) {
                            cout << "+-";
                        } else if (((!(maze[r][c_tmp] & WEST_WALL) && !(maze[r+1][c_tmp] & WEST_WALL)) &&  (maze[r][c_tmp] & SOUTH_WALL) && (maze[r][c_tmp - 1] & SOUTH_WALL))) {
                            cout << "--";
                        } else if ((((maze[r][c_tmp] & WEST_WALL) ||  (maze[r+1][c_tmp] & WEST_WALL)) && !(maze[r][c_tmp] & SOUTH_WALL) && (maze[r][c_tmp - 1] & SOUTH_WALL))) {
                            cout << "+ ";
                        } else {
                            cout << "$$";
                        }
                       // cout << endl;
                        if (c_tmp == numCols - 1) {
                            //cout << w << fw << s << ps;
                            if (maze[r][c_tmp] & SOUTH_WALL) {
                                cout << '+' << endl;
                            } else {
                                cout << "|" << endl;
                            }
                        }
                    }
                }
            }
        }
        }

};



int main() {

    int numRows = 0;
    int numCols = 0;

    int startR;
    int startC;
    int endR;
    int endC;


    numRows = getNextNum(std::cin);
    // cout << numRows << endl;
    numCols = getNextNum(std::cin);
    // cout << numCols << endl;

    Rat rat(numRows, numCols);

    int **maze = new int*[numRows + 1];
    for (int i = 0; i < numRows + 1; i++) {
        maze[i] = new int[numCols];
    }

    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            maze[r][c] = getNextNum(std::cin);
            // cout << maze[r][c] << " ";
        }
        //cout << endl;
    }
    for (int c = 0; c < numCols; c++) {
        maze[numRows][c] = NORTH_WALL;
    }

    rat.setMaze(maze);
    startR = getNextNum(std::cin);
    startC = getNextNum(std::cin);
    maze[startR][startC] |= START;
    endR = getNextNum(std::cin);
    endC = getNextNum(std::cin);
    maze[endR][endC] |= FINISH;
    // cout << startR << startC << endR << endC << endl;

    rat.findFood(startR, startC, endR, endC);

    cout << "Maze problem:" << endl;
    rat.drawMap(true);

    rat.pathPrint();

    rat.drawMap(false);
    return(0);
}
