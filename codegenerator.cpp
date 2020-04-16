#include "codegenerator.h"

QString codegen(QString binString[4])
{
    QString codeString;

    QFile file;
    
    if (file.exists("code_template.asm"))

        file.setFileName("code_template.asm");

    else

        file.setFileName(":/text/code_template.asm");


    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        
        return "Failed. Please check if template file exists in the same directory as this executable.";

    QTextStream in(&file);

    codeString = in.readAll();

    QHash<bool, QString> state;

    state[true] = "ON";

    state[false] = "OFF";
    

    QHash <int, QString> method;

    method[0] = "\t; Function Latch, Counter Reset  = 1\n\nLoadLatchF_CR1\n\n";

    method[1] = "\t; Initialisation Latch, Counter Reset = 0\n\nLoadLatchI_CR0\n\n";
    

    QString loadlatches[2];

    loadlatches[0] = "\t; Counter Reset Method \n\n\tcall\t LoadLatchF_CR1\t; Counter Reset = 1\n\n\tcall\t LoadLatchR\t; Reference counter latch \n\n\tcall\t LoadLatchN\t; N counter latch \n\n\tcall\t LoadLatchF_CR0\t; Counter Reset = 0 ";

    loadlatches[1] = "\t; Initialisation Latch Method \n\n\tcall\t LoadLatchI_CR0\t; Initialisation Latch \n\n\tcall\t LoadLatchF_CR0\t; Function Latch \n\n\tcall\t LoadLatchR\t; Reference counter latch \n\n\tcall\t LoadLatchN\t; N counter latch ";


    QString placeholders[4];

    placeholders[0] = ";Latch_R_Placeholder";

    placeholders[1] = ";Latch_N_Placeholder";

    placeholders[2] = ";Latch_F_Placeholder";

    placeholders[3] = ";Latch_I_Placeholder";



    //Prepare __CONFIG line

    codeString.replace(";WDT_Placeholder", "& _WDT_" + state[wdt]);

    codeString.replace(";MCLRE_Placeholder", "& _MCLRE_" + state[mclr]);

    codeString.replace(";CP_Placeholder", "& _CP_" + state[cp]);


    codeString.replace(";Load_Latches_Placeholder", loadlatches[progMethod] );


    for (int i = 0; i <3; ++i)
    {
        codeString.replace(placeholders[i], latchcmd(binString[i]));
    }

    codeString.replace(placeholders[3], method[progMethod] + latchcmd(binString[3]));


    return codeString;


}


QString latchcmd(QString sequence)
{

    QHash <QChar, QString> bitcmd;
    bitcmd['0'] = "\tbcf\t TxPin\t; TxPin = 0\n\n";
    bitcmd['1'] = "\tbsf\t TxPin\t; TxPin = 1\n\n";

    QString cmd;

    for (int i = 0; i < 24; i++)
    {
        cmd.append(bitcmd[sequence[i]]);
    }

    return cmd;

}
