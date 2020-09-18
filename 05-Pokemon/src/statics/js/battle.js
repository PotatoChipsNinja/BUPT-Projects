var battleType;
var opponentID;
var userPokemonUUID;
var battleID;
var isOver;

var opponentList;
var pokemonList;

var opponentPokemon = {
    name: "",
    totalHP: 0,
    currentHP: 0,
    debuff: "",
    specialAttackName: ""
};
var userPokemon = {
    name: "",
    totalHP: 0,
    currentHP: 0,
    debuff: "",
    specialAttackName: "",
    imgClass: "",
    type: "",
    typeClass: "",
    level: 0,
    attack: 0,
    defense: 0,
    HP: 0
};

var speed = 1.0;
var isPaused = false;
var timer;

loadOpponentList();
loadPokemonList();

/* ------------------------- TEMP ----------------------- */
/*
timer = setTimeout("stepBattle()", 1000 / speed);

opponentList = [
        {
            "HP": 34,
            "attack": 54,
            "defense": 23,
            "exp": 45,
            "expToNextLevel": 56,
            "interval": 6,
            "level": 7,
            "name": 1,
            "opponentID": 0,
            "type": 0
        },
        {
            "HP": 15,
            "attack": 12,
            "defense": 7,
            "exp": 0,
            "expToNextLevel": 5,
            "interval": 7,
            "level": 1,
            "name": 1,
            "opponentID": 1,
            "type": 0
        },
        {
            "HP": 24,
            "attack": 24,
            "defense": 10,
            "exp": 11,
            "expToNextLevel": 18,
            "interval": 6,
            "level": 3,
            "name": 4,
            "opponentID": 2,
            "type": 0
        },
        {
            "HP": 29,
            "attack": 8,
            "defense": 10,
            "exp": 5,
            "expToNextLevel": 11,
            "interval": 6,
            "level": 2,
            "name": 1,
            "opponentID": 3,
            "type": 1
        },
        {
            "HP": 50,
            "attack": 26,
            "defense": 24,
            "exp": 56,
            "expToNextLevel": 68,
            "interval": 3,
            "level": 8,
            "name": 4,
            "opponentID": 4,
            "type": 3
        }
    ];
pokemonList = [
        {
            "HP": 20,
            "UUID": "c09b6e1f-55b9-4f25-8171-737a9d175050",
            "attack": 9,
            "defense": 7,
            "exp": 0,
            "expToNextLevel": 5,
            "interval": 3,
            "level": 1,
            "name": 1,
            "type": 3
        },
        {
            "HP": 20,
            "UUID": "7dc0012f-376c-4d97-b4c6-275c5ccbbde9",
            "attack": 9,
            "defense": 12,
            "exp": 0,
            "expToNextLevel": 5,
            "interval": 7,
            "level": 1,
            "name": 3,
            "type": 2
        },
        {
            "HP": 16,
            "UUID": "f8e46a15-d058-46ac-bdcb-8a80aa311068",
            "attack": 8,
            "defense": 10,
            "exp": 0,
            "expToNextLevel": 5,
            "interval": 3,
            "level": 1,
            "name": 1,
            "type": 3
        }
    ];
chooseOpponent(1);
chooseUserPokemon("7dc0012f-376c-4d97-b4c6-275c5ccbbde9");
*/
/* ----------------- END TEMP ------------------ */

function parsePokemonName(name) {
    var nameClass = "";
    var nameText = "";
    var specialAttackName = "";
    switch (name) {
        case 0:
            nameClass = "card-img-bulbasaur";
            nameText = "妙蛙种子";
            specialAttackName = "寄生种子";
            break;
        case 1:
            nameClass = "card-img-charmander";
            nameText = "小火龙";
            specialAttackName = "喷射火焰";
            break;
        case 2:
            nameClass = "card-img-squirtle";
            nameText = "杰尼龟";
            specialAttackName = "水之波动";
            break;
        case 3:
            nameClass = "card-img-pikachu";
            nameText = "皮卡丘";
            specialAttackName = "十万伏特";
            break;
        case 4:
            nameClass = "card-img-gengar";
            nameText = "耿鬼";
            specialAttackName = "毒液冲击";
            break;
    }
    return {class: nameClass, text: nameText, specialAttackName: specialAttackName};
}

