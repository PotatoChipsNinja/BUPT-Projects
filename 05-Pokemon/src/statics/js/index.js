function medalHTML(type, num) {
    var medalClass = "";
    var medalText = "";
    var medalIcon = "";
    if (type == 0) {
        medalIcon = "fa-medal";
        if (num < 5) {
            medalClass = "medal-gray";
            medalText = "未获得精灵数量奖章";
        } else if (num < 10) {
            medalClass = "medal-bronze";
            medalText = "精灵数量铜奖";
        } else if (num < 20) {
            medalClass = "medal-silver";
            medalText = "精灵数量银奖";
        } else {
            medalClass = "medal-gold";
            medalText = "精灵数量金奖";
        }
    } else {
        medalIcon = "fa-crown";
        if (num < 1) {
            medalClass = "medal-gray";
            medalText = "未获得高级精灵数量奖章";
        } else if (num < 3) {
            medalClass = "medal-bronze";
            medalText = "高级精灵数量铜奖";
        } else if (num < 5) {
            medalClass = "medal-silver";
            medalText = "高级精灵数量银奖";
        } else {
            medalClass = "medal-gold";
            medalText = "高级精灵数量金奖";
        }
    }
    return "<span class=\"medal " + medalClass + "\"  data-toggle=\"tooltip\" data-placement=\"bottom\" title=\"" + medalText + "\"><i class=\"fas " + medalIcon + "\"></i></span>";
}

$("#title-username").text(userName);
$.get("/api/baseInfo", { userName: userName }, function(data) {
    var winRate = data.matchNum == 0 ? 0 : (100 * data.winNum / data.matchNum);
    $("#match-num").text(data.matchNum);
    $("#win-num").text(data.winNum);
    $("#lose-num").text(data.loseNum);
    $("#win-rate").text(winRate.toFixed(1));
    $("#all-num").text(data.pokemonNum);
    $("#advanced-num").text(data.advancedPokemonNum);

    $("#all-num-medal").html(medalHTML(0, data.pokemonNum));
    $("#advanced-num-medal").html(medalHTML(1, data.advancedPokemonNum));
    $(function () {
        $('[data-toggle="tooltip"]').tooltip();
    });

    var dataset = [data.winNum, data.loseNum]
    if (data.winNum == 0 && data.loseNum == 0) {
        dataset = [-1, -1];
    }

    var chart = new Chart($("#win-rate-chart"), {
        type: 'pie',
        data: {
            datasets: [{
                data: dataset,
                backgroundColor: ['#ff6384', '#36a2eb']
            }],
            labels: ["胜利", "失利"]
        },
        options: {
            legend: {
                position: 'left'
            },
            tooltips: {
                callbacks: {
                    label: function(tooltipItem, data) {
                        var label = data.labels[tooltipItem.index] + ": ";
                        label += data.datasets[0].data[tooltipItem.index] == -1 ? "0" : data.datasets[0].data[tooltipItem.index]
                        return label;
                    }
                }
            }
        }
    });
}, "json");