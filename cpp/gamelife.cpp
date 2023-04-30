#include "gamelife.h"
#include <QDebug>
#include <QFile>


//
// CONSTRUCTOR
//---------------------------------------------------------
GameLife::GameLife(int lines, int columns) :
    m_nbLines(lines),
    m_nbColumns(columns)
{
    m_grid = std::vector<int>(lines * columns, 0);

    m_neighbours = { {-1, -1}, {-1, 0}, {-1, 1},
                     { 0,  1}, { 1, 1}, { 1, 0},
                     { 1, -1}, { 0, -1} };
}


//*********************************************************
//                      DATA METHODS
//*********************************************************


//
// INITIALISATION
//---------------------------------------------------------
void GameLife::initialisation(int lines, int columns)
{
    m_nbLines = lines;
    m_nbColumns = columns;

    m_grid = std::vector<int>(lines * columns, 0);

    rand();
}


//
// RANDOM GENERATE
//---------------------------------------------------------
void GameLife::random()
{
    srand(clock());

    for (int y = 0; y < m_nbLines; y++)
    {
        for (int x = 0; x < m_nbColumns; x++)
        {
            m_grid[index(y, x)] = rand() % 2;
        }
    }
}


//
// LOAD
//---------------------------------------------------------
void GameLife::load(QString file)
{
    file.remove(0, 8);

    QFile ifs(file);
    QStringList data;

    if (ifs.open(QIODevice::ReadOnly))
    {
       QTextStream in(&ifs);

       while (!in.atEnd())
       {
           data.push_back(in.readLine());
       }

       unserialize(data);

       ifs.close();
    }
}


//
// SAVE
//---------------------------------------------------------
void GameLife::save(QString file)
{
    file.remove(0, 8);

    QFile ofs(file);

    if (ofs.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&ofs);

        for (auto line : serialize())
        {
            out << line + "\n";
        }

        ofs.close();
    }
}


//
// SERIALIZE (TO STRINGLIST)
//---------------------------------------------------------
QStringList GameLife::serialize()
{
    QStringList data;

    for (int y = 0; y < m_nbLines; y++)
    {
        QString line("");

        for (int x = 0; x < m_nbColumns; x++)
        {
            line += (m_grid[index(y, x)] == 0) ? "0" : "1";
        }

        data.push_back(line);
    }

    return data;
}


//
// UNSERIALIZE (TO GRID)
//---------------------------------------------------------
void GameLife::unserialize(QStringList data)
{
    clear();

    m_nbLines = data.length();
    m_nbColumns = data[0].length();

    QString flat = data.join("");

    for (int i = 0; i < flat.size(); i++)
    {
        m_grid.push_back(flat[i] == "0" ? 0 : 1);
    }
}


//
// CLEAR (CLEAR HISTORY & CLEAR GRID)
//---------------------------------------------------------
void GameLife::clear()
{
    m_grid.clear();
    m_history.clear();
}


//
// GET THE GRID
//---------------------------------------------------------
std::vector<int> GameLife::getGrid() const
{
    return m_grid;
}


//
// SET THE GRID
//---------------------------------------------------------
void GameLife::setGrid(std::vector<int> grid)
{
    m_grid = grid;
}


//
// GET CELL
//---------------------------------------------------------
int GameLife::getCell(int y, int x)
{
    return m_grid[index(y, x)];
}


//
// SET CELL
//---------------------------------------------------------
void GameLife::setCell(int y, int x, int value)
{
    m_grid[index(y, x)] = value;
}


//
// RESIZE GRID
//---------------------------------------------------------
void GameLife::resizeGrid(int lines, int columns)
{
    reset();

    m_nbLines = lines;
    m_nbColumns = columns;

    m_grid = std::vector<int>(lines * columns);
}


//
// GET LINES NUMBER
//---------------------------------------------------------
int GameLife::getLines()
{
    return m_nbLines;
}


//
// GET COLUMNS NUMBER
//---------------------------------------------------------
int GameLife::getColumns()
{
    return m_nbColumns;
}


//
// 2D INDEXATION FOR GRID
//---------------------------------------------------------
int GameLife::index(int line, int column)
{
    return column + (line * m_nbColumns);
}


//*********************************************************
//                    LOGICALS METHODS
//*********************************************************


//
// PLAY
//---------------------------------------------------------
void GameLife::play()
{
    std::vector<int> tmp = m_grid;

    m_history.push_back(m_grid);

    for (int y = 0; y < m_nbLines; y++)
    {
        for (int x = 0; x < m_nbColumns; x++)
        {
            if (m_grid[index(y, x)] == 1 || (m_grid[index(y, x)] == 0 && nbAlivedAround(y, x) > 1))
            {
                tmp[index(y, x)] = resolveStatusOfCell(y, x);
            }
        }
    }

    m_grid = tmp;
}


//
// RESET (HISTORY CLEAR, GRID EMPTY, SIZE REMAIN)
//---------------------------------------------------------
void GameLife::reset()
{
    m_grid = std::vector<int>(m_nbLines * m_nbColumns);
    m_history.clear();
}


//
// BACK TO THE PREVIOUS GRID
//---------------------------------------------------------
void GameLife::back()
{
    if (m_history.size() > 0)
    {
        m_grid = m_history.back();
        m_history.pop_back();
    }
}


//
// CHECK IF CELL IS ALIVED IN WHOLE GRID
//---------------------------------------------------------
bool GameLife::hasCellAlived()
{
    for (int i = 0; i < m_grid.size(); i++)
    {
        if (m_grid[i] == 1)
            return true;
    }

    return false;
}


//
// NUMBER ALIVED AROUND A CELL
//---------------------------------------------------------
int GameLife::nbAlivedAround(int y, int x)
{
    unsigned int nbAlived = 0;

    for (int i = 0; i < m_neighbours.size(); i++)
    {
        int posY = y + m_neighbours[i][0];
        int posX = x + m_neighbours[i][1];

        if (posY <= m_nbLines || posX <= m_nbColumns)
        {
            if (m_grid[index(posY, posX)] == 1)
                nbAlived++;
        }
    }

    return nbAlived;
}


//
// RESOLVE THE STATUS OF A CELL (ALIVE, STASE OR DEAD)
//---------------------------------------------------------
int GameLife::resolveStatusOfCell(int y, int x)
{
    if (nbAlivedAround(y, x) == 3)
    {
        return 1;
    }

    if ((nbAlivedAround(y, x) < 2) || (nbAlivedAround(y, x) > 3))
    {
        return 0;
    }

    return m_grid[index(y, x)];
}
