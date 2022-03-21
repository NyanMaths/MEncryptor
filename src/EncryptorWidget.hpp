#ifndef ENCRYPTORWIDGET_HPP
#define ENCRYPTORWIDGET_HPP


#include <QMap>

#include <QPushButton>
#include <QComboBox>
#include <QTextEdit>

#include <QGroupBox>
#include <QVBoxLayout>

#include "protocols/protocol.hpp"


class EncryptorWidget : public QWidget
{
    Q_OBJECT

    public:
        EncryptorWidget(const QMap<QString, QString>&);
        ~EncryptorWidget();

        void getConfig(QMap<QString, QString>&);


    private slots:
        void cipher();
        void decipher();


    private:
        void loadConfig(const QMap<QString, QString>&);
        void initProtocols();

        void handleOutput();


        const QMap<QString, QString> protocolsNames;
        const QMap<QString, Protocol*> engines;

        const QMap<Protocol::Error, QString> errors;

        QString encryptorOutput;


        QGridLayout* layout;


        QGroupBox* inputBox;
        QVBoxLayout* inputBoxLayout;
          QTextEdit* inputArea;

        QComboBox* protocolSelecter;
        QPushButton* cipherButton;
        QPushButton* decipherButton;
};


#endif  // ENCRYPTORWIDGET_HPP
