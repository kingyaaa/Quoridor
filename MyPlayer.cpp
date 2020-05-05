#include "MyPlayer.h"
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <ctime>
// 面向过程实现开始
//using namespace QuoridorUtils;
//int targetY = 0;
//std::vector<QuoridorUtils::BlockBar> blocks;
//QuoridorUtils::Location randomWalk(const QuoridorUtils::Location& myLoc, const QuoridorUtils::Location& enemyLoc) {
//    bool directions[4];
//    directions[0] = myLoc.y < QuoridorUtils::SIZE;           // 是否可以向上走 
//    directions[1] = myLoc.y > 0;                             // 是否可以向下走 
//    directions[2] = myLoc.x > 0;                             // 是否可以向左走 
//    directions[3] = myLoc.x < QuoridorUtils::SIZE;           // 是否可以向右走 
//    for (auto block : blocks) {                        // 遍历挡板列表, 找到被阻挡的方向 
//        if (block.isH()) {                                   // 水平方向挡板, start 与 stop 的 y 相等 
//            if (block.start.x == myLoc.x - 1 ||
//                block.start.x == myLoc.x - 2) {              // 可能挡路的挡板 
//                if (block.start.y == myLoc.y) {
//                    directions[0] = false;
//                } else if (block.start.y == myLoc.y - 1) {
//                    directions[1] = false;
//                }
//            }
//        }
//        if (block.isV()) {                                   // 竖直方向挡板, start 与 stop 的 x 相等 
//            if (block.start.y == myLoc.y - 1 ||
//                block.start.y == myLoc.y - 2) {              // 可能挡路的挡板 
//                if (block.start.x == myLoc.x) {
//                    directions[2] = false;
//                } else if (block.start.x == myLoc.x - 1) {
//                    directions[3] = false;
//                }
//            }
//        }
//    }
//    int optionsCount = directions[0] + directions[1] + 
//                       directions[2] + directions[3];        // 规则不能堵死棋子, 必定大于等于1 
//    int tmp = rand() % optionsCount;                         // 随机一个 [0, optionsCount - 1] 的整数
//    int directionIndex;                                      // 找出随机方向的下标 
//    for (directionIndex = 0; directionIndex < 4; directionIndex++) {
//        if (directions[directionIndex]) {
//            if (tmp <= 0) {
//                break;
//            }
//            tmp--;
//        }
//    }
//    QuoridorUtils::Location nextLoc = myLoc;
//    switch (directionIndex) {
//    case 0:   // 向上走
//        nextLoc.y++;
//        break;
//    case 1:   // 向下走
//        nextLoc.y--;
//        break;
//    case 2:   // 向左走
//        nextLoc.x--;
//        break;
//    case 3:   // 向右走
//        nextLoc.x++;
//        break;
//    default: ;
//    }
//    if (nextLoc.distance(enemyLoc) == 0) {                   // 下一步是敌军位置则应跳过敌军棋子 
//        nextLoc.x = -1;                                      // 此处不做实现, 默认下一步空操作 
//        nextLoc.y = -1;
//    }
//    return nextLoc;
//}
// 面向过程实现结束

