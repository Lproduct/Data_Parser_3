greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

INCLUDEPATH += C:\\Users\\Lvil\\Lvil\\Work\\AIEA_DataViewer\\Qt_5.4\\Data_Parser_3\\fftw-3.3.4-dll32
LIBS += -LC:\\Users\\Lvil\\Lvil\\Work\\AIEA_DataViewer\\Qt_5.4\\Data_Parser_3\\fftw-3.3.4-dll32 \
-lfftw3-3

SOURCES += \
    main.cpp \
    FenPrincipale.cpp \
    FenEnfantTab.cpp \
    qcustomplot.cpp \
    FenEnfantGraph.cpp \
    DataParser.cpp \
    MathFunction.cpp \
    overhauser.cpp \
    reglin.cpp \
    datasave.cpp \
    tabdata.cpp \
    form.cpp

HEADERS += \
    FenPrincipale.h \
    FenEnfantTab.h \
    qcustomplot.h \
    FenEnfantGraph.h \
    DataParser.h \
    MathFunction.h \
    fftw-3.3.4-dll32/fftw3.h \
    overhauser.hpp \
    vec3.hpp \
    reglin.h \
    datasave.h \
    tabdata.h \
    form.h

RESOURCES += \
    data_parser_3.qrc

FORMS += \
    FenEnfantGraph.ui \
    tabdata.ui \
    form.ui


