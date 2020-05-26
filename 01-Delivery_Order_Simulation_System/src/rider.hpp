#include "header.hpp"

void move(RIDER *rider, int targetX, int targetY)
{
	if(isAdjacent(rider->nowX,rider->nowY,targetX,targetY))return ;
    if(rider->nowX % 2)
    {
        // ����������
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
        // �����ں���
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

// �������ֵ�ͣ��������Ʋ�״��
void check(RIDER *rider, INFO *info)
{
	ORDER *pTemp;
    int flag;       // ����pCurr�Ƿ���ǰ�ƶ�
    for(ORDER *pCurr = rider->orderList; pCurr->next; pCurr = flag ? pCurr->next : pCurr)
    {
        flag = 1;
        if(!pCurr->next->hasPicked)
        {
            // ����Ƿ�ͣ���ڲ͹�
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
            // ����Ƿ�ͣ����ʳ��
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

                // ��������������ǰ�ᵥ
                flag = 0;
                pTemp = info->finishList->next;
                info->finishList->next = pCurr->next;
                pCurr->next = pCurr->next->next;
                info->finishList->next->next = pTemp;
                continue;
            }
        }

        // ��鶩���Ƿ�ʱ
        if(info->nowTime > pCurr->next->orderTime + TIME_OUT_LIMIT)
        {
            info->realMoney -= FINE;
            info->numTimeOut++;

            // ���������뵱ǰ����
            addTimeOutOrder(pCurr->next, info);

            // �������������ѳ�ʱ���Ͷ����б�
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
            // ����Ƿ�ͣ���ڲ͹�
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
            // ����Ƿ�ͣ����ʳ��
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

                // ��������������ǰ�ᵥ
                flag = 0;
                pTemp = info->finishList->next;
                info->finishList->next = pCurr->next;
                pCurr->next = pCurr->next->next;
                info->finishList->next->next = pTemp;
                continue;
            }
        }

        // ��鶩���Ƿ�ϵ�
        if(info->nowTime > pCurr->next->orderTime + BANKRUPT_LIMIT)
        {
            info->state = 2;        // �Ʋ�
            return;
        }
    }

}

