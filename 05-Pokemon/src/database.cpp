#include "database.h"

/* 初始化数据库模块 */
Database::Database()
{
    this->mDB = QSqlDatabase::addDatabase("QSQLITE");
    this->mDB.setDatabaseName(QCoreApplication::applicationDirPath() + "/storage.db");
}

/* 断开数据库连接 */
Database::~Database()
{
    mDB.close();
    qInfo("数据库连接已断开");
}

/* 连接数据库，返回连接结果 */
bool Database::Connect()
{
    if (this->mDB.open())
    {
        qInfo("数据库连接成功");
        this->mQuery = new QSqlQuery;
    }
    else
    {
        qInfo("无法连接到数据库");
        return false;
    }

    // 初始化Users表
    this->mQuery->exec("CREATE TABLE Users (UserName TEXT, Password CHAR(32), IsOnline BOOLEAN, "
                "PokemonNum INT, AdvancedPokemonNum INT, Match INT, Win INT, Lose INT)");
    return true;
}

/* 用户注册，返回注册结果 */
bool Database::Register(QString userName, QString password) const
{
    this->mQuery->exec("SELECT * FROM Users WHERE UserName LIKE '" + userName + "'");
    if (this->mQuery->next())  // 已经注册过
        return false;

    QString passwordMD5;    // 存储密码的MD5
    passwordMD5.append(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex());
    this->mQuery->exec("INSERT INTO Users VALUES ('" + userName + "', '" + passwordMD5 + "', false, 0, 0, 0, 0, 0)");
    this->mQuery->exec("CREATE TABLE " + userName + " (UUID CHAR(36), Type INT, Name INT, Level INT, Exp INT, " +
                "ExpToNextLevel INT, Attack INT, Defense INT, HP INT, Interval INT)");
    qInfo("新用户注册：%s", userName.toStdString().data());
    return true;    // 注册成功
}

/* 用户登录，返回登录结果 */
bool Database::LogIn(QString userName, QString password) const
{
    this->mQuery->exec("SELECT * FROM Users WHERE UserName='" + userName + "'");
    if (!this->mQuery->next())
        return false;   // 没有查询到该用户

    QString passwordMD5;    // 存储密码的MD5
    passwordMD5.append(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex());
    if (this->mQuery->value(1).toString() != passwordMD5)
        return false;   // 密码错误

    // 密码正确
    this->mQuery->exec("UPDATE Users SET IsOnline=true WHERE UserName='" + userName + "'");    // 将IsOnline键值设为true
    qInfo("用户登录：%s", userName.toStdString().data());
    return true;
}

/* 用户登出 */
void Database::LogOut(QString userName) const
{
    this->mQuery->exec("UPDATE Users SET IsOnline=false WHERE UserName='" + userName + "'");    // 将IsOnline键值设为false
    qInfo("用户登出：%s", userName.toStdString().data());
}

/* 获取用户精灵列表 */
QVector<PokemonInfo> Database::GetPokemonList(QString userName) const
{
    PokemonInfo info;
    QVector<PokemonInfo> list;

    this->mQuery->exec("SELECT * FROM " + userName);
    while (this->mQuery->next())
    {
        info.UUID = this->mQuery->value(0).toString();
        info.type = PokemonType(this->mQuery->value(1).toInt());
        info.name = PokemonName(this->mQuery->value(2).toInt());
        info.level = this->mQuery->value(3).toInt();
        info.exp = this->mQuery->value(4).toInt();
        info.expToNextLevel = this->mQuery->value(5).toInt();
        info.attack = this->mQuery->value(6).toInt();
        info.defense = this->mQuery->value(7).toInt();
        info.HP = this->mQuery->value(8).toInt();
        info.interval = this->mQuery->value(9).toInt();
        list.append(info);  // 加入列表
    }
    return list;
}

/* 获取用户精灵信息 */
PokemonInfo Database::GetPokemonInfo(QString userName, QString UUID) const
{
    PokemonInfo info;
    this->mQuery->exec("SELECT * FROM " + userName + " WHERE UUID='" + UUID + "'");
    this->mQuery->next();
    info.UUID = this->mQuery->value(0).toString();
    info.type = PokemonType(this->mQuery->value(1).toInt());
    info.name = PokemonName(this->mQuery->value(2).toInt());
    info.level = this->mQuery->value(3).toInt();
    info.exp = this->mQuery->value(4).toInt();
    info.expToNextLevel = this->mQuery->value(5).toInt();
    info.attack = this->mQuery->value(6).toInt();
    info.defense = this->mQuery->value(7).toInt();
    info.HP = this->mQuery->value(8).toInt();
    info.interval = this->mQuery->value(9).toInt();
    return info;
}

/* 添加用户精灵 */
void Database::AddPokemon(QString userName, PokemonInfo info) const
{
    QString queryString;
    queryString.sprintf("INSERT INTO %s VALUES ('%s', %d, %d, %d, %d, %d, %d, %d, %d, %d)",
                        userName.toStdString().data(), info.UUID.toStdString().data(), info.type,
                        info.name, info.level, info.exp, info.expToNextLevel, info.attack, info.defense,
                        info.HP, info.interval);
    this->mQuery->exec(queryString);

    int pokemonNum = this->GetPokemonNum(userName) + 1;
    int advancedPokemonNum = this->GetAdvancedPokemonNum(userName);
    if (info.level == 15)
        advancedPokemonNum++;
    queryString.sprintf("UPDATE Users SET PokemonNum=%d, AdvancedPokemonNum=%d WHERE UserName='%s'",
                        pokemonNum, advancedPokemonNum, userName.toStdString().data());
    this->mQuery->exec(queryString);
}