function parsePokemonType(type) {
    var typeClass = "";
    var typeText = "";
    switch (type) {
        case 0:
            typeClass = "card-text-strength";
            typeText = "力量型";
            break;
        case 1:
            typeClass = "card-text-tank";
            typeText = "肉盾型";
            break;
        case 2:
            typeClass = "card-text-defense";
            typeText = "防御型";
            break;
        case 3:
            typeClass = "card-text-swift";
            typeText = "敏捷型";
            break;
    }
    return {class: typeClass, text: typeText};
}

function chooseType(type) {
    battleType = type;
    $("#stage-1").attr("hidden", "hidden");
    $("#stage-2").removeAttr("hidden");
}

function chooseOpponent(id) {
    opponentID = id;
    var obj = opponentList.find(item => item.opponentID == id);
    var name = parsePokemonName(obj.name);
    var nameClass = name.class;
    var nameText = name.text;

    opponentPokemon.name = nameText;
    opponentPokemon.specialAttackName = name.specialAttackName;
    opponentPokemon.totalHP = obj.HP;
    opponentPokemon.currentHP = obj.HP;
    $("#opponent-pokemon-total-HP").text(obj.HP);
    $("#opponent-pokemon-current-HP").text(obj.HP);
    $("#opponent-pokemon-name").text(nameText);
    $("#opponent-pokemon-level").text(obj.level);
    $("#pokemon-opponent").addClass(nameClass);

    $("#stage-2").attr("hidden", "hidden");
    $("#stage-3").removeAttr("hidden");
}

function loadOpponentList() {
    $("#opponent-list").empty();
    $.get("/api/opponentList", function(data) {
        opponentList = data.opponentList;
        for (var i = 0; i < data.opponentList.length; i++) {
            var typeText = parsePokemonType(data.opponentList[i].type).text;
            var name = parsePokemonName(data.opponentList[i].name);
            var nameClass = name.class;
            var nameText = name.text;
            var html = `
<div class="card col-2 p-0">
  <div class="card-img-top ` + nameClass + ` mt-1"></div>
  <div class="card-body text-center py-2">
    <h5 class="card-title m-0">` + nameText + `</h5>
    <p class="card-text"><span class="badge badge-warning">` + typeText + `</span> <span class="badge badge-pill badge-info">Lv.` + data.opponentList[i].level + `</span></p>
  </div>
  <ul class="list-group list-group-flush text-center">
    <li class="list-group-item py-2 px-0"><i class="fas fa-dagger"></i> 攻击力：` + data.opponentList[i].attack + `</li>
    <li class="list-group-item py-2 px-0"><i class="fas fa-shield-alt"></i> 防御力：` + data.opponentList[i].defense + `</li>
    <li class="list-group-item py-2 px-0"><i class="fas fa-heartbeat"></i> 生命值：` + data.opponentList[i].HP + `</li>
    <li class="list-group-item py-2 px-0"><i class="fas fa-stopwatch"></i> 攻击间隔：` + data.opponentList[i].interval + ` 秒</li>
  </ul>
  <button type="button" class="btn btn-primary" onclick="chooseOpponent(` + data.opponentList[i].opponentID + `)">选择</button>
</div>
            `;
            $("#opponent-list").append(html);
        }
    }, "json");
}

function refreshOpponentList() {
    $.get("/api/refreshOpponentList", function(data) {
        loadOpponentList();
    }, "json");
}

