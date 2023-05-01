#ifndef SQUAREAPIREQUESTS_H
#define SQUAREAPIREQUESTS_H

#include <QObject>

class SquareAPIRequests : public QObject
{
    Q_OBJECT
public:
    explicit SquareAPIRequests(QObject *parent = nullptr);

signals:

};

#endif // SQUAREAPIREQUESTS_H
