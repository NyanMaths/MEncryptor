#include <fstream>
#include <QFile>

#include "AnalyzerWidget.h"


//////////////  Initialize widget


AnalyzerWidget::AnalyzerWidget () : QWidget ()
{
    layout = new QHBoxLayout (this);


    inputBox = new QGroupBox (tr("Message"));
    inputBoxLayout = new QGridLayout (inputBox);

    inputZone = new QTextEdit;
    inputBoxLayout->addWidget (inputZone, 0, 0, 1, 2);
    inputZone->setAcceptRichText (false);

    analyzeSymbols = new QCheckBox (tr("Also analyze symbols"));
    inputBoxLayout->addWidget (analyzeSymbols, 1, 0);

    caseSensitive = new QCheckBox (tr("Case sensitive"));
    inputBoxLayout->addWidget (caseSensitive, 2, 0);

    bAnalyze = new QPushButton (tr("Analyze"));
    inputBoxLayout->addWidget (bAnalyze, 1, 1);
    connect (bAnalyze, SIGNAL (clicked ()), this, SLOT (analyzeText ()));

    bClear = new QPushButton (tr("Clear"));
    inputBoxLayout->addWidget (bClear, 2, 1);
    connect (bClear, SIGNAL (clicked ()), this, SLOT (clearContents ()));


    outputBox = new QGroupBox (tr("Analysis report"));
    outputBoxLayout = new QGridLayout (outputBox);

    outputZone = new QTextEdit;
    outputBoxLayout->addWidget (outputZone, 0, 0, 1, 2);
    outputZone->setReadOnly (true);

    chooseSortingLabel = new QLabel (tr("Sort by :"));
    outputBoxLayout->addWidget (chooseSortingLabel, 1, 0, 1, 1, Qt::AlignRight);

    sortingSelecter = new QComboBox;
    sortingSelecter->addItem (tr("Alphabetical order"));
    sortingSelecter->addItem (tr("Frequency (croissant)"));
    sortingSelecter->addItem (tr("Frequency (decroissant)"));
    outputBoxLayout->addWidget (sortingSelecter, 1, 1);


    layout->addWidget (inputBox);
    layout->addWidget (outputBox);


    loadOptions ();
}


void AnalyzerWidget::loadOptions ()
{
    QFile settingsFile ("Analyzer Options.pastouche");

    if (settingsFile.open (QIODevice::ReadOnly | QIODevice::Text))
    {
        QStringList data = QString (settingsFile.readAll ()).split ("\n");


        analyzeSymbols->setChecked (data.at (0).toUShort ());

        caseSensitive->setChecked (data.at (1).toUShort ());

        sortingSelecter->setCurrentIndex (data.at (2).toUShort ());
    }
    else
        sortingSelecter->setCurrentIndex (2);
}

AnalyzerWidget::~AnalyzerWidget ()
{
    std::ofstream settingsFile ("Analyzer Options.pastouche");

    if (settingsFile)
    {
        settingsFile<<analyzeSymbols->isChecked ()<<"\n"
                    <<caseSensitive->isChecked ()<<"\n"
                    <<sortingSelecter->currentIndex ();
    }
}


//////////////  Slots


void AnalyzerWidget::analyzeText ()
{
    QString input = inputZone->toPlainText ().remove (" ").remove ("\t").remove ("\n");

    if (!inputZone->toPlainText ().isEmpty ())
    {
        outputZone->clear ();


        QString dict;

        if (!caseSensitive->isChecked ())
            input = input.toUpper ();


        if (analyzeSymbols->isChecked ())
        {
            for (int i = 0 ; i != input.length () ; i++)
            {
                if (!dict.contains (input.at (i)))
                    dict += input.at (i);
            }
        }
        else
        {
            for (int i = 0 ; i != input.length () ; i++)
            {
                if (input.at (i).isLetter () && !dict.contains (input.at (i)))
                    dict += input.at (i);
            }
        }

        displayAnalysisReport (input, dict);
    }
}


void AnalyzerWidget::clearContents ()
{
    inputZone->clear ();
    outputZone->clear ();
}


//////////////  Others


void AnalyzerWidget::displayAnalysisReport (const QString& input, const QString& dict)
{
    QStringList splitDict (dict.split (""));
    splitDict.sort ();
    splitDict.removeAll ("");


    if (sortingSelecter->currentText () == tr("Alphabetical order"))
    {
        for (int i = 0 ; i != splitDict.length () ; i++)
            outputZone->setText (outputZone->toPlainText () + QString::number (input.count (splitDict.at (i))) + tr(" occurrence(s) of character ") + splitDict.at (i) + "\n");
    }
    else
    {
        if (sortingSelecter->currentText () == tr("Frequency (croissant)"))
            splitDict = reverse (splitDict);

        QStringList splitReport;
        unsigned int currentCharIndex;

        while (!splitDict.isEmpty ())
        {
            currentCharIndex = mostFrequent (splitDict, input);

            splitReport += QString::number (input.count (splitDict.at (currentCharIndex))) + tr(" occurrence(s) of character ") + splitDict.at (currentCharIndex);

            splitDict.removeAt (currentCharIndex);
        }

        if (sortingSelecter->currentText () == tr("Frequency (croissant)"))
            splitReport = reverse (splitReport);

        outputZone->setText (splitReport.join ("\n"));
    }
}


//////////////  Tools


unsigned int AnalyzerWidget::mostFrequent (const QStringList& dict, const QString& input)
{
    QString mostFrequent = dict.at (0);

    for (int i = 1 ; i != dict.length () ; i++)
    {
        if (input.count (mostFrequent) < input.count (dict.at (i)))
            mostFrequent = dict.at (i);
    }

    return dict.indexOf (mostFrequent);
}


QStringList AnalyzerWidget::reverse (const QStringList& strList)
{
    QStringList reversedStrList;

    for (int i = strList.length () - 1 ; i != -1 ; i--)
        reversedStrList.push_back (strList.at (i));

    return reversedStrList;
}

