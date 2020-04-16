#include "mainwindow.h"
#include "ui_easypic.h"
#include "codegenerator.cpp"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    QLabel *dblabel[3][24];

    QString bit, padding;

    QHash<int, QString> lPrefix;
    lPrefix[0] = "r";
    lPrefix[1] = "n";
    lPrefix[2] = "f";

    boxes[0] = ui->rGroupBox;

    boxes[1] = ui->nGroupBox;

    boxes[2] = ui->fGroupBox;

    labels[0] = ui->rc_dec_label;

    labels[1] = ui->bc_dec_label;

    labels[2] = ui->ac_dec_label;

    //Set up counter decimal fields
    for (int i = 0; i < 3; ++i)
    {
        counters[i] = new myLineEdit(ui->cGroupBox, 3, i);

        counters[i]->setGeometry(QRect(100, 20 + i * 20, 100, 20));

        counters[i]->setAlignment(Qt::AlignCenter);

        connect(counters[i], SIGNAL(inputChanged(int, int)), this, SLOT(updateOthers(int, int)));

    }

    QValidator *rv = new QIntValidator(1, 16383, this); //Ref counter: 14bits = 16383 max

    counters[0]->setValidator(rv);

    QValidator *bv = new QIntValidator(1, 8191, this); //B counter: 13bits = 8191 max

    counters[1]->setValidator(bv);

    QValidator *av = new QIntValidator(1, 63, this); //A counter: 6bits = 63 max

    counters[2]->setValidator(av);


    QValidator *bitv = new QIntValidator(0, 1, this);

    //Set up bit fields

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 23; j > -1; --j)
        {
            bit.setNum(j);

            dblabel[i][j] = new QLabel(boxes[i]);

            dblabel[i][j]->setText("DB\n" + bit);

            dblabel[i][j]->setGeometry(QRect(0 + (23 - j) * 25, 50, 25, 25));

            dblabel[i][j]->setFrameShape(QFrame::StyledPanel);

            dblabel[i][j]->setScaledContents(true);

            dblabel[i][j]->setAlignment(Qt::AlignCenter);

            dblabel[i][j]->setWordWrap(true);


            bits[i][j] = new myLineEdit(boxes[i], i, j);

            bits[i][j]->setToolTip("DB" + bit);

            bits[i][j]->setMaxLength(1);

            bits[i][j]->setGeometry(QRect(0 + (23 - j) * 25, 75, 25, 25));

            bits[i][j]->setAlignment(Qt::AlignCenter);

            bits[i][j]->setValidator(bitv);

            connect(bits[i][j], SIGNAL(inputChanged(int, int)), this, SLOT(updateOthers(int, int)));

        }
    }



    for (int i = 0; i < 3; ++i)
    {
        bit.setNum(i, 2);

        padding.fill('0', 2 - bit.size());

        bit.prepend(padding);

        for (int j = 0; j < 2; ++j)
        {

            bits[i][j]->setText( bit.section("", 2 - j, 2 - j));

            bits[i][j]->setReadOnly(true);

        }
    }


    connect(ui->wdtCheck, SIGNAL(stateChanged(int)), this,
            SLOT(settingsWindow(int)));

    connect(ui->mclrCheck, SIGNAL(stateChanged(int)), this,
            SLOT(settingsWindow(int)));

    connect(ui->cpCheck, SIGNAL(stateChanged(int)), this,
            SLOT(settingsWindow(int)));

    connect(ui->gp2Check, SIGNAL(stateChanged(int)), this,
            SLOT(settingsWindow(int)));

    connect(ui->asmblrButton, SIGNAL(clicked()), this,
            SLOT(selectAsmblr()));

    connect(ui->mainBox, SIGNAL(currentChanged(int)), this,
            SLOT(settingsWindow(int)));

    connect(ui->submitButton, SIGNAL(clicked()), this,
            SLOT(generateCode()));

    connect(ui->buildButton, SIGNAL(clicked()), this,
            SLOT(build()));

    connect(ui->saveButton, SIGNAL(clicked()), this,
            SLOT(saveAsmFile()));

    connect(ui->svsettingsButton, SIGNAL(clicked()), this,
            SLOT(saveSettings()));

    connect(ui->defaultsButton, SIGNAL(clicked()), this,
            SLOT(defaultSettings()));

    connect(ui->templateButton, SIGNAL(clicked()), this, SLOT(viewTemplate()));


    setWindowIcon(QIcon(":/images/binary.png"));

    ui->submitButton->setEnabled(false);

    ui->saveButton->setEnabled(false);

    ui->buildButton->setEnabled(false);

    //Find assembler if possible
    if (findAsmblr("C:\\Program Files\\Microchip\\MPASM Suite\\"))
    {
        ui->asmblrPathLine->setText(asmblrPath);
        ui->asmblrPathLine->setReadOnly(true);
    }

    else if (findAsmblr("C:\\Program Files (x86)\\Microchip\\MPASM Suite\\"))
    {
        ui->asmblrPathLine->setText(asmblrPath);
        ui->asmblrPathLine->setReadOnly(true);
    }

    //Default values displayed on Settings tab on first run.
    settingsWindow(1);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


