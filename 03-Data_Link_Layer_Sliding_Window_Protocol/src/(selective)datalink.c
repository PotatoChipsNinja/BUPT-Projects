/*
FileName: datalink.c
Function: 数据链路层 选择重传协议
Author:   石宇辉 张昕旸 孙昊
Data:     2020-05
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "protocol.h"
#include "datalink.h"

#define MAX_SEQ    7                    // 最大帧序号
#define NR_BUFS    ((MAX_SEQ + 1) / 2)  // 窗口大小
#define DATA_TIMER 2000                 // 数据定时器超时时间
#define ACK_TIMER  1000                 // ACK定时器超时时间

struct FRAME { 
    unsigned char kind;
    unsigned char ack;
    unsigned char seq;
    unsigned char data[PKT_LEN];
    unsigned int  padding;
};

static unsigned char out_buf[NR_BUFS][PKT_LEN]; // 发送缓冲区
static unsigned char nbuffered = 0;             // 发送缓冲区用量
static unsigned char in_buf[NR_BUFS][PKT_LEN];  // 接收缓冲区 
static int in_buf_len[NR_BUFS];                 // 接收缓冲区里各分组的长度
static bool arrived[NR_BUFS] = {false};         // 接收缓冲区里是否存在该分组

static unsigned char ack_expected = 0;          // 发送窗口下界
static unsigned char next_frame_to_send = 0;    // 发送窗口上界加一
static unsigned char frame_expected = 0;        // 接收窗口下界
static unsigned char too_far = NR_BUFS;         // 接收窗口上界加一

static int phl_ready = 0;                       // 物理层是否就位

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
static void send_data_frame(unsigned char frame_nr)
{
    struct FRAME s;

    s.kind = FRAME_DATA;
    s.seq = frame_nr;
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);
    memcpy(s.data, out_buf[frame_nr % NR_BUFS], PKT_LEN);

    dbg_frame("Send DATA %d %d, ID %d\n", s.seq, s.ack, *(short *)s.data);

    put_frame((unsigned char *)&s, 3 + PKT_LEN);
    start_timer(frame_nr % NR_BUFS, DATA_TIMER);        // 启动数据定时器
    stop_ack_timer();                                   // 停止ACK定时器
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

// 准备向物理层发送 NAK 帧
static void send_nak_frame(void)
{
    struct FRAME s;

    s.kind = FRAME_NAK;
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);

    dbg_frame("Send NAK  %d\n", s.ack);

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
            get_packet(out_buf[next_frame_to_send % NR_BUFS]);   // 从网络层获取分组，取至发送缓冲区
            nbuffered++;
            send_data_frame(next_frame_to_send);
            next_frame_to_send = (next_frame_to_send + 1) % (MAX_SEQ + 1);    // 发送窗口上界右移
            break;

        case PHYSICAL_LAYER_READY:
            phl_ready = 1;
            break;

        case FRAME_RECEIVED: 
            len = recv_frame((unsigned char *)&f, sizeof f);
            if (len < 5 || crc32((unsigned char *)&f, len) != 0) {  // 校验和错误
                dbg_event("**** Receiver Error, Bad CRC Checksum\n");
                send_nak_frame();   // 发送NAK帧
                break;
            }
            if (f.kind == FRAME_ACK)
                dbg_frame("Recv ACK  %d\n", f.ack);
            if (f.kind == FRAME_NAK) {
                dbg_frame("Recv NAK  %d\n", f.ack);
                if (between(ack_expected, (f.ack + 1) % (MAX_SEQ + 1), next_frame_to_send))
                    send_data_frame((f.ack + 1) % (MAX_SEQ + 1));   // 重发数据帧
            }
            if (f.kind == FRAME_DATA) {
                dbg_frame("Recv DATA %d %d, ID %d\n", f.seq, f.ack, *(short *)f.data);
                if (f.seq != frame_expected)
                    send_nak_frame();   // 不是期待的帧，发送NAK
                else
                    start_ack_timer(ACK_TIMER);

                if (between(frame_expected, f.seq, too_far) && !arrived[f.seq % NR_BUFS]) {
                    arrived[f.seq % NR_BUFS] = true;
                    memcpy(in_buf[f.seq % NR_BUFS], f.data, PKT_LEN);   // 将帧内容拷贝至接收缓冲区
                    in_buf_len[f.seq % NR_BUFS] = len - 7;              // 记录帧长度

                    // 上传至网络层
                    while (arrived[frame_expected % NR_BUFS]) {
                        put_packet(in_buf[frame_expected % NR_BUFS], in_buf_len[frame_expected % NR_BUFS]);
                        lprintf("put %d to network layer\n", frame_expected % NR_BUFS);
                        arrived[frame_expected % NR_BUFS] = false;
                        // 接收窗口右移
                        frame_expected = (frame_expected + 1) % (MAX_SEQ + 1);
                        too_far = (too_far + 1) % (MAX_SEQ + 1);
                        start_ack_timer(ACK_TIMER);
                    }
                }
            }

            while (between(ack_expected, f.ack, next_frame_to_send)) {
                stop_timer(ack_expected % NR_BUFS);
                nbuffered--;
                ack_expected = (ack_expected + 1) % (MAX_SEQ + 1);  // 发送窗口下界右移
            }
            break; 

        case DATA_TIMEOUT:
            dbg_event("---- DATA %d timeout\n", arg); 
            send_data_frame(arg);   // 重发超时的数据帧
            break;

        case ACK_TIMEOUT:
            dbg_event("---- ACK timeout\n");
            send_ack_frame();       // 发送单独的ACK确认帧
            break;
        }

        if (nbuffered < NR_BUFS && phl_ready)
            enable_network_layer();
        else
            disable_network_layer();
   }
}
