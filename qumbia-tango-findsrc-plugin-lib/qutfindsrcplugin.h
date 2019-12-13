#ifndef QUTFINDSRCPLUGIN_H
#define QUTFINDSRCPLUGIN_H

#include <QGenericPlugin>
#include <qutfindsrcsplugini.h>

class QuTFindSrcPluginPrivate;

class QuTFindSrcPlugin : public QObject, public QuTFindSrcsPluginI
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QGenericPluginFactoryInterface" FILE "qumbia-tango-findsrc-plugin.json")
    Q_INTERFACES(QuTFindSrcsPluginI)

public:
    explicit QuTFindSrcPlugin(QObject *parent = nullptr);
    ~QuTFindSrcPlugin();

private:

    // QuTFindSrcsPluginI interface
public:
    QStringList matches(const QString &find);
    QString errorMessage() const;
    bool error() const;

private:
    QuTFindSrcPluginPrivate *d;

    bool m_is_command(const QString &find, QString& separator);
};

#endif // QUTFINDSRCPLUGIN_H
