#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifdef UNICODE
#define QStringToTCHAR(x)     (wchar_t*) x.utf16()
#define PQStringToTCHAR(x)    (wchar_t*) x->utf16()
#define TCHARToQString(x)     QString::fromUtf16((x))
#define TCHARToQStringN(x,y)  QString::fromUtf16((x),(y))
#else
#define QStringToTCHAR(x)     x.local8Bit().constData()
#define PQStringToTCHAR(x)    x->local8Bit().constData()
#define TCHARToQString(x)     QString::fromLocal8Bit((x))
#define TCHARToQStringN(x,y)  QString::fromLocal8Bit((x),(y))
#endif

#include <QMainWindow>
#include <QtGui/QLineEdit>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QLabel>
#include <QMessageBox>
#include <QGroupBox>
#include <QUrl>
#include <windows.h>
#include <algorithm>
#include "mylineedit.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:

    MainWindow(QWidget *parent = 0);

    ~MainWindow();

    QString codeString;

    QString asmblrPath;

    QString helpString;

    myLineEdit *counters[3];

    myLineEdit *bits[3][24];

    QHash<int, QLabel *> labels;

    QHash<int, QGroupBox *> boxes;

    QString binString[4];


protected:

    void changeEvent(QEvent *e);

public slots:

    void updateOthers(int latch, int id);

    void settingsWindow(int i);

    QString saveAsmFile();

    int build();

    void generateCode();

    void saveSettings();

    void selectAsmblr();

    void defaultSettings();

    void viewTemplate();

private:

    Ui::MainWindow *ui;

    bool findAsmblr(QString path);

    void setstyle(int, bool);

};


#endif // MAINWINDOW_H

