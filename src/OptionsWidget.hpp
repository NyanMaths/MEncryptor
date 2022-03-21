#ifndef OPTIONSWIDGET_HPP
#define OPTIONSWIDGET_HPP


#include <QGroupBox>
#include <QGridLayout>

#include <QLabel>
#include <QComboBox>


class OptionsWidget : public QWidget
{
    Q_OBJECT


    public:
        OptionsWidget(const QMap<QString, QString>&);

        void getConfig(QMap<QString, QString>&);


    private slots:
        void promptToRestart();


    private:
        void loadConfig(const QMap<QString, QString>&);
        void initDarkPalette();


        QPalette darkPalette;

        QGridLayout* layout;

        QLabel* aboutMEncryptorLabel;

        QGroupBox* UIOptionsBox;
        QGridLayout* UIOptionsBoxLayout;

        QLabel* chooseLanguageLabel;
        QComboBox* languageSelecter;
        QLabel* chooseThemeLabel;
        QComboBox* themeSelecter;
};


#endif  // OPTIONSWIDGET_HPP
