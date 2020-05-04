#include "MyPlayer.h"
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <ctime>
// �������ʵ�ֿ�ʼ
//using namespace QuoridorUtils;
//int targetY = 0;
//std::vector<QuoridorUtils::BlockBar> blocks;
//QuoridorUtils::Location randomWalk(const QuoridorUtils::Location& myLoc, const QuoridorUtils::Location& enemyLoc) {
//    bool directions[4];
//    directions[0] = myLoc.y < QuoridorUtils::SIZE;           // �Ƿ���������� 
//    directions[1] = myLoc.y > 0;                             // �Ƿ���������� 
//    directions[2] = myLoc.x > 0;                             // �Ƿ���������� 
//    directions[3] = myLoc.x < QuoridorUtils::SIZE;           // �Ƿ���������� 
//    for (auto block : blocks) {                        // ���������б�, �ҵ����赲�ķ��� 
//        if (block.isH()) {                                   // ˮƽ���򵲰�, start �� stop �� y ��� 
//            if (block.start.x == myLoc.x - 1 ||
//                block.start.x == myLoc.x - 2) {              // ���ܵ�·�ĵ��� 
//                if (block.start.y == myLoc.y) {
//                    directions[0] = false;
//                } else if (block.start.y == myLoc.y - 1) {
//                    directions[1] = false;
//                }
//            }
//        }
//        if (block.isV()) {                                   // ��ֱ���򵲰�, start �� stop �� x ��� 
//            if (block.start.y == myLoc.y - 1 ||
//                block.start.y == myLoc.y - 2) {              // ���ܵ�·�ĵ��� 
//                if (block.start.x == myLoc.x) {
//                    directions[2] = false;
//                } else if (block.start.x == myLoc.x - 1) {
//                    directions[3] = false;
//                }
//            }
//        }
//    }
//    int optionsCount = directions[0] + directions[1] + 
//                       directions[2] + directions[3];        // �����ܶ�������, �ض����ڵ���1 
//    int tmp = rand() % optionsCount;                         // ���һ�� [0, optionsCount - 1] ������
//    int directionIndex;                                      // �ҳ����������±� 
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
//    case 0:   // ������
//        nextLoc.y++;
//        break;
//    case 1:   // ������
//        nextLoc.y--;
//        break;
//    case 2:   // ������
//        nextLoc.x--;
//        break;
//    case 3:   // ������
//        nextLoc.x++;
//        break;
//    default: ;
//    }
//    if (nextLoc.distance(enemyLoc) == 0) {                   // ��һ���ǵо�λ����Ӧ�����о����� 
//        nextLoc.x = -1;                                      // �˴�����ʵ��, Ĭ����һ���ղ��� 
//        nextLoc.y = -1;
//    }
//    return nextLoc;
//}
// �������ʵ�ֽ���

namespace QuoridorUtils {

/*
 * Step MyPlayer::nextStep(const ChessboardChange& newChange)
 *
 * newChange Ϊ�޸ĵ����, ���ʽΪ
 * (GameStatus status, Location enemyLoc, Location myLoc, BlockBar newEnemyBlockBar)
 * ������������͵����������ͼ, ������û�������µĵ���ʱ,
 * newChange.newEnemyBlockBar.isNan()Ϊtrue
 * 
 * ��Ҫ���ص�����StepΪ
 * (Location myNewLoc, BlockBar myNewBlockBar)
 * ����ѡ�����ƶ�����һ����ǰ�������ĸ����򣩻��߷�һ������:
 * ���ƶ�����ʱ, step.myNewBlockBar.isNan()Ϊtrue; 
 * �����õ���ʱ, step.myNewLocΪnewChange�е�myLoc
 *
 * ���д���(����λ�ô���, ����������, �ƶ�λ�ô���, ͬʱ�ƶ������õ���, ��ʱ��)���ɷ��������,
 * ������Ҫ������ʵ�ִ�����⹦��. 
 *
 *  ������������:                 ��������ṹ���£�
 *  ��-��-��-��-��-��-��-��-��-��          ��-��-��-��-��-��-��-��-��-��9
 *  ��-��-��-��-��-��-��-��-��-�ȨI9       ��-��-��-��-��-��-��-��-��-��8
 *  ��-��-��-��-��-��-��-��-��-�ȨI8       ��-��-��-��-��-��-��-��-��-��7
 *  ��-��-��-��-��-��-��-��-��-�ȨI7       ��-��-��-��-��-��-��-��-��-��6
 *  ��-��-��-��-��-��-��-��-��-�ȨI6       ��-��-��-��-��-��-��-��-��-��5
 *  ��-��-��-��-��-��-��-��-��-�ȨI5       ��-��-��-��-��-��-��-��-��-��4
 *  ��-��-��-��-��-��-��-��-��-�ȨI4       ��-��-��-��-��-��-��-��-��-��3
 *  ��-��-��-��-��-��-��-��-��-�ȨI3       ��-��-��-��-��-��-��-��-��-��2
 *  ��-��-��-��-��-��-��-��-��-�ȨI2       ��-��-��-��-��-��-��-��-��-��1
 *  ��-��-��-��-��-��-��-��-��-���I1 y     ��-��-��-��-��-��-��-��-��-��0 y
 * x  1  2  3  4  5  6  7  8  9          x  0  1  2  3  4  5  6  7  8  9
      0  1  2  3  4  5  6  7  8
 *
 */

// �������ʵ�ֿ�ʼ
Location MyPlayer::randomWalk(const Location& myLoc, const Location& enemyLoc) {
    bool directions[4];
    directions[0] = myLoc.y < SIZE;                          // �Ƿ���������� 
    directions[1] = myLoc.y > 1;                             // �Ƿ���������� 
    directions[2] = myLoc.x > 1;                             // �Ƿ���������� 
    directions[3] = myLoc.x < SIZE;                          // �Ƿ���������� 
    for (auto block : this->blocks) {                        // ���������б�, �ҵ����赲�ķ��� 
        if (block.isH()) {                                   // ˮƽ���򵲰�, start �� stop �� y ��� 
            if (block.start.x == myLoc.x - 1 || block.start.x == myLoc.x - 2) {              // ���ܵ�·�ĵ��� 
                if (block.start.y == myLoc.y) {
                    directions[0] = false;
                } else if (block.start.y == myLoc.y - 1) {
                    directions[1] = false;
                }
            }
        }
        if (block.isV()) {                                   // ��ֱ���򵲰�, start �� stop �� x ��� 
            if (block.start.y == myLoc.y - 1 || block.start.y == myLoc.y - 2) {              // ���ܵ�·�ĵ��� 
                if (block.start.x == myLoc.x) {
                    directions[3] = false;
                } else if (block.start.x == myLoc.x - 1) {
                    directions[2] = false;
                }
            }
        }
    }
    int optionsCount = int(directions[0]) + int(directions[1]) + 
                       int(directions[2]) + int(directions[3]);    // �����в��ܶ�������, �ض����ڵ���1 
    int tmp = rand() % optionsCount;                               // ���һ�� [0, optionsCount - 1] ������
    int directionIndex;                                            // �ҳ����������±� 
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
    case 0:   // ������
        nextLoc.y++;
        break;
    case 1:   // ������
        nextLoc.y--;
        break;
    case 2:   // ������
        nextLoc.x--;
        break;
    case 3:   // ������
        nextLoc.x++;
        break;
    default: ;
    }
    if (nextLoc.distance(enemyLoc) == 0) {                   // ��һ���ǵо�λ����Ӧ�����о����� 
        nextLoc.x = -1;                                      // �˴�����ʵ��, Ĭ����һ���ղ��� 
        nextLoc.y = -1;
    }
    return nextLoc;
}
/******************************************************************
Step MyPlayer::nextStep(const ChessboardChange& newChange) {
    // ��֪: ��ʾ���������Ϊ API ʹ���Լ���Ϸ�����˽ⷶ��, ������֤������ȷ��, �����ճ��հ� 
    if (this->targetY == 0) {                                // �жϲ���¼�Լ���Ŀ�� 
        const auto d = std::chrono::system_clock::now().time_since_epoch();
        const int nanosecond = (std::chrono::nanoseconds(d).count() / 100) % int64_t(INT32_MAX);
        srand(nanosecond);
        if (newChange.myLoc == PLAYER0_LOC) {                // ������ʼ����Ϊ (5,1) ��Ŀ��Ϊ (x,9) 
            this->targetY = PLAYER1_LOC.y;
        } else {                                             // ������ʼ����Ϊ (5,9) ��Ŀ��Ϊ (x,1) 
            this->targetY = PLAYER0_LOC.y;
        }
    }
    std::cout << newChange;                                  // ������յ������ݵ�����̨��ʾ 
    if (!newChange.newEnemyBlockBar.isNan()) {               // �Է������˵��� 
        BlockBar tmp = newChange.newEnemyBlockBar;
        tmp.normalization();                                 // �淶Ϊ start ����С�� stop �������ʽ 
        this->blocks.push_back(tmp);                         // �洢�淶���� 
    }
    Step step;                                               // ��ʼ�� step Ĭ�ϲ��ƶ����Ű� 
    step.myNewLoc = randomWalk(newChange.myLoc, newChange.enemyLoc);
    std::cout << " -> " << step << std::endl;                // ����ҵľ��ߵ�����̨��ʾ 
    return step;
}

************************************************************************/
void MyPlayer::restart() {
    this->blocks.clear();
    this->targetY = 0;
}