function loadPokemonList() {
    $.get("/api/pokemonList", { userName: userName }, function(data) {
        pokemonList = data.pokemonList;
        for (var i = 0; i < data.pokemonList.length; i++) {
            var type = parsePokemonType(data.pokemonList[i].type);
            var typeClass = type.class;
            var typeText = type.text;

            var name = parsePokemonName(data.pokemonList[i].name);
            var nameClass = name.class;
            var nameText = name.text;

            var html = "\
<div class=\"card bg-light mb-3 card-pokemon\"> \
  <div class=\"row no-gutters\"> \
    <div class=\"col-md-3 card-left\"> \
      <div class=\"card-left-img\"> \
        <div class=\"card-left-img-bg " + nameClass + "\"></div> \
      </div> \
      <div class=\"card-left-text " + typeClass + "\">" + typeText + "</div> \
    </div> \
    <div class=\"col-md-9\"> \
      <div class=\"card-body py-2\"> \
        <div class=\"row d-flex justify-content-between\"> \
          <div> \
            <h5 class=\"card-title my-1 ml-2\">" + nameText + " <span class=\"badge badge-pill badge-info\">Lv." + data.pokemonList[i].level + "</span></h5> \
          </div> \
          <div> \
            <button type=\"button\" class=\"btn btn-primary px-2 py-1\" onclick=\"chooseUserPokemon('" + data.pokemonList[i].UUID + "');\">选择</button> \
          </div> \
        </div> \
        <div class=\"row\"> \
          <ul class=\"list-group list-group-flush p-0 col-5\"> \
            <li class=\"list-group-item py-1 px-0\"><span class=\"left-icon-sm\"><i class=\"fas fa-dagger\"></i></span><strong>攻击力：</strong>" + data.pokemonList[i].attack + "</li> \
            <li class=\"list-group-item py-1 px-0\"><span class=\"left-icon-sm\"><i class=\"fas fa-shield-alt\"></i></span><strong>防御力：</strong>" + data.pokemonList[i].defense + "</li> \
            <li class=\"list-group-item py-1 px-0\"><span class=\"left-icon-sm\"><i class=\"fas fa-heartbeat\"></i></span><strong>生命值：</strong>" + data.pokemonList[i].HP + "</li> \
          </ul> \
          <ul class=\"list-group list-group-flush p-0 col-7\"> \
            <li class=\"list-group-item py-1 px-0\"><span class=\"left-icon-sm\"><i class=\"fas fa-stopwatch\"></i></span><strong>攻击间隔：</strong>" + data.pokemonList[i].interval + " 秒</li> \
            <li class=\"list-group-item py-1 px-0\"><span class=\"left-icon-sm\"><i class=\"fas fa-coins\"></i></span><strong>当前经验：</strong>" + data.pokemonList[i].exp + "</li> \
            <li class=\"list-group-item py-1 px-0\"><span class=\"left-icon-sm\"><i class=\"fas fa-sort-size-up\"></i></span><strong>升级所需经验：</strong>" + data.pokemonList[i].expToNextLevel + "</li> \
          </ul> \
        </div> \
      </div> \
    </div> \
  </div> \
</div> \
            ";
            $("#pokemon-list").append(html);
        }
    }, "json");
}

function chooseUserPokemon(UUID) {
    userPokemonUUID = UUID;
    var obj = pokemonList.find(item => item.UUID == UUID);
    var name = parsePokemonName(obj.name);
    var nameClass = name.class;
    var nameText = name.text;

    userPokemon.type = parsePokemonType(obj.type).text;
    userPokemon.typeClass = parsePokemonType(obj.type).class;
    userPokemon.level = obj.level;
    userPokemon.attack = obj.attack;
    userPokemon.defense = obj.defense;
    userPokemon.HP = obj.HP;
    userPokemon.imgClass = nameClass;
    userPokemon.name = nameText;
    userPokemon.specialAttackName = name.specialAttackName;
    userPokemon.totalHP = obj.HP;
    userPokemon.currentHP = obj.HP;
    $("#user-pokemon-total-HP").text(obj.HP);
    $("#user-pokemon-current-HP").text(obj.HP);
    $("#user-pokemon-name").text(nameText);
    $("#user-pokemon-level").text(obj.level);
    $("#pokemon-user").addClass(nameClass);

    $.get("/api/startBattle", { userName: userName, type: battleType, opponentID: opponentID, userPokemonUUID: userPokemonUUID }, function(data) {
        battleID = data.battleID;
        isOver = false;
        $("#stage-3").attr("hidden", "hidden");
        $("#stage-4").removeAttr("hidden");
        timer = setTimeout("stepBattle()", 1000 / speed);
    }, "json");
}

