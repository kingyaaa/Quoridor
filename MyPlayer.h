#pragma once
#include <vector>
#include <queue>
#include "Player.h"

namespace QuoridorUtils {
    struct Node {
        Location NodeLoc;
        Node* preNode;
    };
class MyPlayer final : public Player{
private:
    int matrix[11][11];
    int MyMinPath;
    int EnemyMinPath;
    std::vector<BlockBar> blocks;// 实例所需要, 可删
    std::vector<Node*>myPath;//记录了所经过的点的图集
    std::queue<Node*>qLoc;//队列，用于bfs
    int targetY = 0;// 实例所需要, 可删 
    int contraryY = 0;
    Location randomWalk(const Location& myLoc, const Location& enemyLoc);   // 实例所需要, 可删 
    int bfs(const Location& myLoc, const Location& enemyLoc,int target);
    void clear(std::queue<Node*>& q);
    void visit(int x, int y, Node* p);
    void record(int x, int y,Node* p);
    Location ShortPath(const Location& myLoc,const Location& enemyLoc);//返回棋子下一步移动的坐标
    bool isBlockBar(const Location& myLoc,const int right,const int up);
    void DetectSpan(const Location& myLoc,const int i,Node* p);
    void Destory();
public:
    MyPlayer(const std::string& key) : Player(key) {};            // 必须存在, 请勿修改 
    Step nextStep(const ChessboardChange& newChange) override;    // 必须自行实现 
    void restart() override;                                      // 必须自行实现 
};
}

// 面向过程
//namespace QuoridorUtils {
//class MyPlayer final : public Player {
//public:
//    MyPlayer(const std::string& key) : Player(key) { };          // 必须存在 
//    Step nextStep(const ChessboardChange& newChange) override;   // 必须自行实现 
//    void restart() override;                                     // 必须自行实现 
//};
//}

/*
 *
 * 棋子坐标如下:                 挡板坐标结构如下：
 * ┌-┬-┬-┬-┬-┬-┬-┬-┬-┐         ┌-┬-┬-┬-┬-┬-┬-┬-┬-┐9
 * ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖9       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤8
 * ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖8       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤7
 * ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖7       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤6
 * ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖6       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤5
 * ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖5       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤4
 * ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖4       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤3
 * ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖3       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤2
 * ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖2       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤1
 * └-┴-┴-┴-┴-┴-┴-┴-┴-┘↖1 y     └-┴-┴-┴-┴-┴-┴-┴-┴-┘0 y
 *x 1 2 3 4 5 6 7 8 9         x0 1 2 3 4 5 6 7 8 9
 *
 *
 */