#include "Dijakrsta.h"

// private methods

list<vertexT> Dijakrsta::GetShortestPath(vertexT vertex, const vector<vertexT>& previous)
{
    list <vertexT> path;
    for (; vertex != -1; vertex = previous[vertex])
        path.push_front(vertex);
    return path;
}

void Dijakrsta::ComputePaths (vertexT source, const chanceOfLifeT& adjacencyList, vector<vertexT>& previous, vector <weightT>& minDistance)
{
    minDistance.clear();
    minDistance.resize(adjacencyList.size(), maxWeight);
    minDistance[source] = 0;
    previous.clear();
    previous.resize(adjacencyList.size(), -1);

    set <pair <weightT, vertexT>> vertexQueue;
    vertexQueue.insert(make_pair(minDistance[source], source));

    while (!vertexQueue.empty())
    {
        weightT dist = vertexQueue.begin()->first;
        vertexT u = vertexQueue.begin()->second;
        vertexQueue.erase(vertexQueue.begin());

        const vector <Neighbor>& Neighbors = adjacencyList[u];
        for (const Neighbor& Neighbor_iter : Neighbors)
        {
            vertexT v = Neighbor_iter.target;
            weightT weight = Neighbor_iter.weight;

            // skip walls i.e. nodes with weight = maxWeight
            if (weight == maxWeight)
                continue;

            weightT distance_through_u = dist + weight;
            if (distance_through_u < minDistance[v])
            {
                vertexQueue.erase(std::make_pair(minDistance[v], v));

                minDistance[v] = distance_through_u;
                previous[v] = u;
                vertexQueue.insert(make_pair(minDistance[v], v));
            }
        }
    }
}

int Dijakrsta::getRandom(double chanceOfLife)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distance(0.0, 1.0);

    double randomValue = distance(gen);

    if (randomValue < chanceOfLife) {
        return 1;
    }
    else {
        return 0;
    }
}

// public methods
Dijakrsta::Dijakrsta(bool walls, int rowA, int columnA, int rowB, int columnB)
{
    this->walls = walls;
    this->rowA = rowA;
    this->columnA = columnA;
    this->rowB = rowB;
    this->columnB = columnB;

    // initialize map with walls (1)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if ((i == rowA - 1 && j == columnA - 1) || (i == rowB - 1 && j == columnB - 1)) {
                map[i][j] = 0; // empty the cell for the positions of A and B
            }
            else if (walls)
            {
                map[i][j] = getRandom(0.20); // 20% chance that the cell will be a wall
            }
            else
            {
                map[i][j] = 0;
            }
        }
    }

    // initialize adjacency list and include an additional vertex for the walls
    chanceOfLifeT adjacencyList (rows * columns + 1);

    // need to add edges between cells
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            // Skip walls
            if (map[i][j] == 1) {
                continue;
            }

            // connect to the above cell
            if (i > 0 && map[i - 1][j] != 1) {
                adjacencyList[i * columns + j].push_back(Neighbor((i - 1) * columns + j, 1));  // connect to the above cell
            }

            // connect to the left cell
            if (j > 0 && map[i][j - 1] != 1) {
                adjacencyList[i * columns + j].push_back(Neighbor(i * columns + j - 1, 1));  // connect to the left cell
            }

            // connect to the right cell
            if (j < columns - 1 && map[i][j + 1] != 1) {
                adjacencyList[i * columns + j].push_back(Neighbor(i * columns + j + 1, 1));  // connect to the right cell
            }

            // connect to the below cell
            if (i < rows - 1 && map[i + 1][j] != 1) {
                adjacencyList[i * columns + j].push_back(Neighbor((i + 1) * columns + j, 1));  // connect to the below cell
            }
        }
    }

    // connect walls to other vertices with infinite weight
    if (walls)
    {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                if (map[i][j] == 1) {
                    adjacencyList[rows * columns].push_back(Neighbor(i * columns + j, maxWeight));  // Connect walls to other vertices with infinite weight
                }
            }
        }
    }

    vector<weightT> minDistance;
    vector<vertexT> previous;
    ComputePaths((rowA - 1) * columns + columnA - 1, adjacencyList, previous, minDistance);

    list <vertexT> path = GetShortestPath((rowB - 1) * columns + columnB - 1, previous);
    list<vertexT> adjustedPath = path;

    // draw map with initial positions of A and B
    drawMap(rowA, columnA, rowB, columnB, adjustedPath, map);
    Sleep(100);

    // draw map with help of path
    for (const vertexT& vertex : adjustedPath)
    {
        int row = vertex / columns;
        int column = vertex % columns;
        map[row][column] = 2; // path is visited
        drawMap(rowA, columnA, rowB, columnB, adjustedPath, map);
        Sleep(100);
    }
}

void Dijakrsta::drawMap(int rowA, int columnA, int rowB, int columnB, const list<vertexT>& adjustedPath, int map[20][40])
{
    system("cls"); // clear screen

    // draw map with help of path
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            if (i == rowA - 1 && j == columnA - 1)
                cout << "A ";                           // starting point
            else if (i == rowB - 1 && j == columnB - 1)
                cout << "B ";                           // ending points
            else if (map[i][j] == 2)
                cout << "o ";                           // draw path
            else if (map[i][j] == 1)
                cout << "| ";                           // draw wall
            else
                cout << ". ";                           // draw everything else
        }
        cout << endl;
    }
}
