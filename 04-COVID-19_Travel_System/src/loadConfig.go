package main

import (
    "fmt"
    "os"
    "io/ioutil"
    "encoding/json"
)

type ConfigFile struct {
    Port int
    City []City
    Transport []Transport
}

func LoadConfig() {
    // 打开数据文件
    data, err := ioutil.ReadFile("config.json")
    if err != nil {
        Log("打开数据文件失败，请将 config.json 置于程序所在目录下")
        os.Exit(1)
    }

    // 解析 JSON
    var config ConfigFile
    err = json.Unmarshal(data, &config)
    if err != nil {
        Log("解析 config.json 失败，请检查 JSON 格式是否正确")
        os.Exit(1)
    }

    port = config.Port
    cityList = config.City
    transportList = config.Transport

    Log(fmt.Sprintf("成功加载 %d 个城市", len(cityList)))
    for _, val := range transportList {
        Log(fmt.Sprintf("成功加载 %d 条%s班次信息", len(val.ScheduleList), val.TransportType))
    }
}