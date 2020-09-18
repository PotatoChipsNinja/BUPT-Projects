#ifndef POKEMON_H
#define POKEMON_H

#include <QtMath>
#include <QUuid>

/* 精灵种类枚举类型 */
enum PokemonType
{
    STRENGTH,     // 力量型
    TANK,         // 肉盾型
    DEFENSE,      // 防御型
    SWIFT         // 敏捷型
};

/* 精灵名称枚举类型 */
enum PokemonName
{
    BULBASAUR,    // 妙蛙种子
    CHARMANDER,   // 小火龙
    SQUIRTLE,     // 杰尼龟
    PIKACHU,      // 皮卡丘
    GENGAR        // 耿鬼
};

/* 精灵基础信息结构体（不包含战斗时所需的属性），用于构造函数和信息传递 */
struct PokemonInfo
{
    QString UUID;              // 通用唯一识别码
    PokemonType type;          // 种类
    PokemonName name;          // 名称
    int level;                 // 当前等级
    int exp;                   // 当前经验值
    int expToNextLevel;        // 升级所需经验值
    int attack;                // 攻击力
    int defense;               // 防御力
    int HP;                    // HP
    int interval;              // 攻击间隔
};

/* 攻击结果结构体，用于向Battle类传递信息 */
struct AttackResult
{
    int attackType;            // 攻击类型，1为普通攻击，2为特殊攻击
    bool paralyzed;            // 是否因麻痹无法攻击
    bool criticalStrike;       // 是否打出暴击
    bool miss;                 // 是否被对方闪避
    int hurt;                  // 对对方的实际伤害
    QString giveDebuff;        // 给予对手的负面效果
};

class Pokemon;  // 向前声明

/* 精灵工厂 */
class PokemonFactory
{
public:
    Pokemon* CreatePokemon(PokemonType type, PokemonName name, int level) const; // 创建仅指定等级的精灵
    Pokemon* CreatePokemon(PokemonInfo& info) const;                             // 创建指定全部信息的精灵
};

/* 精灵基类 */
class Pokemon
{
public:
    Pokemon(PokemonInfo& info);                               // 构造函数
    ~Pokemon() {}
    PokemonInfo GetInfo() const;                              // 获取精灵基础信息
    AttackResult BaseAttack(Pokemon& target);                 // 普通攻击（所有精灵共有），返回攻击结果
    virtual AttackResult SpecialAttack(Pokemon& target) = 0;  // 特殊攻击（每个精灵独有），返回攻击结果
    virtual int DebuffEffect(Pokemon& target) = 0;            // 每秒给对方造成的负面效果，返回伤害值
    bool GetHurt(int hurt, bool canAvoid);                    // 受到伤害，若造成伤害则返回真，触发闪避返回假
    int  GetExp(int exp);                                     // 获得经验，返回获得经验后的等级
    void GetDebuff(QString debuffName);                       // 获得负面状态
    int Upgrade();                                            // 升级，返回升级后的等级
    void Recover();                                           // 回复HP，消除负面状态
    bool HasDebuff() const;                                   // 是否处于负面状态
    int GetCurrentHP() const;                                 // 获取当前HP
    bool UpdateWaitingTime();                                 // 更新攻击冷却时间，返回是否可以攻击
protected:
    QString mUUID;              // 通用唯一识别码
    PokemonType mType;          // 种类
    PokemonName mName;          // 名称
    int mLevel;                 // 当前等级
    int mExp;                   // 当前经验值
    int mExpToNextLevel;        // 升级所需经验值
    int mAttack;                // 攻击力
    int mDefense;               // 防御力
    int mTotalHP;               // 总HP
    int mCurrentHP;             // 当前HP
    int mInterval;              // 攻击间隔
    int mWaitingTime;           // 距离下一次攻击的等待时间
    bool mHasDebuff;            // 是否处于负面状态
    QString mDebuffName;        // 负面状态名称（如：中毒）
};

/* 妙蛙种子类 */
class Bulbasaur : public Pokemon
{
public:
    Bulbasaur(PokemonInfo info) : Pokemon(info) {}
    AttackResult SpecialAttack(Pokemon & target);
    int DebuffEffect(Pokemon & target);
};

/* 小火龙类 */
class Charmander : public Pokemon
{
public:
    Charmander(PokemonInfo info) : Pokemon(info) {}
    AttackResult SpecialAttack(Pokemon & target);
    int DebuffEffect(Pokemon & target);
};

/* 杰尼龟类 */
class Squirtle : public Pokemon
{
public:
    Squirtle(PokemonInfo info) : Pokemon(info) {}
    AttackResult SpecialAttack(Pokemon & target);
    int DebuffEffect(Pokemon & target);
};

/* 皮卡丘类 */
class Pikachu : public Pokemon
{
public:
    Pikachu(PokemonInfo info) : Pokemon(info) {}
    AttackResult SpecialAttack(Pokemon & target);
    int DebuffEffect(Pokemon & target);
};

/* 耿鬼类 */
class Gengar : public Pokemon
{
public:
    Gengar(PokemonInfo info) : Pokemon(info) {}
    AttackResult SpecialAttack(Pokemon & target);
    int DebuffEffect(Pokemon & target);
};

#endif // POKEMON_H
