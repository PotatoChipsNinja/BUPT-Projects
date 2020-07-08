package main

/* 城市信息 */
type City struct {
    CityName string          // 城市名
    CityRisk float32         // 城市风险值
}

/* 交通班次信息 */
type Schedule struct {
    Identity        string   // 班次号
    OriginCity      string   // 起点城市
    DestinationCity string   // 终点城市
    DepartureTime   int      // 出发时间，24小时制
    Duration        int      // 持续时间，以小时为单位
}

/* 交通方式信息 */
type Transport struct {
    TransportType string     // 交通方式
    TransportRisk int        // 交通工具风险值
    ScheduleList  []Schedule // 时刻表
}

/* 路径阶段信息 */
type Section struct {
    FromCity        string   // 出发城市
    ToCity          string   // 目的城市
    StartTime       int      // 出发时间
    TransportType   string   // 交通方式
    TransportID     string   // 班次号
    SectionRisk     float32  // 该段风险值
    SectionDuration int      // 该段时间
}

/* 旅程信息 */
type Travel struct {
    TravelerID    string     // 旅客名
    TotalRisk     float32    // 总风险值
    TotalDuration int        // 总时间
    CurrSection   int        // 当前阶段的索引
    CurrProgress  int        // 当前阶段已进行的时长
    Path          []Section  // 旅行路径
}

/* 状态信息，同时用于向前端提供数据 */
type Status struct {
    IsRunning  bool          // 是否正在模拟中
    Time       [2]int        // 当前时间，Time[0]为整小时，Time[1]为二十分之一小时
    TravelList []Travel      // 旅程列表
}