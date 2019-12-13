#include "qutfindsrcplugin.h"

#include <tango.h>
#include <string>
#include <cutango-world.h>
#include <qustring.h>
#include <QtDebug>

class QuTFindSrcPluginPrivate {
public:
    QString error_msg;
};

QuTFindSrcPlugin::QuTFindSrcPlugin(QObject *parent) : QObject(parent)
{
    d = new QuTFindSrcPluginPrivate;
}

QuTFindSrcPlugin::~QuTFindSrcPlugin()
{
    delete d;
}

QStringList QuTFindSrcPlugin::matches(const QString &find)
{
    QStringList matches;
    QString prefix;
    std::string pattern;
    QString cmd_sep; // command separator
    // if find has wildcards, search with get_device_exported
    // otherwise, add a wildcard at the end of find and optimize
    // search
    bool has_wildcards = find.contains("*");
    !has_wildcards ? pattern = find.toStdString() + "*" : pattern = find.toStdString();
    try {
        Tango::Database *db = new Tango::Database;
        Tango::DbDatum dbd;
        std::vector<std::string> v_mat;
        if(!has_wildcards && (find.count('/') == 0 || find.isEmpty())) {
            dbd = db->get_device_domain(pattern);
            dbd >> v_mat;
        }
        else if(!has_wildcards && m_is_command(find, cmd_sep)) {
            std::string dev;
            QString pt_find;
            dev = find.section(cmd_sep, 0, 0).toStdString();
            pt_find = find.section(cmd_sep, -1);
                        // command list
            try {
                Tango::DeviceProxy *devi = new Tango::DeviceProxy(dev);
                Tango::CommandInfoList *cmdlist = devi->command_list_query();
                const Tango::CommandInfoList cmds = *cmdlist;
                for(size_t i = 0; i < cmds.size(); i++)  {
                    if(QuString(cmds[i].cmd_name).startsWith(pt_find) || pt_find.isEmpty())
                        v_mat.push_back("//" + cmds[i].cmd_name);
                }
                delete cmdlist;
                prefix = QuString(dev);
            }
            catch(Tango::DevFailed& e) {
                CuTangoWorld tw;
                d->error_msg = QuString(tw.strerror(e));
            }
        }
        else if(!has_wildcards && find.count('/') == 1) {
            dbd = db->get_device_family(pattern);
            prefix = find.section('/', 0, 0) + "/";
            dbd >> v_mat;
        }
        else if(find.count('/') == 2 || has_wildcards) {
            dbd  = db->get_device_exported(pattern);
            dbd >> v_mat;
        }
        else if(find.count('/') == 3) { // attribute list
            std::string dev = find.section('/', 0, 2).toStdString();
            QString pt_find = find.section('/', -1);
            std::vector<std::string> atts;
            db->get_device_attribute_list(dev, atts);
            prefix = QuString(dev);
            foreach(QuString att, atts) {
                if(att.startsWith(pt_find) || pt_find.isEmpty())
                    v_mat.push_back("/" + att.toStdString());
            }
        }
        else
            qDebug() << __PRETTY_FUNCTION__ << find << find.count('/') << find.endsWith("->") <<
                                                                        find.endsWith("-");
        for(size_t i = 0; i < v_mat.size(); i++)
            matches << prefix + QString::fromStdString(v_mat[i]);
        delete db;
    }
    catch(Tango::DevFailed &e) {
        CuTangoWorld tw;
        d->error_msg = QString::fromStdString(tw.strerror(e));
    }
    if(matches.size() == 1 && !matches[0].endsWith('/') && matches[0].count('/') < 3)
        matches[0] += '/';
    else if(matches.size() ==0 && find.size() > 1 && find.endsWith('-'))
        matches.append(find.section('-', 0, find.count('-') - 1) + ":");
    return matches;
}


QString QuTFindSrcPlugin::errorMessage() const
{
    return d->error_msg;
}

bool QuTFindSrcPlugin::error() const
{
    return !d->error_msg.isEmpty();
}

bool QuTFindSrcPlugin::m_is_command(const QString& find, QString &separator)
{
    separator = "";
    if(find.count('/') == 2 && find.contains("->"))
        separator = "->";
    else if(find.count('/') == 4 && find.count("//") == 1)
        separator = "//";
    return !separator.isEmpty();
}

