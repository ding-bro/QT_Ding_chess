#include "board.h"
#include<QPainter>
#include<QPixmap>
#include <QDesktopWidget>
#include<QApplication>
#include<QMenuBar>
#include<QFont>
#include<QMouseEvent>
#include<QMessageBox>



board::board(QMainWindow *parent) : QMainWindow(parent)
{
    this->initboard();   // 初始窗口

    // 初始化象棋的位置
    for(int i = 0; i < 32; i++)
    {
        this->m_stone[i].stoneinit(i);  // 一开始分配象棋位置
    }

    this->m_recorder_id = -1; // 默认记录棋子值为-1

    this->isblackmove = false;  // 一开始定义红方先走棋
}


void board::initboard()
{

    this->setWindowTitle(Window_name);
    this->setFixedSize(Window_rect_size,Window_rect_size);

    // 窗口自定义合适位置显示
    QDesktopWidget *deskdop=QApplication::desktop();
    move((deskdop->width() - this->width())/2, 0);


    QMenuBar * bar = menuBar();
    setMenuBar(bar);

    QMenu * func = bar->addMenu("功能");

    QAction * explain = func->addAction("简介");
    QAction * exit = func->addAction("退出");

    connect(explain,&QAction::triggered,[=](){
        QString str1 = "操作说明";
        QString str2 = "DingYN:\n并未实现人机对战，红棋先走,黑棋后走,棋盘算法参考CSDN";
        QMessageBox::question(this,str1,str2,QStringLiteral("确定"));
    });

    connect(exit,&QAction::triggered,[=](){
        this->close();
    });
}


void board::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);

    // 画棋盘中的十条横线
    for(int i = 1; i <= 10; i++)
    {
        paint.drawLine(QPoint(Chess_length,i * Chess_length),QPoint(9 * Chess_length,i * Chess_length));
    }

    // 画棋盘中的九条竖线
    for(int i = 1; i <= 9; ++i)
    {
        if(i == 1 || i == 9)  // 当i=1和i=9是，才是一条线画到底
        {
            paint.drawLine(QPoint(i * Chess_length,Chess_length),QPoint(i * Chess_length,10 * Chess_length));
        }
        else    // 划分楚河汉界,一次画一半
        {
            paint.drawLine(QPoint(i * Chess_length,Chess_length),QPoint(i * Chess_length,5 * Chess_length));
            paint.drawLine(QPoint(i * Chess_length,6 * Chess_length),QPoint(i * Chess_length,10 * Chess_length));
        }
    }


    // 画棋盘中的九宫格
    paint.drawLine(QPoint(4 * Chess_length,Chess_length),QPoint(6 * Chess_length,3 * Chess_length));
    paint.drawLine(QPoint(6 * Chess_length,Chess_length),QPoint(4 * Chess_length,3 * Chess_length));
    paint.drawLine(QPoint(4 * Chess_length,8 * Chess_length),QPoint(6 * Chess_length,10 * Chess_length));
    paint.drawLine(QPoint(6 * Chess_length,8 * Chess_length),QPoint(4 * Chess_length,10 * Chess_length));


    // 绘制32颗象棋
    for(int i = 0; i < 32; i++)
    {
        this->drawstone(paint,i);
    }
}


QPoint board::center(int row, int col)
{
    QPoint pos;                                // 坐标与Chess_length相关
    pos.rx() = (col + 1) * this->stone_R * 2;   // 交点X坐标对应列
    pos.ry() = (row + 1) * this->stone_R * 2;   // 交点Y坐标对应行
    return pos;  // 返回对应的QPoint点
}


QPoint board::center(int id)  // 函数重载，简化代码
{
    return center(this->m_stone[id].row,this->m_stone[id].col);
}


