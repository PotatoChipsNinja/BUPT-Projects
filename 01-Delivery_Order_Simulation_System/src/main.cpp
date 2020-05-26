#include "header.hpp"

int main(void)
{
    INFO info;
    init(&info);

    pthread_t tids[3];
    pthread_create(&tids[0], NULL, timer, &info);
    pthread_create(&tids[1], NULL, guiDraw, &info);
    pthread_create(&tids[2], NULL, guiInput, &info);

    while(!info.state)
    {
        if(info.clock)
        {
            // printf("Now is %d\n", info.nowTime);
            info.clock = false;
            clearFinishList(&info);

            info.newRider = false;

            for(int i = 0; i < 9; ++i)
                for(int j = 0; j < 9; ++j)
                    info.cellInfo[i][j] = false;

            // 检查预期金额，招募骑手
            for(int i = info.exptMoney / RIDER_PRICE; i > 0; --i)
            {
                addRider(&info);
                printf("Added rider\n");
            }

            // 检查候选订单，派给骑手
            allocateOrder(&info);

            // 遍历骑手
            for(RIDER *pCurr = info.riderList; pCurr->next; pCurr = pCurr->next)
            {
                // 获取目的地
                ORDER *pNearestOrder = getNextDest(pCurr->next, &info);

                // 向目的地移动
                if(pNearestOrder)
                {
                    if(pNearestOrder->hasPicked)
                        move(pCurr->next, pNearestOrder->toX, pNearestOrder->toY);
                    else
                        move(pCurr->next, pNearestOrder->fromX, pNearestOrder->fromY);
                }
                else if(!isAdjacent(pCurr->next->nowX, pCurr->next->nowY, 8, 8))
                    move(pCurr->next, 8, 8);      // 该骑手空闲，向中心移动

                // 检查停靠情况
                check(pCurr->next, &info);
            }

            // 检查破产
            if(info.realMoney < 0)
                info.state = 2;

            // 检查订单全部完成
            if(info.inputMode && info.numFinished == info.numReceived && !info.savedOrderList->next)
                info.state = 3;

            fileOutput(&info);      // 文件输出
            calcExptMoney(&info);   // 更新预期金额
            info.nowTime++;
            info.draw = 1;
        }
    }

    // 结算
    if(info.state == 1)
        printf("Lost license!\n");
    else if(info.state == 2)
        printf("Go broke!\n");
    else
        printf("All finished!\n");

    getchar();

    pthread_join(tids[0], NULL);
    pthread_join(tids[1], NULL);
    pthread_join(tids[2], NULL);

    return 0;
}

