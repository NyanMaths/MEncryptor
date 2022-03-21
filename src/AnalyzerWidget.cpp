#include "AnalyzerWidget.hpp"


///////////////////  AnalyzerWidget initialization  ///////////////////


AnalyzerWidget::AnalyzerWidget (const QMap<QString, QString>& config) : QWidget (),

    layout(new QHBoxLayout(this)),


    inputBox(new QGroupBox(tr("Message"))),
    inputBoxLayout(new QGridLayout(inputBox)),

    inputZone(new QPlainTextEdit),

    analyzeSymbols(new QCheckBox(tr("Also analyze symbols"))),
    caseSensitive(new QCheckBox(tr("Case sensitive"))),

    bAnalyze(new QPushButton(tr("Analyze"))),
    bClear(new QPushButton(tr("Clear"))),


    outputBox(new QGroupBox(tr("Analysis report"))),
    outputBoxLayout(new QGridLayout(outputBox)),

    outputZone(new QTextEdit),

    chooseSortingLabel(new QLabel(tr("Sort by :"))),
    sortingSelecter(new QComboBox)
{
    outputZone->setReadOnly(true);

    sortingSelecter->addItem(tr("Alphabetical order"));
    sortingSelecter->addItem(tr("Frequency (croissant)"));
    sortingSelecter->addItem(tr("Frequency (decroissant)"));


    inputBoxLayout->addWidget(inputZone, 0, 0, 1, 2);
    inputBoxLayout->addWidget(analyzeSymbols, 1, 0);
    inputBoxLayout->addWidget(caseSensitive, 2, 0);
    inputBoxLayout->addWidget(bAnalyze, 1, 1);
    inputBoxLayout->addWidget(bClear, 2, 1);

    outputBoxLayout->addWidget(outputZone, 0, 0, 1, 2);
    outputBoxLayout->addWidget(chooseSortingLabel, 1, 0, 1, 1, Qt::AlignRight);
    outputBoxLayout->addWidget(sortingSelecter, 1, 1);

    layout->addWidget(inputBox);
    layout->addWidget(outputBox);


    loadConfig(config);
}


/*
I prefer connecting signals and slots after loading the configuration because some slots could be called uselessly.
*/
void AnalyzerWidget::loadConfig (const QMap<QString, QString>& config)
{
    analyzeSymbols->setChecked(config["analyze-symbols"].toUInt());
    caseSensitive->setChecked(config["case-sensitive"].toUInt());
    sortingSelecter->setCurrentIndex(config["sorting-index"].toUInt());


    connect(bAnalyze, &QPushButton::clicked, this, &AnalyzerWidget::analyzeText);
    connect(bClear, &QPushButton::clicked, this, &AnalyzerWidget::clearContents);
    connect(analyzeSymbols, &QCheckBox::stateChanged, this, &AnalyzerWidget::analyzeText);
    connect(caseSensitive, &QCheckBox::stateChanged, this, &AnalyzerWidget::analyzeText);

    connect(sortingSelecter, &QComboBox::currentIndexChanged, this, &AnalyzerWidget::analyzeText);
}

void AnalyzerWidget::getConfig (QMap<QString, QString>& config)
{
    config["analyze-symbols"] = QString::number(analyzeSymbols->isChecked());
    config["case-sensitive"] = QString::number(caseSensitive->isChecked());
    config["sorting-index"] = QString::number(sortingSelecter->currentIndex());
}


///////////////////  Analysis methods  ///////////////////


void AnalyzerWidget::analyzeText ()
{
    QString input(inputZone->toPlainText().remove(QRegularExpression("[ \t\n]")));

    if (!inputZone->toPlainText().isEmpty())
    {
        outputZone->clear();


        QString dict;

        if (!caseSensitive->isChecked())
            input = input.toUpper();


        if (analyzeSymbols->isChecked())
        {
            for (qsizetype i(0) ; i != input.length() ; i++)
            {
                if (!dict.contains(input.at(i)))
                    dict += input.at(i);
            }
        }
        else
        {
            for (qsizetype i(0) ; i != input.length() ; i++)
            {
                if (input.at(i).isLetter() && !dict.contains(input.at(i)))
                    dict += input.at(i);
            }
        }

        displayAnalysisReport(input, dict);
    }
}

void AnalyzerWidget::displayAnalysisReport (const QString& input, const QString& dict)
{
    QVector<QString> splitDict(dict.split(""));
    splitDict.sort();
    splitDict.removeAll("");


    if (sortingSelecter->currentText() == tr("Alphabetical order"))
    {
        for (qsizetype i(0) ; i != splitDict.length() ; i++)
            outputZone->setText(outputZone->toPlainText() + QString::number(input.count(splitDict.at(i))) + tr(" occurrence(s) of character ") + splitDict.at(i) + "\n");
    }
    else
    {
        if (sortingSelecter->currentText() == tr("Frequency (croissant)"))
            splitDict = reverse(splitDict);

        QVector<QString> splitReport;
        qsizetype currentCharIndex;

        while (!splitDict.isEmpty())
        {
            currentCharIndex = mostFrequent(splitDict, input);

            splitReport += QString::number(input.count(splitDict.at(currentCharIndex))) + tr(" occurrence(s) of character ") + splitDict.at(currentCharIndex);

            splitDict.removeAt(currentCharIndex);
        }

        if (sortingSelecter->currentText() == tr("Frequency (croissant)"))
            splitReport = reverse(splitReport);

        outputZone->setText(splitReport.join("\n"));
    }
}


void AnalyzerWidget::clearContents ()
{
    inputZone->clear();
    outputZone->clear();
}


///////////////////  Analysis tools  ///////////////////


unsigned int AnalyzerWidget::mostFrequent (const QVector<QString>& dict, const QString& input)
{
    QString mostFrequent = dict.at(0);

    for (qsizetype i(1) ; i < dict.length() ; i++)
    {
        if (input.count(mostFrequent) < input.count(dict.at(i)))
            mostFrequent = dict.at(i);
    }

    return dict.indexOf(mostFrequent);
}


QVector<QString> AnalyzerWidget::reverse (const QVector<QString>& strList)
{
    QVector<QString> reversedStrList;

    for (qsizetype i(strList.length() - 1) ; i != -1 ; i--)
        reversedStrList.push_back(strList.at(i));

    return reversedStrList;
}

