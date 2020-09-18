$.get("/api/pokemonList", { userName: userName }, function(data) {
    $("#pokemon-num").text(data.pokemonList.length);
    for (var i = 0; i < data.pokemonList.length; i++) {
        var typeClass = "";
        var typeText = "";
        switch (data.pokemonList[i].type) {
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

        var nameClass = "";
        var nameText = "";
        switch (data.pokemonList[i].name) {
            case 0:
                nameClass = "card-img-bulbasaur";
                nameText = "妙蛙种子";
                break;
            case 1:
                nameClass = "card-img-charmander";
                nameText = "小火龙";
                break;
            case 2:
                nameClass = "card-img-squirtle";
                nameText = "杰尼龟";
                break;
            case 3:
                nameClass = "card-img-pikachu";
                nameText = "皮卡丘";
                break;
            case 4:
                nameClass = "card-img-gengar";
                nameText = "耿鬼";
                break;
        }
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
        <div class=\"row\"> \
          <h5 class=\"card-title my-1 ml-2\">" + nameText + " <span class=\"badge badge-pill badge-info\">Lv." + data.pokemonList[i].level + "</span></h5> \
        </div> \
        <div class=\"row\"> \
          <ul class=\"list-group list-group-flush p-0 col-5\"> \
            <li class=\"list-group-item py-1 px-0\"><span class=\"left-icon\"><i class=\"fas fa-dagger\"></i></span><strong>攻击力：</strong>" + data.pokemonList[i].attack + "</li> \
            <li class=\"list-group-item py-1 px-0\"><span class=\"left-icon\"><i class=\"fas fa-shield-alt\"></i></span><strong>防御力：</strong>" + data.pokemonList[i].defense + "</li> \
            <li class=\"list-group-item py-1 px-0\"><span class=\"left-icon\"><i class=\"fas fa-heartbeat\"></i></span><strong>生命值：</strong>" + data.pokemonList[i].HP + "</li> \
          </ul> \
          <ul class=\"list-group list-group-flush p-0 col-7\"> \
            <li class=\"list-group-item py-1 px-0\"><span class=\"left-icon\"><i class=\"fas fa-stopwatch\"></i></span><strong>攻击间隔：</strong>" + data.pokemonList[i].interval + " 秒</li> \
            <li class=\"list-group-item py-1 px-0\"><span class=\"left-icon\"><i class=\"fas fa-coins\"></i></span><strong>当前经验：</strong>" + data.pokemonList[i].exp + "</li> \
            <li class=\"list-group-item py-1 px-0\"><span class=\"left-icon\"><i class=\"fas fa-sort-size-up\"></i></span><strong>升级所需经验：</strong>" + data.pokemonList[i].expToNextLevel + "</li> \
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