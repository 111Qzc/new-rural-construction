#include "AdjMultListNetwork.h"
#include <stack>

class Floyd
{
public:
    int **dist, **path;
    int vexNum;
    explicit Floyd(AdjMultListNetwork &network);
    void updateDistanceMatrix();
    int shortestDistance(int v1, int v2) const;
    stack<int> shortestPath(int v1, int v2) const;
};

Floyd::Floyd(AdjMultListNetwork &network)
{

    vexNum = network.GetVexNum();

    dist = new int *[vexNum];
    path = new int *[vexNum];

    for (int i = 0; i < vexNum; i++)
    {
        dist[i] = new int[vexNum];
        path[i] = new int[vexNum];
        for (int j = 0; j < vexNum; j++)
        {
            dist[i][j] = i == j ? 0 : network.GetDistance(i, j);
            path[i][j] = i == j ? -1 : i;
        }
    }
}

void Floyd::updateDistanceMatrix()
{
    for (int i = 0; i < vexNum; i++)
    {
        for (int j = 0; j < vexNum; j++)
        {
            for (int k = 0; k < vexNum; k++)
            {
                if (dist[j][i] == DIS_INFINITY || dist[i][k] == DIS_INFINITY)
                    continue;
                if (dist[j][i] + dist[i][k] < dist[j][k])
                {
                    dist[j][k] = dist[j][i] + dist[i][k];
                    path[j][k] = path[i][k];
                }
            }
        }
    }
}
int Floyd::shortestDistance(int v1, int v2) const
{
    return dist[v1][v2];
}
stack<int> Floyd::shortestPath(int v1, int v2) const
{
    stack<int> path_;
    int precursor = v2;
    while (precursor != v1)
    {
        path_.push(precursor);
        precursor = path[v1][precursor];
    }
    return path_;
}
