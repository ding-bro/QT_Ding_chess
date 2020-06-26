#ifndef STONE_H
#define STONE_H

#include<QString>


class stone
{  
public:
    stone();

    QString drawstonetext();  // 绘制象棋上的角色文本

    void stoneinit(int id);  // 初始化棋子位置

public:

    // 棋子的角色类型
    enum TYPE
    {
        JIANG,SHI,XIANG,BING,PAO,MA,CHE  // 将，士,象.......
    };


    int row;  // 棋子所在棋盘的行数

    int col;  // 棋子所在棋盘的列数

    int stone_id;   // 棋子id

    bool isdead;   // 棋子是否死亡

    bool isred;   // 棋子是红方还是黑方

    TYPE stone_type;       // 棋子的角色类型
};

#endif // STONE_H



