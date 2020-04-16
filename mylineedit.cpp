#include "mylineedit.h"

//#include <QHash>


myLineEdit::myLineEdit(QWidget *parent, int latch, int id) :
    QLineEdit(parent), latch(latch), id(id)
{
    connect(this, SIGNAL(textEdited(QString)), SLOT(switchField(QString)));

    latch = latch;

    id = id;

    this->setText("0");
}


void myLineEdit::switchField(QString s)
{
    if (s.size() == this->maxLength())
        this->focusNextChild();

    if (s == "")
        this->setText("0");

    emit inputChanged(this->latch, this->id);
}

void myLineEdit::setLatchId(int latch, int id)
{
    this->latch = latch;
    this->id = id;
}
