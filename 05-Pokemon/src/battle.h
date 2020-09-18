#ifndef BATTLE_H
#define BATTLE_H

#include <QJsonObject>
#include "database.h"
#include "pokemon.h"

/* 战斗类型枚举类型 */
enum BattleType
{
    UPGRADE,     // 升级赛
    DUEL         // 决斗赛
};

/* 战斗类 */
class Battle
{
public:
    Battle(QString userName, BattleType type, Pokemon* userPokemon, Pokemon* opponentPokemon);  // 构造函数，初始化新战斗
    QJsonObject Step() const;                                                                   // 模拟下一步战斗，并以JSON形式返回该步结果
    QJsonObject Finish(QString discardPokemonUUID) const;                                       // 结算战斗，并以JSON形式返回结算结果
private:
    QString mUserName;          // 用户名
    BattleType mType;           // 战斗类型
    Pokemon* mUserPokemon;      // 用户精灵
    Pokemon* mOpponentPokemon;  // 对手精灵
};

/* 引入外部全局变量 */
extern Database* gDB;                      // 数据库对象

#endif // BATTLE_H