bool MainWindow::findAsmblr(QString path)
{
    QDir d = QDir::QDir(path);

    if (d.exists("MPASMWIN.exe"))
    {
        asmblrPath = d.absoluteFilePath("MPASMWIN.exe");
        return true;
    }
    return false;
}

void MainWindow::viewTemplate()
{
    QString codeTemplate;

    QFile file(":/text/code_template.asm");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))

        return;

    QTextStream temp(&file);

    codeTemplate = temp.readAll();

    ui->helpBrowser->setText(codeTemplate);
    
}

void MainWindow::settingsWindow(int i)
{
    ui->gp2Check->setEnabled(false);

    if (i == 1)
    {
        ui->wdtCheck->setChecked(wdt);
        ui->mclrCheck->setChecked(mclr);
        ui->cpCheck->setChecked(cp);
        ui->gp2Check->setChecked(gp2);
    }

    bool changed = (ui->wdtCheck->isChecked() != wdt) ||
                   (ui->mclrCheck->isChecked() != mclr) ||
                   (ui->cpCheck->isChecked() != cp) ||
                   (ui->gp2Check->isChecked() != gp2);

    bool notdefault = (ui->wdtCheck->isChecked() != wdtdefault) ||
                      (ui->mclrCheck->isChecked() != mclrdefault) ||
                      (ui->cpCheck->isChecked() != cpdefault) ||
                      (ui->gp2Check->isChecked() != gp2default);

    ui->defaultsButton->setEnabled(notdefault);
    ui->svsettingsButton->setEnabled(changed);
    ui->asmblrPathLine->setText(asmblrPath);

}



void MainWindow::updateOthers(int latch, int id)
{
    int dir = 0;

    int start = 24;

    int length = 0;

    int counter = 4;

    bool valid[3];

    switch (latch)
    {
    case 0:

        if (id > 1 && id < 16) //Ref Counter
        {

            latch = 0;

            counter = 0;

            start = 2;

            length = 14;

            dir = -1;
        }

        else
            dir = 0;

        break;

    case 1:

        if (id > 1 && id < 8) //'A' Counter
        {

            latch = 1;

            counter = 2;

            start = 2;

            length = 6;

            dir = -1;
        }

        else if (id > 7 && id < 21) //'B' Counter
        {

            latch = 1;

            counter = 1;

            start = 8;

            length = 13;

            dir = -1;
        }

        else
            dir = 0;

        break;

    case 2:

        dir = 0;

        break;


    case 3://Counter Decimal fields updated

        switch(id)
        {

        case 0://Ref. Counter

            latch = 0;

            start = 2;

            length = 14;

            dir = 1;

            break;

        case 1://'B' Counter

            latch = 1;

            start = 8;

            length = 13;

            dir = 1;

            break;

        case 2://'A' Counter

            latch = 1;

            start = 2;

            length = 6;

            dir = 1;

            break;

        default:
            break;

        }

        break;

    default://Other fields edited, nothing to be done.

        break;

    }


    if (dir == 1)
    {
        QString padding, cString;

        int decValue;

        decValue = counters[id]->text().toInt();

        cString.setNum(decValue, 2);

        padding.fill('0', length - cString.size());

        cString.prepend(padding);

        for (int j = start; j < start + length; ++j)
        {

            bits[latch][j]->setText(cString.section("", length - j + start, length - j + start));

        }
    }

    else if (dir == -1)
    {
        QString cBinary, cDecimal;

        bool ok;

        cBinary = "";

        for (int j = start ; j < start + length; ++j)
        {

            cBinary.prepend(bits[latch][j]->text());


        }

        cDecimal.setNum(cBinary.toInt(&ok, 2));

        counters[counter]->setText(cDecimal);

    }


    for (int counter = 0; counter < 3; ++counter)
    {
        valid[counter] = counters[counter]->hasAcceptableInput();

        setstyle(counter, valid[counter]);
    }

    ui->submitButton->setEnabled(valid[0] && valid[1] && valid[2]);
}

