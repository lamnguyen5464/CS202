#include "Header.h"

using namespace std;
GlobalConfig::GlobalConfig()
{
    lastSignal = ' ';
    matrix = new bool* [Game::getRows()];
    for (int i = 0; i < Game::getRows(); ++i) {
        matrix[i] = new bool [Game::getColumns()] {0}; 
    }
}

GlobalConfig *GlobalConfig::instance = nullptr;

GlobalConfig *GlobalConfig::getInstance()
{
    if (instance == nullptr)
    {
        instance = new GlobalConfig();
    }
    return instance;
}

void GlobalConfig::resetMatrix()
{
    drawing_matrix.clear();
    while (drawingQueue.size()){
        drawingQueue.pop();
    }
    vector<char> tmp;
    for (int i = 0; i <= Game().getColumns(); i++)
    {
        tmp.push_back(' ');
    }

    for (int i = 0; i <= Game().getColumns(); i++)
    {
        drawing_matrix.push_back(tmp);
    }
}

void GlobalConfig::initNewData(int setScore)
{
    currentScore = setScore;
    if (setScore == 0) {
        GlobalConfig::getInstance()->d1 = 0;
        GlobalConfig::getInstance()->d2 = 0;
        GlobalConfig::getInstance()->d3 = 0;
        GlobalConfig::getInstance()->d4 = 0;
        // GlobalConfig::getInstance()->resetMatrix();
    }
    updated = false;
    People::getPeople()->curX = Game::getColumns() / 2;
    People::getPeople()->curY = Game::getRows() - 1;
}
