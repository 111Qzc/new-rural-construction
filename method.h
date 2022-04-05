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

//题目一 Qzc
class Prim          //最小生成树，双亲表示法
{
public:
    Prim(AdjMultListNetwork& cc);
    ~Prim();

    void Project(AdjMultListNetwork& cc);               //

    void operPrim(AdjMultListNetwork& cc, int index);   //计算index的最小生成树
    int GetLowIndex();                                  //返回最小权值的边
    void GetLowCost(AdjMultListNetwork& cc);            //更新数组，调整V U集合
    void CoutPrim(int index,int tab, AdjMultListNetwork& cc,int flag);//输出树
    void PrintTree(AdjMultListNetwork& cc, int index);          //输出信息
private:
    string* country;        //村庄名字，data
    int* nearvex;           //辅助数组，parent
    double* lowcost;        //权重
    
    int vexNum;
    double AllCost;
    int AllDistance;


};
Prim::Prim(AdjMultListNetwork& cc)
{
    vexNum = cc.GetVexNum();
    AllCost = 0;
    AllDistance = 0;
    country = new string[vexNum];
    nearvex = new int[vexNum];
    lowcost = new double[vexNum];

}
void Prim::Project(AdjMultListNetwork& cc)
{
    int* distance = new int[vexNum];       //存储各方案的总路径
    int* Index = new int[vexNum];          //存储可行方案的序号
    int num = 0,index;                     //方案数目，序号
    for (int i = 0; i < vexNum; i++)
    {
        operPrim(cc, i);
        CoutPrim(i, 0, cc, 0);
        for (index = 0; index < num; index++)
        {
            if (distance[index] == AllDistance)
                break;
        }
        if (index >= num && index < vexNum)
        {
            distance[index] = AllDistance;
            num++;
            Index[index] = i;
        }
    }
    cout << "一共有" << num << "种方案,总耗资为" << AllCost << endl;
    cout << "------------------------" << endl;

    for (int i = 0; i < num; i++)
    {
        operPrim(cc, Index[i]);
        PrintTree(cc, Index[i]);
    }
}
void Prim::operPrim(AdjMultListNetwork& cc,int index)
{
    AllCost = 0;
    AllDistance = 0;
    for (int i = 0; i < vexNum; i++)
    {
        country[i] = cc.GetName(i);
        if (i != index)
        {
            lowcost[i] = cc.GetCost(i, index);
            nearvex[i] = index;
        }
        else
        {
            lowcost[i] = 0;
            nearvex[i] = -1;
        }
    }
    int tag = vexNum - 1;
    while (tag--)
    {
        lowcost[GetLowIndex()] = 0;
        GetLowCost(cc);
    }
    
}
void Prim::PrintTree(AdjMultListNetwork& cc,int index)
{
    CoutPrim(index, 0, cc,1);
    cout << "公路总长：" << AllDistance << endl;
    cout << "总耗资：" << AllCost << endl;
    cout << "------------------------" << endl;
}

void Prim::GetLowCost(AdjMultListNetwork& cc)
{
    for (int i = 0; i < vexNum; i++)
    {
        if(lowcost[i] == 0)
            for (int j = 0; j < vexNum; j++)
            {
                if (i != j)
                {
                    double temCost = cc.GetCost(i, j);
                    if (lowcost[j] && lowcost[j] > temCost)
                    {
                        lowcost[j] = temCost;
                        nearvex[j] = i;
                    }
                }
            }
    }
}
int Prim::GetLowIndex()
{
    int minIndex = 0;
    while (!lowcost[minIndex])
    {
        minIndex++;
    }
    for (int i = 0; i < vexNum; i++)
    {
        if (lowcost[i] && lowcost[minIndex] > lowcost[i])
            minIndex = i;
    }
    return minIndex;

}
void Prim::CoutPrim(int index, int tab, AdjMultListNetwork& cc,int flag)
{
    if (flag)
    {
        for (int i = 0; i < tab; i++)
            cout << '\t';
        if (nearvex[index] != -1)
            cout << "-(" << cc.GetCost(index, nearvex[index]) << ")->村" << country[index] << endl;
        else
            cout << "-->村" + country[index] << endl;
    }
    for (int i = 0; i < vexNum; i++)
    {
        if (nearvex[i] == index)
        {
            CoutPrim(i, tab + 1, cc, flag);
            AllCost += cc.GetCost(index, i);
            AllDistance += cc.GetDistance(index, i);
        }
    }

}