// ��ļ������
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
	{//���������ֵ�δ��ʱ���Ͷ�����Ϣ���������ʹ˿����͸պò���ʱ����Ķ�����������������������·����̵���ΪĿ�ĵ�
		if (pCurr->next->hasPicked)//��������ȡ����Ŀ�ĵ�Ϊ�Ͳ͵ص�
			dist = calcDist(pCurr->next->toX, pCurr->next->toY, rider->nowX, rider->nowY);
		else//������δȡ��Ӧ�ȵ���͹�ȡ��
			dist = calcDist(pCurr->next->fromX, pCurr->next->fromY, rider->nowX, rider->nowY);
		if (dist + info->nowTime - pCurr->next->orderTime == TIME_OUT_LIMIT)
		{
			flag = 1;//һ����־��ʾ���ڴ˿����͸պò���ʱ�����δ��ʱ����
			pOrder1 = pCurr->next;
			break;
		}
		if (dist < minDist)
		{//������̾������̾���Ķ���
			minDist = dist;
			pOrder = pCurr->next;
		}
	}

	for (ORDER* pCurr = rider->timeOutList; pCurr->next; pCurr = pCurr->next)//����Ŀ�ĵأ����������ֵ��ѳ�ʱ���Ͷ�����Ϣ���������һ·����̵���ΪĿ�ĵ�
	{
		if (pCurr->next->hasPicked)
			dist = calcDist(pCurr->next->toX, pCurr->next->toY, rider->nowX, rider->nowY);
		else
			dist = calcDist(pCurr->next->fromX, pCurr->next->fromY, rider->nowX, rider->nowY);
		if (dist + info->nowTime - pCurr->next->orderTime == BANKRUPT_LIMIT)
		{
			flag = 2;//һ����־��ʾ���ڴ˿����͸պò�����ִ�յ�����ѳ�ʱ����
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

// �������� 
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

//�ͷ�����������ռ� 
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

//��������������� 
//�ҵ����µ��Ӳ������ֵ����������������е��ӳ�ʱ�������ٵ��������ʱ���ٵ����� 
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
		//�����µĵ�������
		while (tempOrder)
		{
			fakeOrderList = CreateFakeOrder(fakeOrderList, *tempOrder);
			tempOrder = tempOrder->next;
		}
		//������ֱ�ӷ��������
		pTemp = fakeInfo->savedOrderList->next;
		fakeOrderList = CreateFakeOrder(fakeOrderList, *pTemp);
		prider->orderList = fakeOrderList;
		//�����µĳ�ʱ��������
		fakeOrderList = NULL;
		tempOrder = prider->timeOutList;
		while (tempOrder)
		{
			fakeOrderList = CreateFakeOrder(fakeOrderList, *tempOrder);
			tempOrder = tempOrder->next;
		}
		prider->timeOutList = fakeOrderList;
		// ��ȡĿ�ĵ�
		ORDER* pNearestOrder = getNextDest(prider, fakeInfo);
		while (pNearestOrder)
		{
			// ��Ŀ�ĵ��ƶ�
			if (pNearestOrder->hasPicked)
			{
				move(prider, pNearestOrder->toX, pNearestOrder->toY);
			}
			else
			{
				move(prider, pNearestOrder->fromX, pNearestOrder->fromY);
			}
			int flag;       // ����pCurr�Ƿ���ǰ�ƶ�
			for (ORDER* pCurr = prider->orderList; pCurr->next; pCurr = flag ? pCurr->next : pCurr)
			{
				flag = 1;
				if (!pCurr->next->hasPicked)
				{
					// ����Ƿ�ͣ���ڲ͹�
					if (isAdjacent(prider->nowX, prider->nowY, pCurr->next->fromX, pCurr->next->fromY))
					{
						pCurr->next->hasPicked = true;
					}
				}
				else
				{
					// ����Ƿ�ͣ����ʳ��
					if (isAdjacent(prider->nowX, prider->nowY, pCurr->next->toX, pCurr->next->toY))
					{
						flag = 0;
						pTemp = pCurr->next;
						pCurr->next = pCurr->next->next;
						free(pTemp);
						continue;
					}
				}

				// ��鶩���Ƿ�ʱ
				if (fakeInfo->nowTime > pCurr->next->orderTime + TIME_OUT_LIMIT)
				{
					fakeInfo->numTimeOut++;

					// �������������ѳ�ʱ���Ͷ����б�
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
					// ����Ƿ�ͣ���ڲ͹�
					if (isAdjacent(prider->nowX, prider->nowY, pCurr->next->fromX, pCurr->next->fromY))
					{
						pCurr->next->hasPicked = true;
					}
				}
				else
				{
					// ����Ƿ�ͣ����ʳ��
					if (isAdjacent(prider->nowX, prider->nowY, pCurr->next->toX, pCurr->next->toY))
					{
						// ��������������ǰ�ᵥ
						flag = 0;
						pTemp = pCurr->next;
						pCurr->next = pCurr->next->next;
						free(pTemp);
						continue;
					}
				}
			}
			//�ֶ�����ʱ��
			fakeInfo->nowTime++;
			//�ҵ���һ����ַ
			pNearestOrder = getNextDest(prider, fakeInfo);
		}
		//�������ٵĳ�ʱ������ʱ
		if (mintimeout > fakeInfo->numTimeOut)
		{
			mintimeout = fakeInfo->numTimeOut;
			mintime = fakeInfo->nowTime;
			bestRiderId = prider->riderId;
		}
		//��ʱ����ͬ���������ʱ
		else if (mintimeout == fakeInfo->numTimeOut && fakeInfo->nowTime < mintime)
		{
			mintime = fakeInfo->nowTime;
			bestRiderId = prider->riderId;
		}
		*fakeInfo = info;
		//�ͷſռ�
		ReleaseOrderList(prider->orderList);
		ReleaseOrderList(prider->timeOutList);
		fakeRider = fakeRider->next;
	}
	free(fakeInfo);
	free(createRider);
	return bestRiderId;//�����������ID
}



// ���������������
// ������������
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
			info->state = 1;        // �����ֿɷ��䣬������Ӫҵִ��
			return;
		}
	    int bestRider = FindBestRiderId(*info);			//Ѱ���������ID 
		RIDER* pCurr = info->riderList;
		while ( pCurr->next->riderId!=bestRider)		// ����ID�ҵ�������ֵĵ�ַ 
		{
			pCurr = pCurr->next;
		}
		pRider=pCurr->next;
		//���������ѽӶ�������
		pRider->numReceived++;
		// ���������������
		pTemp = pRider->orderList->next;
		pRider->orderList->next = info->savedOrderList->next;
		info->savedOrderList->next = info->savedOrderList->next->next;
		pRider->orderList->next->next = pTemp;
	}
}

