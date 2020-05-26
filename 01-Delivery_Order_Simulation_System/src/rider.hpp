#include "header.hpp"

void move(RIDER *rider, int targetX, int targetY)
{
	if(isAdjacent(rider->nowX,rider->nowY,targetX,targetY))return ;
    if(rider->nowX % 2)
    {
        // 骑手在纵列
        if(abs(rider->nowY - targetY) > 2)
        {
            rider->nowY += rider->nowY > targetY ? -2 : 2;
        }
        else
        {
            if(rider->nowY == 0)
                rider->nowY++;
            else if(rider->nowY == 16)
                rider->nowY--;
            else
                rider->nowY += rider->nowY > targetY ? -1 : 1;
            rider->nowX += rider->nowX > targetX ? -1 : 1;
        }
    }
    else
    {
        // 骑手在横行
        if(abs(rider->nowX - targetX) > 2)
        {
            rider->nowX += rider->nowX > targetX ? -2 : 2;
        }
        else
        {
            if(rider->nowX == 0)
                rider->nowX++;
            else if(rider->nowX == 16)
                rider->nowX--;
            else
                rider->nowX += rider->nowX > targetX ? -1 : 1;
            rider->nowY += rider->nowY > targetY ? -1 : 1;
        }
    }
}

// 检查该骑手的停靠、罚款、破产状况
void check(RIDER *rider, INFO *info)
{
	ORDER *pTemp;
    int flag;       // 控制pCurr是否向前移动
    for(ORDER *pCurr = rider->orderList; pCurr->next; pCurr = flag ? pCurr->next : pCurr)
    {
        flag = 1;
        if(!pCurr->next->hasPicked)
        {
            // 检查是否停靠在餐馆
            if(isAdjacent(rider->nowX, rider->nowY, pCurr->next->fromX, pCurr->next->fromY))
            {
                if(rider->stopInfo[0][0] == 0)
                {
                    rider->stopInfo[0][0] = 1;
                    rider->stopInfo[0][1] = pCurr->next->fromX;
                    rider->stopInfo[0][2] = pCurr->next->fromY;
                }
                else if(rider->stopInfo[0][1] == pCurr->next->fromX && rider->stopInfo[0][2] == pCurr->next->fromY)
                {
                    if(rider->stopInfo[0][0] == 2)
                        rider->stopInfo[0][0] = 3;
                }
                else
                {
                    if(rider->stopInfo[1][0] == 0)
                    {
                        rider->stopInfo[1][0] = 1;
                        rider->stopInfo[1][1] = pCurr->next->fromX;
                        rider->stopInfo[1][2] = pCurr->next->fromY;
                    }
                    else if(rider->stopInfo[1][0] == 2)
                        rider->stopInfo[1][0] = 3;
                }

                pCurr->next->hasPicked = true;
            }
        }
        else
        {
            // 检查是否停靠在食客
            if(isAdjacent(rider->nowX, rider->nowY, pCurr->next->toX, pCurr->next->toY))
            {
                info->realMoney += INCOME;
                info->numFinished++;

                if(rider->stopInfo[0][0] == 0)
                {
                    rider->stopInfo[0][0] = 2;
                    rider->stopInfo[0][1] = pCurr->next->toX;
                    rider->stopInfo[0][2] = pCurr->next->toY;
                }
                else if(rider->stopInfo[0][1] == pCurr->next->toX && rider->stopInfo[0][2] == pCurr->next->toY)
                {
                    if(rider->stopInfo[0][0] == 1)
                        rider->stopInfo[0][0] = 3;
                }
                else
                {
                    if(rider->stopInfo[1][0] == 0)
                    {
                        rider->stopInfo[1][0] = 2;
                        rider->stopInfo[1][1] = pCurr->next->toX;
                        rider->stopInfo[1][2] = pCurr->next->toY;
                    }
                    else if(rider->stopInfo[1][0] == 1)
                        rider->stopInfo[1][0] = 3;
                }

                // 将订单移送至当前结单
                flag = 0;
                pTemp = info->finishList->next;
                info->finishList->next = pCurr->next;
                pCurr->next = pCurr->next->next;
                info->finishList->next->next = pTemp;
                continue;
            }
        }

        // 检查订单是否超时
        if(info->nowTime > pCurr->next->orderTime + TIME_OUT_LIMIT)
        {
            info->realMoney -= FINE;
            info->numTimeOut++;

            // 将订单加入当前罚单
            addTimeOutOrder(pCurr->next, info);

            // 将订单移送至已超时待送订单列表
            flag = 0;
            pTemp = rider->timeOutList->next;
            rider->timeOutList->next = pCurr->next;
            pCurr->next = pCurr->next->next;
            rider->timeOutList->next->next = pTemp;
        }
    }

    for(ORDER *pCurr = rider->timeOutList; pCurr->next; pCurr = flag ? pCurr->next : pCurr)
    {
        flag = 1;
        if(!pCurr->next->hasPicked)
        {
            // 检查是否停靠在餐馆
            if(isAdjacent(rider->nowX, rider->nowY, pCurr->next->fromX, pCurr->next->fromY))
            {
                if(rider->stopInfo[0][0] == 0)
                {
                    rider->stopInfo[0][0] = 1;
                    rider->stopInfo[0][1] = pCurr->next->fromX;
                    rider->stopInfo[0][2] = pCurr->next->fromY;
                }
                else if(rider->stopInfo[0][1] == pCurr->next->fromX && rider->stopInfo[0][2] == pCurr->next->fromY)
                {
                    if(rider->stopInfo[0][0] == 2)
                        rider->stopInfo[0][0] = 3;
                }
                else
                {
                    if(rider->stopInfo[1][0] == 0)
                    {
                        rider->stopInfo[1][0] = 1;
                        rider->stopInfo[1][1] = pCurr->next->fromX;
                        rider->stopInfo[1][2] = pCurr->next->fromY;
                    }
                    else if(rider->stopInfo[1][0] == 2)
                        rider->stopInfo[1][0] = 3;
                }

                pCurr->next->hasPicked = true;
            }
        }
        else
        {
            // 检查是否停靠在食客
            if(isAdjacent(rider->nowX, rider->nowY, pCurr->next->toX, pCurr->next->toY))
            {
                info->numFinished++;

                if(rider->stopInfo[0][0] == 0)
                {
                    rider->stopInfo[0][0] = 2;
                    rider->stopInfo[0][1] = pCurr->next->toX;
                    rider->stopInfo[0][2] = pCurr->next->toY;
                }
                else if(rider->stopInfo[0][1] == pCurr->next->toX && rider->stopInfo[0][2] == pCurr->next->toY)
                {
                    if(rider->stopInfo[0][0] == 1)
                        rider->stopInfo[0][0] = 3;
                }
                else
                {
                    if(rider->stopInfo[1][0] == 0)
                    {
                        rider->stopInfo[1][0] = 2;
                        rider->stopInfo[1][1] = pCurr->next->toX;
                        rider->stopInfo[1][2] = pCurr->next->toY;
                    }
                    else if(rider->stopInfo[1][0] == 1)
                        rider->stopInfo[1][0] = 3;
                }

                // 将订单移送至当前结单
                flag = 0;
                pTemp = info->finishList->next;
                info->finishList->next = pCurr->next;
                pCurr->next = pCurr->next->next;
                info->finishList->next->next = pTemp;
                continue;
            }
        }

        // 检查订单是否废单
        if(info->nowTime > pCurr->next->orderTime + BANKRUPT_LIMIT)
        {
            info->state = 2;        // 破产
            return;
        }
    }

}

