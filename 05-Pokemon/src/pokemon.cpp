#include "pokemon.h"

/* 通过精灵工厂创建指定种类和等级的精灵，返回其指针 */
Pokemon* PokemonFactory::CreatePokemon(PokemonType type, PokemonName name, int level) const
{
    Pokemon* ans;
    PokemonInfo info;
    info.UUID = QUuid::createUuid().toString().mid(1, 36);
    info.type = type;
    info.name = name;

    /*
       先将精灵初始化为1级，然后再给予其经验将其升级至指定等级
       1级精灵各属性值赋值规则：
         一般的：
           攻击力、防御力在7-10之间取随机整数
           总HP在15-20之间取随机整数
           攻击间隔在6-7之间取随机整数
         特别的：
           力量型精灵攻击力在12-15之间取随机整数
           防御型精灵防御力在12-15之间取随机整数
           肉盾型精灵总HP在20-25之间取随机整数
           敏捷型精灵攻击间隔在3-4之间取随机整数
    */
    info.level = 1;
    info.exp = 0;
    info.expToNextLevel = 5;
    info.attack = qrand() % 4 + 7;
    info.defense = qrand() % 4 + 7;
    info.HP = qrand() % 6 + 15;
    info.interval = qrand() % 2 + 6;
    switch (type)
    {
    case STRENGTH:
        info.attack += 5;
        break;
    case DEFENSE:
        info.defense += 5;
        break;
    case TANK:
        info.HP += 5;
        break;
    case SWIFT:
        info.interval -= 3;
        break;
    }

    /*
       各等级最低经验即升级所需经验规则：
         Lv.1: 0
         Lv.2: 5  (+5)
         Lv.3: 11 (+6)
         Lv.4: 18 (+7)
         Lv.5: 26 (+8)
         ...
    */
    int addExp = 0;     // 升值用户指定等级所需的经验值
    for (int i = 0; i < level-1; ++i)
        addExp += 5 + i;

    switch (name)
    {
    case BULBASAUR:
        ans = new Bulbasaur(info);
        break;
    case CHARMANDER:
        ans = new Charmander(info);
        break;
    case SQUIRTLE:
        ans = new Squirtle(info);
        break;
    case PIKACHU:
        ans = new Pikachu(info);
        break;
    case GENGAR:
        ans = new Gengar(info);
        break;
    }

    ans->GetExp(addExp);    // 将精灵升至指定的等级
    ans->Recover();
    return ans;
}

/* 通过精灵工厂创建指定全部信息的精灵，返回其指针 */
Pokemon* PokemonFactory::CreatePokemon(PokemonInfo& info) const
{
    Pokemon* ans;
    switch (info.name)
    {
    case BULBASAUR:
        ans = new Bulbasaur(info);
        break;
    case CHARMANDER:
        ans = new Charmander(info);
        break;
    case SQUIRTLE:
        ans = new Squirtle(info);
        break;
    case PIKACHU:
        ans = new Pikachu(info);
        break;
    case GENGAR:
        ans = new Gengar(info);
        break;
    }
    return ans;
}

/* 精灵类构造函数，设置私有属性 */
Pokemon::Pokemon(PokemonInfo& info)
{
    this->mUUID = info.UUID;
    this->mType = info.type;
    this->mName = info.name;
    this->mLevel = info.level;
    this->mExp = info.exp;
    this->mExpToNextLevel = info.expToNextLevel;
    this->mAttack = info.attack;
    this->mDefense = info.defense;
    this->mTotalHP = info.HP;
    this->mCurrentHP = info.HP;
    this->mInterval = info.interval;
    this->mWaitingTime = 0;
    this->mHasDebuff = false;
    this->mDebuffName = "";
}

/* 获取精灵基础信息 */
PokemonInfo Pokemon::GetInfo() const
{
    PokemonInfo info;
    info.UUID = this->mUUID;
    info.type = this->mType;
    info.name = this->mName;
    info.level = this->mLevel;
    info.exp = this->mExp;
    info.expToNextLevel = this->mExpToNextLevel;
    info.attack = this->mAttack;
    info.defense = this->mDefense;
    info.HP = this->mTotalHP;
    info.interval = this->mInterval;
    return info;
}

