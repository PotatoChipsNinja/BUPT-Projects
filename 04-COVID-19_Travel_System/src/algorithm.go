package main

import (
    "encoding/json"
    "container/heap"
    "strconv"
    "fmt"
)

const INT_MAX = int(^uint(0) >> 1)

var graph []Vertex
var cityRisk map[string]int   // 城市风险值 * 10
var vertexIndex map[string]map[int]int    // vertexMap["北京"][6]为"6点的北京"节点的索引
var travelerNum int = 0

type Vertex struct {
    CityName string
    Time     int
    ArcList  []Arc
}

type Arc struct {
    TargetIndex   int
    TransportType string
    TransportID   string
    Duration      int
    Risk          int   // 风险值 * 10
}

type HeapNode struct {
    Index    int
    Risk     int        // 风险值 * 10
    Duration int
}

type MinHeap []HeapNode     // 定义小根堆，即最小优先队列

func AddTravel(origin string, destination string, startTime int, timeLimit int) []byte {
    startIndex, exist := vertexIndex[origin][startTime]
    if !exist {
        // 图中不存在"当前时刻的起点"这一节点，需要建立临时节点
        startIndex = len(graph)
        graph = append(graph, Vertex{origin, startTime, []Arc{}})

        // 创建该节点与其余同地点节点间的弧
        for time, index := range vertexIndex[origin] {
            // 建立已有的点到该点的弧
            interval := startTime - time
            if interval < 0 {
                interval += 24
            }
            graph[index].ArcList = append(graph[index].ArcList, Arc{
                startIndex,
                "停留",
                "",
                interval,
                // 停留的风险值 = 城市风险值 * 停留时间
                cityRisk[origin] * interval})
            
            // 建立该点到已有的点的弧
            interval = time - startTime
            if interval < 0 {
                interval += 24
            }
            graph[startIndex].ArcList = append(graph[startIndex].ArcList, Arc{
                index,
                "停留",
                "",
                interval,
                // 停留的风险值 = 城市风险值 * 停留时间
                cityRisk[origin] * interval})
        }
    }

    var exitIndex int
    var duration int
    var risk int    // 风险值 * 10
    var path []Section
    var rawPath interface{}

    if timeLimit == 0 {
        Log(fmt.Sprintf("收到规划请求：%s -> %s，无时间限制", origin, destination))
        exitIndex, duration, risk, rawPath = NoTimeLimit(startIndex, destination)
    } else {
        Log(fmt.Sprintf("收到规划请求：%s -> %s，时间限制：%d小时", origin, destination, timeLimit))
        exitIndex, duration, risk, rawPath = HasTimeLimit(startIndex, destination, timeLimit)
    }

    var returnStr []byte
    if exitIndex == -1 {
        Log("规划失败")
        returnStr = []byte("{\"fail\":true}")
    } else {
        var fromID, arcIndex int
        var remain int = risk
        for exitIndex != startIndex {
            if timeLimit == 0 {
                fromID, arcIndex = rawPath.([][2]int)[exitIndex][0], rawPath.([][2]int)[exitIndex][1]
            } else {
                fromID, arcIndex = rawPath.([]map[int][2]int)[exitIndex][remain][0], rawPath.([]map[int][2]int)[exitIndex][remain][1]
            }
            path = append(path, Section{
                graph[fromID].CityName,
                graph[exitIndex].CityName,
                graph[fromID].Time,
                graph[fromID].ArcList[arcIndex].TransportType,
                graph[fromID].ArcList[arcIndex].TransportID,
                float32(graph[fromID].ArcList[arcIndex].Risk) / 10,
                graph[fromID].ArcList[arcIndex].Duration})
            exitIndex = fromID
            remain -= graph[fromID].ArcList[arcIndex].Risk
        }
        path = reversePath(path)
        path = mergePath(path)
        travelerNum++
        ans := Travel{"旅客" + strconv.Itoa(travelerNum), float32(risk) / 10, duration, 0, 0, path}
        status.TravelList = append(status.TravelList, ans)
        returnStr, _ = json.Marshal(ans)
        pathStr, _ := json.Marshal(path)
        logStr := fmt.Sprintf("%s规划成功，总风险：%v，总时间：%v小时，路线：%s", ans.TravelerID, ans.TotalRisk, ans.TotalDuration, string(pathStr))
        Log(logStr)
    }

    // 删除临时建立的节点
    if !exist {
        for _, index := range vertexIndex[origin] {
            graph[index].ArcList = graph[index].ArcList[:len(graph[index].ArcList)-1]
        }
        graph = graph[:startIndex]
    }
    return returnStr
}

func reversePath(s []Section) []Section {
    for i, j := 0, len(s)-1; i < j; i, j = i+1, j-1 {
        s[i], s[j] = s[j], s[i]
    }
    return s
}

// 将连续的“在某地停留”合并
func mergePath(s []Section) []Section {
    ans := []Section {s[0]}
    last := s[0]
    for i := 1; i < len(s); i += 1 {
        if s[i].TransportType == "停留" && last.TransportType == "停留" {
            ans[len(ans)-1].SectionRisk = ((ans[len(ans)-1].SectionRisk * 10) + (s[i].SectionRisk * 10)) / 10
            ans[len(ans)-1].SectionDuration += s[i].SectionDuration
        } else {
            ans = append(ans, s[i])
            last = s[i]
        }
    }
    return ans
}