// 招募新骑手
void addRider(INFO *info)
{
    info->realMoney -= RIDER_PRICE;

    RIDER *pCurrRider = info->riderList;
    while(pCurrRider->next)
        pCurrRider = pCurrRider->next;
    pCurrRider->next = (RIDER *)malloc(sizeof(RIDER));
    pCurrRider->next->riderId = pCurrRider->riderId + 1;
    pCurrRider = pCurrRider->next;

    pCurrRider->nowX = INIT_X;
    pCurrRider->nowY = INIT_Y;

    pCurrRider->stopInfo[0][0] = pCurrRider->stopInfo[1][0] = 0;

    pCurrRider->orderList = (ORDER *)malloc(sizeof(ORDER));
    pCurrRider->orderList->next = NULL;

    pCurrRider->timeOutList = (ORDER *)malloc(sizeof(ORDER));
    pCurrRider->timeOutList->next = NULL;

    pCurrRider->next = NULL;
    info->numRider++;

    info->newRider = true;
}

//********************************************
ORDER* getNextDest(RIDER* rider, INFO* info)
{
	int dist, minDist = DIST_MAX, flag = 0;
	ORDER* pOrder = NULL, * pOrder1 = NULL, * pOrder2 = NULL;

	for (ORDER* pCurr = rider->orderList; pCurr->next; pCurr = pCurr->next)
	{//遍历该骑手的未超时待送订单信息，优先派送此刻派送刚好不超时到达的订单，若不存在则优先派送路径最短的作为目的地
		if (pCurr->next->hasPicked)//若订单已取餐则目的地为送餐地点
			dist = calcDist(pCurr->next->toX, pCurr->next->toY, rider->nowX, rider->nowY);
		else//若订单未取餐应先到达餐馆取餐
			dist = calcDist(pCurr->next->fromX, pCurr->next->fromY, rider->nowX, rider->nowY);
		if (dist + info->nowTime - pCurr->next->orderTime == TIME_OUT_LIMIT)
		{
			flag = 1;//一个标志表示存在此刻派送刚好不超时到达的未超时订单
			pOrder1 = pCurr->next;
			break;
		}
		if (dist < minDist)
		{//更新最短距离和最短距离的订单
			minDist = dist;
			pOrder = pCurr->next;
		}
	}

	for (ORDER* pCurr = rider->timeOutList; pCurr->next; pCurr = pCurr->next)//更新目的地，遍历该骑手的已超时待送订单信息，计算出下一路径最短的作为目的地
	{
		if (pCurr->next->hasPicked)
			dist = calcDist(pCurr->next->toX, pCurr->next->toY, rider->nowX, rider->nowY);
		else
			dist = calcDist(pCurr->next->fromX, pCurr->next->fromY, rider->nowX, rider->nowY);
		if (dist + info->nowTime - pCurr->next->orderTime == BANKRUPT_LIMIT)
		{
			flag = 2;//一个标志表示存在此刻派送刚好不吊销执照到达的已超时订单
			pOrder2 = pCurr->next;
			break;
		}
		if (dist < minDist)
		{
			minDist = dist;
			pOrder = pCurr->next;
		}
	}

	if (flag == 0) return pOrder;
	else if (flag == 1) return pOrder1;
	else return pOrder2;
}

