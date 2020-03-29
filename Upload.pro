QT       += core gui \
            network \
            sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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
    alarm.cpp \
    config.cpp \
    gumbo_parser/attribute.c \
    gumbo_parser/char_ref.c \
    gumbo_parser/error.c \
    gumbo_parser/parser.c \
    gumbo_parser/string_buffer.c \
    gumbo_parser/string_piece.c \
    gumbo_parser/tag.c \
    gumbo_parser/tokenizer.c \
    gumbo_parser/utf8.c \
    gumbo_parser/util.c \
    gumbo_parser/vector.c \
    gumbo_query/CParser.cpp \
    gumbo_query/Document.cpp \
    gumbo_query/Node.cpp \
    gumbo_query/Object.cpp \
    gumbo_query/QueryUtil.cpp \
    gumbo_query/Selection.cpp \
    gumbo_query/Selector.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    qlog.cpp \
    record.cpp \
    seting.cpp \
    sqlitedata.cpp \
    task.cpp \
    testitem.cpp \
    tool.cpp \
    uploadrecord.cpp \
    uploadthread.cpp \
    user.cpp

HEADERS += \
    alarm.h \
    config.h \
    gumbo_parser/attribute.h \
    gumbo_parser/char_ref.h \
    gumbo_parser/char_ref.rl \
    gumbo_parser/error.h \
    gumbo_parser/gumbo.h \
    gumbo_parser/insertion_mode.h \
    gumbo_parser/parser.h \
    gumbo_parser/string_buffer.h \
    gumbo_parser/string_piece.h \
    gumbo_parser/tag_enum.h \
    gumbo_parser/tag_gperf.h \
    gumbo_parser/tag_sizes.h \
    gumbo_parser/tag_strings.h \
    gumbo_parser/token_type.h \
    gumbo_parser/tokenizer.h \
    gumbo_parser/tokenizer_states.h \
    gumbo_parser/utf8.h \
    gumbo_parser/util.h \
    gumbo_parser/vector.h \
    gumbo_query/CParser.h \
    gumbo_query/Document.h \
    gumbo_query/Node.h \
    gumbo_query/Object.h \
    gumbo_query/QueryUtil.h \
    gumbo_query/Selection.h \
    gumbo_query/Selector.h \
    login.h \
    mainwindow.h \
    qlog.h \
    record.h \
    seting.h \
    sqlitedata.h \
    task.h \
    testitem.h \
    tool.h \
    uploadrecord.h \
    uploadthread.h \
    user.h

FORMS += \
    login.ui \
    mainwindow.ui \
    record.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images/images.qrc

DISTFILES +=

RC_ICONS = CloudPlat.ico
