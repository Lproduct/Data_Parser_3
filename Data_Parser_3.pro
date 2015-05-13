greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


LIBS += -LC:\Users\User\Lvil\Work\AIEA_DataViewer\Qt_5.4\Data_Parser_3\fftw-3.3.4-dll32\ -lfftw3-3
INCLUDEPATH += C:\Users\User\Lvil\Work\AIEA_DataViewer\Qt_5.4\Data_Parser_3\fftw-3.3.4-dll32

SOURCES += \
    main.cpp \
    FenPrincipale.cpp \
    FenEnfantTab.cpp \
    qcustomplot.cpp \
    FenEnfantGraph.cpp \
    DataParser.cpp \
    MathFunction.cpp \
    overhauser.cpp

HEADERS += \
    FenPrincipale.h \
    FenEnfantTab.h \
    qcustomplot.h \
    FenEnfantGraph.h \
    DataParser.h \
    MathFunction.h \
    fftw-3.3.4-dll32/fftw3.h \
    overhauser.hpp \
    vec3.hpp

RESOURCES += \
    data_parser_3.qrc

FORMS += \
    FenEnfantGraph.ui


