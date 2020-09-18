#include "server.h"

/* 构造函数 */
Server::Server()
{
    this->mListenerSettings = new QSettings("PCNinja", "Pokemon-server");
    this->mFileSettings = new QSettings("PCNinja", "Pokemon-server");
    this->mFileSettings->setValue("path", "://statics");
    this->mFileSettings->setValue("encoding", "UTF-8");
}

/* 监听 */
void Server::Listen(int port) const
{
    this->mListenerSettings->setValue("port", port);
    stefanfrings::StaticFileController* staticFileController = new stefanfrings::StaticFileController(this->mFileSettings);
    new stefanfrings::HttpListener(this->mListenerSettings, new HttpRequestHandler(staticFileController));
}

/* HTTP请求处理器 */
HttpRequestHandler::HttpRequestHandler(stefanfrings::StaticFileController* staticFileController, QObject* parent)
    : stefanfrings::HttpRequestHandler(parent)
{
    this->mStaticFileController = staticFileController;
}

/* 重写原service函数 */
void HttpRequestHandler::service(stefanfrings::HttpRequest &request, stefanfrings::HttpResponse &response)
{
    QByteArray path = request.getPath();
    if (path.startsWith("/api/"))
    {
        QJsonDocument document;
        if (path == "/api/register")
            document.setObject(this->Register(request.getParameter("userName"), request.getParameter("password")));
        else if (path == "/api/logIn")
            document.setObject(this->LogIn(request.getParameter("userName"), request.getParameter("password")));
        else if (path == "/api/logOut")
            document.setObject(this->LogOut(request.getParameter("userName")));
        else if (path == "/api/baseInfo")
            document.setObject(this->BaseInfo(request.getParameter("userName")));
        else if (path == "/api/pokemonList")
            document.setObject(this->PokemonList(request.getParameter("userName")));
        else if (path == "/api/userList")
            document.setObject(this->UserList());
        else if (path == "/api/opponentList")
            document.setObject(this->OpponentList());
        else if (path == "/api/refreshOpponentList")
            document.setObject(this->RefreshOpponentList());
        else if (path == "/api/startBattle")
            document.setObject(this->StartBattle(request.getParameter("userName"), BattleType(request.getParameter("type").toInt()),
                                                 request.getParameter("opponentID").toInt(), request.getParameter("userPokemonUUID")));
        else if (path == "/api/stepBattle")
            document.setObject(this->StepBattle(request.getParameter("battleID")));
        else if (path == "/api/skipBattle")
            document.setObject(this->SkipBattle(request.getParameter("battleID")));
        else if (path == "/api/finishBattle")
            document.setObject(this->FinishBattle(request.getParameter("battleID"), request.getParameter("discardPokemonUUID")));
        else
        {
            // API地址错误
            response.setStatus(404, "Not found");
            response.write("The URL of API is wrong.", true);
            return;
        }
        response.setHeader("Content-Type", "application/json; charset=utf-8");
        response.write(document.toJson(), true);
    }
    else
        this->mStaticFileController->service(request, response);    // 提供静态文件服务
}

/* 注册 */
QJsonObject HttpRequestHandler::Register(QString userName, QString password) const
{
    QJsonObject json;
    bool result = gDB->Register(userName, password);
    if (result)
    {
        PokemonFactory pokemonFactory;
        Pokemon* pPM;

        // 设置随机种子
        QTime time = QTime::currentTime();
        qsrand(time.msec() + time.second() * 1000);

        // 注册成功后随机发放3只1级精灵
        for (int i = 0; i < 3; ++i)
        {
            pPM = pokemonFactory.CreatePokemon(PokemonType(qrand() % 4), PokemonName(qrand() % 5), 1);
            gDB->AddPokemon(userName, pPM->GetInfo());
            delete pPM;
        }
    }
    json.insert("result", result);
    return json;
}

/* 登录 */
QJsonObject HttpRequestHandler::LogIn(QString userName, QString password) const
{
    QJsonObject json;
    json.insert("result", gDB->LogIn(userName, password));
    return json;
}

/* 登出 */
QJsonObject HttpRequestHandler::LogOut(QString userName) const
{
    QJsonObject json;
    gDB->LogOut(userName);
    json.insert("result", true);
    return json;
}

/* 用户基本信息 */
QJsonObject HttpRequestHandler::BaseInfo(QString userName) const
{
    QJsonObject json
    {
        {"pokemonNum", gDB->GetPokemonNum(userName)},
        {"advancedPokemonNum", gDB->GetAdvancedPokemonNum(userName)},
        {"matchNum", gDB->GetMatchNum(userName)},
        {"winNum", gDB->GetWinNum(userName)},
        {"loseNum", gDB->GetLoseNum(userName)}
    };
    return json;
}

