#include <bits/stdc++.h>
using namespace std;
/*
�ö�ά���鴢��һ��20X30�ĵ�ͼ������ͼ��ʾ�����У�10,13��~��20,13������15,7��~��20,7������20��2��~��20,13��Ϊ�ϰ��������ͨ����
�ֱ��ԣ�11,11������22,2��Ϊ��ʼ����յ㣬���ԣ�15,15������17��0��Ϊ��ʼ����յ㣬��A*�㷨Ѱ��·�������·�����������±����У�
*/

#define MAP_WIDTH 30 ///< ��ͼ�Ŀ��
#define MAP_HEIGHT 20 ///< ��ͼ�ĸ߶�

/// ��ʾ��ͼ�е�Ľṹ��
struct MapNode
{
    int PreX{};
    int PreY{};
    bool bPassed = false;
};

/// ��ʾ���ȶ����е�Ľṹ��
struct PriorityQueueNode
{
    int X;
    int Y;
    float HeuristicVal;///< ����ʽ��������õ���ֵ
    PriorityQueueNode(int X, int Y, float HeuristicVal): X(X), Y(Y), HeuristicVal(HeuristicVal){};
};

/// ���ȶ��е��Զ���ȽϽṹ��
struct Cmp{

    /// \brief ����ʽ����ֵ�ϴ�Ľڵ����ȼ��ϵ�
    bool operator ()(PriorityQueueNode a, PriorityQueueNode b){
        return a.HeuristicVal > b.HeuristicVal;
    }
};

int StartX = 11;///< ���X����
int StartY = 11;///< ���Y����

int TargetX = 22;///< �յ�X����
int TargetY = 2;///< �յ�Y����

float XY_COST = 1;///< ˮƽ�ƶ��Ĵ���ֵ
float DIAGONAL_COST = 1.414;///< б���ƶ��Ĵ���ֵ

/// \brief ��������ʽ��������ڵ�Ķ�Ӧֵ
/// \param X X����
/// \param Y Y����
/// \return ������
float CalculateHeuristicVal(int X, int Y)
{
    int DX = abs(X - TargetX);
    int DY = abs(Y - TargetY);
    return XY_COST * (float)(DX + DY) + (DIAGONAL_COST - 2 * XY_COST) * (float)min(DX, DY);
}

/// \brief �ݹ��ӡ·��
/// \param InputMapNode ��ͼ�ڵ�
/// \param Map ��ͼ
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

/// \brief �����ͼ�ϵ�ĳ���ڵ㣨����������ʽ����ֵ����ӵȣ�
/// \param X �ڵ�X����
/// \param Y �ڵ�Y����
/// \param PreX �ýڵ㸸�ڵ��X����
/// \param PreY �ýڵ�ĸ�ڵ��Y����
/// \param PriorityQueue ���ȶ���
/// \param Map ��ͼ
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

/// \brief �ڵ�ͼ�������ϰ�
/// \param Map ��ͼ
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

/// \brief A*�㷨����Ҫ�߼�����
/// \param PriorityQueue ���ȶ���
/// \param Map ��ͼ
void FindPath(priority_queue<PriorityQueueNode, vector<PriorityQueueNode>, Cmp>& PriorityQueue,
                MapNode (*Map)[MAP_WIDTH])
{
    while(!PriorityQueue.empty())
    {
        PriorityQueueNode PriorityNode = PriorityQueue.top();
        if(PriorityNode.X == TargetX && PriorityNode.Y == TargetY)///����Ѿ������յ�
        {
            PrintPath(Map[TargetY][TargetX], Map);
            cout<<"("<<TargetX<<','<<TargetY<<')'<<endl;
            break;
        }
        else
        {
            PriorityQueue.pop();/// �˶�
            Map[PriorityNode.Y][PriorityNode.X].bPassed = true;

            int X = PriorityNode.X;
            int Y = PriorityNode.Y;

            /// �������ڽڵ�
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

    PriorityQueue.push(PriorityQueueNode(StartX, StartY, 0));///������

    SetObstacles(Map);///�����ϰ�

    cout<<"�ֱ���(11,11)��(22,2)Ϊ��ʼ����յ��·��Ϊ:"<<endl;
    FindPath(PriorityQueue, Map);

    /*******************************************************************************/
    /// ��ʼ�����ȶ��к͵�ͼ
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

    ///�����µ������յ�
    StartX = 15;
    StartY = 15;
    TargetX = 17;
    TargetY = 0;

    PriorityQueue.push(PriorityQueueNode(StartX, StartY, 0));
    SetObstacles(Map);

    cout<<"\n�ֱ���(15,15)��(17,0)Ϊ��ʼ����յ��·��Ϊ:"<<endl;
    FindPath(PriorityQueue, Map);

    system("pause");

    return 0;
}
