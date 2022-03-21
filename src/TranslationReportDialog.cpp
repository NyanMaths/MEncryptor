#include <QApplication>
#include <QDesktopServices>
#include <QFile>

#include <QFileDialog>
#include <QMessageBox>

#include "TranslationReportDialog.hpp"


///////////////////  TranslationReportDialog initialization  ///////////////////


TranslationReportDialog::TranslationReportDialog (const QString& translatedMessage) : QDialog(),

    layout(new QGridLayout(this)),


    report(new QPlainTextEdit(translatedMessage)),

    saveReportButton(new QPushButton(tr("Save translated message as..."))),
    emailReportButton(new QPushButton(tr("Email message")))
{
    report->setReadOnly(true);


    layout->addWidget(report, 0, 0, 1, 2);
    layout->addWidget(saveReportButton, 1, 0);
    layout->addWidget(emailReportButton, 1, 1);


    connect(saveReportButton, &QPushButton::clicked, this, &TranslationReportDialog::saveReport);
    connect(emailReportButton, &QPushButton::clicked, this, &TranslationReportDialog::emailReport);
}


///////////////////  Actions  ///////////////////


void TranslationReportDialog::saveReport ()
{
    QString destFileName(QFileDialog::getSaveFileName(this, tr("Save translated message as..."), tr("Untitled.txt"), "", nullptr, QFileDialog::DontUseNativeDialog));

    if (!destFileName.isEmpty())
    {
        QFile destination(destFileName);

        if (destination.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        {
            QTextStream destinationStream(&destination);

            destinationStream<<report->toPlainText();

            QMessageBox::information(this, tr("Operation successful, Houston !"), tr("File ") + destFileName + tr(" saved !"));
        }
        else
            QMessageBox::critical(this, tr("Unknown error"), tr("Impossible to save anything, maybe you tried a protected folder and/or I do not have the permissions to write here."));
    }
}

void TranslationReportDialog::emailReport ()  // Not working anymore for some reason
{
    QDesktopServices::openUrl(QUrl(tr("mailto:someone@mail.com?body=") + report->toPlainText(), QUrl::TolerantMode));
}
