package main

import (
    "fmt"
    "os"
    "time"
)

var logFile *os.File
var err error

func InitLog() {
    logFile, err = os.Create("log/" + time.Now().Format("20060102_150405") + ".log")
    if err != nil {
        fmt.Println("创建日志文件失败，请检查相关读写权限")
        os.Exit(1)
    }
}

func Log(content string) {
    content = "[" + time.Now().Format("2006-01-02 15:04:05") + "] " + content
    fmt.Println(content)
    logFile.WriteString(content + "\r\n")
}