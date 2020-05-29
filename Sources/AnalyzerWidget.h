#ifndef ANALYZERWIDGET_H
#define ANALYZERWIDGET_H


#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QTextEdit>
#include <QCheckBox>

#include <QGroupBox>
#include <QHBoxLayout>


class AnalyzerWidget : public QWidget
{
    Q_OBJECT

    public:
        AnalyzerWidget ();
        ~AnalyzerWidget ();


    private slots:
        void analyzeText ();
        void clearContents ();


    private:
        void loadOptions ();

        void displayAnalysisReport (const QString&, const QString&);

        unsigned int mostFrequent (const QStringList&, const QString&);
        QStringList reverse (const QStringList&);


        QHBoxLayout* layout;

        QGroupBox* inputBox;
        QGridLayout* inputBoxLayout;
          QTextEdit* inputZone;
          QCheckBox* analyzeSymbols;
          QCheckBox* caseSensitive;
          QPushButton* bAnalyze;
          QPushButton* bClear;

        QGroupBox* outputBox;
        QGridLayout* outputBoxLayout;
          QLabel* chooseSortingLabel;
          QComboBox* sortingSelecter;
          QTextEdit* outputZone;
};


#endif // ANALYZERWIDGET_H
