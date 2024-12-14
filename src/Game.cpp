#include "Header.h"

void Game::clearConsole()
{
    system("CLS");
}

int Game::getRows()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return rows;
}

int Game::getColumns()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return columns;
}

void Game::goTo(int posX, int posY)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Position;
    Position.X = posX;
    Position.Y = posY;

    SetConsoleCursorPosition(hStdout, Position);
}

void Game::setWindowSize(int width, int height)
{
    HWND consoleWindow = GetConsoleWindow();
    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
    style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
    SetWindowLong(consoleWindow, GWL_STYLE, style);

    RECT r;
    GetWindowRect(consoleWindow, &r); //stores the console's current dimensions

    MoveWindow(consoleWindow, r.left, r.top, width, height, TRUE);
}

bool Game::controlDirectionKey(int &curX, int &curY, char signal)
{
    switch (signal)
    {
    case 'W':
        // if curY == 1 next stage
        curY--;
        return 1;
    case 'S':
        curY++;
        return 1;
    case 'A':
        curX--;
        return 1;
    case 'D':
        curX++;
        return 1;
    }
    return 0;
}

bool Game::haveStopSignal()
{
    char sign = GlobalConfig::getInstance()->lastSignal;
    return (sign == 'L' || sign == 'Q');
}

void Game::drawPixelInQueue()
{
    while (!Game().haveStopSignal())
    {
        while (GlobalConfig::getInstance()->drawingQueue.size() != 0)
        {
            if (Game().haveStopSignal())
                return;
            Pixel *p = GlobalConfig::getInstance()->drawingQueue.front();
            GlobalConfig::getInstance()->drawingQueue.pop();

            char tmp = GlobalConfig::getInstance()->drawing_matrix[p->x][p->y];

            if (p->pixel != ' ' && tmp != ' ' && p->pixel != tmp)
            {
                GlobalConfig::getInstance()->lastSignal = 'L';
                return;
            }

            GlobalConfig::getInstance()->drawing_matrix[p->x][p->y] = p->pixel;

            Game().goTo(p->x, p->y);
            if (p->pixel >= '0' && p->pixel <= '9')
            {
                Game().textColor(ColorCode_Yellow);
            }
            else
            {
                if (p->pixel == '+' || p->pixel == '-')
                {
                    char currentLight = GlobalConfig::getInstance()->light;
                    if (currentLight == p->pixel)
                        Game().textColor(p->pixel == '+' ? ColorCode_Green : ColorCode_Red);
                    else
                        Game().textColor(ColorCode_Grey);
                }
                else
                {
                    Game().textColor(p->pixel == '@' ? ColorCode_Red : ColorCode_Grey);
                }
            }
            cout << p->pixel;
        }
    }
}

void Game::addPixelToQueue(int x, int y, char pixel)
{
    Pixel *p = new Pixel(x, y, pixel);
    //validate
    if (x < 0 || y < 0 || x > Game().getColumns() || y > Game().getRows())
    {
        return;
    }

    GlobalConfig::getInstance()->drawingQueue.push(p);
    // for (int i = 0; i < People::getPeople()->body.size(); ++i) {
    //     if (People::getPeople()->curX + People::getPeople()->body[i].x == p->x && People::getPeople()->curY + People::getPeople()->body[i].y 
    //     && pixel != ' ') GlobalConfig::getInstance()->lastSignal = 'Q';
    // }
    if (pixel == ' ') GlobalConfig::getInstance()->matrix[x][y] = false;
    else
        GlobalConfig::getInstance()->matrix[x][y] = true;
}

void Game::eventKeyBoardListener()
{
    while (1)
    {
        char t = toupper(_getch());
        if (t == 'L') t = ' ';
        GlobalConfig::getInstance()->lastSignal = t;
        if (t == 'Q')
            return;
    }
}

void truckRun()
{
    Truck obj;
    obj.run();
}

void carRun()
{
    Car obj;
    obj.run();
}

void birdRun()
{
    Bird obj;
    obj.run();
}

void dinasourRun()
{
    Dinasour obj;
    obj.run();
}

