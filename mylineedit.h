#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>
#include <QHash>


class myLineEdit : public QLineEdit
{
Q_OBJECT
public:
    explicit myLineEdit(QWidget *parent = 0, int latch = 0, int id = 0);
    void setLatchId(int, int);

signals:
    void inputChanged(int, int);

public slots:
    void switchField(QString);

private:
    int latch;
    int id;

};

#endif // MYLINEEDIT_H
