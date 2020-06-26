#ifndef BOARD_H
#define BOARD_H

#include <QMainWindow>
#include"mydata.h"
#include"stone.h"


class board : public QMainWindow
{
    Q_OBJECT
public:
    explicit board(QMainWindow *parent = nullptr);

    void initboard();  // 初始化窗口

    void paintEvent(QPaintEvent *event);

    void drawstone(QPainter & paint,int id);  // 绘制象棋,(窗口已于paint相关联)

    QPoint center(int row,int col);  // 获取线与线的交点，圆形棋子的圆点
    QPoint center(int id);  // 函数重载，用于简化代码，传进棋子id，返回center值

    //void mousePressEvent(QMouseEvent *event);

    bool getrowcol(QPoint pix,int & row,int & col);  // 返回的是bool类型，判断是否点击在棋盘里面

    bool canmove(int moveid,int row,int col,int killid); // 判断棋子是否能移动

    bool hasStone(int row, int col);  // 判断是否有棋

    // 判断棋子所在行和列中是否有棋子,1代表有，0代表无,bool值代表行或列
    int hasStone(int a, int b, bool bRow, int row,int col);

    // 各个角色的走棋规则
    bool canmove_JIANG(int moveid,int row,int col,int killid);
    bool canmove_SHI(int moveid,int row,int col,int killid);
    bool canmove_XIANG(int moveid,int row,int col,int killid);
    bool canmove_CHE(int moveid,int row,int col,int killid);
    bool canmove_MA(int moveid,int row,int col,int killid);
    bool canmove_PAO(int moveid,int row,int col,int killid);
    bool canmove_BING(int moveid,int row,int col,int killid);

    void mouseReleaseEvent(QMouseEvent *event);  // 释放事件用于走棋

public:
    stone m_stone[32];  // 32颗象棋

    int stone_R = Chess_length/2;  // 圆形棋子的半径 = d/2

    int m_recorder_id;  // 记录棋子

    //int m_click_id; // 记录点击

    bool isblackmove;  // 轮到谁走，一开始置为true，代表游戏开始红方先走,红方走完后，黑方才能走


signals:

public slots:
};

#endif // BOARD_H




