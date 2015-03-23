greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

SOURCES += \
    main.cpp \
    FenPrincipale.cpp \
    FenEnfant.cpp \
    FenEnfantTab.cpp \
    qcustomplot.cpp

HEADERS += \
    FenPrincipale.h \
    FenEnfant.h \
    FenEnfantTab.h \
    qcustomplot.h

RESOURCES += \
    data_parser_3.qrc