void peopleRun()
{
    People::getPeople()->draw();
    while (!Game().haveStopSignal())
    {
        int oldX = People::getPeople()->curX, oldY = People::getPeople()->curY;
        if (Game().controlDirectionKey(People::getPeople()->curX, People::getPeople()->curY, GlobalConfig::getInstance()->lastSignal))
        {
            People::getPeople()->erase(oldX, oldY);
            People::getPeople()->draw();

            if (!Game().haveStopSignal())
                GlobalConfig::getInstance()->lastSignal = ' ';
        }
    }
}

void Game::showScore()
{
    int currentColumn = Game().getColumns() - 5;
    int score = GlobalConfig::getInstance()->currentScore;

    string scoreStr = to_string(score);

    for (int i = 0; i < scoreStr.length(); i++)
    {
        Game().addPixelToQueue(currentColumn + i, 2, ' ');
        Game().addPixelToQueue(currentColumn + i, 2, scoreStr[i]);
    }
}

void Game::onNextLevel()
{
    GlobalConfig::getInstance()->initNewData(GlobalConfig::getInstance()->currentScore + 10);
    GlobalConfig::getInstance()->first = false;
    Game().showScore();
}

void Game::showTrafficLight()
{
    while (!Game().haveStopSignal())
    {
        GlobalConfig::getInstance()->light = (GlobalConfig::getInstance()->light == '+') ? '-' : '+';
        //erase
        Game().addPixelToQueue(Game().getColumns() - 5, 28, ' ');
        Game().addPixelToQueue(Game().getColumns() - 5, 29, ' ');
        Game().addPixelToQueue(1, 23, ' ');
        Game().addPixelToQueue(1, 24, ' ');
        //change
        Game().addPixelToQueue(Game().getColumns() - 5, 28, '+');
        Game().addPixelToQueue(Game().getColumns() - 5, 29, '-');
        Game().addPixelToQueue(1, 23, '+');
        Game().addPixelToQueue(1, 24, '-');
        Sleep(2000);
    }
}



void Game::fontsize(int x, int y)
{
    PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX();
    lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), 0, lpConsoleCurrentFontEx);
    lpConsoleCurrentFontEx->dwFontSize.X = x;
    lpConsoleCurrentFontEx->dwFontSize.Y = y;
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), 0, lpConsoleCurrentFontEx);
}

void Game::textColor(int x)
{
    HANDLE cout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    cout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(cout_handle, x);
}

void Game::drawRectangle(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY)
{
    --topLeftX;
    --topLeftY;
    ++bottomRightX;
    ++bottomRightY;

    int i;

    Game().goTo(topLeftX, topLeftY);
    cout << char(218);
    for (i = topLeftX + 1; i < bottomRightX; ++i)
        cout << char(196);
    cout << char(191);

    for (i = topLeftY + 1; i < bottomRightY; ++i)
    {
        Game().goTo(topLeftX, i);
        cout << char(179);
        Game().goTo(bottomRightX, i);
        cout << char(179);
    }

    Game().goTo(topLeftX, bottomRightY);
    cout << char(192);
    for (i = topLeftX + 1; i < bottomRightX; ++i)
        cout << char(196);
    cout << char(217);
}

void Game::removeRectangle(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY)
{
    --topLeftX;
    --topLeftY;
    ++bottomRightX;
    ++bottomRightY;

    int i;

    Game().goTo(topLeftX, topLeftY);
    cout << " ";
    for (i = topLeftX + 1; i < bottomRightX; ++i)
        cout << " ";
    cout << " ";

    for (i = topLeftY + 1; i < bottomRightY; ++i)
    {
        Game().goTo(topLeftX, i);
        cout << " ";
        Game().goTo(bottomRightX, i);
        cout << " ";
    }

    Game().goTo(topLeftX, bottomRightY);
    cout << " ";
    for (i = topLeftX + 1; i < bottomRightX; ++i)
        cout << " ";
    cout << " ";
}
void Game::showReplayMenu()
{
    string s1 = "YOU lOSE, GUYS :((";
    string s2 = " Current Score: ";
    string s3 = "Press Q to back to menu";
    Game().clearConsole();

    Game().textColor(ColorCode_Pink);
    Game().drawRectangle((getColumns() - s3.length()) / 2 - 1, getRows() / 2 - 7, getColumns() - (getColumns() - s3.length()) / 2 - 1, getRows() / 2 - 1);
    Game().goTo((getColumns() - s1.length()) / 2, getRows() / 2 - 6);

    Game().textColor(ColorCode_Blue);
    cout << s1 << endl;
    
    Game().goTo((getColumns() - s1.length()) / 2, getRows() / 2 - 4);
    cout << s2 << GlobalConfig::getInstance()->currentScore << endl;
   
    Game().textColor(ColorCode_DarkGreen);
    Game().goTo((getColumns() - s3.length()) / 2, getRows() / 2 - 2);
    cout << s3 << endl;
}