function changeSpeed() {
    var input = $("#speed-input").val();
    var row = 1.897 * Math.exp(0.03 * input - 2) - 0.156739;
    speed = row.toFixed(1);
    $("#speed-output").text(row.toFixed(1));
    clearTimeout(timer);
    if (!isPaused)
        timer = setTimeout("stepBattle()", 1000 / speed);
}

function resetSpeed() {
    speed = 1.0;
    $("#speed-input").val(50);
    $("#speed-output").text("1.0");
    clearTimeout(timer);
    if (!isPaused)
        timer = setTimeout("stepBattle()", 1000 / speed);
}

function pauseSimulation() {
    clearTimeout(timer);
    isPaused = true;
    $("#pause-button").removeClass("btn-warning");
    $("#pause-button").addClass("btn-danger");
    $("#pause-button").html(`<i class="fas fa-play"></i> 继续模拟`);
    $("#pause-button").attr("onclick", "continueSimulation();");
}

function continueSimulation() {
    clearTimeout(timer);
    timer = setTimeout("stepBattle()", 1000 / speed);
    isPaused = false;
    $("#pause-button").removeClass("btn-danger");
    $("#pause-button").addClass("btn-warning");
    $("#pause-button").html(`<i class="fas fa-pause"></i> 暂停模拟`);
    $("#pause-button").attr("onclick", "pauseSimulation();");
}

function skip() {
    if (isOver)
        return;
    clearTimeout(timer);
    $.get("/api/skipBattle", { battleID: battleID }, function(data) {
        if (!data.error)
        {
            if (data.isUserWin) {
                // 用户胜利
                isOver = true;
                win();
            } else {
                // 对手胜利
                isOver = true;
                if (battleType == 0) {
                    // 升级赛
                    $.get("/api/finishBattle", {battleID: battleID}, "json");
                    $("#stage-4").attr("hidden", "hidden");
                    $("#stage-lose-upgrade").removeAttr("hidden");
                } else {
                    // 决斗赛
                    $("#stage-4").attr("hidden", "hidden");
                    $("#stage-lose-duel").removeAttr("hidden");
                    loadDiscardList();
                }
            }
        }
    }, "json");
}

function stepBattle() {
    if (isOver)
        return;
    $.get("/api/stepBattle", { battleID: battleID }, function(data) {
        if (!data.error)
        {
            var timeOut = 0;
            if (data.ourDebuffHurt > 0) {
                setTimeout(function() {getDubuffHurt(0, data.ourDebuffHurt)}, timeOut / speed);
                timeOut += 200;
            }
            if (data.oppositeDebuffHurt > 0) {
                setTimeout(function() {getDubuffHurt(1, data.oppositeDebuffHurt)}, timeOut / speed);
                timeOut += 200;
            }
            if (data.ourAttack.attackType > 0) {
                setTimeout(function() {attack(0, data.ourAttack)}, timeOut / speed);
                timeOut += 200;
            }
            if (data.oppositeAttack.attackType > 0) {
                setTimeout(function() {attack(1, data.oppositeAttack)}, timeOut / speed);
                timeOut += 200;
            }

            if (data.oppositeCurrentHP == 0) {
                // 用户胜利
                isOver = true;
                setTimeout(function() {
                    logger(opponentPokemon.name + "(对方)倒下了，战斗胜利");
                    $("#pokemon-opponent").animate({"opacity": '0'}, 200);
                    setTimeout(function() {win();}, 1000);
                }, timeOut / speed);
            } else if (data.ourCurrentHP == 0) {
                // 对手胜利
                isOver = true;
                setTimeout(function() {
                    logger(userPokemon.name + "(我方)倒下了，战斗失败");
                    $("#pokemon-user").animate({"opacity": '0'}, 200);
                    setTimeout(function() {
                        if (battleType == 0) {
                            // 升级赛
                            $.get("/api/finishBattle", {battleID: battleID}, "json");
                            $("#stage-4").attr("hidden", "hidden");
                            $("#stage-lose-upgrade").removeAttr("hidden");
                        } else {
                            // 决斗赛
                            $("#stage-4").attr("hidden", "hidden");
                            $("#stage-lose-duel").removeAttr("hidden");
                            loadDiscardList();
                        }
                    }, 1000);
                }, timeOut / speed);
            } else {
                clearTimeout(timer);
                if (!isPaused) {
                    timer = setTimeout("stepBattle()", 1000 / speed);
                }
            }
        }
    }, "json");

    $("#clock").text(('0' + (parseInt($("#clock").text()) + 1)).slice(-2));
}