/* 用户精灵列表 */
QJsonObject HttpRequestHandler::PokemonList(QString userName) const
{
    QJsonObject json;
    QJsonArray arr;
    QVector<PokemonInfo> pokemonList = gDB->GetPokemonList(userName);
    for (auto& it : pokemonList)
        arr.append(QJsonObject
                   {
                       {"UUID", it.UUID},
                       {"type", it.type},
                       {"name", it.name},
                       {"level", it.level},
                       {"exp", it.exp},
                       {"expToNextLevel", it.expToNextLevel},
                       {"attack", it.attack},
                       {"defense", it.defense},
                       {"HP", it.HP},
                       {"interval", it.interval}
                   }
        );
    json.insert("pokemonList", arr);
    return json;
}

/* 用户列表 */
QJsonObject HttpRequestHandler::UserList() const
{
    QJsonObject json;
    QJsonArray arr;
    QVector<UserInfo> userList = gDB->GetUserList();
    for (auto& it : userList)
        arr.append(QJsonObject
                   {
                       {"userName", it.userName},
                       {"isOnline", it.isOnline},
                       {"pokemonNum", it.pokemonNum},
                       {"advancedPokemonNum", it.advancedPokemonNum},
                       {"matchNum", it.matchNum},
                       {"winNum", it.winNum},
                       {"loseNum", it.loseNum}
                   }
        );
    json.insert("userList", arr);
    return json;
}

/* 虚拟对手列表 */
QJsonObject HttpRequestHandler::OpponentList() const
{
    QJsonObject json;
    QJsonArray arr;
    PokemonInfo info;
    for (int i = 0; i < 5; ++i)
    {
        info = gOpponentList[i]->GetInfo();
        arr.append(QJsonObject
                   {
                       {"opponentID", i},
                       {"type", info.type},
                       {"name", info.name},
                       {"level", info.level},
                       {"exp", info.exp},
                       {"expToNextLevel", info.expToNextLevel},
                       {"attack", info.attack},
                       {"defense", info.defense},
                       {"HP", info.HP},
                       {"interval", info.interval}
                   }
        );
    }
    json.insert("opponentList", arr);
    return json;
}

/* 刷新虚拟对手列表 */
QJsonObject HttpRequestHandler::RefreshOpponentList() const
{
    // 释放原虚拟对手
    for (auto& it : gOpponentList)
        delete it;
    gOpponentList.clear();

    // 创建新虚拟对手
    PokemonFactory pokemonFactory;
    for (int i = 0; i < 5; ++i)
        gOpponentList.append(pokemonFactory.CreatePokemon(PokemonType(qrand() % 4), PokemonName(qrand() % 5), qrand() % 8 + 1));

    QJsonObject json;
    json.insert("result", true);
    return json;
}

/* 开始新战斗，battleType：升级赛为0，决斗赛为1 */
QJsonObject HttpRequestHandler::StartBattle(QString userName, BattleType type, int opponentID, QString userPokemonUUID) const
{
    PokemonInfo userPokemonInfo = gDB->GetPokemonInfo(userName, userPokemonUUID);
    PokemonInfo opponentPokemonInfo = gOpponentList[opponentID]->GetInfo();

    PokemonFactory pokemonFactory;
    Pokemon* userPokemon = pokemonFactory.CreatePokemon(userPokemonInfo);
    Pokemon* opponentPokemon = pokemonFactory.CreatePokemon(opponentPokemonInfo);

    QString battleID = QUuid::createUuid().toString().mid(1, 36);
    gBattleList.insert(battleID, new Battle(userName, type, userPokemon, opponentPokemon));

    QJsonObject json;
    json.insert("battleID", battleID);
    return json;
}

/* 模拟下一步战斗 */
QJsonObject HttpRequestHandler::StepBattle(QString battleID) const
{
    if (gBattleList.find(battleID) == gBattleList.end())
        return QJsonObject{ {"error", true} };  // 查不到此战斗，返回错误信息

    Battle* pBattle = gBattleList.find(battleID).value();
    return pBattle->Step();
}

/* 跳过模拟 */
QJsonObject HttpRequestHandler::SkipBattle(QString battleID) const
{
    if (gBattleList.find(battleID) == gBattleList.end())
        return QJsonObject{ {"error", true} };  // 查不到此战斗，返回错误信息

    Battle* pBattle = gBattleList.find(battleID).value();
    QJsonObject stepResult, json;
    while (true)
    {
        stepResult = pBattle->Step();
        if (stepResult["oppositeCurrentHP"].toInt() == 0)
        {
            json.insert("isUserWin", true);
            break;
        }
        else if (stepResult["ourCurrentHP"].toInt() == 0)
        {
            json.insert("isUserWin", false);
            break;
        }
    }
    return json;
}

/* 结算战斗 */
QJsonObject HttpRequestHandler::FinishBattle(QString battleID, QString discardPokemonUUID) const
{
    if (gBattleList.find(battleID) == gBattleList.end())
        return QJsonObject{ {"error", true} };  // 查不到此战斗，返回错误信息

    Battle* pBattle = gBattleList.find(battleID).value();
    QJsonObject json = pBattle->Finish(discardPokemonUUID);
    delete pBattle;     // 释放战斗对象
    gBattleList.remove(battleID);
    return json;
}
