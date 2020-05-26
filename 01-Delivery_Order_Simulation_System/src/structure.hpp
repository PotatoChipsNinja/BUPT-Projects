// ���ݽṹ����
typedef struct order
{
    int orderId;                // ������
    int orderTime;              // �µ�ʱ��
    int fromX, fromY;           // �͹�����
    int toX, toY;               // �ͻ�����
    bool hasPicked;             // �Ƿ���ȡ��
    struct order *next;         // ��һ����ָ��
} ORDER;

typedef struct rider
{
    int riderId;                // ���ֱ��
    int nowX, nowY;             // ��ǰ����
    int numReceived;            // �ѽӵ���
    int stopInfo[2][3];         // ͣ����Ϣ��stopInfo[i]={ͣ��״̬, ͣ��λ��x, ͣ��λ��y}��ͣ��״̬��0Ϊδͣ����1Ϊͣ���͹ݣ�2Ϊͣ��ʳ�ͣ�3Ϊͣ���Ϳ�
    ORDER *orderList;           // δ��ʱ���Ͷ����б�
    ORDER *timeOutList;         // �ѳ�ʱ���Ͷ����б�
    struct rider *next;         // ��һ����ָ��
} RIDER;

typedef struct info
{
    int nowTime;                // ��ǰʱ��
    int realMoney;              // ʵ�ʽ��
    int exptMoney;              // Ԥ�ڽ��
    int numReceived;            // �ӵ���
    int numFinished;            // �����
    int numTimeOut;             // ��ʱ��
    int numRider;               // ������
    int state;                  // ��Ӫ״̬��0��ʾ������Ӫ��1��ʾ������Ӫҵִ�գ�2��ʾ�Ʋ���3��ʾ��������
    int restaurant[2];          // ѡ�в͹�λ��
    int client[2];              // ѡ��ʳ��λ��
    int lastOrderId;            // ���һ������
    bool inputMode;             // ����ģʽ��0��ʾ������룬1��ʾ�ļ�����
    bool cellInfo[9][9];        // ��Ԫ��״̬��0��ʾδ��ͣ����1��ʾ��ͣ��
    bool clock;                 // ʱ���ź�
    bool draw;                  // ��ͼ�ź�
    bool newRider;              // ������
    ORDER *finishList;          // ��ǰ�ᵥ
    ORDER *timeOutList;         // ��ǰ����
    ORDER *savedOrderList;      // ��ѡ�����б�
    RIDER *riderList;           // �����б�
    PIMAGE img[8];              // ��ͼ
} INFO;

