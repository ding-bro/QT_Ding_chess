#include "stone.h"

stone::stone()
{

}


QString stone::drawstonetext()
{
    switch (this->stone_type)
    {
    case this->JIANG:
        return "将";
    case this->SHI:
        return "士";
    case this->XIANG:
        return "象";
    case this->PAO:
        return "炮";
    case this->CHE:
        return "车";
    case this->BING:
        return "兵";
    case this->MA:
        return "马";
    default:
        return "获取象棋角色文本错误！！！";
    }
}


void stone::stoneinit(int id)
{

    // 初始化象棋的位置
    struct
    {
        int row,col;
        stone::TYPE type;
    }pos[16] = {
    {0,0,stone::CHE},
    {0,1,stone::MA},
    {0,2,stone::XIANG},
    {0,3,stone::SHI},
    {0,4,stone::JIANG},
    {0,5,stone::SHI},
    {0,6,stone::XIANG},
    {0,7,stone::MA},
    {0,8,stone::CHE},

    {2,1,stone::PAO},
    {2,7,stone::PAO},
    {3,0,stone::BING},
    {3,2,stone::BING},
    {3,4,stone::BING},
    {3,6,stone::BING},
    {3,8,stone::BING},
    };


    this->stone_id = id;
    this->isdead = false;  // 一开始棋子置为不死亡状态
    this->isred = id < 16;   // 一半为红，一半为黑，当id<16时为红棋

    if(id < 16)  // 红棋
    {
        this->row = pos[id].row;
        this->col = pos[id].col;
        this->stone_type = pos[id].type;
    }
    else  // 黑棋
    {
        this->row = 9 - pos[id - 16].row;
        this->col = 8 - pos[id - 16].col;
        this->stone_type = pos[id - 16].type;
    }
}


















