#ifndef ENCRYPTORWIDGET_H
#define ENCRYPTORWIDGET_H


#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QTextEdit>

#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>


class EncryptorWidget : public QWidget
{
    Q_OBJECT

    public:
        EncryptorWidget ();
        ~EncryptorWidget ();

        void updateOptionsImage (QString);


    private slots:
        void updateProcessText ();


        void startProcessAndDisplay ();
        void startProcessAndSave ();


    private:
        void initOptionsWidget ();
        void initProtocols ();
        void loadOptions ();

        QString reverse (const QString&);


        QStringList errors;

        QString encryptorOutput;


        QHBoxLayout* layout;

        QGroupBox* inputBox;
        QVBoxLayout* inputBoxLayout;
          QTextEdit* inputZone;

        QWidget* optionsWidget;
        QVBoxLayout* optionsLayout;
          QLabel* processImage;
          QLabel* chooseProcessLabel;
          QPushButton* processSelecter;
          QLabel* chooseProtocolLabel;
          QComboBox* protocolSelecter;

        QGroupBox* outputBox;
        QVBoxLayout* outputBoxLayout;
          QPushButton* bDisplayOutput;
          QPushButton* bSaveOutput;


        void chooseProcess (bool);
          unsigned short int e_LeetSpeak ();
          unsigned short int d_LeetSpeak ();

          unsigned short int reversedWords ();

          unsigned short int reversedSentence ();

          unsigned short int e_Greek ();
          unsigned short int d_Greek ();

          unsigned short int e_Cyrillic ();
          unsigned short int d_Cyrillic ();

          unsigned short int e_UnicodeValues (unsigned short int);
          unsigned short int d_UnicodeValues (unsigned short int);

          unsigned short int e_Shift ();
          unsigned short int d_Shift ();

          unsigned short int e_Braille ();
          unsigned short int d_Braille ();

          unsigned short int e_Values ();
          unsigned short int d_Values ();

          unsigned short int e_Vigenere ();
          unsigned short int d_Vigenere ();

          unsigned short int e_Morse ();
          unsigned short int d_Morse ();

        unsigned short int protocolCallBack_1 (bool, unsigned int);
        unsigned short int protocolCallBack_2 (bool, unsigned int);
        void outputHandling (bool);
};


#endif // ENCRYPTORWIDGET_H
