#include <QCoreApplication>
#include <QTime>
#include "pokemon.h"
#include "database.h"
#include "battle.h"
#include "server.h"

/* 定义全局变量 */
Database* gDB;                      // 数据库对象
QVector<Pokemon*> gOpponentList;    // 虚拟对手列表
QMap<QString, Battle*> gBattleList; // 战斗列表

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    int port = 5000;                        // 默认端口为5000
    if (argc == 3 && QString(argv[1]) == "-port")
        port = QString(argv[2]).toInt();    // 用户自定义端口号

    // 设置随机种子
    QTime time = QTime::currentTime();
    qsrand(time.msec() + time.second() * 1000);

    // 连接数据库
    gDB = new Database;
    if (!gDB->Connect())
        return app.exec();  // 数据库连接失败

    PokemonFactory pokemonFactory;  // 创建精灵工厂
    // 随机创建5个1-8级的虚拟对手
    for (int i = 0; i < 5; ++i)
        gOpponentList.append(pokemonFactory.CreatePokemon(PokemonType(qrand() % 4),
                                                          PokemonName(qrand() % 5),
                                                          qrand() % 8 + 1));

    // 启动HTTP服务器
    Server server;
    server.Listen(port);

    return app.exec();
}
