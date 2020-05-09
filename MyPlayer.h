#pragma once
#include <vector>
#include <queue>
#include<deque>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <ctime>
#define random(a,b) (rand() % (b - a + 1))+ a
#include "Player.h"

namespace QuoridorUtils {
    struct Node {
        Location NodeLoc;
        Node* preNode;
    };
    struct BlockBarState {
        BlockBar ImageBlockBar;
        int m_distance;
        int e_distance;
        int DifferValue;
    };
class MyPlayer final : public Player{
private:
    int matrix[11][11];
    int MyMinPath;
    int EnemyMinPath;
    int AssessOfSetBlock;
    int AssessOfMoving;
    //int assess;
    bool havetoMove;
    std::deque<BlockBar> blocks;// ʵ������Ҫ, ��ɾ
    std::vector<Node*>myPath;//��¼���������ĵ��ͼ��
    std::vector<Location>enemyPath;
    std::vector<BlockBarState>imagePath;
    std::queue<Node*>qLoc;//���У�����bfs
    int enemyUsedBlockBlar = 0;
    int LeftBlockBar = 10;
    int targetY = 0;// ʵ������Ҫ, ��ɾ
    int contraryY = 0;
    //int steps = 0;
    Location randomWalk(const Location& myLoc, const Location& enemyLoc);   // ʵ������Ҫ, ��ɾ 
    int bfs(const Location& myLoc, const Location& enemyLoc,int target);
    void clear(std::queue<Node*>& q);
    void visit(int x, int y, Node* p);
    void record(int x, int y,Node* p);
    int ContraryTarget(const int target);
    Location ShortPath(const Location& myLoc,const Location& enemyLoc);//����������һ���ƶ�������
    bool isBlockBar(const Location& myLoc,const int right,const int up);
    void DetectSpan(const Location& myLoc,const int i,Node* p,const int target);
    BlockBar setBlockBar(const Location& myLoc, const Location& enemyLoc, const int target);
    void Destory();
    bool doublication(const BlockBar& newBlockBar);
    void UpDateBlocks(const BlockBar& newBlockBar);
    void PopNewBlockbar(const BlockBar& newBlockBar);
public:
    MyPlayer(const std::string& key) : Player(key) {};            // �������, �����޸� 
    Step nextStep(const ChessboardChange& newChange) override;    // ��������ʵ�� 
    void restart() override;                                      // ��������ʵ�� 
};
}

// �������
//namespace QuoridorUtils {
//class MyPlayer final : public Player {
//public:
//    MyPlayer(const std::string& key) : Player(key) { };          // ������� 
//    Step nextStep(const ChessboardChange& newChange) override;   // ��������ʵ�� 
//    void restart() override;                                     // ��������ʵ�� 
//};
//}

/*
 *
 * ������������:                 ��������ṹ���£�
 * ��-��-��-��-��-��-��-��-��-��         ��-��-��-��-��-��-��-��-��-��9
 * ��-��-��-��-��-��-��-��-��-�ȨI9       ��-��-��-��-��-��-��-��-��-��8
 * ��-��-��-��-��-��-��-��-��-�ȨI8       ��-��-��-��-��-��-��-��-��-��7
 * ��-��-��-��-��-��-��-��-��-�ȨI7       ��-��-��-��-��-��-��-��-��-��6
 * ��-��-��-��-��-��-��-��-��-�ȨI6       ��-��-��-��-��-��-��-��-��-��5
 * ��-��-��-��-��-��-��-��-��-�ȨI5       ��-��-��-��-��-��-��-��-��-��4
 * ��-��-��-��-��-��-��-��-��-�ȨI4       ��-��-��-��-��-��-��-��-��-��3
 * ��-��-��-��-��-��-��-��-��-�ȨI3       ��-��-��-��-��-��-��-��-��-��2
 * ��-��-��-��-��-��-��-��-��-�ȨI2       ��-��-��-��-��-��-��-��-��-��1
 * ��-��-��-��-��-��-��-��-��-���I1 y     ��-��-��-��-��-��-��-��-��-��0 y
 *x 1 2 3 4 5 6 7 8 9         x0 1 2 3 4 5 6 7 8 9
 *
 *
 */