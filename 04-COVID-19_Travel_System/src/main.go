package main

import (
    "sync"
)

var port          int            // 服务端口
var cityList      []City         // 城市列表
var transportList []Transport    // 交通班次表
var status        Status         // 状态信息，同时用于向前端提供数据
var wg            sync.WaitGroup // 多线程同步控制

func main() {
    InitLog()
    Log("正在初始化")
    LoadConfig()        // 从文件中加载城市和交通信息
    CreateGraph()       // 根据加载的数据建图
    status = Status{false, [2]int {0, 0}, []Travel {}}
    Log("初始化完成")

    //AddTravel("北京", "上海", 8, 9)
    //AddTravel("北京", "上海", 10, 0)

    wg.Add(1)
    go StartServer()    // 启动 Web 服务
    wg.Wait()           // 等待其他线程结束
}