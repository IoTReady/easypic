#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <QString>

#include <QHash>

#include <QFile>

#include <QTextStream>


//defaults for resetting.
bool wdtdefault = false;

bool mclrdefault = false;

bool cpdefault = false;

bool gp2default = true;

int progMethod = 1;

//values used in present program
bool wdt = wdtdefault;

bool mclr = mclrdefault;

bool cp = cpdefault;

bool gp2 = gp2default;


QString codegen(QString binString[4]);

//QString header();

//QString flags();

//QString init();

QString latchcmd(QString);

//QString end();


#endif // CODEGENERATOR_H
