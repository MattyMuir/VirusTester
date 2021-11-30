#include <iostream>
#include <string>
#include <vector>
#include <queue>

#include "SimpleSet.h"

#define USER_INPUT 0
#define PRINT_BOARD 0

enum class Direction {N = 'N', S = 'S', W = 'W', E = 'E' };

struct Square
{
    Square() {}
    Square(int x_, int y_, Direction dir_)
        : x(x_), y(y_), dir(dir_) {}

    int x = 0, y = 0;
    Direction dir = Direction::N;
    bool checked = false;
};

int InputInt(std::string message)
{
    std::cout << message;
    std::string inputStr;
    std::getline(std::cin, inputStr);

    return std::stoi(inputStr);
}

std::string InputStr(std::string message)
{
    std::cout << message;
    std::string inputStr;
    std::getline(std::cin, inputStr);

    return inputStr;
}

int main()
{
    std::srand(time(0));
    std::vector<std::vector<Square>> squares;
    
    int w = InputInt("Enter width: ");
    int h = InputInt("Enter height: ");

#if USER_INPUT == 1
    for (int y = 0; y < h; y++)
    {
        std::string row = InputStr("");
        squares.push_back(std::vector<Square>());

        for (int x = 0; x < w; x++)
        {
            squares[y].push_back(Square(x, y, static_cast<Direction>(row[x])));
        }
    }
#else
    for (int y = 0; y < h; y++)
    {
        squares.push_back(std::vector<Square>());
        for (int x = 0; x < w; x++)
        {
            Direction d;
            switch (rand() % 4)
            {
            case 0:
                d = Direction::N;
                break;
            case 1:
                d = Direction::S;
                break;
            case 2:
                d = Direction::E;
                break;
            case 3:
                d = Direction::W;
                break;
            }
            squares[y].push_back(Square(x, y, d));
        }
    }
#endif

#if PRINT_BOARD == 1
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            std::cout << static_cast<char>(squares[y][x].dir) << ",";
        }
        std::cout << std::endl;
    }
#endif

    // Declare hashset with custom hash function
    auto hash = [](const Square& obj) { return 104729 * obj.x + 212369 * obj.y; };
    auto pred = [](const Square& obj1, const Square& obj2) { return (obj1.x == obj2.x) && (obj1.y == obj2.y); };
    DeclareSet(checked, Square, 10);

    std::queue<Square> pathQueue;

    int nChecked = 0;
    int nTesters = 0;
    while (true)
    {
        // Find a starting square which is unchecked
        bool foundStart = false;
        for (int y = 0; (y < h) && !foundStart; y++)
        {
            for (int x = 0; (x < w) && !foundStart; x++)
            {
                if (!squares[y][x].checked)
                {
                    foundStart = true;
                    pathQueue.push(squares[y][x]);
                }
            }
        }

        // If none found (all checked) program is complete
        if (!foundStart)
        {
            break;
        }
        nTesters++;

        // Find all squares connected to starting square
        while (pathQueue.size() > 0)
        {
            Square& front = pathQueue.front();
            int x = front.x;
            int y = front.y;
            if (!checked.contains(front))
            {
                squares[y][x].checked = true;
                nChecked++;

                // Push neighbours (very ugly code but Im lazy)
                if (x + 1 < w)
                {
                    if (squares[y][x + 1].dir == Direction::W || front.dir == Direction::E)
                    {
                        pathQueue.push(squares[y][x + 1]);
                    }
                }
                if (x - 1 > 0)
                {
                    if (squares[y][x - 1].dir == Direction::E || front.dir == Direction::W)
                    {
                        pathQueue.push(squares[y][x - 1]);
                    }
                }
                if (y + 1 < h)
                {
                    if (squares[y + 1][x].dir == Direction::N || front.dir == Direction::S)
                    {
                        pathQueue.push(squares[y + 1][x]);
                    }
                }
                if (y - 1 > 0)
                {
                    if (squares[y - 1][x].dir == Direction::S || front.dir == Direction::N)
                    {
                        pathQueue.push(squares[y - 1][x]);
                    }
                }

                checked.insert(front);
            }
            pathQueue.pop();
        }
        checked.clear();
    }
    std::cout << nTesters << std::endl;
    std::cin.get();
}