// 初始化时建图
func CreateGraph() {
    vertexIndex = make(map[string]map[int]int)
    cityRisk = make(map[string]int)
    for _, city := range cityList {
        vertexIndex[city.CityName] = make(map[int]int)
        cityRisk[city.CityName] = int(city.CityRisk * 10)
    }

    // 创建不同地点间的弧
    for _, transport := range transportList {
        for _, schedule := range transport.ScheduleList {
            startIndex, startExist := vertexIndex[schedule.OriginCity][schedule.DepartureTime]
            endIndex, endExist := vertexIndex[schedule.DestinationCity][(schedule.DepartureTime + schedule.Duration) % 24]
            if !startExist {
                startIndex = len(graph)
                graph = append(graph, Vertex{schedule.OriginCity, schedule.DepartureTime, []Arc{}})
                vertexIndex[schedule.OriginCity][schedule.DepartureTime] = startIndex
            }
            if !endExist {
                endIndex = len(graph)
                graph = append(graph, Vertex{schedule.DestinationCity, (schedule.DepartureTime + schedule.Duration) % 24, []Arc{}})
                vertexIndex[schedule.DestinationCity][(schedule.DepartureTime + schedule.Duration) % 24] = endIndex
            }
            graph[startIndex].ArcList = append(graph[startIndex].ArcList, Arc{
                endIndex,
                transport.TransportType,
                schedule.Identity,
                schedule.Duration,
                // 乘坐交通工具的风险值 = 出发城市风险值 * 交通工具风险值 * 乘坐时间
                cityRisk[schedule.OriginCity] * transport.TransportRisk * schedule.Duration})
        }
    }

    // 创建同一地点间的弧
    for _, city := range cityList {
        for fromTime, fromIndex := range vertexIndex[city.CityName] {
            for toTime, toIndex := range vertexIndex[city.CityName] {
                if fromTime != toTime {
                    interval := toTime - fromTime
                    if interval < 0 {
                        interval += 24
                    }

                    graph[fromIndex].ArcList = append(graph[fromIndex].ArcList, Arc{
                        toIndex,
                        "停留",
                        "",
                        interval,
                        // 停留的风险值 = 城市风险值 * 停留时间
                        cityRisk[city.CityName] * interval})
                }
            }
        }
    }
}

func NoTimeLimit(startIndex int, destination string) (int, int, int, [][2]int) {
    graphLen := len(graph)
    visited := make([]bool, graphLen)
    path := make([][2]int, graphLen)  // path[i] = [i号节点的前一节点, 前一节点的弧序号]
    risk := make([]int, graphLen)
    h := &MinHeap{HeapNode{startIndex, 0, 0}}

    for i := 0; i < graphLen; i++ {
        visited[i] = false
        risk[i] = INT_MAX
    }
    risk[startIndex] = 0

    var exitIndex int = -1
    var totalRisk int
    var totalDuration int
    for h.Len() > 0 {
        x := heap.Pop(h).(HeapNode)
        if visited[x.Index] {
            continue
        }
        visited[x.Index] = true
        if graph[x.Index].CityName == destination {
            exitIndex = x.Index
            totalRisk = x.Risk
            totalDuration = x.Duration
            break
        }
        for index, val := range graph[x.Index].ArcList {
            if risk[val.TargetIndex] > risk[x.Index] + val.Risk {
                risk[val.TargetIndex] = risk[x.Index] + val.Risk
                heap.Push(h, HeapNode{val.TargetIndex, risk[val.TargetIndex], x.Duration + val.Duration})
                path[val.TargetIndex] = [2]int {x.Index, index}
            }
        }
    }
    return exitIndex, totalDuration, totalRisk, path
}

func HasTimeLimit(startIndex int, destination string, timeLimit int) (int, int, int, []map[int][2]int) {
    graphLen := len(graph)
    path := make([]map[int][2]int, graphLen)  // path[i] = [i号节点的前一节点, 前一节点的弧序号]
    h := &MinHeap{HeapNode{startIndex, 0, 0}}

    for i := 0; i < graphLen; i++ {
        path[i] = make(map[int][2]int)
    }

    var exitIndex int = -1
    var totalRisk int
    var totalDuration int
    for h.Len() > 0 {
        x := heap.Pop(h).(HeapNode)
        if graph[x.Index].CityName == destination {
            exitIndex = x.Index
            totalRisk = x.Risk
            totalDuration = x.Duration
            break
        }
        for index, val := range graph[x.Index].ArcList {
            if x.Duration + val.Duration <= timeLimit {
                heap.Push(h, HeapNode{val.TargetIndex, x.Risk + val.Risk, x.Duration + val.Duration})
                path[val.TargetIndex][x.Risk + val.Risk] = [2]int {x.Index, index}
            }
        }
    }
    return exitIndex, totalDuration, totalRisk, path
}

/* 绑定 MinHeap 的方法 */

func (h MinHeap) Len() int {
    return len(h)
}

func (h MinHeap) Less(i, j int) bool {
    return h[i].Risk < h[j].Risk  // 保证是基于风险值的小根堆
}

func (h MinHeap) Swap(i, j int) {
    h[i], h[j] = h[j], h[i]
}

func (h *MinHeap) Push(x interface{}) {
    *h = append(*h, x.(HeapNode))
}

func (h *MinHeap) Pop() interface{} {
    old := *h
    n := len(old)
    x := old[n-1]
    *h = old[0 : n-1]
    return x
}