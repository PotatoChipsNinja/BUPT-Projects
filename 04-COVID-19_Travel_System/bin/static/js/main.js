function Init() {
  $.get("/api/getCity", function(data) {
    InitMap(data);
    for (var i = 0; i < data.length; i++) {
      $("#origin").append("<option>" + data[i].CityName + "</option>");
      $("#destination").append("<option>" + data[i].CityName + "</option>");
    }
  });
  $.get("/api/getTransInfo", function(data) {
    DrawTransInfo(data);
  });
  console.log("Initialization completed");
  isRefreshing = false;
  Refresh();
  if (currStatus.IsRunning) StartTimer();
}

function StartTimer() {
  $.get("/api/startTimer");
  $("#time-btn").removeClass("btn-primary");
  $("#time-btn").addClass("btn-danger");
  $("#time-btn").html("<i class=\"fas fa-pause\"></i> 暂停模拟");
  $("#time-btn").attr("onclick", "PauseTimer()");
}

function PauseTimer() {
  $.get("/api/pauseTimer");
  $("#time-btn").removeClass("btn-danger");
  $("#time-btn").addClass("btn-primary");
  $("#time-btn").html("<i class=\"fas fa-play\"></i> 开始模拟");
  $("#time-btn").attr("onclick", "StartTimer()");
}

function AddTravel() {
  $("#addTravelModal").modal('hide');
  var origin = $("#origin").val();
  var destination = $("#destination").val();
  var timeLimit = hasTimeLimit ? parseInt($("#time-limit").val()) : 0;
  if (origin == "请选择..." || destination == "请选择..." || origin == destination || (hasTimeLimit && !(timeLimit > 0))) {
    $("#info-content").text("存在无效输入，请检查您的输入");
    $("#infoModal").modal('show');
    return;
  }
  $.get("/api/addTravel", {origin: origin, destination: destination, timeLimit: timeLimit}, function(data) {
    if ("fail" in data) {
      $("#info-content").text("很遗憾，规划失败！");
      $("#infoModal").modal('show');
    } else {
      $("#info-content").html("已成功为<strong>" + data.TravelerID + "</strong>规划旅程！<br>请在右侧“状态信息”面板中查看规划结果详情。");
      $("#infoModal").modal('show');
      Refresh();
    }
  })
}

function Refresh() {
  if (isRefreshing) return; // 刷新正在执行中，不进行新的刷新
  isRefreshing = true;
  $.get("/api/getStatus", function(data) {
    currStatus = data;
  })
  if (currStatus.TravelList.length > 0) {
    $("#welcome").hide();
    $("#traveler-info").html("<i class=\"fas fa-info-circle\"></i> 当前共有" + currStatus.TravelList.length + "位旅客正在旅行中");
  } else {
    $("#traveler-info").html("<i class=\"fas fa-info-circle\"></i> 当前没有任何旅程正在进行");
  }
  UpdateClock();
  $("#travel-panel").css("width", (currStatus.TravelList.length * (18+0.3)) + "rem");

  // 删除已完成的旅程
  var currTravelerArr = [];
  for (var i = 0; i < currStatus.TravelList.length; i++) currTravelerArr.push(currStatus.TravelList[i].TravelerID);
  for (var i = 0; i < traveler.length; i++) {
    if (!currTravelerArr.includes(traveler[i])) {
      // 删除地图上的路线
      chart.series.find(x => x.name === traveler[i]).remove();
      chart.series.find(x => x.name === traveler[i] + "位置").remove();

      $("#travel-" + traveler[i].slice(2)).remove(); // 删除面板中的信息
      traveler.splice(i, 1);  // 删除traveler[i]
    }
  }

  for (var i = 0; i < currStatus.TravelList.length; i++) {
    if (i < traveler.length) {
      // 更新traveler[i]
      chart.series.find(x => x.name === traveler[i] + "位置").setData([GetCurrPos(currStatus.TravelList[i])]);
      // 更新面板信息
      UpdateCard(currStatus.TravelList[i]);
    } else {
      traveler.push(currStatus.TravelList[i].TravelerID);  // 新增traveler[i]
      // 在地图上画出线路
      AddChart(currStatus.TravelList[i]);
      // 在面板中新增信息
      AddCard(currStatus.TravelList[i]);
    }
  }
  console.log("Refresh completed");
  isRefreshing = false;
}