void clearAndShowMenu()
{
    Game().clearConsole();
    //instruction

    string title = "---------MENU---------";
    string name = "ROSSING ROAD";
    Game().goTo(1, 30);
    cout << "Type \'W\' for up" << endl
         << " Type \'S\' for down";
    Game().drawRectangle(1, 29, 18, 32);

    //content
    Game().goTo((Game().getColumns() - title.length()) / 2, 18); //allign center
    Game().textColor(ColorCode_Yellow);
    cout << title;
    Game().textColor(ColorCode_DarkBlue);
    Game().goTo(Game().getColumns() / 2, 2);

    Game().drawTitle(7, 82);
    Game().textColor(default_ColorCode);

    Game().drawRectangle(Game().getColumns() / 2 - 10, Game().getRows() / 4 + 8, Game().getColumns() / 2 + 10, Game().getRows() / 4 + 15);
}

void Game::showGroundPlay()
{
    GlobalConfig::getInstance()->resetMatrix();
    Game().clearConsole();
    Game().showScore();
    Game().goTo(0, 40);

    cout << "@ Press Q to quit and save" << endl;

    Game().textColor(ColorCode_DarkGreen);
    Game().goTo(0, 12);
    cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
    Game().goTo(0, 21);
    cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
    Game().goTo(0, 27);
    cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
    Game().goTo(0, 33);
    cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;

    thread keyboardListener(Game().eventKeyBoardListener);
    thread draw(Game().drawPixelInQueue);

    thread people(peopleRun);
    thread truck(truckRun);
    thread car(carRun);
    thread bird(birdRun);
    thread dinasour(dinasourRun);
    thread light(showTrafficLight);

    draw.join();

    if (GlobalConfig::getInstance()->lastSignal == 'L')
    {
        showReplayMenu();
        GlobalConfig::getInstance()->initNewData(0);
    }
    Game().saveGame();

    light.join();
    people.join();
    truck.join();
    car.join();
    bird.join();
    dinasour.join();

    keyboardListener.join();

    GlobalConfig::getInstance()->lastSignal = ' ';
}

void Game::showMenu()
{
    const int numOfOptions = 3;
    string options[numOfOptions] = {
        "New game",
        "Load game",
        "Exit",
    };
    int choice = 0;

    clearAndShowMenu();

    while (1)
    {
        for (int i = 0; i < 3; i++)
        {
            Game().goTo((Game().getColumns() - options[i].length()) / 2, 20 + i * 2);
            Game().textColor(choice == i ? ColorCode_DarkGreen : ColorCode_Grey);
            cout << options[i];
        }

        char getKey = toupper(_getch());
        switch (getKey)
        {
        case 'W':
            choice = (!choice) ? numOfOptions - 1 : choice - 1;
            break;
        case 'S':
            choice = (choice + 1) % numOfOptions;
            break;
        case 13: //Enter
            switch (choice)
            {
            case 0:
                GlobalConfig::getInstance()->initNewData(0);
                GlobalConfig::getInstance()->first = true;
                Game().showGroundPlay();
                break;
            case 1:
                GlobalConfig::getInstance()->first = true;
                Game().loadGame();
                Game().showGroundPlay();
                break;
            case 2:
                return;
            }
            clearAndShowMenu();
        }
    }
}

void Game::saveGame()
{
    ofstream gameData;
    gameData.open("game_data.txt");
    gameData << GlobalConfig::getInstance()->currentScore << endl;
    // gameData << People::getPeople()->curX << " " << People::getPeople()->curY << endl;
    gameData << GlobalConfig::getInstance()->d1 << " " << GlobalConfig::getInstance()->d2 << " " << GlobalConfig::getInstance()->d3 << " " << GlobalConfig::getInstance()->d4 << endl;
    gameData.close();
}

