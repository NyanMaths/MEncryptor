#ifndef ANALYZERWIDGET_HPP
#define ANALYZERWIDGET_HPP


#include <QMap>

#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QPlainTextEdit>

#include <QGroupBox>
#include <QHBoxLayout>


class AnalyzerWidget : public QWidget
{
    Q_OBJECT


    public:
        AnalyzerWidget(const QMap<QString, QString>&);

        void getConfig(QMap<QString, QString>&);


    private slots:
        void analyzeText();
        void clearContents();


    private:
        void loadConfig(const QMap<QString, QString>&);

        void displayAnalysisReport(const QString&, const QString&);

        unsigned int mostFrequent(const QVector<QString>&, const QString&);
        QVector<QString> reverse(const QVector<QString>&);


        QHBoxLayout* layout;

        QGroupBox* inputBox;
        QGridLayout* inputBoxLayout;
          QPlainTextEdit* inputZone;
          QCheckBox* analyzeSymbols;
          QCheckBox* caseSensitive;
          QPushButton* bAnalyze;
          QPushButton* bClear;

        QGroupBox* outputBox;
        QGridLayout* outputBoxLayout;
          QTextEdit* outputZone;
          QLabel* chooseSortingLabel;
          QComboBox* sortingSelecter;

};


#endif  // ANALYZERWIDGET_HPP
