#ifndef GAMELIFE_H
#define GAMELIFE_H

#include <vector>
#include <QStringList>

class GameLife
{
public:
    GameLife(int lines = 10, int columns = 10);


//
// DATA METHODS
//----------------------------------------------------------
public:
    void                initialisation(int lines, int columns);
    void                random();

    void                load(QString file);
    void                save(QString file);

    QStringList         serialize();
    void                unserialize(QStringList data);

    void                clear();

    std::vector<int>    getGrid() const;
    void                setGrid(std::vector<int> grid);

    int                 getCell(int y, int x);
    void                setCell(int y, int x, int value);

    void                resizeGrid(int lines, int columns);

    int                 getLines();
    int                 getColumns();

private:
    int                 index(int line, int column);


//
// LOGICAL METHODS
//----------------------------------------------------------
public:
    void                play();
    void                reset();
    void                back();
    bool                hasCellAlived();

private:
    int                 nbAlivedAround(int y, int x);
    int                 resolveStatusOfCell(int y, int x);

//
// VARIABLES MEMBERS
//----------------------------------------------------------
private:
    int                           m_nbLines;
    int                           m_nbColumns;

    std::vector<int>              m_grid;
    std::vector<std::vector<int>> m_history;
    std::vector<std::vector<int>> m_neighbours;
};

#endif // GAMELIFE_H
