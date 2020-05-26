// 数据结构声明
typedef struct order
{
    int orderId;                // 订单号
    int orderTime;              // 下单时间
    int fromX, fromY;           // 餐馆坐标
    int toX, toY;               // 客户坐标
    bool hasPicked;             // 是否已取餐
    struct order *next;         // 下一订单指针
} ORDER;

typedef struct rider
{
    int riderId;                // 骑手编号
    int nowX, nowY;             // 当前坐标
    int numReceived;            // 已接单数
    int stopInfo[2][3];         // 停靠信息，stopInfo[i]={停靠状态, 停靠位置x, 停靠位置y}，停靠状态：0为未停靠，1为停靠餐馆，2为停靠食客，3为停靠餐客
    ORDER *orderList;           // 未超时待送订单列表
    ORDER *timeOutList;         // 已超时待送订单列表
    struct rider *next;         // 下一骑手指针
} RIDER;

typedef struct info
{
    int nowTime;                // 当前时间
    int realMoney;              // 实际金额
    int exptMoney;              // 预期金额
    int numReceived;            // 接单数
    int numFinished;            // 完成数
    int numTimeOut;             // 超时数
    int numRider;               // 骑手数
    int state;                  // 经营状态，0表示正常运营，1表示被吊销营业执照，2表示破产，3表示正常结束
    int restaurant[2];          // 选中餐馆位置
    int client[2];              // 选中食客位置
    int lastOrderId;            // 最后一单单号
    bool inputMode;             // 输入模式，0表示鼠标输入，1表示文件输入
    bool cellInfo[9][9];        // 单元格状态，0表示未被停靠，1表示被停靠
    bool clock;                 // 时钟信号
    bool draw;                  // 绘图信号
    bool newRider;              // 新骑手
    ORDER *finishList;          // 当前结单
    ORDER *timeOutList;         // 当前罚单
    ORDER *savedOrderList;      // 候选订单列表
    RIDER *riderList;           // 骑手列表
    PIMAGE img[8];              // 贴图
} INFO;

