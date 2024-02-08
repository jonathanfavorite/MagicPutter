#ifndef GAME_DATA_H
#define GAME_DATA_H

struct GameData {
    int currentHole;
    int currentStrokes;
    int maxHole;
    int minHole;
};

class GameDataManager {
public:
    GameDataManager();
    void setHole(int hole);
    int getHole() const;
    void setStrokes(int strokes);
    int getStrokes() const;
    GameData getGameData() const;
    int nextHole();
    int previousHole();
    int resetHole();
    void resetStrokes();
    int incrementStrokes();
    int decrementStrokes();

private:
    GameData gameData;
    bool canIncrementHole();
    bool canDecrementHole();
};


#endif // GAME_DATA_H