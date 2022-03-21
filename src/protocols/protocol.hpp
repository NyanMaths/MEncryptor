#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP


#include <cinttypes>
#include <QWidget>


class Protocol : public QObject
{
    Q_OBJECT

    public:

        enum Error : uint8_t
        {
            NoError = 0u,
            MissingDataFile = 1u,
            CorruptedMessage = 2u,
            InvalidKey = 3u,
            Aborted = 42u
        };


        virtual ~Protocol() = 0;

        static QWidget* mainWindow();
        static void setMainWindow(QWidget*);

        virtual Protocol::Error cipher(QString&) const = 0;
        virtual Protocol::Error decipher(QString&) const = 0;


        static QString reversed(const QString&);


    private:
        static QWidget* _mainWindow;
};


#endif  // PROTOCOL_HPP