function InitMap(cityInfo) {
  var riskData = [];
  var riskColor;
  for(i = 0; i < CityData.length; ++i) {
    switch(cityInfo.find(x => x.CityName === CityData[i].id).CityRisk) {
      case 0.2:
        riskColor = "rgba(0, 255, 0, 0.4)";
        break;
      case 0.5:
        riskColor = "rgba(255, 255, 0, 0.4)";
        break;
      case 0.9:
        riskColor = "rgba(255, 0, 0, 0.4)";
        break;
    }
    riskData.push({"name": CityData[i].province, "color": riskColor});
  }

  var chartConfig = {
    title: {
      text: null
    },
    chart: {
      backgroundColor: 'rgba(0,0,0,0)'
    },
    legend: {
      align: 'left',
      layout: 'vertical',
      floating: true
    },
    style: {
      backgroundColor: '#FCFFC5'
    },
    credits: {
      enabled: false
    },
    plotOptions: {
      series: {
        states: {
          inactive: {
            opacity: 1  // 使得鼠标移动不会造成变化
          }
        }
      }
    },
    mapNavigation: {
      enabled: true,
      enableDoubleClickZoom: false
    },
    tooltip: {
      enabled: false
    },
    series: [{
      // 中国地图
      type: 'map',
      mapData: Highcharts.maps['cn/china'],
      name: 'Basemap',
      borderColor: '#707070',
      nullColor: 'rgba(200, 200, 200, 1)',
      showInLegend: false,
      data: riskData,
      joinBy: 'name'
    }, {
      // 城市点
      type: 'mappoint',
      name: 'Cities',
      dataLabels: {
        format: '{point.id}'
      },
      marker: {
        fillColor: '#FFFFFF',
        lineWidth: 2,
        lineColor: "#434348"
      },
      showInLegend: false,
      data: CityData
    }]
  };

  chart = Highcharts.mapChart('map', chartConfig);
  transChart = Highcharts.mapChart('trans-map', chartConfig);
}

function pointsToPath(from, to, invertArc) {
  var arcPointX = (from.x + to.x) / (invertArc ? 2.4 : 1.6),
    arcPointY = (from.y + to.y) / (invertArc ? 2.4 : 1.6);
  return 'M' + from.x + ',' + from.y + 'Q' + arcPointX + ' ' + arcPointY + ',' + to.x + ' ' + to.y;
}

// 在transChart上画交通线路
function DrawTransInfo(transInfo) {
  for (var i = 0; i < transInfo.length; i++) {
    schedule = transInfo[i].ScheduleList;
    data = [];

    for (var j = 0; j < schedule.length; j++) {
      data.push({
        id: schedule[j].Identity,
        path: pointsToPath(chart.get(schedule[j].OriginCity), chart.get(schedule[j].DestinationCity))
      });
    }

    transChart.addSeries({
      name: transInfo[i].TransportType,
      type: 'mapline',
      lineWidth: 2,
      color: Highcharts.getOptions().colors[i],
      data: data
    });
  }
}

function AddChart(travel) {
  var pointArr = [CityData.find(x => x.id === travel.Path[0].FromCity)];
  for (var i = 0; i < travel.Path.length; i++) {
    if (travel.Path[i].ToCity != pointArr[pointArr.length-1].id) {
      pointArr.push(CityData.find(x => x.id === travel.Path[i].ToCity));
    }
  }

  chart.addSeries({
    name: travel.TravelerID,
    type: 'mappoint',
    lineWidth: 2,
    color: Highcharts.getOptions().colors[parseInt(travel.TravelerID.substring(2)) - 1],
    data: pointArr
  });

  chart.addSeries({
    name: travel.TravelerID + '位置',
    type: 'mappoint',
    color: Highcharts.getOptions().colors[3],
    data: [GetCurrPos(travel)],
    showInLegend: false,
    marker: {
      height: 40,
      width: 40,
      symbol: "url(/assets/user.svg)"
    }
  });
}

function GetCurrPos(travel) {
  var from = CityData.find(x => x.id === travel.Path[travel.CurrSection].FromCity);
  var to = CityData.find(x => x.id === travel.Path[travel.CurrSection].ToCity);
  var progress = (travel.CurrProgress + currStatus.Time[1]/20) / travel.Path[travel.CurrSection].SectionDuration;
  var currLat = from.lat * (1 - progress) + to.lat * progress;
  var currLon = from.lon * (1 - progress) + to.lon * progress;
  var ans = {lat: currLat, lon: currLon};
  return ans;
}

function AddCard(travel) {
  var newID = "travel-" + travel.TravelerID.slice(2);
  var newElement = `<div class="card bg-light travel-card" id="${newID}">
                      <div class="card-header d-flex justify-content-between align-items-center">
                        <span>${travel.TravelerID}</span><span>${travel.Path[0].FromCity}<i class="fas fa-angle-double-right animate-bounce-right"></i>${travel.Path[travel.Path.length-1].ToCity}</span>
                      </div>
                      <ul class="list-group list-group-flush">
                      </ul>
                      <div class="card-footer text-center">
                        共${travel.TotalDuration}小时，风险${travel.TotalRisk}
                      </div>
                    </div>`;
  $("#travel-panel").append(newElement);
  for (var i = 0; i < travel.Path.length; i++) {
    var newLi;
    var toTime = (travel.Path[i].StartTime + travel.Path[i].SectionDuration) % 24;
    var fromTimeStr = (travel.Path[i].StartTime > 9 ? "" : "0") + travel.Path[i].StartTime;
    var toTimeStr = (toTime > 9 ? "" : "0") + toTime;
    var liType;
    var statusIcon;
    if (i < travel.CurrSection) {
      liType = "list-group-item-success";
      statusIcon = "fa-check";
    } else if (i == travel.CurrSection) {
      liType = "list-group-item-primary";
      statusIcon = "fa-spinner animate-rotate";
    } else {
      liType = "list-group-item-secondary";
      statusIcon = "fa-hourglass-start";
    }

    if (travel.Path[i].TransportType == "停留") {
      newLi = `<li class="list-group-item ${liType}"><i class="fas ${statusIcon}"></i> ${fromTimeStr}:00-${toTimeStr}:00 停留${travel.Path[i].FromCity}<span class="badge badge-primary badge-pill">${travel.Path[i].SectionRisk}</span></li>`;
    } else {
      var transportIcon;
      switch (travel.Path[i].TransportType) {
        case "飞机":
          transportIcon = "fa-plane";
          break;
        case "火车":
          transportIcon = "fa-subway";
          break;
        case "汽车":
          transportIcon = "fa-bus";
          break;
      }
      newLi = `<li class="list-group-item ${liType}"><i class="fas ${statusIcon}"></i> ${fromTimeStr}:00-${toTimeStr}:00 ${travel.Path[i].FromCity}<i class="fas ${transportIcon}"></i>${travel.Path[i].ToCity} (${travel.Path[i].TransportID})<span class="badge badge-primary badge-pill">${travel.Path[i].SectionRisk}</span></li>`;
    }

    $("#" + newID + " ul").append(newLi);
  }
}