function getDubuffHurt(side, hurt) {
    var hurtSide = side == 1 ? opponentPokemon.name+"(对方)" : userPokemon.name+"(我方)";
    var debuffName = side == 1 ? opponentPokemon.debuff : userPokemon.debuff;
    adjustHP(side, -hurt);
    if (debuffName == "寄生") {
        adjustHP(1-side, hurt);
        logger(hurtSide + "因【寄生】被吸收了" + hurt + "点生命值");
    } else {
        logger(hurtSide + "因【" + debuffName + "】受到了" + hurt + "点伤害");
    }
}

function attack(side, info) {
    var attackSide = side == 1 ? opponentPokemon.name+"(对方)" : userPokemon.name+"(我方)";
    var hurtSide = side == 0 ? opponentPokemon.name+"(对方)" : userPokemon.name+"(我方)";
    var attackObj = side == 0 ? $("#pokemon-user") : $("#pokemon-opponent");
    var animateName = side == 0 ? "to-right" : "to-left";
    var specialAttackName = side == 0 ? userPokemon.specialAttackName : opponentPokemon.specialAttackName;
    var logStr = attackSide + "发动了";
    if (info.attackType == 1) {
        logStr += "【普通攻击】";
    } else {
        logStr += "【特殊攻击：" + specialAttackName + "】";
    }

    if (info.paralyzed) {
        logStr += "，但因【麻痹】而无法动弹";
    } else {
        if (info.criticalStrike) {
            logStr += "，且打出了【暴击】";
        }
        if (info.miss) {
            logStr += "，但被" + hurtSide + "闪避了";
            animateHPInfo(1-side, 0);
        } else {
            logStr += "，对" + hurtSide + "造成了" + info.hurt + "点伤害";
            adjustHP(1-side, -info.hurt);
        }

        attackObj.css("animation", animateName + " " + (400 / speed) + "ms");
        setTimeout(function() {attackObj.css("animation", "");}, 400 / speed);
    }
    logger(logStr);

    if (info.giveDebuff != "") {
        getDebuff(1-side, info.giveDebuff);
    }
    if (specialAttackName == "水之波动" && info.attackType == 2 && !info.paralyzed) {
        clearDebuff(side);
    }
}

function getDebuff(side, name) {
    var sideStr = side == 1 ? opponentPokemon.name+"(对方)" : userPokemon.name+"(我方)";
    var html;
    switch (name) {
        case "寄生":
            html = `<span class="badge badge-success">寄生</span>`;
            break;
        case "烧伤":
            html = `<span class="badge badge-danger">烧伤</span>`;
            break;
        case "麻痹":
            html = `<span class="badge badge-warning">麻痹</span>`;
            break;
        case "中毒":
            html = `<span class="badge badge-dark">中毒</span>`;
            break;
    }
    if (side == 0) {
        userPokemon.debuff = name;
        $("#user-pokemon-debuff").html(html);
    } else {
        opponentPokemon.debuff = name;
        $("#opponent-pokemon-debuff").html(html);
    }
    logger(sideStr + "进入了【" + name + "】状态");
}