/* 移除用户精灵 */
void Database::DelPokemon(QString userName, QString UUID) const
{
    int pokemonNum = this->GetPokemonNum(userName) - 1;
    int advancedPokemonNum = this->GetAdvancedPokemonNum(userName);
    this->mQuery->exec("SELECT Level FROM " + userName + " WHERE UUID='" + UUID + "'");    // 获取精灵原等级
    this->mQuery->next();
    if (this->mQuery->value(0).toInt() == 15)
        advancedPokemonNum--;

    QString queryString;
    queryString.sprintf("UPDATE Users SET PokemonNum=%d, AdvancedPokemonNum=%d WHERE UserName='%s'",
                        pokemonNum, advancedPokemonNum, userName.toStdString().data());
    this->mQuery->exec(queryString);   // 更新Users表

    this->mQuery->exec("DELETE FROM " + userName + " WHERE UUID='" + UUID + "'");
}

/* 更新用户精灵 */
void Database::UpdatePokemon(QString userName, QString UUID, PokemonInfo info) const
{
    this->mQuery->exec("SELECT Level FROM " + userName + " WHERE UUID='" + UUID + "'");    // 获取精灵原等级
    this->mQuery->next();
    int level = this->mQuery->value(0).toInt();    // 精灵原等级

    QString queryString;
    if (info.level == 15 && level < 15) // 若更新后的精灵等级升至15级，则需要更新Users表中的AdvancedPokemonNum项
    {
        int advancedPokemonNum = this->GetAdvancedPokemonNum(userName) + 1;
        queryString.sprintf("UPDATE Users SET AdvancedPokemonNum=%d WHERE UserName='%s'",
                            advancedPokemonNum, userName.toStdString().data());
        this->mQuery->exec(queryString);   // 更新Users表
    }

    queryString.sprintf("UPDATE %s SET Level=%d, Exp=%d, ExpToNextLevel=%d, Attack=%d, Defense=%d, HP=%d, Interval=%d WHERE UUID='%s'",
                        userName.toStdString().data(), info.level, info.exp, info.expToNextLevel, info.attack,
                        info.defense, info.HP, info.interval, UUID.toStdString().data());
    this->mQuery->exec(queryString);
}

/* 增加用户比赛场次 */
void Database::AddMatchNum(QString userName, bool isWin) const
{
    this->mQuery->exec("SELECT Match, Win, Lose FROM Users WHERE UserName='" + userName + "'");
    this->mQuery->next();

    int matchNum = this->mQuery->value(0).toInt() + 1;
    int winNum = this->mQuery->value(1).toInt();
    int loseNum = this->mQuery->value(2).toInt();
    if (isWin)
        winNum++;
    else
        loseNum++;

    QString queryString;
    queryString.sprintf("UPDATE Users SET Match=%d, Win=%d, Lose=%d WHERE UserName='%s'",
                        matchNum, winNum, loseNum, userName.toStdString().data());
    this->mQuery->exec(queryString);
}

/* 获取用户列表 */
QVector<UserInfo> Database::GetUserList() const
{
    UserInfo info;
    QVector<UserInfo> list;

    this->mQuery->exec("SELECT * FROM Users");
    while (this->mQuery->next())
    {
        info.userName = this->mQuery->value(0).toString();
        info.isOnline = this->mQuery->value(2).toBool();
        info.pokemonNum = this->mQuery->value(3).toInt();
        info.advancedPokemonNum = this->mQuery->value(4).toInt();
        info.matchNum = this->mQuery->value(5).toInt();
        info.winNum = this->mQuery->value(6).toInt();
        info.loseNum = this->mQuery->value(7).toInt();
        list.append(info);  // 加入列表
    }
    return list;
}

/* 获取用户精灵数量 */
int Database::GetPokemonNum(QString userName) const
{
    this->mQuery->exec("SELECT PokemonNum FROM Users WHERE UserName='" + userName + "'");
    this->mQuery->next();
    return this->mQuery->value(0).toInt();
}

/* 获取用户高级精灵数量 */
int Database::GetAdvancedPokemonNum(QString userName) const
{
    this->mQuery->exec("SELECT AdvancedPokemonNum FROM Users WHERE UserName='" + userName + "'");
    this->mQuery->next();
    return this->mQuery->value(0).toInt();
}

/* 获取用户比赛次数 */
int Database::GetMatchNum(QString userName) const
{
    this->mQuery->exec("SELECT Match FROM Users WHERE UserName='" + userName + "'");
    this->mQuery->next();
    return this->mQuery->value(0).toInt();
}

/* 获取用户胜利次数 */
int Database::GetWinNum(QString userName) const
{
    this->mQuery->exec("SELECT Win FROM Users WHERE UserName='" + userName + "'");
    this->mQuery->next();
    return this->mQuery->value(0).toInt();
}

/* 获取用户失败次数 */
int Database::GetLoseNum(QString userName) const
{
    this->mQuery->exec("SELECT Lose FROM Users WHERE UserName='" + userName + "'");
    this->mQuery->next();
    return this->mQuery->value(0).toInt();
}