void Game::loadGame()
{
    ifstream gameData;
    gameData.open("game_data.txt");
    if (!gameData.is_open())
    {
        //set init config
        GlobalConfig::getInstance()->initNewData(0);
    }
    else
    {
        gameData >> GlobalConfig::getInstance()->currentScore;
        gameData >> GlobalConfig::getInstance()->d1 >> GlobalConfig::getInstance()->d2 >> GlobalConfig::getInstance()->d3 >> GlobalConfig::getInstance()->d4;
        gameData.close();
        People::getPeople()->curX = Game::getColumns() / 2;
        People::getPeople()->curY = Game::getRows() - 1;
    }
}

void Game::drawTitle(int height, int width)
{

    const signed char TITLE[][82] = {{
                                         32,
                                         95,
                                         95,
                                         95,
                                         95,
                                         95,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         '_',
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         '_',
                                         '_',
                                         '_',
                                         '_',
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                     },
                                     {
                                         (char)178,
                                         32,
                                         32,
                                         '_',
                                         '_',
                                         32,
                                         92,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         (char)178,
                                         32,
                                         (char)178,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         '/',
                                         32,
                                         '_',
                                         '_',
                                         '_',
                                         (char)178,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         '_',
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                         32,
                                     },
                                     {(char)178, 32, (char)178, '_', '_', ')', 32, ')', 32, 32, '_', '_', '_', 32, 32, 32, 32, '_', '_', 32, '_', 32, 32, 32, '_', '_', (char)178, 32, (char)178, 32, 32, 32, 32, 32, 32, '/', 32, 32, (char)178, 32, 32, 32, 32, '_', 32, '_', '_', 32, 32, 32, '_', '_', '_', 32, 32, 32, '_', '_', '_', 32, 32, '_', '_', '_', 32, '(', '_', ')', 32, '_', 32, '_', '_', 32, 32, 32, 32, '_', '_', 32, '_', 32},
                                     {(char)178, 32, 32, '_', 32, 32, '/', 32, 32, '/', 32, '_', 32, '\\', 32, 32, '/', 32, '_', '\'', 32, (char)178, 32, '/', 32, '_', 32, 32, (char)178, 32, 32, 32, 32, 32, (char)124, '|', 32, 32, (char)178, 32, 32, 32, (char)178, 32, '\'', '_', '_', (char)178, 32, '/', 32, '_', 32, '\\', 32, '/', 32, '_', '_', (char)178, '/', 32, '_', '_', (char)178, (char)178, 32, (char)178, '|', 32, '\'', '_', 32, '\\', 32, 32, '/', 32, '_', '\'', 32, (char)178},
                                     {(char)178, 32, (char)178, 32, 92, 32, 92, 32, '(', 32, '(', '_', ')', 32, ')', '(', 32, '(', '_', ')', 32, (char)178, '(', 32, '(', '_', ')', 32, (char)178, 32, 32, 32, 32, 32, 32, '\\', 32, 32, (char)178, '_', '_', '_', (char)178, 32, (char)178, 32, 32, 32, '(', 32, '(', '_', ')', 32, ')', '\\', '_', '_', 32, '\\', '\\', '_', '_', 32, '\\', (char)178, 32, (char)178, '|', 32, (char)178, 32, '|', 32, '|', '(', 32, '(', '_', ')', 32, (char)178},
                                     {(char)178, '_', (char)178, 32, 32, 92, '_', 92, 32, '\\', '_', '_', '_', '/', 32, 32, '\\', '_', '_', ',', '_', (char)178, 32, '\\', '_', '_', ',', '_', (char)178, 32, 32, 32, 32, 32, 32, 32, '\\', '_', '_', '_', '_', (char)178, (char)178, '_', (char)178, 32, 32, 32, 32, '\\', '_', '_', '_', '/', 32, (char)178, '_', '_', '_', '/', (char)178, '_', '_', '_', '/', (char)178, '_', (char)178, '|', '_', (char)178, 32, '|', '_', '|', 32, '\\', '_', '_', ',', 32, (char)178},
                                     {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, (char)178, '_', '_', '_', '/'}};

    //(char)168->(char)124
    int row = (getRows() - height) / 2 - 10;
    int col = (getColumns() - width) / 2;
    int i, j;
    for (i = 0; i < height; ++i)
    {
        goTo(col, row++);
        for (j = 0; j < width; ++j)
        {
            Game().textColor(ColorCode_Yellow);
            cout << TITLE[i][j];
        }
    }
    Game().textColor(default_ColorCode);
}