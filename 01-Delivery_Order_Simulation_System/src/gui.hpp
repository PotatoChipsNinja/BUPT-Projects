#include "header.hpp"

// ������ೡ��
void drawScene(INFO *info)
{
    setcolor(BLACK);
    setfillcolor(WHITE);
    rectangle(EDGE_WIDTH - 1, EDGE_WIDTH - 1, EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 1, EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 1);


    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
        {
            if(info->restaurant[0] != -1 && info->restaurant[0] / 2 == i && info->restaurant[1] / 2 == j)
                putimage(EDGE_WIDTH + i * (CELL_SIZE + STREET_WIDTH), EDGE_WIDTH + j * (CELL_SIZE + STREET_WIDTH), info->img[6]);
            else if(info->client[0] != -1 && info->client[0] / 2 == i && info->client[1] / 2 == j)
                putimage(EDGE_WIDTH + i * (CELL_SIZE + STREET_WIDTH), EDGE_WIDTH + j * (CELL_SIZE + STREET_WIDTH), info->img[7]);
            else
                putimage(EDGE_WIDTH + i * (CELL_SIZE + STREET_WIDTH), EDGE_WIDTH + j * (CELL_SIZE + STREET_WIDTH), info->img[4]);
            
            if(info->cellInfo[i][j])
                rectangle(EDGE_WIDTH + i * (CELL_SIZE + STREET_WIDTH), EDGE_WIDTH + j * (CELL_SIZE + STREET_WIDTH), EDGE_WIDTH + i * (CELL_SIZE + STREET_WIDTH) + CELL_SIZE, EDGE_WIDTH + j * (CELL_SIZE + STREET_WIDTH) + CELL_SIZE);
        }

    char str[10];
    setfont(14, 0, "����");
    setfillcolor(RED);
    for(RIDER *pRider = info->riderList->next; pRider; pRider = pRider->next)
    {
        for(ORDER *pOrder = pRider->orderList->next; pOrder; pOrder = pOrder->next)
        {
            // ��ӡA
            if(!pOrder->hasPicked)
            {
                itoa(pOrder->orderId, str, 10);
                strcat(str, "A");
                outtextxy(EDGE_WIDTH + pOrder->fromX / 2 * (CELL_SIZE + STREET_WIDTH) + 12, EDGE_WIDTH + pOrder->fromY / 2 * (CELL_SIZE + STREET_WIDTH) + 15, str);
            }
            // ��ӡB
            itoa(pOrder->orderId, str, 10);
            strcat(str, "B");
            outtextxy(EDGE_WIDTH + pOrder->toX / 2 * (CELL_SIZE + STREET_WIDTH) + 12, EDGE_WIDTH + pOrder->toY / 2 * (CELL_SIZE + STREET_WIDTH) + 15, str);
        }
        for(ORDER *pOrder = pRider->timeOutList->next; pOrder; pOrder = pOrder->next)
        {
            // ��ӡA
            if(!pOrder->hasPicked)
            {
                itoa(pOrder->orderId, str, 10);
                strcat(str, "A");
                outtextxy(EDGE_WIDTH + pOrder->fromX / 2 * (CELL_SIZE + STREET_WIDTH) + 12, EDGE_WIDTH + pOrder->fromY / 2 * (CELL_SIZE + STREET_WIDTH) + 15, str);
            }
            // ��ӡB
            itoa(pOrder->orderId, str, 10);
            strcat(str, "B");
            outtextxy(EDGE_WIDTH + pOrder->toX / 2 * (CELL_SIZE + STREET_WIDTH) + 12, EDGE_WIDTH + pOrder->toY / 2 * (CELL_SIZE + STREET_WIDTH) + 15, str);
        }
        putimage(EDGE_WIDTH + (pRider->nowX + 1) / 2 * CELL_SIZE + pRider->nowX / 2 * STREET_WIDTH, EDGE_WIDTH + (pRider->nowY + 1) / 2 * CELL_SIZE + pRider->nowY / 2 * STREET_WIDTH, info->img[5]);
        outtextxy(EDGE_WIDTH + (pRider->nowX + 1) / 2 * CELL_SIZE + pRider->nowX / 2 * STREET_WIDTH, EDGE_WIDTH + (pRider->nowY + 1) / 2 * CELL_SIZE + pRider->nowY / 2 * STREET_WIDTH, itoa(pRider->riderId, str, 10));
    }
}