/* 普通攻击（所有精灵共有），返回攻击结果 */
AttackResult Pokemon::BaseAttack(Pokemon& target)
{
    this->mWaitingTime = this->mInterval;   // 重置攻击冷却时间
    AttackResult result;
    result.attackType = 1;
    // 首先判断攻击方是否处于麻痹状态
    if (this->mHasDebuff && this->mDebuffName == "麻痹")
        if (qrand() % 2)    // 有50%的几率无法攻击
        {
            result.paralyzed = true;
            result.criticalStrike = false;
            result.miss = false;
            result.hurt = 0;
            return result;
        }
    result.paralyzed = false;

    /*
       普通攻击伤害计算公式：
       攻击伤害 = (攻击方等级 * 0.4 + 2) * 攻击方攻击力 / 防御方防御力
    */
    double hurt = (this->mLevel * 0.4 + 2) * this->mAttack / target.GetInfo().defense;

    /*
       暴击机制：
       普通攻击有10%的几率打出暴击，伤害增加一倍
    */
    if (qrand() % 10 == 0)
    {
        hurt *= 2;
        result.criticalStrike = true;
    }
    else
        result.criticalStrike = false;

    result.miss = !target.GetHurt(qCeil(hurt), true);
    result.hurt = result.miss ? 0 : qCeil(hurt);
    result.giveDebuff = "";
    return result;
}

/* 受到伤害，若造成伤害则返回真，触发闪避返回假 */
bool Pokemon::GetHurt(int hurt, bool canAvoid)
{
    /*
       闪避机制：
       攻击造成的伤害有10%的几率被闪避
    */
    if (canAvoid)
        if (qrand() % 10 == 0)
            return false;

    this->mCurrentHP -= hurt;
    if (this->mCurrentHP <= 0)
        this->mCurrentHP = 0;
    return true;
}

/* 获得经验，返回获得经验后的等级 */
int Pokemon::GetExp(int exp)
{
    this->mExp += exp;
    if (this->mExp >= this->mExpToNextLevel)
        this->Upgrade();    // 升级
    return this->mLevel;
}

/* 获得负面状态 */
void Pokemon::GetDebuff(QString debuffName)
{
    this->mHasDebuff = true;
    this->mDebuffName = debuffName;
}

/* 升级，返回升级后的等级 */
int Pokemon::Upgrade()
{
    while(this->mExp >= this->mExpToNextLevel && this->mLevel < 15) // 最高15级
    {
        this->mLevel++;
        this->mExpToNextLevel += this->mLevel + 4;

        /*
           升级属性提升规则：
             一般的：
               攻击力、防御力、总HP各提升10%-15%（向上取整）
               具体提升比例在上述范围内随机产生
             特别的：
               力量型精灵攻击力额外再提升10%
               肉盾型精灵总HP额外再提升10%
               防御型精灵防御力额外再提升10%
        */
        this->mAttack = qCeil((qrand() % 6 + 110) / 100.0 * this->mAttack);   // 攻击力提升10%-15%
        this->mDefense = qCeil((qrand() % 6 + 110) / 100.0 * this->mDefense); // 防御力提升10%-15%
        this->mTotalHP = qCeil((qrand() % 6+ 110) / 100.0 * this->mTotalHP);  // 总HP提升10%-15%
        switch (this->mType)
        {
        case STRENGTH:
            this->mAttack = qCeil(1.1 * this->mAttack);   // 力量型精灵攻击力额外提升10%
            break;
        case TANK:
            this->mTotalHP = qCeil(1.1 * this->mTotalHP); // 肉盾型精灵总HP额外提升10%
            break;
        case DEFENSE:
            this->mDefense = qCeil(1.1 * this->mDefense); // 防御性精灵防御力额外提升10%
            break;
        default:
            break;
        }
    }
    return this->mLevel;
}

/* 回复HP，消除负面状态 */
void Pokemon::Recover()
{
    this->mCurrentHP = this->mTotalHP;
    this->mHasDebuff = false;
    this->mDebuffName = "";
    this->mWaitingTime = 0;
}

