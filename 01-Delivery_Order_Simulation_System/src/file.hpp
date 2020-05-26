#include "header.hpp"

bool fileInput(INFO *info)
{
    FILE *fp = fopen(IN_FILE, "r");     // 尝试打开文件
    if(!fp)
        return false;     // 文件打开失败

    ORDER *pCurr = info->savedOrderList;
    ORDER temp;
    temp.next = NULL;
    temp.hasPicked = false;
    while(fscanf(fp, "%d%d%d%d%d%d", &temp.orderId, &temp.orderTime, &temp.fromX, &temp.fromY, &temp.toX, &temp.toY) != EOF)
    {
        // 将订单加入候选订单列表
        pCurr->next = (ORDER *)malloc(sizeof(ORDER));
        pCurr = pCurr->next;
        *pCurr = temp;
    }
    fclose(fp);
    return true;
}

void fileClearOutput(void)
{
    FILE *fp = fopen(OUT_FILE, "w");     // 尝试打开文件
    if(!fp)
        return;     // 文件打开失败
    fclose(fp);
}

void fileOutput(INFO *info)
{
    FILE *fp = fopen(OUT_FILE, "a");     // 尝试打开文件
    if(!fp)
        return;     // 文件打开失败

    fprintf(fp, "时间:%d\n钱:%d\n接单数:%d\n完成数:%d;结单:", info->nowTime, info->realMoney, info->numReceived, info->numFinished);

    // 打印结单
    for(ORDER *pCurr = info->finishList->next; pCurr; pCurr = pCurr->next)
    {
        fprintf(fp, "%d", pCurr->orderId);
        if(pCurr->next)
            fprintf(fp, " ");
    }
    fprintf(fp, ";\n超时数:%d;罚单:", info->numTimeOut);

    // 打印罚单
    for(ORDER *pCurr = info->timeOutList; pCurr->next; pCurr = pCurr->next)
    {
        fprintf(fp, "%d", pCurr->next->orderId);
        if(pCurr->next->next)
            fprintf(fp, " ");
    }
    fprintf(fp, ";\n");

    // 打印骑手
    for(RIDER *pCurr = info->riderList; pCurr->next; pCurr = pCurr->next)
    {
        fprintf(fp, "骑手%d位置:%d,%d;停靠:", pCurr->next->riderId, pCurr->next->nowX, pCurr->next->nowY);
        // 打印停靠信息
        if(pCurr->next->stopInfo[0][0])
        {
            switch(pCurr->next->stopInfo[0][0])
            {
                case 1:
                    fprintf(fp, "餐馆");
                    break;
                case 2:
                    fprintf(fp, "食客");
                    break;
                case 3:
                    fprintf(fp, "餐客");
                    break;
            }
            fprintf(fp, " %d %d", pCurr->next->stopInfo[0][1], pCurr->next->stopInfo[0][2]);
            info->cellInfo[pCurr->next->stopInfo[0][1] / 2][pCurr->next->stopInfo[0][2] / 2] = true;
        }
        if(pCurr->next->stopInfo[1][0])
        {
            fprintf(fp, " ");
            switch(pCurr->next->stopInfo[1][0])
            {
                case 1:
                    fprintf(fp, "餐馆");
                    break;
                case 2:
                    fprintf(fp, "食客");
                    break;
                case 3:
                    fprintf(fp, "餐客");
                    break;
            }
            fprintf(fp, " %d %d", pCurr->next->stopInfo[1][1], pCurr->next->stopInfo[1][2]);
            info->cellInfo[pCurr->next->stopInfo[1][1] / 2][pCurr->next->stopInfo[1][2] / 2] = true;
        }
        pCurr->next->stopInfo[0][0] = pCurr->next->stopInfo[1][0] = 0;
        fprintf(fp, ";\n");
    }

    fclose(fp);
}

