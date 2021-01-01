#include "Header.h"

People::People()
{
    pixel = '@';
    body.push_back(Coord(-1, 0));
    body.push_back(Coord(-1, -1));
    body.push_back(Coord(0, 0));
    body.push_back(Coord(-2, 0));
}

void People::draw()
{
    if (curY <= 2){
        GlobalConfig::getInstance()->initNewData(GlobalConfig::getInstance()->currentScore + 10);
        Game().onNextLevel();
        return;
    }

    for (int i = 0; i < body.size(); ++i)
    {
        Game::addPixelToQueue(curX + body[i].x, curY + body[i].y, pixel);
    }
}

void People::erase(int x, int y)
{
    for (int i = 0; i < body.size(); i++)
    {
        Game().addPixelToQueue(x + body[i].x, y + body[i].y, ' ');
    }
}

People* People::getPeople() {
    if (ppl == nullptr) {
        ppl = new People;
    }
    return ppl;
}

People* People::ppl = nullptr;