package main

import (
    "net/http"
    "strconv"
    "encoding/json"
    "io/ioutil"
    "fmt"
    "os"
)

func ServerTest() {
    for {
        res, err := http.Get("http://localhost:" + strconv.FormatInt(int64(port), 10) + "/api/isServing")
        if err == nil {
            defer res.Body.Close()
            body, _ := ioutil.ReadAll(res.Body)
            if string(body) == "true" {
                Log(fmt.Sprintf("Web 服务启动成功，请访问 http://localhost:%d 进入系统", port))
                break
            }
        }
    }
}

func APIHandler(w http.ResponseWriter, req *http.Request) {
    w.Header().Set("Access-Control-Allow-Origin", "*")
    w.Header().Set("content-type", "application/json")

    query := req.URL.Query()
    switch req.URL.Path {
        case "/api/isServing":
            w.Write([]byte("true"))
        case "/api/getCity":
            data, _ := json.Marshal(cityList)
            w.Write(data)
        case "/api/getTransInfo":
            data, _ := json.Marshal(transportList)
            w.Write(data)
        case "/api/getStatus":
            data, _ := json.Marshal(status)
            w.Write(data)
        case "/api/startTimer":
            if !status.IsRunning {
                wg.Add(1)
                go StartTimer()
            }
        case "/api/pauseTimer":
            if status.IsRunning {
                PauseTimer()
            }
        case "/api/addTravel":
            origin := query.Get("origin")
            destination := query.Get("destination")
            timeLimit, _ := strconv.Atoi(query.Get("timeLimit"))
            w.Write(AddTravel(origin, destination, status.Time[0], timeLimit))
        case "/api/exit":
            Log("系统已退出，您可以到log目录中查看日志")
            os.Exit(1)
    }
}

func StartServer() {
    http.HandleFunc("/api/", APIHandler)
    http.Handle("/", http.FileServer(http.Dir("static")))
    go ServerTest()
    err := http.ListenAndServe(":" + strconv.FormatInt(int64(port), 10), nil)
    if err != nil {
        Log(fmt.Sprintf("Web 服务启动失败，请检查您的防火墙设置并确保 %d 端口没有被占用", port))
        os.Exit(1)
    }
    wg.Done()
}