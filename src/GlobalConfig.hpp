#ifndef GlobalConfig_hpp
#define GlobalConfig_hpp

#include "Header.h"
#include <queue>

using namespace std;

class GlobalConfig{
    private:
    GlobalConfig();
    static GlobalConfig* instance;
    public:

    int currentScore = 0;
    bool first = true;
    bool updated;
    int d1 = 0, d2 = 0, d3 = 0, d4 = 0;
    int currentPeopleX, currentPeopleY;
    queue<Pixel*> drawingQueue;
    bool** matrix;
    char lastSignal;
    vector< vector<char> > drawing_matrix;
    char light = '+'; //green

    void resetMatrix();
    void initNewData(int setScore);
    static GlobalConfig* getInstance();
    
    ~GlobalConfig();
};



#endif