/* 是否处于负面状态 */
bool Pokemon::HasDebuff() const
{
    return this->mHasDebuff;
}

/* 获取当前HP */
int Pokemon::GetCurrentHP() const
{
    return this->mCurrentHP;
}

/* 更新攻击冷却时间，返回是否可以攻击 */
bool Pokemon::UpdateWaitingTime()
{
    this->mWaitingTime--;
    if (this->mWaitingTime <= 0)
    {
        this->mWaitingTime = 0;
        return true;
    }
    return false;
}

/*
   妙蛙种子特殊攻击：寄生种子
   效果：使对方受到普通攻击80%的伤害，并使对方进入“寄生”状态
*/
AttackResult Bulbasaur::SpecialAttack(Pokemon &target)
{
    this->mWaitingTime = this->mInterval;   // 重置攻击冷却时间
    AttackResult result;
    result.attackType = 2;
    // 首先判断攻击方是否处于麻痹状态
    if (this->mHasDebuff && this->mDebuffName == "麻痹")
        if (qrand() % 2)    // 有50%的几率无法攻击
        {
            result.paralyzed = true;
            result.criticalStrike = false;
            result.miss = false;
            result.hurt = 0;
            return result;
        }
    result.paralyzed = false;
    result.criticalStrike = false;

    double hurt = (this->mLevel * 0.4 + 2) * this->mAttack / target.GetInfo().defense;
    hurt *= 0.8;
    result.miss = !target.GetHurt(qCeil(hurt), true);
    result.hurt = result.miss ? 0 : qCeil(hurt);
    target.GetDebuff("寄生");
    result.giveDebuff = "寄生";
    return result;
}

/*
   妙蛙种子可给予对方的负面状态：寄生
   效果：被寄生的精灵每秒钟会被妙蛙种子吸收总HP的5%（被寄生者HP减5%，妙蛙种子HP加5%）
*/
int Bulbasaur::DebuffEffect(Pokemon &target)
{
    double hurt = target.GetInfo().HP * 0.05;
    target.GetHurt(qCeil(hurt), false);
    this->mCurrentHP += qCeil(hurt);
    if (this->mCurrentHP > this->mTotalHP)
        this->mCurrentHP = this->mTotalHP;
    return qCeil(hurt);
}

/*
   小火龙特殊攻击：喷射火焰
   效果：使对方受到普通攻击120%的伤害，并使对方进入“烧伤”状态
*/
AttackResult Charmander::SpecialAttack(Pokemon &target)
{
    this->mWaitingTime = this->mInterval;   // 重置攻击冷却时间
    AttackResult result;
    result.attackType = 2;
    // 首先判断攻击方是否处于麻痹状态
    if (this->mHasDebuff && this->mDebuffName == "麻痹")
        if (qrand() % 2)    // 有50%的几率无法攻击
        {
            result.paralyzed = true;
            result.criticalStrike = false;
            result.miss = false;
            result.hurt = 0;
            return result;
        }
    result.paralyzed = false;
    result.criticalStrike = false;

    double hurt = (this->mLevel * 0.4 + 2) * this->mAttack / target.GetInfo().defense;
    hurt *= 1.2;
    result.miss = !target.GetHurt(qCeil(hurt), true);
    result.hurt = result.miss ? 0 : qCeil(hurt);
    target.GetDebuff("烧伤");
    result.giveDebuff = "烧伤";
    return result;
}

/*
   小火龙可给予对方的负面状态：烧伤
   效果：被烧伤的精灵每秒钟会受到总HP的5%的伤害
*/
int Charmander::DebuffEffect(Pokemon &target)
{
    double hurt = target.GetInfo().HP * 0.05;
    target.GetHurt(qCeil(hurt), false);
    return qCeil(hurt);
}

