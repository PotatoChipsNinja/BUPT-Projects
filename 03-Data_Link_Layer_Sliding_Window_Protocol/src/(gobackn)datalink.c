/*
FileName: datalink.c
Function: 数据链路层 回退N步协议
Author:   石宇辉 张昕旸 孙昊
Data:     2020-05
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "protocol.h"
#include "datalink.h"

#define MAX_SEQ    7        // 最大帧序号
#define DATA_TIMER 2000     // 数据定时器超时时间
#define ACK_TIMER  1000     // ACK定时器超时时间

struct FRAME { 
    unsigned char kind;
    unsigned char ack;
    unsigned char seq;
    unsigned char data[PKT_LEN];
    unsigned int  padding;
};

static unsigned char frame_nr = 0;                  // 准备发送的帧序号
static unsigned char nbuffered = 0;                 // 缓冲区已使用的数量
static unsigned char buffer[MAX_SEQ + 1][PKT_LEN];  // 发送缓冲区
static unsigned char frame_expected = 0;            // 期待接收的帧序号
static unsigned char ack_expected = 0;              // 期待的ACK序号
static int phl_ready = 0;                           // 物理层是否就位

// 判断所接收到的序号是否在窗口范围内，即返回是否满足 a <= b < c
static bool between(unsigned char a, unsigned char b, unsigned char c)
{
    if (((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c < a)))
        return true;
    else
        return false;
}

// 在帧末尾添加校验和，并递交给物理层发送
static void put_frame(unsigned char *frame, int len)
{
    *(unsigned int *)(frame + len) = crc32(frame, len); // 加入校验和
    send_frame(frame, len + 4);                         // 向物理层缓冲区发送帧
    phl_ready = 0;
}

// 准备向物理层发送数据帧
static void send_data_frame(void)
{
    struct FRAME s;

    s.kind = FRAME_DATA;
    s.seq = frame_nr;
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);
    memcpy(s.data, buffer[frame_nr], PKT_LEN);

    dbg_frame("Send DATA %d %d, ID %d\n", s.seq, s.ack, *(short *)s.data);

    put_frame((unsigned char *)&s, 3 + PKT_LEN);
    start_timer(frame_nr, DATA_TIMER);      // 启动数据定时器
}

// 准备向物理层发送 ACK 帧
static void send_ack_frame(void)
{
    struct FRAME s;

    s.kind = FRAME_ACK;
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);

    dbg_frame("Send ACK  %d\n", s.ack);

    put_frame((unsigned char *)&s, 2);
    stop_ack_timer();       // 停止ACK定时器
}

int main(int argc, char **argv)
{
    int event, arg;
    struct FRAME f;
    int len = 0;

    protocol_init(argc, argv); 
    lprintf("Designed by Shi Yuhui, Zhang Xinyang, Sun Hao, build: " __DATE__"  "__TIME__"\n");

    disable_network_layer();

    for (;;) {
        event = wait_for_event(&arg);

        switch (event) {
        case NETWORK_LAYER_READY:
            get_packet(buffer[frame_nr]);   // 从网络层获取分组
            nbuffered++;
            send_data_frame();
            frame_nr = (frame_nr + 1) % (MAX_SEQ + 1);  // 准备发送的帧序号右移
            break;

        case PHYSICAL_LAYER_READY:
            phl_ready = 1;
            break;

        case FRAME_RECEIVED: 
            len = recv_frame((unsigned char *)&f, sizeof f);
            if (len < 5 || crc32((unsigned char *)&f, len) != 0) {  // 校验和错误
                dbg_event("**** Receiver Error, Bad CRC Checksum\n");
                break;
            }
            if (f.kind == FRAME_ACK)
                dbg_frame("Recv ACK  %d\n", f.ack);
            if (f.kind == FRAME_DATA) {
                dbg_frame("Recv DATA %d %d, ID %d\n", f.seq, f.ack, *(short *)f.data);
                if (f.seq == frame_expected) {      // 收到了期待的帧
                    put_packet(f.data, len - 7);    // 向网络层发送分组
                    frame_expected = (frame_expected + 1) % (MAX_SEQ + 1);  // 期待接收下一帧
                }
                start_ack_timer(ACK_TIMER);
            }

            while (between(ack_expected, f.ack, frame_nr)) {
                stop_timer(ack_expected);       // 停止数据定时器
                nbuffered--;
                ack_expected = (ack_expected + 1) % (MAX_SEQ + 1);  // 发送窗口下界右移
            }
            break; 

        case DATA_TIMEOUT:
            dbg_event("---- DATA %d timeout\n", arg);
            frame_nr = ack_expected;
            for (int i = 0; i < nbuffered; ++i) {       // 从头开始重新发送
                send_data_frame();
                frame_nr = (frame_nr + 1) % (MAX_SEQ + 1);
            }
            break;

        case ACK_TIMEOUT:
            dbg_event("---- ACK timeout\n");
            send_ack_frame();       // 发送单独的ACK确认帧
            break;
        }

        if (nbuffered < MAX_SEQ && phl_ready)
            enable_network_layer();
        else
            disable_network_layer();
   }
}
