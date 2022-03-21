#ifndef TRANSLATIONREPORTDIALOG_HPP
#define TRANSLATIONREPORTDIALOG_HPP


#include <QDialog>
#include <QPushButton>
#include <QPlainTextEdit>

#include <QGridLayout>


class TranslationReportDialog : public QDialog
{
    public:
        TranslationReportDialog(const QString&);


    private slots:
        void saveReport();
        void emailReport();


    private:
        QGridLayout* layout;

        QPlainTextEdit* report;
        QPushButton* saveReportButton;
        QPushButton* emailReportButton;
};


#endif  // TRANSLATIONREPORTDIALOG_HPP