void board::drawstone(QPainter & paint, int id)
{
    if(this->m_stone[id].isdead)
    {
        return;
    }


    QPoint point = this->center(id);  // 获取线与线之间的交点，也就是棋子的中心点

    // 文本矩形框
    QRect textrect = QRect(point.x() - this->stone_R,point.y() - this->stone_R,this->stone_R * 2,this->stone_R * 2);

    // 设置字体大小，类型
    QFont font;
    font.setPointSize(40);
    font.setFamily("微软雅黑");
    paint.setFont(font);

    if(id == this->m_recorder_id)  // 如果棋子被点中
    {
        paint.setBrush(QColor(Qt::darkRed)); // 设置棋子颜色  深红色
    }
    else  //  棋子处于没有点击的状态
    {
        paint.setBrush(QBrush(QColor(201, 186, 131)));  // 设置棋子颜色   泥黄色
    }


    paint.setPen(Qt::black);

    paint.drawEllipse(this->center(id),this->stone_R,this->stone_R);  // 绘制圆形棋子

    if(this->m_stone[id].isred)   // 当棋子数小于16时，为红色
    {
        paint.setPen(QColor(Qt::black));  // 为了调换红黑位置而不违反棋盘算法，故此违反逻辑置为黑色
    }
    else
    {
        paint.setPen(QColor(Qt::red));
    }

    paint.drawText(textrect,this->m_stone[id].drawstonetext(),QTextOption(Qt::AlignCenter));  // 绘制文本,居中显示,文本函数另封装

}


bool board::getrowcol(QPoint pix, int &row, int &col)
{
    /*
        遍历整个棋盘去判断,原理在 CSDN 可查。
        在棋子的圆形范围内：
        任意点击一点为pix,那么pix点与center点(线与线之间的交点)之间的距离就是呈三角形显示
        距离为三角形的斜边，斜边利用勾股定理计算 ->-> 斜边的平方=(pix.x)的平方+(pix.y)的平方
        如果斜边的平方 < 棋子圆形r的平方，就说明这个点就是在棋子的范围内,那么就返回true
    */

   for(row = 0; row <= 9; row++)
   {
       for(col = 0; col <= 8; col++)
       {
           QPoint point = center(row,col);  // 获取交点
           int distance_x = point.x() - pix.x();
           int distance_y = point.y() - pix.y();
           int distance = distance_x * distance_x + distance_y * distance_y; // 点击点 与 交点 距离的平方
           if(distance < this->stone_R * this->stone_R) // 如果斜边的平方 < 棋子圆形r的平方
           {
               return true;   // 在棋子的范围内
           }
       }
   }
   return false;  // 遍历整个棋盘后，点击点依然不在棋子的范围内
}

// 走棋规则
bool board::canmove_JIANG(int moveid, int row, int col, int killid)
{
    /*
        将 的移动范围是在九宫格内
        每次移动的长度是一格
    */

    // 将棋的行走向
    if(this->m_stone[moveid].isred) // 如果是红方
    {
        if(row > 2)  // 超出了红方的九宫格范围
        {
            return false;  // 返回false
        }
    }
    else  // 如果是黑方
    {
        if(row < 7)
        {
            return false;
        }
    }

    // 将棋的列走向
    if(col < 3)  // 列的九宫格不分红黑
    {
        return false;
    }
    if(col > 5)
    {
        return false;
    }

    /*
        算法参考：
        求点击的行列与棋子的行列的差值
        行和列的差值有一位必然等于1，代表是在一条直线上
        差值如果为-1，代表是走了一格
    */



    int distance_row = this->m_stone[moveid].row - row;
    int distance_col = this->m_stone[moveid].col - col;
    int distance = abs(distance_row) * 10 + abs(distance_col);
    if(distance == 1 || distance == 10)
    {
        return true;
    }
    return false;
}

