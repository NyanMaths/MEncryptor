#ifndef APPLICATION_H
#define APPLICATION_H


#include <QFontDatabase>
#include <QTranslator>
#include <QScreen>

#include <QTabWidget>
#include "EncryptorWidget.h"
#include "AnalyzerWidget.h"
#include "OptionsWidget.h"


class Application : public QTabWidget
{
    Q_OBJECT

    public:
        Application();


    private:
        QTranslator* translator;
        QTranslator* messageBoxesTranslator;


        EncryptorWidget* encryptorTab;

        AnalyzerWidget* analyzerTab;

        OptionsWidget* optionsTab;
};


#endif // APPLICATION_H
