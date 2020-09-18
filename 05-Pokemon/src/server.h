#ifndef SERVER_H
#define SERVER_H

#include <QCoreApplication>
#include <QSettings>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTime>
#include "pokemon.h"
#include "battle.h"
#include "database.h"
#include "httplistener.h"
#include "httprequesthandler.h"
#include "staticfilecontroller.h"

/* HTTP服务类 */
class Server
{
public:
    Server();                                                   // 构造函数
    void Listen(int port) const;                                // 监听函数
private:
    QSettings* mListenerSettings;                               // 监听配置
    QSettings* mFileSettings;                                   // 静态文件配置
};

/* 请求处理器，继承QtWebApp的处理器 */
class HttpRequestHandler : public stefanfrings::HttpRequestHandler
{
public:
    HttpRequestHandler(stefanfrings::StaticFileController* staticFileController, QObject* parent=0);          // 构造函数
    void service(stefanfrings::HttpRequest& request, stefanfrings::HttpResponse& response);                   // 重写原service函数
    QJsonObject Register(QString userName, QString password) const;                                           // 注册
    QJsonObject LogIn(QString userName, QString password) const;                                              // 登录
    QJsonObject LogOut(QString userName) const;                                                               // 登出
    QJsonObject BaseInfo(QString userName) const;                                                             // 用户基本信息
    QJsonObject PokemonList(QString userName) const;                                                          // 用户精灵列表
    QJsonObject UserList() const;                                                                             // 用户列表
    QJsonObject OpponentList() const;                                                                         // 虚拟对手列表
    QJsonObject RefreshOpponentList() const;                                                                  // 刷新虚拟对手列表
    QJsonObject StartBattle(QString userName, BattleType type, int opponentID, QString userPokemonUUID) const;// 开始新战斗
    QJsonObject StepBattle(QString battleID) const;                                                           // 模拟下一步战斗
    QJsonObject SkipBattle(QString battleID) const;                                                           // 跳过模拟
    QJsonObject FinishBattle(QString battleID, QString discardPokemonUUID) const;                             // 结算战斗
private:
    stefanfrings::StaticFileController* mStaticFileController;                                                // 静态文件控制器
};

/* 引入外部全局变量 */
extern Database* gDB;                      // 数据库对象
extern QVector<Pokemon*> gOpponentList;    // 虚拟对手列表
extern QMap<QString, Battle*> gBattleList; // 战斗列表

#endif // SERVER_H
