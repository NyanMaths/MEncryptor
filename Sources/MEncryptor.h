#ifndef MENCRYPTOR_H
#define MENCRYPTOR_H


#include <cstdlib>

#include <QProcess>
#include <QFontDatabase>
#include <QTranslator>
#include <QScreen>

#include <QTabWidget>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QTextEdit>

#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>


class MEncryptor : public QTabWidget
{
    Q_OBJECT

    public:
        MEncryptor ();

        ~MEncryptor ();


    private slots:
        void setProcessText ();
        void startProcessAndDisplay ();
        void startProcessAndSave ();

        void analyzeText ();
        void clearContents ();

        void languageModified (int);


    private:
        void initOptions ();

        void initEncryptor ();
          void initProtocols ();

        void initAnalyzer ();

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

          unsigned short int e_Values ();
          unsigned short int d_Values ();

        void outputHandling (bool);


        QString reverse (const QString&);


   //////////////////////////////////////////


        std::vector<std::string> options;

        QTranslator* translator;
        QTranslator* messageBoxesTranslator;


        QString output;

        QWidget* encryptorTab;
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

        QWidget* analyzerTab;
        QHBoxLayout* analyzerTabLayout;
          QGroupBox* analyzerInputBox;
          QGridLayout* analyzerInputBoxLayout;
            QTextEdit* analyzerInputZone;
            QCheckBox* analyzeSymbols;
            QCheckBox* caseSensitive;
            QPushButton* bAnalyze;
            QPushButton* bClear;

        QGroupBox* analyzerOutputBox;
        QVBoxLayout* analyzerOutputBoxLayout;
          QTextEdit* analyzerOutputZone;

        //////////////////////////////////

        QWidget* othersTab;
        QVBoxLayout* othersTabLayout;
          QLabel* aboutMRecorderLabel;
          QComboBox* languageSelecter;
};


#endif // MENCRYPTOR_H
