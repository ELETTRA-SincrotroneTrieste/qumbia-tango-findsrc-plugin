include(/usr/local/cumbia-libs/include/cumbia-qtcontrols/cumbia-qtcontrols.pri)


# The application will be installed under INSTALL_ROOT (i.e. prefix)
#
# WARNING: INSTALL_ROOT is defined both by qumbia-epics-controls.pri and qumbia-tango-controls.pri
# The first definition in qumbia-tango-controls.pri is used.
#
#
# To set the prefix at build time, call
# qmake   "INSTALL_ROOT=/my/custom/path"
#
isEmpty(INSTALL_ROOT) {
    INSTALL_ROOT = /usr/local/cumbia-libs
}

PKGCONFIG += qumbia-tango-controls cumbia-tango tango

TEMPLATE = lib
CONFIG += plugin

CONFIG += c++17

DEFINES -= QT_NO_DEBUG_OUTPUT

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    qutfindsrcplugin.cpp

HEADERS += \
    qutfindsrcplugin.h

DISTFILES += qumbia-tango-findsrc-plugin.json

TARGET = qumbia-tango-findsrc-plugin

unix {
    target.path = $${DEFINES_CUMBIA_QTCONTROLS_PLUGIN_DIR}
    INSTALLS += target
}

message("cumbia-tango-findsrc-plugin: plugin installation dir:  $${DEFINES_CUMBIA_QTCONTROLS_PLUGIN_DIR}")
# message("cumbia-tango-findsrc-plugin: include installation dir: $${INC_PATH}")
