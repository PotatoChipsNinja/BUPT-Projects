package main

import (
    "time"
    "fmt"
)

func StartTimer() {
    Log("模拟开始")
    status.IsRunning = true
    PrintStatus()
    for status.IsRunning {
        time.Sleep(time.Millisecond * 500)
        status.Time[1]++
        if status.Time[1] == 20 {
            status.Time[1] = 0
            status.Time[0]++
            if status.Time[0] == 24 {
                status.Time[0] = 0
            }
            UpdateTravel()
            PrintStatus()
        }
    }
    wg.Done()
}

func PauseTimer() {
    Log("模拟暂停")
    status.IsRunning = false
}

func UpdateTravel() {
    for i := 0; i < len(status.TravelList); i++ {
        status.TravelList[i].CurrProgress++
        if status.TravelList[i].CurrProgress >= status.TravelList[i].Path[status.TravelList[i].CurrSection].SectionDuration {
            // 该阶段已完成，进入下一阶段
            status.TravelList[i].CurrSection++
            status.TravelList[i].CurrProgress = 0
            if status.TravelList[i].CurrSection >= len(status.TravelList[i].Path) {
                // 该旅程已全部完成，从TravelList中删除
                Log(status.TravelList[i].TravelerID + "已完成全部行程")
                status.TravelList = append(status.TravelList[:i], status.TravelList[i+1:]...)
                i--
            }
        }
    }
}

func PrintStatus() {
    logStr := fmt.Sprintf("当前时间：%02d:00", status.Time[0])
    for _, travel := range status.TravelList {
        if travel.Path[travel.CurrSection].TransportType == "停留" {
            logStr += fmt.Sprintf("，%s：停留%s", travel.TravelerID, travel.Path[travel.CurrSection].FromCity)
        } else {
            logStr += fmt.Sprintf("，%s：%s->%s(%s%s)",
            travel.TravelerID,
            travel.Path[travel.CurrSection].FromCity,
            travel.Path[travel.CurrSection].ToCity,
            travel.Path[travel.CurrSection].TransportType,
            travel.Path[travel.CurrSection].TransportID)
        }
    }
    Log(logStr)
}