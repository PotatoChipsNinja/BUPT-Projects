#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include "pokemon.h"

/* 用户基础信息，用于传递用户列表 */
struct UserInfo
{
    QString userName;       // 用户名
    bool isOnline;          // 是否在线
    int pokemonNum;         // 精灵数量
    int advancedPokemonNum; // 高级精灵数量
    int matchNum;           // 比赛次数
    int winNum;             // 胜利次数
    int loseNum;            // 失败次数
};

/* 数据库类 */
class Database
{
public:
    Database();                                                                  // 初始化数据库模块
    ~Database();                                                                 // 断开数据库连接
    bool Connect();                                                              // 连接数据库，返回连接结果
    bool Register(QString userName, QString password) const;                     // 用户注册，返回注册结果
    bool LogIn(QString userName, QString password) const;                        // 用户登录，返回登录结果
    void LogOut(QString userName) const;                                         // 用户登出
    QVector<PokemonInfo> GetPokemonList(QString userName) const;                 // 获取用户精灵列表
    PokemonInfo GetPokemonInfo(QString userName, QString UUID) const;            // 获取用户精灵信息
    void AddPokemon(QString userName, PokemonInfo info) const;                   // 添加用户精灵
    void DelPokemon(QString userName, QString UUID) const;                       // 移除用户精灵
    void UpdatePokemon(QString userName, QString UUID, PokemonInfo info) const;  // 更新用户精灵
    void AddMatchNum(QString userName, bool isWin) const;                        // 增加用户比赛场次
    QVector<UserInfo> GetUserList() const;                                       // 获取用户列表
    int GetPokemonNum(QString userName) const;                                   // 获取用户精灵数量
    int GetAdvancedPokemonNum(QString userName) const;                           // 获取用户高级精灵数量
    int GetMatchNum(QString userName) const;                                     // 获取用户比赛次数
    int GetWinNum(QString userName) const;                                       // 获取用户胜利次数
    int GetLoseNum(QString userName) const;                                      // 获取用户失败次数
private:
    QSqlDatabase mDB;
    QSqlQuery* mQuery;
};

#endif // DATABASE_H