bool board::canmove_SHI(int moveid, int row, int col, int killid)
{
    /*
        士 的移动范围是在九宫格内
        每次移动的长度是一格，但只能走对角线
    */

    // 士 棋的行走向
    if(this->m_stone[moveid].isred) // 如果是红方
    {
        if(row > 2)  // 超出了红方的九宫格范围
        {
            return false;  // 返回false
        }
    }
    else  // 如果是黑方
    {
        if(row < 7)
        {
            return false;
        }
    }

    // 士 棋的列走向
    if(col < 3)  // 列的九宫格不分红黑
    {
        return false;
    }
    if(col > 5)
    {
        return false;
    }

    int distance_row = this->m_stone[moveid].row - row;
    int distance_col = this->m_stone[moveid].col - col;
    int distance = abs(distance_row) * 10 + abs(distance_col);
    if(distance == 11)   // 只能走对角线
    {
        return true;
    }

    return false;
}

bool board::canmove_XIANG(int moveid, int row, int col, int killid)
{
    if(this->m_stone[moveid].isred)
    {
        if(row > 4)
        {
            return false;
        }
    }
    else
    {
        if(row < 5)
        {
            return false;
        }
    }

    int distance_row = abs(this->m_stone[moveid].row - row);
    int distance_col = abs(this->m_stone[moveid].col - col);
    if(distance_row == 2 && distance_col == 2)
    {
        int r = (this->m_stone[moveid].row + row)/2;
        int c = (this->m_stone[moveid].col + col)/2;
        if(hasStone(r,c))   // 判断是否有棋
        {
            return false;
        }
        return true;
    }
    return false;
}