// �����Ҳ���Ϣ
void drawInfo(INFO *info)
{
    char str[10];

    putimage(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 50, 95, info->img[0]);
    putimage(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 50, 145, info->img[1]);
    putimage(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 50, 190, info->img[2]);
    putimage(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 50, 240, info->img[3]);
    putimage(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 50, 290, info->img[5]);

    setcolor(BLACK);
    setfont(20, 0, "����");
    setfontbkcolor(EGERGB(0xf0, 0xf0, 0xf0));
    outtextxy(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 100, 100, "��ǰʱ�䵥λ��");
    outtextxy(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 100, 150, "Ǯ����");
    outtextxy(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 100, 200, "����ɵ�����");
    outtextxy(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 100, 250, "��ʱ������");
    outtextxy(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 100, 300, "��������");

    outtextxy(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 250, 100, itoa(info->nowTime-1, str, 10));
    outtextxy(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 250, 150, itoa(info->realMoney, str, 10));
    outtextxy(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 250, 200, itoa(info->numFinished, str, 10));
    outtextxy(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 250, 250, itoa(info->numTimeOut, str, 10));
    outtextxy(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 250, 300, itoa(info->numRider, str, 10));

    outtextxy(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 60, 350, "�ᵥ��");
    int i = 0;
    for(ORDER *pCurr = info->finishList->next; pCurr; pCurr = pCurr->next)
    {
        outtextxy(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 140 + i, 350, itoa(pCurr->orderId, str, 10));
        i += 50;
    }

    if(info->newRider)
    {
        outtextxy(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 60, 500, "�������֣�");
    }
}

// ����������Ķ������붩���б�
void addOrder(INFO *info)
{
    ORDER temp;

    temp.orderId = ++info->lastOrderId;
    temp.orderTime = info->nowTime;
    temp.hasPicked = false;

    temp.fromX = info->restaurant[0];
    temp.fromY = info->restaurant[1];
    temp.toX = info->client[0];
    temp.toY = info->client[1];

    ORDER *pTemp = info->savedOrderList->next;
    info->savedOrderList->next = (ORDER *)malloc(sizeof(ORDER));
    *info->savedOrderList->next = temp;
    info->savedOrderList->next->next = pTemp;

    printf("addOrder\n");
}

// �����Ƴ��������֡��������Ҳ���ƻ�����Ϣ
void update(INFO *info)
{
    if(info->draw)
    {
        info->draw = false;
        cleardevice();
    }

    drawScene(info);
    drawInfo(info);
    delay_ms(REFRESH_INTERVAL);
}

// ͼ�ν������
void * guiDraw(void *arg)
{
    INFO *info = (INFO *)arg;
        
    while(!info->state)
        update(info);
	
	update(info);

    getch();
    closegraph();
    pthread_exit(0);
}

// ��ȡ�������
void * guiInput(void *arg)
{
    INFO *info = (INFO *)arg;

    mouse_msg msg;
    int x, y, i;
    int state = 0;      // 0Ϊδѡ�У�1Ϊѡ�в͹ݣ�2Ϊѡ��ʳ��
    while(!info->state)
    {
        if(!info->inputMode && mousemsg())
        {
            msg = getmouse();
            if(msg.is_left() && msg.is_down())
            {
                for(i = 0; i < 9; ++i)
                    if(msg.x > EDGE_WIDTH + i * (CELL_SIZE + STREET_WIDTH) && msg.x < EDGE_WIDTH + i * (CELL_SIZE + STREET_WIDTH) + CELL_SIZE)
                    {
                        x = 2 * i;
                        break;
                    }
                if(i == 9)
                {
                    state = 0;
                    continue;
                }
                for(i = 0; i < 9; ++i)
                    if(msg.y > EDGE_WIDTH + i * (CELL_SIZE + STREET_WIDTH) && msg.y < EDGE_WIDTH + i * (CELL_SIZE + STREET_WIDTH) + CELL_SIZE)
                    {
                        y = 2 * i;
                        break;
                    }
                if(i == 9)
                {
                    state = 0;
                    continue;
                }
                state++;
                if(state == 1)
                {
                    info->restaurant[0] = x;
                    info->restaurant[1] = y;
                }
                if(state == 2)
                {
                    info->client[0] = x;
                    info->client[1] = y;
                    addOrder(info);
                    drawScene(info);
                    setfont(20, 0, "����");
                    char str[100] = "�͹� (";
                    char temp[10];
                    strcat(str, itoa(info->restaurant[0], temp, 10));
                    strcat(str, ", ");
                    strcat(str, itoa(info->restaurant[1], temp, 10));
                    strcat(str, ")  ʳ�� (");
                    strcat(str, itoa(info->client[0], temp, 10));
                    strcat(str, ", ");
                    strcat(str, itoa(info->client[1], temp, 10));
                    strcat(str, ")");
                    outtextxy(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 60, 400, "׷���µ���");
                    outtextxy(2*EDGE_WIDTH + 9*CELL_SIZE + 8*STREET_WIDTH + 60, 450, str);
                    delay_ms(1000);
                    info->restaurant[0] = info->client[0] = -1;
                    state = 0;
                }
            }
        }
    }
}

