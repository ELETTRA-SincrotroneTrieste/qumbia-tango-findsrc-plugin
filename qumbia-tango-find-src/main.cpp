#include <QCoreApplication>
#include <cupluginloader.h>
#include <qutfindsrcsplugini.h>
#include <chrono>
#include <cumacros.h>

int main(int argc, char *argv[])
{
    CuPluginLoader pl;
    QuTFindSrcsPluginI *findSrcPI = pl.get<QuTFindSrcsPluginI>("libqumbia-tango-findsrc-plugin.so", nullptr);
    if(findSrcPI) {
        bool debug = argc > 2 && strcmp(argv[2], "--debug") == 0;
        std::chrono::time_point<std::chrono::steady_clock> t1, t2;
        if(debug)
            t1 = std::chrono::steady_clock::now();
        QString find;
        if(argc > 1)
            find = QString(argv[1]);
        QStringList matches = findSrcPI->matches(find);
        foreach(QString match, matches)
            printf("%s ", qstoc(match));
        if(matches.size() > 0)
            printf("\n");
        if(debug) {
            t2 = std::chrono::steady_clock::now();
            printf("took %ldus\n", std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
        }
    }
    return EXIT_SUCCESS;
}
