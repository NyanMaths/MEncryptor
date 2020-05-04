#ifndef MENCRYPTOR_H
#define MENCRYPTOR_H


#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cwchar>

#include <QApplication>
#include <QObject>
#include <QProcess>
#include <QFileInfo>
#include <QFontDatabase>
#include <QTranslator>

#include <QWidget>
#include <QScreen>
#include <QTabWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QTextEdit>

#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QMessageBox>
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>


class MEncryptor : public QTabWidget
{
    Q_OBJECT

    public:
        MEncryptor ();

        ~MEncryptor ();


    public slots:
        void setProcessText ();
        void languageModified (int);

        void startProcessAndDisplay ();
        void startProcessAndSave ();


    private:
        void initEncryptor ();
          void initProtocols ();

        void initOthers ();


        void chooseProcess (bool);
          unsigned short int protocolCallBack_1 (bool, unsigned int);


          unsigned short int e_LeetSpeak ();
          unsigned short int d_LeetSpeak ();

          unsigned short int reversedWords ();

          unsigned short int reversedSentence ();

          unsigned short int e_ASCIIValues ();
          unsigned short int d_ASCIIValues ();

          unsigned short int e_Shift ();
          unsigned short int d_Shift ();

          unsigned short int e_Braille ();
          unsigned short int d_Braille ();


        void outputHandling (bool);

        void restart ();


        QString reverse (const QString&);


        std::string language;
        QString output;
        QStringList processes;

        QWidget* encryptorTab;
        QWidget* othersTab;

        QFont UIFont;

        QTranslator* translator;


        QHBoxLayout* encryptorLayout;
          QGroupBox* inputBox;
          QVBoxLayout* inputBoxLayout;
            QTextEdit* inputZone;

          QWidget* processWidget;
          QVBoxLayout* processLayout;
            QLabel* processImage;
            QLabel* chooseProcessLabel;
            QPushButton* processSelecter;
            QLabel* chooseProtocolLabel;
            QComboBox* protocolSelecter;

          QGroupBox* outputBox;
          QVBoxLayout* outputBoxLayout;
            QPushButton* bDisplayOutput;
            QPushButton* bSaveOutput;

        //////////////////////////////////

        QVBoxLayout* othersTabLayout;
          QLabel* aboutMRecorderLabel;
          QComboBox* languageSelecter;
};


#endif // MENCRYPTOR_H
