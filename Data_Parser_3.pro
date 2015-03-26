greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

SOURCES += \
    main.cpp \
    FenPrincipale.cpp \
    FenEnfantTab.cpp \
    qcustomplot.cpp \
    FenEnfantGraph.cpp \
    DataParser.cpp

HEADERS += \
    FenPrincipale.h \
    FenEnfantTab.h \
    qcustomplot.h \
    FenEnfantGraph.h \
    DataParser.h

RESOURCES += \
    data_parser_3.qrc

FORMS += \
    FenEnfantGraph.ui