function clearDebuff(side) {
    var sideStr = side == 1 ? opponentPokemon.name+"(对方)" : userPokemon.name+"(我方)";
    if (side == 0) {
        userPokemon.debuff = "";
        $("#user-pokemon-debuff").empty();
    } else {
        opponentPokemon.debuff = "";
        $("#opponent-pokemon-debuff").empty();
    }
    logger(sideStr + "解除了自己的异常状态");
}

function logger(str) {
    $("#battle-log").text($("#battle-log").text() + "\n• " + str);
    $("#battle-log").scrollTop($("#battle-log")[0].scrollHeight);
}

function adjustHP(side, HP) {
    animateFlash(side);
    animateHPInfo(side, HP);
    if (side == 0) {
        userPokemon.currentHP += HP;
        if (userPokemon.currentHP > userPokemon.totalHP)
            userPokemon.currentHP = userPokemon.totalHP;
        if (userPokemon.currentHP < 0)
            userPokemon.currentHP = 0;
        $("#user-pokemon-current-HP").text(userPokemon.currentHP);
        $("#user-pokemon-HP-bar").animate({"width": (100 * userPokemon.currentHP / userPokemon.totalHP) + "%"}, 100 / speed);
    } else {
        opponentPokemon.currentHP += HP;
        if (opponentPokemon.currentHP > opponentPokemon.totalHP)
            opponentPokemon.currentHP = opponentPokemon.totalHP;
        if (opponentPokemon.currentHP < 0)
            opponentPokemon.currentHP = 0;
        $("#opponent-pokemon-current-HP").text(opponentPokemon.currentHP);
        $("#opponent-pokemon-HP-bar").animate({"width": (100 * opponentPokemon.currentHP / opponentPokemon.totalHP) + "%"}, 100 / speed);
    }
}

function animateFlash(side) {
    var obj = side == 0 ? $("#pokemon-user") : $("#pokemon-opponent");
    obj.animate({"opacity": '0.5'}, 100 / speed);
    setTimeout(function() {obj.animate({"opacity": '1'}, 100 / speed);}, 100 / speed);
}

function animateHPInfo(side, HP) {
    var obj = side == 0 ? $("#user-pokemon-HP-info") : $("#opponent-pokemon-HP-info");
    obj.stop(true);
    obj.css("opacity", '1');
    obj.css("font-size", '0');
    if (HP > 0) {
        obj.text('+' + HP);
        obj.css("color", "green");
    } else if (HP < 0) {
        obj.text(HP);
        obj.css("color", "red");
    } else {
        obj.text("MISS");
        obj.css("color", "red");
    }
    obj.animate({"font-size": "1.2rem"}, 100 / speed);
    setTimeout(function() {obj.animate({"opacity": '0'}, 200 / speed);}, 200 / speed);
    setTimeout(function() {obj.text("");}, 400 / speed);
}

function loadDiscardList() {
    var pool = pokemonList.slice(0);
    var selected = [];
    for (var i = 0; i < 3 && pool.length > 0; i++) {
        selected.push(pool.splice(Math.floor(Math.random() * pool.length), 1)[0]);
    }

    for (var i = 0; i < selected.length; i++) {
        var typeText = parsePokemonType(selected[i].type).text;
        var name = parsePokemonName(selected[i].name);
        var nameClass = name.class;
        var nameText = name.text;
        var html = `
<div class="card col-2 p-0 mx-3">
  <div class="card-img-top ` + nameClass + ` mt-1"></div>
  <div class="card-body text-center py-2">
    <h5 class="card-title m-0">` + nameText + `</h5>
    <p class="card-text"><span class="badge badge-warning">` + typeText + `</span> <span class="badge badge-pill badge-info">Lv.` + selected[i].level + `</span></p>
  </div>
  <ul class="list-group list-group-flush text-center">
    <li class="list-group-item py-2 px-0"><i class="fas fa-dagger"></i> 攻击力：` + selected[i].attack + `</li>
    <li class="list-group-item py-2 px-0"><i class="fas fa-shield-alt"></i> 防御力：` + selected[i].defense + `</li>
    <li class="list-group-item py-2 px-0"><i class="fas fa-heartbeat"></i> 生命值：` + selected[i].HP + `</li>
    <li class="list-group-item py-2 px-0"><i class="fas fa-stopwatch"></i> 攻击间隔：` + selected[i].interval + ` 秒</li>
  </ul>
  <button type="button" class="btn btn-primary" onclick="discard('` + selected[i].UUID + `', '` + nameText + `')">选择</button>
</div>
        `;
        $("#discard-list").append(html);
    }
}