QString MainWindow::saveAsmFile()
{
    QString fileName = "";
    fileName = QFileDialog::getSaveFileName(this, tr("Save Assembly Code in a File"),
                                            "",tr("Assembly Code(*.asm *.as)"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return "0";

    QTextStream out(&file);
    out << MainWindow::codeString;

    return fileName;

}

int MainWindow::build()
{
    TCHAR*              cmdline;
    STARTUPINFO         startup_info;
    PROCESS_INFORMATION process_info;

    QString cmd = "\"" + asmblrPath + "\" " + ui->buildOptionsLine->text() + " ";

    QString fileName = saveAsmFile();

    if (fileName != "0")

        cmd += "\"" + fileName + "\"";

    else
        return 2;

    cmdline = QStringToTCHAR(cmd);

    std::fill_n( (char*)&process_info, sizeof( PROCESS_INFORMATION ), 0 );

    std::fill_n( (char*)&startup_info, sizeof( STARTUPINFO ),         0 );

    startup_info.cb = sizeof( STARTUPINFO );

    if (!CreateProcess(
            NULL,
            cmdline,
            NULL,
            NULL,
            TRUE,
            NORMAL_PRIORITY_CLASS,
            NULL,
            NULL,
            &startup_info,
            &process_info
            ))
        return 2;

    CloseHandle( process_info.hThread );

    // wait for the child program to terminate

    WaitForSingleObject( process_info.hProcess, INFINITE );

    return 0;
}

void MainWindow::defaultSettings()
{
    ui->wdtCheck->setChecked(wdtdefault);
    ui->mclrCheck->setChecked(mclrdefault);
    ui->cpCheck->setChecked(cpdefault);
    ui->gp2Check->setChecked(gp2default);
}

void MainWindow::setstyle(int counter, bool ok)
{
    if (ok)
    {
        labels[counter]->setStyleSheet("color: rgb(0, 170, 0);");
        return;
    }

    labels[counter]->setStyleSheet("color: rgb(255, 0, 0);");
}

void MainWindow::generateCode()
{

    for (int i = 0; i < 3; ++i)
    {
        binString[i] = "";

        for (int j = 0; j < 24; ++j)
        {
            binString[i].prepend(bits[i][j]->text()) ;
        }
    }

    progMethod = ui->progMethodSlider->value();

    if (progMethod == 1)
    {
        //Initialisation Latch Method

        binString[2].replace(21, 1, "0"); //Counter Reset bit = 0

        binString[3] = binString[2];

        binString[3].replace(23, 1, "1"); //Initialisation latch

    }

    else if (progMethod == 0)
    {
        //Counter Reset Method

        binString[3] = binString[2];

        binString[2].replace(21, 1, "1"); //Counter Reset bit = 1 : loaded first

        binString[3].replace(21, 1, "0"); //Counter Reset bit = 0 : loaded last
    }




    MainWindow::codeString = codegen(binString);

    ui->codeBrowser->setPlainText(codeString);
    ui->saveButton->setEnabled(true);
    ui->buildButton->setEnabled(asmblrPath!="" && ui->saveButton->isEnabled());

}

void MainWindow::saveSettings()
{
    wdt = ui->wdtCheck->isChecked();
    mclr = ui->mclrCheck->isChecked();
    cp = ui->cpCheck->isChecked();
    gp2 = ui->gp2Check->isChecked();

    if (ui->codeBrowser)
        ui->codeBrowser->setPlainText("Settings changed. Please regenerate the code.");
    ui->saveButton->setEnabled(false);

}

void MainWindow::selectAsmblr()
{
    QString fileName;

    if (asmblrPath != "")
        fileName = asmblrPath;
    else
        fileName = "MPASMWIN.exe";

    QString path = QFileDialog::getOpenFileName(this, tr("Select MPASWIN.exe"),
                                                fileName, tr("Executable (*.exe)"));

    if (path != "")
    {
        asmblrPath = path;
        ui->asmblrPathLine->setText(asmblrPath);
        ui->asmblrPathLine->setReadOnly(true);
        ui->buildButton->setEnabled(ui->saveButton->isEnabled());
    }

}