namespace QuoridorUtils {

/*
 * Step MyPlayer::nextStep(const ChessboardChange& newChange)
 *
 * newChange 为修改的棋局, 其格式为
 * (GameStatus status, Location enemyLoc, Location myLoc, BlockBar newEnemyBlockBar)
 * 其中棋子坐标和挡板坐标见下图, 当对手没有添加新的挡板时,
 * newChange.newEnemyBlockBar.isNan()为true
 * 
 * 需要返回的数据Step为
 * (Location myNewLoc, BlockBar myNewBlockBar)
 * 仅能选择让移动棋子一步（前后左右四个方向）或者放一个挡板:
 * 当移动棋子时, step.myNewBlockBar.isNan()为true; 
 * 当放置挡板时, step.myNewLoc为newChange中的myLoc
 *
 * 所有错误(挡板位置错误, 挡板已用完, 移动位置错误, 同时移动并放置挡板, 超时等)均由服务器检测,
 * 可能需要在自行实现错误避免功能. 
 *
 *  棋子坐标如下:                 挡板坐标结构如下：
 *  ┌-┬-┬-┬-┬-┬-┬-┬-┬-┐          ┌-┬-┬-┬-┬-┬-┬-┬-┬-┐9
 *  ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖9       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤8
 *  ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖8       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤7
 *  ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖7       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤6
 *  ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖6       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤5
 *  ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖5       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤4
 *  ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖4       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤3
 *  ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖3       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤2
 *  ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖2       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤1
 *  └-┴-┴-┴-┴-┴-┴-┴-┴-┘↖1 y     └-┴-┴-┴-┴-┴-┴-┴-┴-┘0 y
 * x  1  2  3  4  5  6  7  8  9          x  0  1  2  3  4  5  6  7  8  9
      0  1  2  3  4  5  6  7  8
 *
 */

// 面向对象实现开始
Location MyPlayer::randomWalk(const Location& myLoc, const Location& enemyLoc) {
    bool directions[4];
    directions[0] = myLoc.y < SIZE;                          // 是否可以向上走 
    directions[1] = myLoc.y > 1;                             // 是否可以向下走 
    directions[2] = myLoc.x > 1;                             // 是否可以向左走 
    directions[3] = myLoc.x < SIZE;                          // 是否可以向右走 
    for (auto block : this->blocks) {                        // 遍历挡板列表, 找到被阻挡的方向 
        if (block.isH()) {                                   // 水平方向挡板, start 与 stop 的 y 相等 
            if (block.start.x == myLoc.x - 1 || block.start.x == myLoc.x - 2) {              // 可能挡路的挡板 
                if (block.start.y == myLoc.y) {
                    directions[0] = false;
                } else if (block.start.y == myLoc.y - 1) {
                    directions[1] = false;
                }
            }
        }
        if (block.isV()) {                                   // 竖直方向挡板, start 与 stop 的 x 相等 
            if (block.start.y == myLoc.y - 1 || block.start.y == myLoc.y - 2) {              // 可能挡路的挡板 
                if (block.start.x == myLoc.x) {
                    directions[3] = false;
                } else if (block.start.x == myLoc.x - 1) {
                    directions[2] = false;
                }
            }
        }
    }
    int optionsCount = int(directions[0]) + int(directions[1]) + 
                       int(directions[2]) + int(directions[3]);    // 规则中不能堵死棋子, 必定大于等于1 
    int tmp = rand() % optionsCount;                               // 随机一个 [0, optionsCount - 1] 的整数
    int directionIndex;                                            // 找出随机方向的下标 
    for (directionIndex = 0; directionIndex < 4; directionIndex++) {
        if (directions[directionIndex]) {
            if (tmp <= 0) {
                break;
            }
            tmp--;
        }
    }
    Location nextLoc = myLoc;
    switch (directionIndex) {
    case 0:   // 向上走
        nextLoc.y++;
        break;
    case 1:   // 向下走
        nextLoc.y--;
        break;
    case 2:   // 向左走
        nextLoc.x--;
        break;
    case 3:   // 向右走
        nextLoc.x++;
        break;
    default: ;
    }
    if (nextLoc.distance(enemyLoc) == 0) {                   // 下一步是敌军位置则应跳过敌军棋子 
        nextLoc.x = -1;                                      // 此处不做实现, 默认下一步空操作 
        nextLoc.y = -1;
    }
    return nextLoc;
}
/******************************************************************
Step MyPlayer::nextStep(const ChessboardChange& newChange) {
    // 须知: 本示例代码仅作为 API 使用以及游戏规则了解范例, 甚至保证不了正确性, 切勿照抄照搬 
    if (this->targetY == 0) {                                // 判断并记录自己的目标 
        const auto d = std::chrono::system_clock::now().time_since_epoch();
        const int nanosecond = (std::chrono::nanoseconds(d).count() / 100) % int64_t(INT32_MAX);
        srand(nanosecond);
        if (newChange.myLoc == PLAYER0_LOC) {                // 己方初始坐标为 (5,1) 则目标为 (x,9) 
            this->targetY = PLAYER1_LOC.y;
        } else {                                             // 己方初始坐标为 (5,9) 则目标为 (x,1) 
            this->targetY = PLAYER0_LOC.y;
        }
    }
    std::cout << newChange;                                  // 输出接收到的数据到控制台显示 
    if (!newChange.newEnemyBlockBar.isNan()) {               // 对方放置了挡板 
        BlockBar tmp = newChange.newEnemyBlockBar;
        tmp.normalization();                                 // 规范为 start 坐标小于 stop 坐标的形式 
        this->blocks.push_back(tmp);                         // 存储规范挡板 
    }
    Step step;                                               // 初始化 step 默认不移动不放板 
    step.myNewLoc = randomWalk(newChange.myLoc, newChange.enemyLoc);
    std::cout << " -> " << step << std::endl;                // 输出我的决策到控制台显示 
    return step;
}

************************************************************************/
void MyPlayer::restart() {
    this->blocks.clear();
    this->targetY = 0;
    this->contraryY = 0;
    this->LeftBlockBar = 10;
    this->steps = 0;
}




//我的实现
Step MyPlayer::nextStep(const ChessboardChange& newChange) {
    //初始化准备
    this->blocks.reserve(25);
    enemyPath.clear();
    MyMinPath = 0;
    EnemyMinPath = 0;
    
    Step step;
    steps++;
    //判断方向:我方的目标线，对方的目标线
    if (this->targetY == 0) {                                // 判断并记录自己的目标 
        const auto d = std::chrono::system_clock::now().time_since_epoch();
        const int nanosecond = (std::chrono::nanoseconds(d).count() / 100) % int64_t(INT32_MAX);
        srand(nanosecond);
        if (newChange.myLoc == PLAYER0_LOC) {                // 己方初始坐标为 (5,1) 则目标为 (x,9) 
            this->targetY = PLAYER1_LOC.y;
            this->contraryY = PLAYER0_LOC.y;
        }
        else {                                             // 己方初始坐标为 (5,9) 则目标为 (x,1) 
            this->targetY = PLAYER0_LOC.y;
            this->contraryY = PLAYER1_LOC.y;                //记录对手的目标，与我方相反。
        }
    }
    //接收目前的局势信息
    std::cout << newChange;                     
    // 输出接收到的数据到控制台显示 
    if (!newChange.newEnemyBlockBar.isNan()) {               // 对方放置了挡板 
        BlockBar tmp = newChange.newEnemyBlockBar;
        tmp.normalization();                                 // 规范为 start 坐标小于 stop 坐标的形式 
        this->blocks.push_back(tmp);                         // 存储规范挡板 
    }
    //判断最短路径
    std::cout << std::endl << "对手的步数:";
    EnemyMinPath = bfs(newChange.enemyLoc, newChange.myLoc, this->contraryY);           //计算对手的最短路径
    std::cout << EnemyMinPath << std::endl;
    ShortPath(newChange.enemyLoc, newChange.myLoc);                                     //打印出对手的最短路径 
    //Step step;                                               // 初始化 step 默认不移动不放板 
    if(LeftBlockBar == 0)
    {
        std::cout << "我的目标线：y = " << targetY << std::endl;
        std::cout << std::endl << "我的步数:";
        MyMinPath = bfs(newChange.myLoc, newChange.enemyLoc, this->targetY);
        std::cout << MyMinPath << std::endl;
        step.myNewLoc = ShortPath(newChange.myLoc, newChange.enemyLoc);
        std::cout << std::endl << " -> " << step << std::endl;                // 输出我的决策到控制台显示
    }
    if (LeftBlockBar > 0) {                                                                               //在木板还有剩余的情况下可以尝试放板子
        step.myNewBlockBar = setBlockBar(newChange.enemyLoc, newChange.myLoc, this->contraryY);           //尝试放置木板，对对手造成阻碍
        this->blocks.push_back(step.myNewBlockBar);
        LeftBlockBar--;
        step.myNewLoc.x = -1;
        step.myNewLoc.y = -1;
    }
    std::cout << step << std::endl;
    return step;
}
/**********************************
    检测是否前方有挡板，若有挡板则下一步不能走这个方向。
    参数：棋子所在的坐标，棋子准备移动的方向。
***********************************/
bool MyPlayer::isBlockBar(const Location& myLoc,const int right,const int up)
{
    std::vector<BlockBar>::iterator it;
    if (up == 1 && right == 0)//检测上方
    {
        for (it = this->blocks.begin(); it != this->blocks.end();it++) {
            if ((*it).isH()) {
                if (((*it).start.x == myLoc.x - 1 || (*it).start.x == myLoc.x - 2) && (*it).start.y == myLoc.y) {
                    return true;
                }
            }
        }
    }
    if (up == -1 && right == 0) {
        for (it = this->blocks.begin(); it != this->blocks.end(); it++) {
            if ((*it).isH()) {
                if (((*it).start.x == myLoc.x - 1 || (*it).start.x == myLoc.x - 2) && (*it).start.y == myLoc.y  - 1) {
                    return true;
                }
            }
        }
    }
    if (up == 0 && right == -1)//检测左方
    {
        for (it = this->blocks.begin(); it != this->blocks.end(); it++) {
            if ((*it).isV()) {                                   // 竖直方向挡板, start 与 stop 的 x 相等 
                if (((*it).start.y == myLoc.y - 1 || (*it).start.y == myLoc.y - 2) && (*it).start.x == myLoc.x - 1) {
                    return true;// 可能挡路的挡板 
                }
            }
        }
    }
    if (up == 0 && right == 1)//检测右方
    {
        for (it = this->blocks.begin(); it != this->blocks.end(); it++) {
            if ((*it).isV()) {                                   // 竖直方向挡板, start 与 stop 的 x 相等 
                if (((*it).start.y == myLoc.y - 1 || (*it).start.y == myLoc.y - 2) && (*it).start.x == myLoc.x) {
                    return true;// 可能挡路的挡板 
                }
            }
        }
    }
    return false;
}
void MyPlayer::clear(std::queue<Node*>& q) {
    std::queue<Node*>empty;
    swap(empty, q);
}

int MyPlayer::bfs(const Location& myLoc,const Location& enemyLoc,int target)
{
    //地图二维数组
    //地图上的木板坐标需要更新,走过的点计为1,对手棋子所在的点计为2.
    //0 到 9
    //int matrix[9][9];
    int bfsSteps = 0;
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            matrix[i][j] = 0;
        }
    }
    //matrix[enemyLoc.x][enemyLoc.y] = 2;
    myPath.clear();//清空存储路径的vector?需不需要晚点
    clear(qLoc);//清空queue
    int dir[4][2] = { {0,1}, {0,-1},{-1,0}, {1,0} };//上下左右
    Node* p = NULL;
    Node* head = new Node;
    head->NodeLoc.x = myLoc.x;
    head->NodeLoc.y = myLoc.y;
    head->preNode = p;
    qLoc.push(head);
    while (!qLoc.empty())
    {
        Node* p = qLoc.front();
        myPath.push_back(p);
        qLoc.pop();
        bfsSteps++;
        if (bfsSteps == 1) {
            matrix[enemyLoc.x][enemyLoc.y] = 2;
        }
        if (bfsSteps > 1) {
            matrix[enemyLoc.x][enemyLoc.y] = 0;
        }
        //判断终点
        if (target == 9 && (p->NodeLoc.y == target || p->NodeLoc.y  == target + 1))
            break;
        if (target == 1 && (p->NodeLoc.y == target || p->NodeLoc.y == target - 1))
            break;
        for (int i = 0; i < 4; i++) {
            //判断条件：不走到边框外，不走到墙上，不走到对手身上。
            //分情况看是否能移动
            //if语句中包含了所有合法的情况
            //普通的跨越，是不会越过边线的
            if (!isBlockBar(p->NodeLoc, dir[i][0], dir[i][1]) && p->NodeLoc.x + dir[i][0] >= 1 && p->NodeLoc.x + dir[i][0] <= SIZE && p->NodeLoc.y + dir[i][1] >= 1 && p->NodeLoc.y + dir[i][1] <= SIZE && matrix[p->NodeLoc.x + dir[i][0]][p->NodeLoc.y + dir[i][1]] == 0) {
                record(p->NodeLoc.x + dir[i][0], p->NodeLoc.y + dir[i][1], p);
            }
            //检测对手是不是在我的面前
            if (bfsSteps == 1 && matrix[p->NodeLoc.x + dir[i][0]][p->NodeLoc.y + dir[i][1]] == 2) {
                DetectSpan(enemyLoc, i, p, target);
                continue;
                //visit越过棋子后的那个点
            }
        }
    }
    int sum = -1;
    Node* pr = myPath[myPath.size() - 1];//最短路径的终点
    if (target == 9 && pr->NodeLoc.y != target && pr->NodeLoc.y != target + 1)
        return -1;
    if (target == 1 && pr->NodeLoc.y != target && pr->NodeLoc.y != target - 1)
        return -1;
    while (pr != NULL) {
        sum++;
        pr = pr->preNode;
    }
    //std::cout << sum << std::endl;
    return sum;
}
void MyPlayer::record(int x,int y,Node* p)
{
    matrix[x][y] = 1;
    visit(x, y, p);
}
/***************************************************************************
函数：bool DetectSpan:行进过程中遇到了对方棋子，检测能否合法的跨越对方棋子;
参数：Location& myLoc:我的棋子现在寻路过程中走到的坐标
      int i:表示棋子的哪个方向有对手在眼前：0:上方；1：下方；2：左方；3：右方。
返回值：true:可以合法跨越;false:这一步不能跨;
****************************************************************************/
void MyPlayer::DetectSpan(const Location& enemyLoc, const int i,Node* p,const int target)
{
    Location tmp;
    //例：在向上跳跃棋子的过程中，被跳跃的棋子的正上方和正下方都不应该有木板遮挡。
    //且棋子是可以飞过底线的
    if (i == 0) {
        tmp.x = enemyLoc.x;
        tmp.y = enemyLoc.y + 1;
        if (target == 9) {
            if (!isBlockBar(enemyLoc, 0, 1) && !isBlockBar(enemyLoc, 0, -1) && tmp.x >= 1 && tmp.x <= SIZE && tmp.y >= 1 && tmp.y <= SIZE + 1 && matrix[tmp.x][tmp.y] == 0) {
                record(tmp.x, tmp.y, p);
                return;
            }
        }
        else {
            if (!isBlockBar(enemyLoc, 0, 1) && !isBlockBar(enemyLoc, 0, -1) && tmp.x >= 1 && tmp.x <= SIZE && tmp.y >= 1 && tmp.y <= SIZE && matrix[tmp.x][tmp.y] == 0) {
                record(tmp.x, tmp.y, p);
                return;
            }
        }
    }
    if (i == 1) {
        tmp.x = enemyLoc.x;
        tmp.y = enemyLoc.y - 1;
        if (target == 1) {
            if (!isBlockBar(enemyLoc, 0, -1) && !isBlockBar(enemyLoc, 0, 1) && tmp.x >= 1 && tmp.x <= SIZE && tmp.y >= 0 && tmp.y <= SIZE && matrix[tmp.x][tmp.y] == 0) {
                record(tmp.x, tmp.y, p);
                return;
            }
        }
        else {
            if (!isBlockBar(enemyLoc, 0, -1) && !isBlockBar(enemyLoc, 0, 1) && tmp.x >= 1 && tmp.x <= SIZE && tmp.y >= 1 && tmp.y <= SIZE && matrix[tmp.x][tmp.y] == 0) {
                record(tmp.x, tmp.y, p);
                return;
            }
        }
    }
    if (i == 2) {
        tmp.x = enemyLoc.x - 1;
        tmp.y = enemyLoc.y;
        if (!isBlockBar(enemyLoc, -1, 0) && !isBlockBar(enemyLoc,1,0) && tmp.x >= 1 && tmp.x <= SIZE && tmp.y >= 1 && tmp.y <= SIZE && matrix[tmp.x][tmp.y] == 0) {
            record(tmp.x, tmp.y, p);
            return;
        }
    }
    if (i == 3) {
        tmp.x = enemyLoc.x + 1;
        tmp.y = enemyLoc.y;
        if (!isBlockBar(enemyLoc, 1, 0) && !isBlockBar(enemyLoc,-1,0) && tmp.x >= 1 && tmp.x <= SIZE && tmp.y >= 1 && tmp.y <= SIZE && matrix[tmp.x][tmp.y] == 0) {
            record(tmp.x, tmp.y, p);
            return;
        }
    }
}
void MyPlayer::visit(int x, int y, Node* p)
{
    Node* pr = new Node;
    pr->NodeLoc.x = x;
    pr->NodeLoc.y = y;
    pr->preNode = p;
    qLoc.push(pr);
}
Location MyPlayer::ShortPath(const Location& myLoc,const Location& enemyLoc)
{
    //int sum = -1;
    Location nextLoc;
    Node* p = myPath[myPath.size() - 1];
    //nextLoc = p->NodeLoc;
    //std::cout << std::endl;
    while (p != NULL)
    {
        //sum++;
        std::cout << "(" << p->NodeLoc.x << "," << p->NodeLoc.y << ")";
        if (p->preNode != NULL)
            std::cout << "<-";
        if (p->preNode != NULL && p->preNode->NodeLoc == myLoc) {
            nextLoc = p->NodeLoc;
        }
        p = p->preNode;
    }
    if (enemyPath.empty())//对手最短路径信息为空，则存储
    {
        Node* q = myPath[myPath.size() - 1];
        while (q != NULL) {
            Location pr;
            pr = q->NodeLoc;
            //q = q->preNode;
            q = q->preNode;
            enemyPath.push_back(pr);
        }
    }
    //std::cout << std::endl;
    //std::cout << sum << std::endl;
    //Destory();
    return nextLoc;
}
void MyPlayer::Destory()
{
    for (int i = 0; i < myPath.size(); ++i) {
        delete myPath[i];
    }
}
BlockBar MyPlayer::setBlockBar(const Location& myLoc, const Location& enemyLoc, const int target)
{
    BlockBar tarBlockBar;
    int distance = 0;
    int dir[8][2] = { {-2,0},{-1,0},{-2,-1},{-1,-1},{-1,-1},{-1,-2},{0,-1},{0,-2} };//八种可能放置木板的情况的起始坐标较棋子坐标的偏移量。
    //std::cout << std::endl;
    std::vector<Location>::reverse_iterator it;//反向迭代器
    std::vector<BlockBar>::iterator bl;
    for (it = enemyPath.rbegin();it != enemyPath.rend();++it){
        //std::cout << "(" << it->x << "," << it->y << ")";
        //std::cout << "<-";
        for (int i = 0; i < 8; ++i) {
            Location newBlockBarStart;
            Location newBlockBarStop;
            newBlockBarStart.x = (*it).x + dir[i][0];
            newBlockBarStart.y = (*it).y + dir[i][1];
            if (i < 4) {
                newBlockBarStop.x = newBlockBarStart.x + 2;
                newBlockBarStop.y = newBlockBarStart.y;
            }
            else{
                newBlockBarStop.x = newBlockBarStart.x;
                newBlockBarStop.y = newBlockBarStart.y + 2;
            }
            BlockBar newBlockbar(newBlockBarStart, newBlockBarStop);
            if (!newBlockbar.isNan() && !doublication(newBlockbar))                //表示挡板在合法范围内且挡板不与其他挡板重合
            {
                UpDateBlocks(newBlockbar);
                //TODO
                //bfs:对手的视角
                distance = bfs(myLoc, enemyLoc, target);//判断是否会把路堵死了,若路被封死，则bfs的返回值是-1;
                if (distance != -1) {
                    //ShortPath(enemyLoc, myLoc);
                    //std::cout << std::endl;
                    BlockBarState imageState;
                    imageState.distance = distance;
                    imageState.ImageBlockBar = newBlockbar;
                    this->imagePath.push_back(imageState);
                    PopNewBlockbar(newBlockbar);
                }
                if (distance == -1) {
                    PopNewBlockbar(newBlockbar);
                }
            }
        }
    }
    std::vector<BlockBarState>::iterator pv;
    int influence = 0;
    for (pv = this->imagePath.begin(); pv != this->imagePath.end() - 1; ++pv) {
        //std::cout << "预设木板：" << (*pv).ImageBlockBar << "bfs距离:" << (*pv).distance << std::endl;
        if ((*pv).distance > influence)
            influence = (*pv).distance;                                      //找出最大的distance
    }
    for (pv = this->imagePath.begin(); pv != this->imagePath.end(); ++pv) {
        if (influence == (*pv).distance) {
            tarBlockBar = (*pv).ImageBlockBar;
            std::cout << "影响值" << (*pv).distance << std::endl;
            break;
        }
    }
    std::cout << "值得放置的木板:"<<tarBlockBar << std::endl;
    //this->blocks.push_back(tarBlockBar);
    this->imagePath.clear();
    return tarBlockBar;//返回影响值最大的木板
}
bool MyPlayer::doublication(const BlockBar& newBlockBar)
{
    std::vector<BlockBar>::iterator it;
    for (it = this->blocks.begin(); it != this->blocks.end(); ++it) {
        //if (newBlockBar == (*it) || (*it).start.x < 1 || (*it).start.x > 8 || (*it).start.y < 1 || (*it).start.y > 8 || (*it).stop.x < 1 || (*it).stop.x > 8 || (*it).stop.y < 1 || (*it).stop.y > 8)
        //    return true;
        if ((*it).isH())//横的挡板
        {
            if (newBlockBar.start.x >= (*it).start.x - 1 && newBlockBar.start.x <= (*it).start.x + 1 && newBlockBar.start.y == (*it).start.y)
                return true;
        }
        if ((*it).isV()) {
            if (newBlockBar.start.y >= (*it).start.y - 1 && newBlockBar.start.y <= (*it).start.y + 1 && newBlockBar.start.x == (*it).start.x)
                return true;
        }
    }
    return false;
}
void MyPlayer::UpDateBlocks(const BlockBar& newBlockBar)
{
    this->blocks.push_back(newBlockBar);
}
void MyPlayer::PopNewBlockbar(const BlockBar& newBlockBar)
{
    std::vector<BlockBar>::iterator it = this->blocks.end() - 1;
    it = this->blocks.erase(it);
}
// 面向对象实现结束