//�ҵ�ʵ��
Step MyPlayer::nextStep(const ChessboardChange& newChange) {
    this->blocks.reserve(20);
    MyMinPath = 0;
    EnemyMinPath = 0;
    if (this->targetY == 0) {                                // �жϲ���¼�Լ���Ŀ�� 
        const auto d = std::chrono::system_clock::now().time_since_epoch();
        const int nanosecond = (std::chrono::nanoseconds(d).count() / 100) % int64_t(INT32_MAX);
        srand(nanosecond);
        if (newChange.myLoc == PLAYER0_LOC) {                // ������ʼ����Ϊ (5,1) ��Ŀ��Ϊ (x,9) 
            this->targetY = PLAYER1_LOC.y;
            this->contraryY = PLAYER0_LOC.y;
        }
        else {                                             // ������ʼ����Ϊ (5,9) ��Ŀ��Ϊ (x,1) 
            this->targetY = PLAYER0_LOC.y;
            this->contraryY = PLAYER1_LOC.y;                //��¼���ֵ�Ŀ�꣬���ҷ��෴��
        }
    }
    std::cout << newChange;                                  // ������յ������ݵ�����̨��ʾ 
    if (!newChange.newEnemyBlockBar.isNan()) {               // �Է������˵��� 
        BlockBar tmp = newChange.newEnemyBlockBar;
        tmp.normalization();                                 // �淶Ϊ start ����С�� stop �������ʽ 
        this->blocks.push_back(tmp);                         // �洢�淶���� 
    }

    std::cout << std::endl << "���ֵĲ���" << std::endl;
    EnemyMinPath = bfs(newChange.enemyLoc, newChange.myLoc, this->contraryY);           //������ֵ����·��
    ShortPath(newChange.enemyLoc, newChange.myLoc);

    std::cout << std::endl << "�ҵĲ���:";
    MyMinPath =  bfs(newChange.myLoc, newChange.enemyLoc,this->targetY);                 
    Step step;                                               // ��ʼ�� step Ĭ�ϲ��ƶ����Ű� 
    step.myNewLoc = ShortPath(newChange.myLoc,newChange.enemyLoc);
    std::cout << std::endl << " -> " << step << std::endl;                // ����ҵľ��ߵ�����̨��ʾ 
    return step;
}
/**********************************
    ����Ƿ�ǰ���е��壬���е�������һ���������������
    �������������ڵ����꣬����׼���ƶ��ķ���
***********************************/
bool MyPlayer::isBlockBar(const Location& myLoc,const int right,const int up)
{
    std::vector<BlockBar>::iterator it;
    if (up == 1 && right == 0)//����Ϸ�
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
    if (up == 0 && right == -1)//�����
    {
        for (it = this->blocks.begin(); it != this->blocks.end(); it++) {
            if ((*it).isV()) {                                   // ��ֱ���򵲰�, start �� stop �� x ��� 
                if (((*it).start.y == myLoc.y - 1 || (*it).start.y == myLoc.y - 2) && (*it).start.x == myLoc.x - 1) {
                    return true;// ���ܵ�·�ĵ��� 
                }
            }
        }
    }
    if (up == 0 && right == 1)//����ҷ�
    {
        for (it = this->blocks.begin(); it != this->blocks.end(); it++) {
            if ((*it).isV()) {                                   // ��ֱ���򵲰�, start �� stop �� x ��� 
                if (((*it).start.y == myLoc.y - 1 || (*it).start.y == myLoc.y - 2) && (*it).start.x == myLoc.x) {
                    return true;// ���ܵ�·�ĵ��� 
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
    //��ͼ��ά����
    //��ͼ�ϵ�ľ��������Ҫ����,�߹��ĵ��Ϊ1,�����������ڵĵ��Ϊ2.
    //0 �� 9
    //int matrix[9][9];
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            matrix[i][j] = 0;
        }
    }
    matrix[enemyLoc.x][enemyLoc.y] = 2;
    myPath.clear();//��մ洢·����vector?�費��Ҫ����
    clear(qLoc);//���queue
    int dir[4][2] = { {0,1}, {0,-1},{-1,0}, {1,0} };//��������
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
        //�ж��յ�
        if (target == 9 && (p->NodeLoc.y == target || p->NodeLoc.y  == target + 1))
            break;
        if (target == 1 && (p->NodeLoc.y == target || p->NodeLoc.y == target - 1))
            break;
        for (int i = 0; i < 4; i++) {
            //�ж����������ߵ��߿��⣬���ߵ�ǽ�ϣ����ߵ��������ϡ�
            //��������Ƿ����ƶ�
            //����ҷ������ܷ��Խ����
            if (matrix[p->NodeLoc.x + dir[i][0]][p->NodeLoc.y + dir[i][1]] == 2) {
                DetectSpan(enemyLoc, i, p,target);
                continue;
                //visitԽ�����Ӻ���Ǹ���
            }
            //if����а��������кϷ������
            //��ͨ�Ŀ�Խ���ǲ���Խ�����ߵ�
            if (!isBlockBar(p->NodeLoc, dir[i][0], dir[i][1]) && p->NodeLoc.x + dir[i][0] >= 1 && p->NodeLoc.x + dir[i][0] <= SIZE && p->NodeLoc.y + dir[i][1] >= 1 && p->NodeLoc.y + dir[i][1] <= SIZE && matrix[p->NodeLoc.x + dir[i][0]][p->NodeLoc.y + dir[i][1]] == 0) {
                record(p->NodeLoc.x + dir[i][0], p->NodeLoc.y + dir[i][1], p);
            }
            if (!isBlockBar(p->NodeLoc, dir[i][0], dir[i][1]) && p->NodeLoc.x + dir[i][0] >= 1 && p->NodeLoc.x + dir[i][0] <= SIZE && p->NodeLoc.y + dir[i][1] >= 1 && p->NodeLoc.y + dir[i][1] <= SIZE && matrix[p->NodeLoc.x + dir[i][0]][p->NodeLoc.y + dir[i][1]] == 0) {
                record(p->NodeLoc.x + dir[i][0], p->NodeLoc.y + dir[i][1], p);
            }
        }
    }
    int sum = 0;
    Node* pr = myPath[myPath.size() - 1];
    while (pr != NULL) {
        sum++;
        pr = pr->preNode;
    }
    std::cout << sum << std::endl;
    return sum;
}
void MyPlayer::record(int x,int y,Node* p)
{
    matrix[x][y] = 1;
    visit(x, y, p);
}
/***************************************************************************
������bool DetectSpan:�н������������˶Է����ӣ�����ܷ�Ϸ��Ŀ�Խ�Է�����;
������Location& myLoc:�ҵ���������Ѱ·�������ߵ�������
      int i:��ʾ���ӵ��ĸ������ж�������ǰ��0:�Ϸ���1���·���2���󷽣�3���ҷ���
����ֵ��true:���ԺϷ���Խ;false:��һ�����ܿ�;
****************************************************************************/
void MyPlayer::DetectSpan(const Location& enemyLoc, const int i,Node* p,const int target)
{
    Location tmp;
    //������������Ծ���ӵĹ����У�����Ծ�����ӵ����Ϸ������·�����Ӧ����ľ���ڵ���
    //�������ǿ��Էɹ����ߵ�
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
    int sum = 0;
    Location nextLoc;
    Node* p = myPath[myPath.size() - 1];
    //nextLoc = p->NodeLoc;
    //std::cout << std::endl;
    while (p != NULL)
    {
        sum++;
    //    if (p->preNode != NULL)
    //        p = p->preNode;
    //}
        std::cout << "(" << p->NodeLoc.x << "," << p->NodeLoc.y << ")";
        if (p->preNode != NULL)
            std::cout << "<-";
        if (p->preNode != NULL && p->preNode->NodeLoc == myLoc) {
            nextLoc = p->NodeLoc;
        }
        p = p->preNode;
    }
    //std::cout << std::endl;
    //std::cout << sum << std::endl;
    Destory();
    return nextLoc;
}
void MyPlayer::Destory()
{
    for (int i = 0; i < myPath.size(); ++i) {
        delete myPath[i];
    }
}
// �������ʵ�ֽ���

// �������ʵ�ֿ�ʼ
//Step MyPlayer::nextStep(const ChessboardChange& newChange) {
//    // ��֪: ��ʾ���������Ϊ API ʹ���Լ���Ϸ�����˽ⷶ��, ������֤������ȷ��, �����ճ��հ� 
//    if (targetY == 0) {                                // �жϲ���¼�Լ���Ŀ��
//        const auto d = std::chrono::system_clock::now().time_since_epoch();
//        const int nanosecond = (std::chrono::nanoseconds(d).count() / 100) % int64_t(INT32_MAX);
//        srand(nanosecond);
//        if (newChange.myLoc == PLAYER0_LOC) {                // ������ʼ����Ϊ (5,1) ��Ŀ��Ϊ (x,9)
//            targetY = PLAYER1_LOC.y;
//        } else {                                             // ������ʼ����Ϊ (5,9) ��Ŀ��Ϊ (x,1)
//            targetY = PLAYER0_LOC.y;
//        }
//    }
//    std::cout << newChange << std::endl;                     // ������յ������ݵ�����̨��ʾ  
//    if (!newChange.newEnemyBlockBar.isNan()) {               // �Է������˵��� 
//        BlockBar tmp = newChange.newEnemyBlockBar;
//        tmp.normalization();                                 // �淶Ϊ start ����С�� stop �������ʽ 
//        blocks.push_back(tmp);                         // �洢���� 
//    }
//    Step step;                                               // ��ʼ�� step Ĭ�ϲ��ƶ����Ű� 
//    step.myNewLoc = randomWalk(newChange.myLoc, newChange.enemyLoc);
//    std::cout << step << std::endl;                          // ����ҵľ��ߵ�����̨��ʾ 
//    return step;
//}
//
//void MyPlayer::restart() {
//    blocks.clear();
//    targetY = 0;
//}
// �������ʵ�ֽ���
}
