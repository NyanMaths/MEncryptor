#ifndef OPTIONSWIDGET_H
#define OPTIONSWIDGET_H


#include <QApplication>

#include "EncryptorWidget.h"

#include <QGroupBox>
#include <QGridLayout>

#include <QMessageBox>


class OptionsWidget : public QWidget
{
    Q_OBJECT

    public:
        OptionsWidget (EncryptorWidget*);
        ~OptionsWidget ();


    private slots:
        void changeLanguage ();
        void changeTheme (int);


    private:
        void initPalettes ();

        void loadOptions ();


        QPalette lightPalette;
        QPalette darkPalette;

        EncryptorWidget* encryptorTab;


        QGridLayout* layout;

        QLabel* aboutMEncryptorLabel;

        QGroupBox* UIOptionsBox;
        QGridLayout* UIOptionsBoxLayout;

        QLabel* chooseLanguageLabel;
        QComboBox* languageSelecter;
        QLabel* chooseThemeLabel;
        QComboBox* themeSelecter;
};


#endif // OPTIONSWIDGET_H
