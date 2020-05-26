#include "header.hpp"

// 程序初始化
void init(INFO *info)
{
    // 数据初始化
    info->nowTime = info->numReceived = info->numFinished = info->numTimeOut = info->numRider = info->state = 0;
    info->clock = true;
    info->draw = false;
    info->newRider = false;
    info->realMoney = info->exptMoney = INIT_MONEY;

    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            info->cellInfo[i][j] = false;

    info->finishList = (ORDER *)malloc(sizeof(ORDER));
    info->finishList->next = NULL;

    info->timeOutList = (ORDER *)malloc(sizeof(ORDER));
    info->timeOutList->next = NULL;

    info->savedOrderList = (ORDER *)malloc(sizeof(ORDER));
    info->savedOrderList->next = NULL;

    info->riderList = (RIDER *)malloc(sizeof(RIDER));
    info->riderList->riderId = -1;
    info->riderList->next = NULL;

    info->restaurant[0] = -1;
    info->client[0] = -1;
    info->lastOrderId = 0;

    info->inputMode = fileInput(info);      // 从文件读入候选订单
    fileClearOutput();

    initgraph(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 350, 2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH);    // 图形界面初始化
    setbkcolor(EGERGB(0xf0, 0xf0, 0xf0));

    for(int i = 0; i < 8; ++i)
        info->img[i] = newimage();
    getimage(info->img[0], "img/time.png", 0, 0);
    getimage(info->img[1], "img/money.png", 0, 0);
    getimage(info->img[2], "img/finish.png", 0, 0);
    getimage(info->img[3], "img/timeout.png", 0, 0);
    getimage(info->img[4], "img/house.png", 0, 0);
    getimage(info->img[5], "img/moto.png", 0, 0);
    getimage(info->img[6], "img/rest.png", 0, 0);
    getimage(info->img[7], "img/client.png", 0, 0);
}

// 判断(ax, ay)与(bx, by)是否相邻
bool isAdjacent(int ax, int ay, int bx, int by)
{
    return (abs(ax-bx) + abs(ay-by)) == 1;
}

// 判断是否在同一行或列 
int SameLine(int ax, int ay, int bx, int by)
{
	return ax == bx || ay == by ? 1 : 0;
}

// 计算(ax, ay)与(bx, by)间的距离
int calcDist(int ax, int ay, int bx, int by)
{
	return (abs(ax - bx) + abs(ay - by))/2 + SameLine(ax,ay,bx,by);
}

// 将订单加入当前罚单
void addTimeOutOrder(ORDER *order, INFO *info)
{
    ORDER *pCurr = info->timeOutList;
    while(pCurr->next)
        pCurr = pCurr->next;
    pCurr->next = (ORDER *)malloc(sizeof(ORDER));
    pCurr->next->orderId = order->orderId;      // 只需复制订单号
    pCurr->next->next = NULL;
}

// 清空结单列表
void clearFinishList(INFO *info)
{
    ORDER *pTemp;
    while(info->finishList->next)
    {
        pTemp = info->finishList->next;
        info->finishList->next = pTemp->next;
        free(pTemp);
    }
}

// 更新预期金额
// 测试用，赋实际金额
void calcExptMoney(INFO *info)
{
    info->exptMoney = info->realMoney - 50 ;
}

