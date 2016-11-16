unix:!android {
    isEmpty(target.path) {
        qnx {
            target.path = /tmp/$${TARGET}/bin
        } else {
            target.path = /opt/$${TARGET}/bin
        }
        export(target.path)
    }
    INSTALLS += target
}

android {
    message( "My android build..." )
    deployment.path = /assets
    deployment.files += database/RussianCook.sqlite \

    images.path = /assets/images
    images.files = images/*
    content.path =  /assets/content
    content.files = content/*
    INSTALLS += images
    INSTALLS += content


} else {
    message( "My Win build in...")
    message($$OUT_PWD )
    deployment.path = $$OUT_PWD/
    #deployment.files += database/RussianCook.sqlite \
}

INSTALLS += deployment

export(INSTALLS)

HEADERS += \
    $$PWD/aqp/alt_key.hpp \
    $$PWD/aqp/aqp.hpp \
    $$PWD/aqp/kuhn_munkres.hpp \
    $$PWD/itemtypes.hpp \
    $$PWD/smileyitem.hpp \
    $$PWD/swatch.hpp \
    $$PWD/textitem.hpp \
    $$PWD/textitemdialog.hpp \
    $$PWD/mainwindow.hpp \
    $$PWD/boxitem.hpp \
    $$PWD/graphicsview.hpp \
    $$PWD/textedit.hpp \
    $$PWD/global.hpp \
    $$PWD/propmanager.h \
    $$PWD/SSobjects/ssindicator.h \
    $$PWD/commands.h \
    $$PWD/graphicscene.h \
    $$PWD/SSobjects/DialogIndSource/checkboxdelegate.h \
    $$PWD/SSobjects/DialogIndSource/fileedit.h \
    $$PWD/SSobjects/DialogIndSource/imagemodeldelegate.hpp \
    $$PWD/SSobjects/DialogIndSource/proxymodel.hpp \
    $$PWD/SSobjects/DialogIndSource/ssitemdialog.h \
    $$PWD/SSobjects/DialogIndSource/standardtablemodel.hpp \
    $$PWD/SSobjects/DialogIndSource/templateimage.h \
    $$PWD/SSobjects/DialogIndSource/templmodelinfo.h \
    $$PWD/SSobjects/DialogIndSource/uniqueproxymodel.hpp

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/aqp/alt_key.cpp \
    $$PWD/aqp/aqp.cpp \
    $$PWD/aqp/kuhn_munkres.cpp \
    $$PWD/swatch.cpp \
    $$PWD/textedit.cpp \
    $$PWD/textitem.cpp \
    $$PWD/textitemdialog.cpp \
    $$PWD/boxitem.cpp \
    $$PWD/mainwindow.cpp \
    $$PWD/smileyitem.cpp \
    $$PWD/global.cpp \
    $$PWD/propmanager.cpp \
    $$PWD/SSobjects/ssindicator.cpp \
    $$PWD/commands.cpp \
    $$PWD/graphicscene.cpp \
    $$PWD/SSobjects/DialogIndSource/checkboxdelegate.cpp \
    $$PWD/SSobjects/DialogIndSource/fileedit.cpp \
    $$PWD/SSobjects/DialogIndSource/imagemodeldelegate.cpp \
    $$PWD/SSobjects/DialogIndSource/proxymodel.cpp \
    $$PWD/SSobjects/DialogIndSource/ssitemdialog.cpp \
    $$PWD/SSobjects/DialogIndSource/standardtablemodel.cpp \
    $$PWD/SSobjects/DialogIndSource/templateimage.cpp \
    $$PWD/SSobjects/DialogIndSource/uniqueproxymodel.cpp

