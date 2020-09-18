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

$.get("/api/userList", function(data) {
    var onlineUserNum = 0;
    for (var i = 0; i < data.userList.length; i++) {
        var winRate = data.userList[i].matchNum == 0 ? 0 : (100 * data.userList[i].winNum / data.userList[i].matchNum);
        data.userList[i]["winRate"] = winRate.toFixed(1);
        if (data.userList[i].isOnline)
            onlineUserNum++;
    }

    $("#all-user-num").text(data.userList.length);
    $("#online-user-num").text(onlineUserNum);

    new gridjs.Grid({
        columns: [{
            id: 'userName',
            name: '用户名'
        }, {
            id: 'isOnline',
            name: '在线',
            formatter: (cell) => cell ? gridjs.html(`<i class="fas fa-check icon-green"></i>`) : gridjs.html(`<i class="fas fa-times icon-red"></i>`)
        }, {
            id: 'matchNum',
            name: '战斗场次'
        }, {
            id: 'winNum',
            name: '胜利场次'
        }, {
            id: 'loseNum',
            name: '失利场次'
        }, {
            id: 'winRate',
            name: '胜率',
            formatter: (cell) => cell + "%"
        }, {
            id: 'pokemonNum',
            name: '精灵数量',
            formatter: (cell) => gridjs.html(cell + medalHTML(0, cell))
        }, {
            id: 'advancedPokemonNum',
            name: '高级精灵数量',
            formatter: (cell) => gridjs.html(cell + medalHTML(1, cell))
        }],
        data: data.userList,
        autoWidth: true,
        sort: true,
        search: {
            selector: (cell, rowIndex, cellIndex) => cellIndex === 0 ? cell : ''
        },
        className: {
            th: 'px-0 py-3 text-center',
            td: 'px-0 py-2 text-center'
        }
    }).render(document.getElementById("table"));

    $(function () {
        $('[data-toggle="tooltip"]').tooltip();
    });

    $(".gridjs-search-input").attr("placeholder", "搜索用户...");
    $(".gridjs-search-input").attr("aria-label", "搜索用户...");
}, "json");