// 建立链表 
ORDER* CreateFakeOrder(ORDER* newOrder, ORDER dataOrder)
{
	if (!newOrder)
	{
		newOrder = (ORDER*)malloc(sizeof(ORDER));
		*newOrder = dataOrder;
		newOrder->next = NULL;
	}
	else
	{
		newOrder->next = CreateFakeOrder(newOrder->next, dataOrder);
	}
	return newOrder;
}

//释放虚拟链表储存空间 
void ReleaseOrderList(ORDER* orderList)
{
	ORDER* temp;
	temp = orderList;
	while (orderList)
	{
		orderList = orderList->next;
		free(temp);
		temp = orderList;
	}
}

//虚拟遍历所有数据 
//找到将新单子插入骑手单子链表中跑完所有单子超时单数最少的情况下用时最少的骑手 
int FindBestRiderId(INFO info)
{
	int mintime = INF, mintimeout = INF, bestRiderId;
	INFO* fakeInfo;
	fakeInfo = (INFO*)malloc(sizeof(INFO));
	*fakeInfo = info;
	RIDER* prider,*fakeRider,*createRider;
	createRider=(RIDER*)malloc(sizeof(RIDER));
	fakeRider = fakeInfo->riderList->next;
	
	while (fakeRider)
	{
		*createRider = *fakeRider;
		prider = createRider;
		ORDER* fakeOrderList = NULL, * tempOrder,* pTemp;
		tempOrder = prider->orderList;
		//建造新的单子链表
		while (tempOrder)
		{
			fakeOrderList = CreateFakeOrder(fakeOrderList, *tempOrder);
			tempOrder = tempOrder->next;
		}
		//将单子直接分配给骑手
		pTemp = fakeInfo->savedOrderList->next;
		fakeOrderList = CreateFakeOrder(fakeOrderList, *pTemp);
		prider->orderList = fakeOrderList;
		//建造新的超时单子链表
		fakeOrderList = NULL;
		tempOrder = prider->timeOutList;
		while (tempOrder)
		{
			fakeOrderList = CreateFakeOrder(fakeOrderList, *tempOrder);
			tempOrder = tempOrder->next;
		}
		prider->timeOutList = fakeOrderList;
		// 获取目的地
		ORDER* pNearestOrder = getNextDest(prider, fakeInfo);
		while (pNearestOrder)
		{
			// 向目的地移动
			if (pNearestOrder->hasPicked)
			{
				move(prider, pNearestOrder->toX, pNearestOrder->toY);
			}
			else
			{
				move(prider, pNearestOrder->fromX, pNearestOrder->fromY);
			}
			int flag;       // 控制pCurr是否向前移动
			for (ORDER* pCurr = prider->orderList; pCurr->next; pCurr = flag ? pCurr->next : pCurr)
			{
				flag = 1;
				if (!pCurr->next->hasPicked)
				{
					// 检查是否停靠在餐馆
					if (isAdjacent(prider->nowX, prider->nowY, pCurr->next->fromX, pCurr->next->fromY))
					{
						pCurr->next->hasPicked = true;
					}
				}
				else
				{
					// 检查是否停靠在食客
					if (isAdjacent(prider->nowX, prider->nowY, pCurr->next->toX, pCurr->next->toY))
					{
						flag = 0;
						pTemp = pCurr->next;
						pCurr->next = pCurr->next->next;
						free(pTemp);
						continue;
					}
				}

				// 检查订单是否超时
				if (fakeInfo->nowTime > pCurr->next->orderTime + TIME_OUT_LIMIT)
				{
					fakeInfo->numTimeOut++;

					// 将订单移送至已超时待送订单列表
					flag = 0;
					pTemp = prider->timeOutList->next;
					prider->timeOutList->next = pCurr->next;
					pCurr->next = pCurr->next->next;
					prider->timeOutList->next->next = pTemp;
				}
			}

			for (ORDER* pCurr = prider->timeOutList; pCurr->next; pCurr = flag ? pCurr->next : pCurr)
			{
				flag = 1;
				if (!pCurr->next->hasPicked)
				{
					// 检查是否停靠在餐馆
					if (isAdjacent(prider->nowX, prider->nowY, pCurr->next->fromX, pCurr->next->fromY))
					{
						pCurr->next->hasPicked = true;
					}
				}
				else
				{
					// 检查是否停靠在食客
					if (isAdjacent(prider->nowX, prider->nowY, pCurr->next->toX, pCurr->next->toY))
					{
						// 将订单移送至当前结单
						flag = 0;
						pTemp = pCurr->next;
						pCurr->next = pCurr->next->next;
						free(pTemp);
						continue;
					}
				}
			}
			//手动更新时间
			fakeInfo->nowTime++;
			//找到下一个地址
			pNearestOrder = getNextDest(prider, fakeInfo);
		}
		//更新最少的超时数和用时
		if (mintimeout > fakeInfo->numTimeOut)
		{
			mintimeout = fakeInfo->numTimeOut;
			mintime = fakeInfo->nowTime;
			bestRiderId = prider->riderId;
		}
		//超时数相同更新最短用时
		else if (mintimeout == fakeInfo->numTimeOut && fakeInfo->nowTime < mintime)
		{
			mintime = fakeInfo->nowTime;
			bestRiderId = prider->riderId;
		}
		*fakeInfo = info;
		//释放空间
		ReleaseOrderList(prider->orderList);
		ReleaseOrderList(prider->timeOutList);
		fakeRider = fakeRider->next;
	}
	free(fakeInfo);
	free(createRider);
	return bestRiderId;//返回最佳骑手ID
}



// 将订单分配给骑手
// 分配给最佳骑手
void allocateOrder(INFO * info)
{
	int minDist, dist;
	RIDER* pRider = NULL;
	ORDER* pTemp;
	while (info->savedOrderList->next && info->savedOrderList->next->orderTime == info->nowTime)
	{
		info->numReceived++;
		if (!info->riderList->next)
		{
			info->state = 1;        // 无骑手可分配，被吊销营业执照
			return;
		}
	    int bestRider = FindBestRiderId(*info);			//寻找最佳骑手ID 
		RIDER* pCurr = info->riderList;
		while ( pCurr->next->riderId!=bestRider)		// 根据ID找到最佳骑手的地址 
		{
			pCurr = pCurr->next;
		}
		pRider=pCurr->next;
		//更新骑手已接订单数；
		pRider->numReceived++;
		// 将订单分配给骑手
		pTemp = pRider->orderList->next;
		pRider->orderList->next = info->savedOrderList->next;
		info->savedOrderList->next = info->savedOrderList->next->next;
		pRider->orderList->next->next = pTemp;
	}
}

