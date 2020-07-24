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

include ($${INSTALL_ROOT}/include/cumbia-qtcontrols/cumbia-qtcontrols.pri)

# comment this line if you want to install bash completion scripts
# system wide
isEmpty(BASH_COMPLETION_DIR) {
    BASH_COMPLETION_DIR=$${INSTALL_ROOT}/share/bash-completion.d
}

isEmpty(BASH_COMPLETION_DIR) {
    message("cumbia: BASH_COMPLETION_DIR is empty: using pkg-config to find system wide bash completion dir")
    BASH_COMPLETION_DIR=$$system(pkg-config --variable=completionsdir bash-completion)
}

TARGET = bin/qumbia-tango-find-src
TEMPLATE = app

PKGCONFIG += qumbia-tango-controls

QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle
CONFIG += debug

VERSION = 1.0.0

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
        main.cpp

message("bash completion dir is $${BASH_COMPLETION_DIR}")


completion.path = $${BASH_COMPLETION_DIR}
completion.files = bash-completion.d/qumbia-tango-find-src

inst.files = $${TARGET}
inst.path = $${INSTALL_ROOT}/bin

INSTALLS += inst completion

DISTFILES += \
    bash_completion.d/qumbia-tango-find-src