function UpdateCard(travel) {
  var ul = $("#travel-" + travel.TravelerID.slice(2) + " ul").children();
  for (var i = 0; i < travel.CurrSection; i++) {
    if (ul.eq(i).hasClass("list-group-item-success")) continue;
    ul.eq(i).removeClass("list-group-item-primary list-group-item-secondary");
    ul.eq(i).addClass("list-group-item-success");
    var icon = ul.eq(i).children().eq(0);
    icon.removeClass("fa-spinner animate-rotate fa-hourglass-start");
    icon.addClass("fa-check");
  }

  var currLi = ul.eq(travel.CurrSection);
  if (currLi.hasClass("list-group-item-secondary")) {
    currLi.removeClass("list-group-item-secondary");
    currLi.addClass("list-group-item-primary");
    currLi.children().eq(0).removeClass("fa-hourglass-start");
    currLi.children().eq(0).addClass("fa-spinner animate-rotate");
  }
}

function UpdateClock() {
  let hour = currStatus.Time[0];
  let twentieth = currStatus.Time[1];

  // 更新文字时钟
  $("#digital").text((hour > 9 ? "" : "0") + hour + ":00");

  // 更新图形时钟
  let hh = (hour + twentieth/20) * 30 % 360;
  let mm = twentieth * 18 % 360;
  if (hh == 0) hh = 360;
  if (mm == 0) mm = 360;

  if (clockData.hours.val != hh) {
    clockData.hours.val = hh;
    clockData.hours.el.classList.add('moving');
    clockData.hours.el.style.transform = `translate(-50%, -75%) rotate(${clockData.hours.val}deg)`;
  }

  if (clockData.minutes.val != mm) {
    clockData.minutes.val = mm;
    clockData.minutes.el.classList.add('moving');
    clockData.minutes.el.style.transform = `translate(-50%, -75%) rotate(${clockData.minutes.val}deg)`;
  }
}

$('input:radio').click(function(){
  if ($(this).attr('id') == 'customRadioInline2') {
    $('#form-time-limit').removeAttr("hidden");
    hasTimeLimit = true;
  } else {
    $('#form-time-limit').attr("hidden", "hidden");
    hasTimeLimit = false;
  }
});

function ClearForm() {
  $("#origin").val("请选择...");
  $("#destination").val("请选择...");
  $("#time-limit").val("");
}

function Exit() {
  $("body").hide();
  $.ajaxSettings.async = true;
  $.get("/api/exit");
  setTimeout("window.location.href = 'about:blank'", 1000);
}



/* ********************** */
/* 注意：以下为顺序执行部分 */
/* ********************** */

var chart;        // 主界面地图
var transChart;   // 交通线路图
var isRefreshing = true;
var currStatus;
var traveler = [];
var hasTimeLimit = false;
$.ajaxSettings.async = false;

// 初始化时钟
for (var i = 0; i < 60; i++) $(".clock").append("<div class=\"memory\"></div>");
$(".clock").append("<div class=\"needle\" id=\"hours\"></div>");
$(".clock").append("<div class=\"needle\" id=\"minutes\"></div>");
var clockData = {
  hours: {
    el: document.querySelector('#hours'),
    val: 0
  },
  minutes: {
    el: document.querySelector('#minutes'),
    val: 0
  }
};

clockData.minutes.el.addEventListener('transitionend', function() {
  clockData.hours.el.classList.remove('moving');
  clockData.minutes.el.classList.remove('moving');

  if (clockData.hours.val == 360) {
    clockData.hours.el.style.transform = 'translate(-50%, -75%) rotate(0deg)';
  }

  if (clockData.minutes.val == 360) {
    clockData.minutes.el.style.transform = 'translate(-50%, -75%) rotate(0deg)';
  }
});

Init();
setInterval("Refresh()", 500);