function discard(UUID, name) {
    $.get("/api/finishBattle", { battleID: battleID, discardPokemonUUID: UUID }, function(data) {
        if (!data.error)
        {
            $("#discarded-pokemon-name").text(name);
            if (pokemonList.length == 1) {
                $("#discard-all").removeAttr("hidden");
            }
            $("#to-discard").attr("hidden", "hidden");
            $("#has-discard").removeAttr("hidden");
        }
    }, "json");
}

function win() {
    $.get("/api/finishBattle", {battleID: battleID}, function(data) {
        if (!data.error)
        {
            $("#stage-4").attr("hidden", "hidden");
            $("#stage-win").removeAttr("hidden");
            if (battleType == 1) {
                $("#duel-win-text").removeAttr("hidden");
            }
            $("#win-pokemon").text(userPokemon.name);
            $("#get-exp").text(data.getExp);
            $("#card-pokemon-finish").html(`
<div class="row no-gutters">
  <div class="col-md-3 card-left">
    <div class="card-left-img pt-2">
      <div class="card-left-img-bg ` + userPokemon.imgClass + `"></div>
    </div>
    <div class="card-left-text ` + userPokemon.typeClass + `">` + userPokemon.type + `</div>
  </div>
  <div class="col-md-9">
    <div class="card-body py-2">
      <div class="row">
        <h5 class="card-title my-1 ml-2">` + userPokemon.name + ` <span class="badge badge-pill badge-info">Lv.` + userPokemon.level + `</span> ` + (data.level > userPokemon.level ? `<i class="fas fa-angle-double-right animate-bounce-right"></i> <span class="badge badge-pill badge-info">Lv.` + data.level + `</span>` : ``) + `</h5>
      </div>
      <div class="row">
        <ul class="list-group list-group-flush p-0 col-6">
          <li class="list-group-item py-1 px-0"><span class="left-icon"><i class="fas fa-dagger"></i></span><strong>攻击力：</strong>` + data.attack + (data.attack > userPokemon.attack ? ` <span style="color: red;">(<i class="fas fa-arrow-alt-up"></i>` + (data.attack - userPokemon.attack) + `)</span>` : ``) + `</li>
          <li class="list-group-item py-1 px-0"><span class="left-icon"><i class="fas fa-shield-alt"></i></span><strong>防御力：</strong>` + data.defense + (data.defense > userPokemon.defense ? ` <span style="color: red;">(<i class="fas fa-arrow-alt-up"></i>` + (data.defense - userPokemon.defense) + `)</span>` : ``) + `</li>
          <li class="list-group-item py-1 px-0"><span class="left-icon"><i class="fas fa-heartbeat"></i></span><strong>生命值：</strong>` + data.HP + (data.HP > userPokemon.HP ? ` <span style="color: red;">(<i class="fas fa-arrow-alt-up"></i>` + (data.HP - userPokemon.HP) + `)</span>` : ``) + `</li>
        </ul>
        <ul class="list-group list-group-flush p-0 col-6">
          <li class="list-group-item py-1 px-0"><span class="left-icon"><i class="fas fa-stopwatch"></i></span><strong>攻击间隔：</strong>` + data.interval + ` 秒</li>
          <li class="list-group-item py-1 px-0"><span class="left-icon"><i class="fas fa-coins"></i></span><strong>当前经验：</strong>` + data.exp + `</li>
          <li class="list-group-item py-1 px-0"><span class="left-icon"><i class="fas fa-sort-size-up"></i></span><strong>升级所需经验：</strong>` + data.expToNextLevel + `</li>
        </ul>
      </div>
    </div>
  </div>
</div>
            `);
        }
    }, "json");
}