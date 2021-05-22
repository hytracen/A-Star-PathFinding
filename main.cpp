#include <bits/stdc++.h>
using namespace std;
/*
用二维数组储存一个20X30的地图，如下图所示，其中（10,13）~（20,13），（15,7）~（20,7），（20，2）~（20,13）为障碍物，即不能通过。
分别以（11,11），（22,2）为起始点和终点，再以（15,15），（17，0）为起始点和终点，用A*算法寻找路径，输出路径（即数组下标序列）
*/

#define MAP_WIDTH 30 ///< 地图的宽度
#define MAP_HEIGHT 20 ///< 地图的高度

/// 表示地图中点的结构体
struct MapNode
{
    int PreX{};
    int PreY{};
    bool bPassed = false;
};

/// 表示优先队列中点的结构体
struct PriorityQueueNode
{
    int X;
    int Y;
    float HeuristicVal;///< 启发式函数计算得到的值
    PriorityQueueNode(int X, int Y, float HeuristicVal): X(X), Y(Y), HeuristicVal(HeuristicVal){};
};

/// 优先队列的自定义比较结构体
struct Cmp{

    /// \brief 启发式函数值较大的节点优先级较低
    bool operator ()(PriorityQueueNode a, PriorityQueueNode b){
        return a.HeuristicVal > b.HeuristicVal;
    }
};

int StartX = 11;///< 起点X坐标
int StartY = 11;///< 起点Y坐标

int TargetX = 22;///< 终点X坐标
int TargetY = 2;///< 终点Y坐标

float XY_COST = 1;///< 水平移动的代价值
float DIAGONAL_COST = 1.414;///< 斜向移动的代价值

/// \brief 根据启发式函数计算节点的对应值
/// \param X X坐标
/// \param Y Y坐标
/// \return 计算结果
float CalculateHeuristicVal(int X, int Y)
{
    int DX = abs(X - TargetX);
    int DY = abs(Y - TargetY);
    return XY_COST * (float)(DX + DY) + (DIAGONAL_COST - 2 * XY_COST) * (float)min(DX, DY);
}

/// \brief 递归打印路径
/// \param InputMapNode 地图节点
/// \param Map 地图
void PrintPath(MapNode InputMapNode, MapNode (*Map)[MAP_WIDTH])
{
    if(InputMapNode.PreX == StartX && InputMapNode.PreY == StartY)
    {
        cout<<'('<<InputMapNode.PreX<<','<<InputMapNode.PreY<<')'<<endl;
        return;
    }
    PrintPath(Map[InputMapNode.PreY][InputMapNode.PreX], Map);
    cout<<'('<<InputMapNode.PreX<<','<<InputMapNode.PreY<<')'<<endl;
}

/// \brief 处理地图上的某个节点（即计算启发式函数值，入队等）
/// \param X 节点X坐标
/// \param Y 节点Y坐标
/// \param PreX 该节点父节点的X坐标
/// \param PreY 该节点母节点的Y坐标
/// \param PriorityQueue 优先队列
/// \param Map 地图
void ProcessNode(int X, int Y, int PreX, int PreY,
                    priority_queue<PriorityQueueNode, vector<PriorityQueueNode>, Cmp>& PriorityQueue,
                        MapNode (*Map)[MAP_WIDTH])
{
    if(X<0 || X>=MAP_WIDTH || Y<0 || Y>=MAP_HEIGHT)
    {
        return;
    }
    MapNode& TheMapNode = Map[Y][X];
    if(!TheMapNode.bPassed)
    {
        TheMapNode.PreX = PreX;
        TheMapNode.PreY = PreY;
        PriorityQueue.push(PriorityQueueNode(X, Y, CalculateHeuristicVal(X, Y)));
    }
}

/// \brief 在地图中设置障碍
/// \param Map 地图
void SetObstacles(MapNode (*Map)[MAP_WIDTH])
{
    for(int x = 10; x <= 20; ++x)
    {
        Map[13][x].bPassed = true;
    }
    for(int x = 15; x <= 20; ++x)
    {
        Map[7][x].bPassed = true;
    }
    for(int y = 2; y <= 13; ++y)
    {
        Map[y][20].bPassed = true;
    }
}

/// \brief A*算法的主要逻辑部分
/// \param PriorityQueue 优先队列
/// \param Map 地图
void FindPath(priority_queue<PriorityQueueNode, vector<PriorityQueueNode>, Cmp>& PriorityQueue,
                MapNode (*Map)[MAP_WIDTH])
{
    while(!PriorityQueue.empty())
    {
        PriorityQueueNode PriorityNode = PriorityQueue.top();
        if(PriorityNode.X == TargetX && PriorityNode.Y == TargetY)///如果已经到达终点
        {
            PrintPath(Map[TargetY][TargetX], Map);
            cout<<"("<<TargetX<<','<<TargetY<<')'<<endl;
            break;
        }
        else
        {
            PriorityQueue.pop();/// 退队
            Map[PriorityNode.Y][PriorityNode.X].bPassed = true;

            int X = PriorityNode.X;
            int Y = PriorityNode.Y;

            /// 处理相邻节点
            ProcessNode(X+1, Y, X, Y, PriorityQueue, Map);
            ProcessNode(X-1, Y, X, Y, PriorityQueue, Map);
            ProcessNode(X, Y-1, X, Y, PriorityQueue, Map);
            ProcessNode(X, Y+1, X, Y, PriorityQueue, Map);
            ProcessNode(X+1, Y+1, X, Y, PriorityQueue, Map);
            ProcessNode(X+1, Y-1, X, Y, PriorityQueue, Map);
            ProcessNode(X-1, Y+1, X, Y, PriorityQueue, Map);
            ProcessNode(X-1, Y-1, X, Y, PriorityQueue, Map);
        }

    }
}

int main()
{
    MapNode Map[20][30];
    priority_queue<PriorityQueueNode, vector<PriorityQueueNode>, Cmp> PriorityQueue;

    PriorityQueue.push(PriorityQueueNode(StartX, StartY, 0));///起点入队

    SetObstacles(Map);///设置障碍

    cout<<"分别以(11,11)，(22,2)为起始点和终点的路径为:"<<endl;
    FindPath(PriorityQueue, Map);

    /*******************************************************************************/
    /// 初始化优先队列和地图
    while (!PriorityQueue.empty())
    {
        PriorityQueue.pop();
    }

    for(auto & i : Map)
    {
        for(auto & j : i)
        {
            j.bPassed = false;
        }
    }

    ///设置新的起点和终点
    StartX = 15;
    StartY = 15;
    TargetX = 17;
    TargetY = 0;

    PriorityQueue.push(PriorityQueueNode(StartX, StartY, 0));
    SetObstacles(Map);

    cout<<"\n分别以(15,15)，(17,0)为起始点和终点的路径为:"<<endl;
    FindPath(PriorityQueue, Map);

    system("pause");

    return 0;
}