bool board::canmove_CHE(int moveid, int row, int col, int killid)
{
    if(this->m_stone[moveid].row == row)
    {
        if(hasStone(this->m_stone[moveid].col,col,true,row,col))  // 是否有棋子
        {
            return false;
        }
        return true;
    }
    else if(this->m_stone[moveid].col == col)
    {
        if(hasStone(this->m_stone[moveid].row,row,false,row,col))
        {
           return false;
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool board::canmove_MA(int moveid, int row, int col, int killid)
{
    int distance_row = abs(this->m_stone[moveid].row - row);
    int distance_col = abs(this->m_stone[moveid].col - col);

    if((distance_row == 2 && distance_col == 1) || ( distance_row==1 && distance_col==2))
    {
        int r, c;

        if(distance_col==2)
        {
            r = this->m_stone[moveid].row;
            c = (this->m_stone[moveid].col + col) / 2;
        }
        else
        {
            r = (this->m_stone[moveid].row + row) / 2;
            c = this->m_stone[moveid].col;
        }
        if(hasStone(r,c))
        {
          return false;
        }

        return true;
    }
    return false;
}

bool board::canmove_PAO(int moveid, int row, int col, int killid)
{
    if(killid == -1)
    {
          return this->canmove_CHE(moveid,row,col,killid);
    }
    else
    {
        if(this->m_stone[moveid].row == row)
        {
            if(hasStone(this->m_stone[moveid].col, col,true,row,col) == 1)
            {
               return true;
            }
            return false;
        }
        else if(this->m_stone[moveid].col == col)
        {
            if(hasStone(this->m_stone[moveid].row, row,false,row,col) == 1)
            {
                return true;
            }
            return false;
        }
        else
        {
            return false;
        }
    }
}

bool board::canmove_BING(int moveid, int row, int col, int killid)
{
    if(this->m_stone[moveid].isred)
    {
        if(row < this->m_stone[moveid].row)
        {
            return false;
        }
        if(this->m_stone[moveid].row == 3 || this->m_stone[moveid].row == 4)
        {
            if(this->m_stone[moveid].col != col)
            {
                return false;
            }
        }
    }
    else
    {
        if(row > this->m_stone[moveid].row)
        {
            return false;
        }
        if(this->m_stone[moveid].row == 5 || this->m_stone[moveid].row == 6)
        {
            if(this->m_stone[moveid].col != col)
            {
               return false;
            }
        }
    }

    int distance_row = abs(this->m_stone[moveid].row - row);
    int distance_col = abs(this->m_stone[moveid].col - col);

    if(distance_row + distance_col == 1)
    {
        return true;
    }
    return false;
}



bool board::canmove(int moveid, int row, int col, int killid)
{
    if(killid == -1){}
    else
    {
        if(this->m_stone[moveid].isred == this->m_stone[killid].isred)  // 棋子颜色如果相同
        {
            this->m_recorder_id = killid;  // 选中棋子 = 点击项,不进行棋子覆盖
            update();
            return false;
        }
    }


     //各个角色的走棋规则
    switch (this->m_stone[moveid].stone_type) {
    case stone::JIANG:
        return this->canmove_JIANG(moveid,row,col,killid);
        break;
    case stone::SHI:
        return this->canmove_SHI(moveid,row,col,killid);
        break;
    case stone::XIANG:
        return this->canmove_XIANG(moveid,row,col,killid);
        break;
    case stone::CHE:
        return this->canmove_CHE(moveid,row,col,killid);
        break;
    case stone::MA:
        return this->canmove_MA(moveid,row,col,killid);
        break;
    case stone::PAO:
        return this->canmove_PAO(moveid,row,col,killid);
        break;
    case stone::BING:
        return this->canmove_BING(moveid,row,col,killid);
        break;
    }

    return true;
}


bool board::hasStone(int row, int col)
{
    for(int i = 0; i < 32; i++)
        if(this->m_stone[i].isdead == false && this->m_stone[i].row == row && this->m_stone[i].col == col)
            return true;
    return false;
}


int board::hasStone(int a, int b, bool bRow, int row,int col)
{
    int t = 0;
    int mn = qMin(a,b);
    int mx = qMax(a,b);
    for(int i = mn + 1; i < mx; i++)
    {
        if(bRow)  // 判断1行中是否有有气质
        {
            if(hasStone(row,i))
            {
               t++;
            }
        }
        else   // 判断1列中是否有棋子
        {
            if(hasStone(i,col))
            {
               t++;
            }
        }
    }
    return t;
}



void board::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint pix = event->pos();  // 设置像素点为鼠标点击位置

    int row, col;
    bool m_ret = this->getrowcol(pix,row,col);
    if(m_ret == false)
    {
        return;  // 不在范围内直接return
    }

    int m_click_id = -1; // 默认点击项为-1  此处要建立临时变量，方便结束函数时销毁

    for(int i = 0; i < 32; i++)  // 遍历32颗棋子
    {
        // 如果遍历后的棋子的行==所点击的行，列==所点击的列，并且棋子处于不死亡状态
        if(this->m_stone[i].row == row && this->m_stone[i].col == col && this->m_stone[i].isdead == false)
        {
            if(i < 32)
            {
                m_click_id = i;  // 记录点击项
            }
        }
    }


    if(this->m_recorder_id == -1)  // 如果没有棋子被选中
    {
        if(m_click_id != -1)  // 确认了点击项是正确的
        {
            if(this->isblackmove == this->m_stone[m_click_id].isred)  // 只有点击的棋子颜色与轮到你走棋时的颜色相同时,才会允许点击
            {
                this->m_recorder_id = m_click_id;  // 记录棋子 = 记录点击
                update(); // 重新调用paintEvent,重复显示棋子
            }
        }
    }
    else  // 有棋子被点击，进行移动操作
    {
        if(this->canmove(this->m_recorder_id,row,col,m_click_id))  // 先判断是否能走棋
        {
            this->m_stone[this->m_recorder_id].row = row;  // 棋子的行，列 置为 当前点击的行，列,实现移动
            this->m_stone[this->m_recorder_id].col = col;
            if(m_click_id != -1)
            {
                this->m_stone[m_click_id].isdead = true;  // 点击的棋子碰到了棋子，该棋子置为死亡状态
            }
            this->m_recorder_id = -1;
            this->isblackmove = !this->isblackmove;   // 走完棋之后，对isredmove，判断回合进行取反
            update();  // 重新调用paintEvent，死亡状态的棋子不应该显示
        }
    }
}




