// 面向过程实现开始
//Step MyPlayer::nextStep(const ChessboardChange& newChange) {
//    // 须知: 本示例代码仅作为 API 使用以及游戏规则了解范例, 甚至保证不了正确性, 切勿照抄照搬 
//    if (targetY == 0) {                                // 判断并记录自己的目标
//        const auto d = std::chrono::system_clock::now().time_since_epoch();
//        const int nanosecond = (std::chrono::nanoseconds(d).count() / 100) % int64_t(INT32_MAX);
//        srand(nanosecond);
//        if (newChange.myLoc == PLAYER0_LOC) {                // 己方初始坐标为 (5,1) 则目标为 (x,9)
//            targetY = PLAYER1_LOC.y;
//        } else {                                             // 己方初始坐标为 (5,9) 则目标为 (x,1)
//            targetY = PLAYER0_LOC.y;
//        }
//    }
//    std::cout << newChange << std::endl;                     // 输出接收到的数据到控制台显示  
//    if (!newChange.newEnemyBlockBar.isNan()) {               // 对方放置了挡板 
//        BlockBar tmp = newChange.newEnemyBlockBar;
//        tmp.normalization();                                 // 规范为 start 坐标小于 stop 坐标的形式 
//        blocks.push_back(tmp);                         // 存储挡板 
//    }
//    Step step;                                               // 初始化 step 默认不移动不放板 
//    step.myNewLoc = randomWalk(newChange.myLoc, newChange.enemyLoc);
//    std::cout << step << std::endl;                          // 输出我的决策到控制台显示 
//    return step;
//}
//
//void MyPlayer::restart() {
//    blocks.clear();
//    targetY = 0;
//}
// 面向过程实现结束
}