#include "battle.h"

/* 构造函数，初始化新战斗 */
Battle::Battle(QString userName, BattleType type, Pokemon* userPokemon, Pokemon* opponentPokemon)
{
    this->mUserName = userName;
    this->mType = type;
    this->mUserPokemon = userPokemon;
    this->mOpponentPokemon = opponentPokemon;
    this->mUserPokemon->Recover();
    this->mOpponentPokemon->Recover();
    qInfo("用户%s发起了一场%s", userName.toStdString().data(), type == UPGRADE ? "升级赛" : "决斗赛");
}

/* 模拟下一步战斗，并以JSON形式返回该步结果 */
QJsonObject Battle::Step() const
{
    QJsonObject json
    {
        {"ourDebuffHurt", 0},
        {"oppositeDebuffHurt", 0},
        {"ourAttack", QJsonObject
            {
                {"attackType", 0},
                {"paralyzed", false},
                {"criticalStrike", false},
                {"miss", false},
                {"hurt", 0},
                {"giveDebuff", ""}
            }
        },
        {"oppositeAttack", QJsonObject
            {
                {"attackType", 0},
                {"paralyzed", false},
                {"criticalStrike", false},
                {"miss", false},
                {"hurt", 0},
                {"giveDebuff", ""}
            }
        },
        {"ourCurrentHP", 0},
        {"oppositeCurrentHP", 0}
    };

    // 产生由负面状态引起的效果
    if (this->mUserPokemon->HasDebuff())
        json["ourDebuffHurt"] = this->mOpponentPokemon->DebuffEffect(*this->mUserPokemon);
    if (this->mOpponentPokemon->HasDebuff())
        json["oppositeDebuffHurt"] = this->mUserPokemon->DebuffEffect(*this->mOpponentPokemon);

    // 更新攻击冷却时间
    if (this->mUserPokemon->UpdateWaitingTime())
    {
        // 可以发动攻击
        AttackResult attackResult;
        if (qrand() % 10 < 3)   // 30%发动特殊攻击，70%发动普通攻击
            attackResult = this->mUserPokemon->SpecialAttack(*this->mOpponentPokemon);
        else
            attackResult = this->mUserPokemon->BaseAttack(*this->mOpponentPokemon);
        json["ourAttack"] = QJsonObject
                            {
                                {"attackType", attackResult.attackType},
                                {"paralyzed", attackResult.paralyzed},
                                {"criticalStrike", attackResult.criticalStrike},
                                {"miss", attackResult.miss},
                                {"hurt", attackResult.hurt},
                                {"giveDebuff", attackResult.giveDebuff.toStdString().data()}
                            };
    }
    if (this->mOpponentPokemon->UpdateWaitingTime())
    {
        // 可以发动攻击
        AttackResult attackResult;
        if (qrand() % 10 < 3)   // 30%发动特殊攻击，70%发动普通攻击
            attackResult = this->mOpponentPokemon->SpecialAttack(*this->mUserPokemon);
        else
            attackResult = this->mOpponentPokemon->BaseAttack(*this->mUserPokemon);
        json["oppositeAttack"] = QJsonObject
                                 {
                                     {"attackType", attackResult.attackType},
                                     {"paralyzed", attackResult.paralyzed},
                                     {"criticalStrike", attackResult.criticalStrike},
                                     {"miss", attackResult.miss},
                                     {"hurt", attackResult.hurt},
                                     {"giveDebuff", attackResult.giveDebuff.toStdString().data()}
                                 };
    }

    json["ourCurrentHP"] = this->mUserPokemon->GetCurrentHP();
    json["oppositeCurrentHP"] = this->mOpponentPokemon->GetCurrentHP();
    return json;
}

/* 结算战斗，并以JSON形式返回结算结果 */
QJsonObject Battle::Finish(QString discardPokemonUUID) const
{
    int getExp = 0; // 战斗获得的经验值

    if (this->mOpponentPokemon->GetCurrentHP() == 0)
    {
        // 用户胜利
        qInfo("用户%s在%s中胜利", this->mUserName.toStdString().data(), this->mType == UPGRADE ? "升级赛" : "决斗赛");
        gDB->AddMatchNum(this->mUserName, true);

        /*
           战斗胜利获得经验公式：
           获得经验值 = 对方等级 * 对方经验值 / 7
        */
        getExp = qCeil(this->mOpponentPokemon->GetInfo().level * (this->mOpponentPokemon->GetInfo().exp + 1.0) / 7.0);
        this->mUserPokemon->GetExp(getExp);
        gDB->UpdatePokemon(this->mUserName, this->mUserPokemon->GetInfo().UUID, this->mUserPokemon->GetInfo());

        // 决斗赛胜利将获得对方精灵
        if (this->mType == DUEL)
            gDB->AddPokemon(this->mUserName, this->mOpponentPokemon->GetInfo());
    }
    else
    {
        // 虚拟对手胜利
        qInfo("用户%s在%s中败北", this->mUserName.toStdString().data(), this->mType == UPGRADE ? "升级赛" : "决斗赛");
        gDB->AddMatchNum(this->mUserName, false);

        // 决斗赛失败将失去一只精灵
        if (this->mType == DUEL)
        {
            gDB->DelPokemon(this->mUserName, discardPokemonUUID);
            if (gDB->GetPokemonNum(this->mUserName) == 0)
            {
                // 用户精灵数为0，随机发放一只1级精灵
                PokemonFactory pokemonFactory;
                Pokemon* pPM = pokemonFactory.CreatePokemon(PokemonType(qrand() % 4), PokemonName(qrand() % 5), 1);
                gDB->AddPokemon(this->mUserName, pPM->GetInfo());
                delete pPM;
            }
        }
    }

    QJsonObject json
    {
        {"getExp", getExp},
        {"level", this->mUserPokemon->GetInfo().level},
        {"exp", this->mUserPokemon->GetInfo().exp},
        {"expToNextLevel", this->mUserPokemon->GetInfo().expToNextLevel},
        {"attack", this->mUserPokemon->GetInfo().attack},
        {"defense", this->mUserPokemon->GetInfo().defense},
        {"HP", this->mUserPokemon->GetInfo().HP},
        {"interval", this->mUserPokemon->GetInfo().interval}
    };

    // 释放战斗双方对象
    delete this->mUserPokemon;
    delete this->mOpponentPokemon;
    return json;
}