/*
   杰尼龟特殊攻击：水之波动
   效果：使对方受到普通攻击50%的伤害，并解除自身的负面状态
*/
AttackResult Squirtle::SpecialAttack(Pokemon &target)
{
    this->mWaitingTime = this->mInterval;   // 重置攻击冷却时间
    AttackResult result;
    result.attackType = 2;
    // 首先判断攻击方是否处于麻痹状态
    if (this->mHasDebuff && this->mDebuffName == "麻痹")
        if (qrand() % 2)    // 有50%的几率无法攻击
        {
            result.paralyzed = true;
            result.criticalStrike = false;
            result.miss = false;
            result.hurt = 0;
            return result;
        }
    result.paralyzed = false;
    result.criticalStrike = false;

    double hurt = (this->mLevel * 0.4 + 2) * this->mAttack / target.GetInfo().defense;
    hurt *= 0.5;
    result.miss = !target.GetHurt(qCeil(hurt), true);
    result.hurt = result.miss ? 0 : qCeil(hurt);
    result.giveDebuff = "";
    if (this->mHasDebuff)
    {
        // 解除自己的负面状态
        this->mHasDebuff = false;
        this->mDebuffName = "";
    }
    return result;
}

/*
   杰尼龟的特殊攻击不会给予对方负面状态
*/
int Squirtle::DebuffEffect(Pokemon &target)
{
    // 无每秒固定效果
    return 0;
}

/*
   皮卡丘特殊攻击：十万伏特
   效果：使对方受到普通攻击120%的伤害，并使对方进入“麻痹”状态
*/
AttackResult Pikachu::SpecialAttack(Pokemon &target)
{
    this->mWaitingTime = this->mInterval;   // 重置攻击冷却时间
    AttackResult result;
    result.attackType = 2;
    // 首先判断攻击方是否处于麻痹状态
    if (this->mHasDebuff && this->mDebuffName == "麻痹")
        if (qrand() % 2)    // 有50%的几率无法攻击
        {
            result.paralyzed = true;
            result.criticalStrike = false;
            result.miss = false;
            result.hurt = 0;
            return result;
        }
    result.paralyzed = false;
    result.criticalStrike = false;

    double hurt = (this->mLevel * 0.4 + 2) * this->mAttack / target.GetInfo().defense;
    hurt *= 1.2;
    result.miss = !target.GetHurt(qCeil(hurt), true);
    result.hurt = result.miss ? 0 : qCeil(hurt);
    target.GetDebuff("麻痹");
    result.giveDebuff = "麻痹";
    return result;
}

/*
   皮卡丘可给予对方的负面状态：麻痹
   效果：被麻痹的精灵在发动普通攻击或特殊攻击时，有50%的几率动弹不得，无法发动攻击
*/
int Pikachu::DebuffEffect(Pokemon &target)
{
    // “麻痹”效果体现在BaseAttack()和SpecialAttack()方法中，无每秒固定效果
    return 0;
}

/*
   耿鬼特殊攻击：毒液冲击
   效果：使对方受到普通攻击80%的伤害，并使对方进入“中毒”状态
*/
AttackResult Gengar::SpecialAttack(Pokemon &target)
{
    this->mWaitingTime = this->mInterval;   // 重置攻击冷却时间
    AttackResult result;
    result.attackType = 2;
    // 首先判断攻击方是否处于麻痹状态
    if (this->mHasDebuff && this->mDebuffName == "麻痹")
        if (qrand() % 2)    // 有50%的几率无法攻击
        {
            result.paralyzed = true;
            result.criticalStrike = false;
            result.miss = false;
            result.hurt = 0;
            return result;
        }
    result.paralyzed = false;
    result.criticalStrike = false;

    double hurt = (this->mLevel * 0.4 + 2) * this->mAttack / target.GetInfo().defense;
    hurt *= 0.8;
    result.miss = !target.GetHurt(qCeil(hurt), true);
    result.hurt = result.miss ? 0 : qCeil(hurt);
    target.GetDebuff("中毒");
    result.giveDebuff = "中毒";
    return result;
}

/*
   耿鬼可给予对方的负面状态：中毒
   效果：中毒的精灵每秒钟会受到总HP的5%的伤害
*/
int Gengar::DebuffEffect(Pokemon &target)
{
    double hurt = target.GetInfo().HP * 0.05;
    target.GetHurt(qCeil(hurt), false);
    return qCeil(hurt);
}
