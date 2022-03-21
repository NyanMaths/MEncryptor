#ifndef APPLICATION_HPP
#define APPLICATION_HPP


#include <QTranslator>
#include <QMap>

#include <QTabWidget>

#include "EncryptorWidget.hpp"
#include "AnalyzerWidget.hpp"
#include "OptionsWidget.hpp"


class Application : public QTabWidget
{
    Q_OBJECT

    public:
        Application();
        ~Application();


    private:
        void readConfig();


        QTranslator* translator;
        QTranslator* messageBoxesTranslator;


        QMap<QString, QString> config;


        EncryptorWidget* encryptor;
        AnalyzerWidget* frequencyAnalyzer;
        OptionsWidget* options;
};


#endif  // APPLICATION_HPP
