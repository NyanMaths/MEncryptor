# Classic versioning : major.minor.patch
VERSION = 3.0.0

QT += core gui widgets
CONFIG += c++20

INCLUDEPATH += src


HEADERS += \
    src/Application.hpp \
    src/EncryptorWidget.hpp \
    src/AnalyzerWidget.hpp \
    src/OptionsWidget.hpp \
    src/TranslationReportDialog.hpp \
    src/protocols/morse.hpp \
    src/protocols/protocol.hpp \
    src/protocols/braille.hpp \
    src/protocols/cyrillic.hpp \
    src/protocols/greek.hpp \
    src/protocols/leet-speak.hpp \
    src/protocols/reversed-sentence.hpp \
    src/protocols/reversed-words.hpp \
    src/protocols/shift.hpp \
    src/protocols/unicode-values.hpp \
    src/protocols/values.hpp \
    src/protocols/vigenere.hpp

SOURCES += \
    src/Application.cpp \
    src/EncryptorWidget.cpp \
    src/AnalyzerWidget.cpp \
    src/OptionsWidget.cpp \
    src/TranslationReportDialog.cpp \
    src/protocols/morse.cpp \
    src/protocols/protocol.cpp \
    src/protocols/braille.cpp \
    src/protocols/cyrillic.cpp \
    src/protocols/greek.cpp \
    src/protocols/leet-speak.cpp \
    src/protocols/reversed-sentence.cpp \
    src/protocols/reversed-words.cpp \
    src/protocols/shift.cpp \
    src/protocols/unicode-values.cpp \
    src/protocols/values.cpp \
    src/protocols/vigenere.cpp \
    src/main.cpp

DISTFILES += \
    lang/mencryptor-fr.ts


TARGET = ../release/bin/mencryptor
TRANSLATIONS = lang/mencryptor-fr.ts
