#include "control.h"
#include <QGridLayout>
#include "rechenfeld.h"

Rechenfeld::Rechenfeld(QWidget *parent) : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout;

    xerror = new QLabel("Keine gültige Kennlinie");
    yerror = new QLabel("Keine gültige Kennlinie");
    xlabel = new QLabel("x=");
    ylabel = new QLabel("y=");
    xfeld = new QTextEdit();
    yfeld = new QTextEdit();

    layout->addWidget(xerror,0,0,1,2);
    layout->addWidget(yerror,2,0,1,2);
    layout->addWidget(xlabel,1,0,1,1);
    layout->addWidget(ylabel,3,0,1,1);
    layout->addWidget(xfeld,1,1,1,1);
    layout->addWidget(yfeld,3,1,1,1);

    layout->setRowStretch(0,1);
    layout->setRowStretch(1,10);
    layout->setRowStretch(2,1);
    layout->setRowStretch(3,10);

    layout->setColumnStretch(0,1);
    layout->setColumnStretch(1,30);

    setLayout(layout);

    connect(xfeld, SIGNAL (textChanged()),this, SLOT (handle_input()));
    connect(yfeld, SIGNAL (textChanged()),this, SLOT (handle_input()));
}


int Rechenfeld::addblock(Rechenblock* block)
{
    bloecke.push_back(block);
    return(Rechenfeld::activeblock->von_unten(block));
}

DECODEERROR Rechenfeld::decode(QChar c)
{
    DECODEERROR error=NODECODEERROR;
    switch(state)
    {
    case START: error=start(c);break;
    case KLAMMERAUF: error=klammerauf(c);break;
    case KLAMMERZU: error=klammerzu(c);break;
    case TIME: error=time(c);break;
    case INTEGRATE: error=integrate(c);break;
    case DERIVATE: error=derivate(c);break;
    case ZAHL: error=zahl(c);break;
    case PUNKT: error=punkt(c);break;
    case PARAMETER: error=parameter(c);break;
    default: error=MISSINGSTATE;
    }
    return error;
}

void Rechenfeld::handle_input()   //This needs a mutex
{
    DECODEERROR error=NODECODEERROR;
    QString progress="";
    int klammerzaehler=0;
    zahlstring="";
    parameterstring="";
    Control::datenmutex.lock();
    qDebug("handleinput");
    Control::xAchse->eingaenge.clear();
    Control::yAchse->eingaenge.clear();
    activeparameter.clear();
    while (bloecke.size())
    {
        Rechenblock* block = bloecke.front();
        bloecke.pop_front();
        delete block;
    }
    Rechenfeld::activeblock=Control::xAchse;

    QString text=xfeld->toPlainText().append('\0');
    for(QChar c : text)
    {
        if (c=='(') klammerzaehler++;
        else if (c==')') klammerzaehler--;
    }
    if (klammerzaehler>0) error=ZUVIELEKLAMMERN;
    else if (klammerzaehler<0) error=ZUWENIGKLAMMERN;
    else
    {
        state=START;
        for(QChar c : text)
        {
            progress.append(c);
            if ((error=decode(c)) != NODECODEERROR)break;
        }
    }
    switch(error)
    {
    case NODECODEERROR: xerror->setText(Control::xAchse->eingaenge.size()?Control::xAchse->print():QString("Geben Sie die x-Achsengleichung ein"));break;
    case ZUVIELEKLAMMERN: xerror->setText(QString::number(klammerzaehler).append(" zu viele offene Klammern"));break;
    case ZUWENIGKLAMMERN: xerror->setText(QString::number(-klammerzaehler).append(" zu viele geschlossene Klammern"));break;
    case UNGUELTIG: xerror->setText(QString("Ungültige eingabe: ").append(progress));break;
    case MISSINGPARAMETER: xerror->setText(QString("Falscher Parameter: ").append(zahlstring).append(":").append(parameterstring));break;
    case MISSINGSTATE: xerror->setText("Huch da hat der Programmierer wohl geschlafen");break;
    default: xerror->setText("Beten wir einfach, dass das nie einer sieht");
    }
    if (error!=NODECODEERROR)
    {
        Control::xAchse->eingaenge.clear();
    }


    error=NODECODEERROR;
    progress="";
    klammerzaehler=0;
    zahlstring="";
    parameterstring="";
    Rechenfeld::activeblock=Control::yAchse;
    text=yfeld->toPlainText().append('\0');
    for(QChar c : text)
    {
        if (c=='(') klammerzaehler++;
        else if (c==')') klammerzaehler--;
    }
    if (klammerzaehler>0) error=ZUVIELEKLAMMERN;
    else if (klammerzaehler<0) error=ZUWENIGKLAMMERN;
    else
    {
        state=START;
        for(QChar c : text)
        {
            progress.append(c);
            if ((error=decode(c)) != NODECODEERROR)break;
        }
    }
    switch(error)
    {
    case NODECODEERROR: yerror->setText(Control::yAchse->eingaenge.size()?Control::yAchse->print():QString("Geben Sie die y-Achsengleichung ein"));break;
    case ZUVIELEKLAMMERN: yerror->setText(QString::number(klammerzaehler).append(" zu viele offene Klammern"));break;
    case ZUWENIGKLAMMERN: yerror->setText(QString::number(-klammerzaehler).append(" zu viele geschlossene Klammern"));break;
    case UNGUELTIG: yerror->setText(QString("Ungültige eingabe: ").append(progress));break;
    case MISSINGPARAMETER: yerror->setText(QString("Falscher Parameter: ").append(zahlstring).append(":").append(parameterstring));break;
    case MISSINGSTATE: yerror->setText("Huch da hat der Programmierer wohl geschlafen");break;
    default: yerror->setText("Beten wir einfach, dass das nie einer sieht");
    }
    if (error!=NODECODEERROR)
    {
        Control::yAchse->eingaenge.clear();
    }

    qDebug("input_end\n");
    Control::datenmutex.unlock();
}

DECODEERROR Rechenfeld::start(QChar c)
{
    DECODEERROR error=NODECODEERROR;
    if (c=='(')                 {addblock(new Klammerauf_Block());state=KLAMMERAUF;}
    else if(c=='T' || c=='t')   {addblock(new Time_Block());state=TIME;}
    else if(c=='S' || c=='s')   {addblock(new Integrate_Block());state=INTEGRATE;}
    else if(c=='D' || c=='d')   {addblock(new Derivate_Block());state=DERIVATE;}
    else if(c>='0' && c<='9')   {zahlstring=c;state=ZAHL;}
    else if(c=='.')             {zahlstring="0.";state=PUNKT;}
    else                        {error=UNGUELTIG;}
    return(error);
}

DECODEERROR Rechenfeld::klammerauf(QChar c)
{
    DECODEERROR error=NODECODEERROR;
    if (c=='+')                 {addblock(new Plus_Block());state=START;}
    else if (c=='-')            {addblock(new Minus_Block());state=START;}
    else if (c=='*')            {addblock(new Mal_Block());state=START;}
    else if (c=='/')            {addblock(new Geteilt_Block());state=START;}
    else if (c=='(')            {addblock(new Klammerauf_Block());state=KLAMMERAUF;}
    else if(c=='T' || c=='t')   {addblock(new Time_Block());state=TIME;}
    else if(c=='S' || c=='s')   {addblock(new Integrate_Block());state=INTEGRATE;}
    else if(c=='D' || c=='d')   {addblock(new Derivate_Block());state=DERIVATE;}
    else if(c>='0' && c<='9')   {zahlstring=c;state=ZAHL;}
    else if(c=='.')             {zahlstring="0.";state=PUNKT;}
    else if(c=='\0')            {error=UNGUELTIG;}
    else                        {error=UNGUELTIG;}
    return(error);
}

DECODEERROR Rechenfeld::klammerzu(QChar c)
{
    DECODEERROR error=NODECODEERROR;
    if (c=='+')                 {addblock(new Plus_Block());state=START;}
    else if (c=='-')            {addblock(new Minus_Block());state=START;}
    else if (c=='*')            {addblock(new Mal_Block());state=START;}
    else if (c=='/')            {addblock(new Geteilt_Block());state=START;}
    else if (c=='(')            {addblock(new Mal_Block());addblock(new Klammerauf_Block());state=KLAMMERAUF;}
    else if (c==')')            {addblock(new Klammerzu_Block());state=KLAMMERZU;}
    else if(c=='T' || c=='t')   {addblock(new Mal_Block());addblock(new Time_Block());state=TIME;}
    else if(c=='S' || c=='s')   {addblock(new Mal_Block());addblock(new Integrate_Block());state=INTEGRATE;}
    else if(c=='D' || c=='d')   {addblock(new Mal_Block());addblock(new Derivate_Block());state=DERIVATE;}
    else if(c>='0' && c<='9')   {addblock(new Mal_Block());zahlstring=c;state=ZAHL;}
    else if(c=='.')             {addblock(new Mal_Block());zahlstring="0.";state=PUNKT;}
    else if(c=='\0')            {;}
    else                        {error=UNGUELTIG;}
    return(error);
}

DECODEERROR Rechenfeld::time(QChar c)
{
    DECODEERROR error=NODECODEERROR;
    if (c=='+')                 {addblock(new Plus_Block());state=START;}
    else if (c=='-')            {addblock(new Minus_Block());state=START;}
    else if (c=='*')            {addblock(new Mal_Block());state=START;}
    else if (c=='/')            {addblock(new Geteilt_Block());state=START;}
    else if (c=='(')            {addblock(new Mal_Block());addblock(new Klammerauf_Block());state=KLAMMERAUF;}
    else if (c==')')            {addblock(new Klammerzu_Block());state=KLAMMERZU;}
    else if(c=='T' || c=='t')   {addblock(new Mal_Block());addblock(new Time_Block());state=TIME;}
    else if(c=='S' || c=='s')   {addblock(new Mal_Block());addblock(new Integrate_Block());state=INTEGRATE;}
    else if(c=='D' || c=='d')   {addblock(new Mal_Block());addblock(new Derivate_Block());state=DERIVATE;}
    else if(c>='0' && c<='9')   {addblock(new Mal_Block());zahlstring=c;state=ZAHL;}
    else if(c=='.')             {addblock(new Mal_Block());zahlstring="0.";state=PUNKT;}
    else if(c=='\0')            {;}
    else                        {error=UNGUELTIG;}
    return(error);
}

DECODEERROR Rechenfeld::integrate(QChar c)
{
    DECODEERROR error=NODECODEERROR;
    if (c=='(')                 {addblock(new Klammerauf_Block());state=KLAMMERAUF;}
    else if(c=='T' || c=='t')   {addblock(new Time_Block());state=TIME;}
    else if(c=='S' || c=='s')   {addblock(new Integrate_Block());state=INTEGRATE;}
    else if(c=='D' || c=='d')   {addblock(new Derivate_Block());state=DERIVATE;}
    else if(c>='0' && c<='9')   {zahlstring=c;state=ZAHL;}
    else if(c=='.')             {zahlstring="0.";state=PUNKT;}
    else if(c=='\0')            {error=UNGUELTIG;}
    else                        {error=UNGUELTIG;}
    return(error);
}

DECODEERROR Rechenfeld::derivate(QChar c)
{
    DECODEERROR error=NODECODEERROR;
    if (c=='(')                 {addblock(new Klammerauf_Block());state=KLAMMERAUF;}
    else if(c=='T' || c=='t')   {addblock(new Time_Block());state=TIME;}
    else if(c=='S' || c=='s')   {addblock(new Integrate_Block());state=INTEGRATE;}
    else if(c=='D' || c=='d')   {addblock(new Derivate_Block());state=DERIVATE;}
    else if(c>='0' && c<='9')   {zahlstring=c;state=ZAHL;}
    else if(c=='.')             {zahlstring="0.";state=PUNKT;}
    else if(c=='\0')            {error=UNGUELTIG;}
    else                        {error=UNGUELTIG;}
    return(error);
}

DECODEERROR Rechenfeld::zahl(QChar c)
{
    DECODEERROR error=NODECODEERROR;
    if (c=='+')                 {addblock(new Constant_Block(zahlstring.toInt()));addblock(new Plus_Block());state=START;}
    else if (c=='-')            {addblock(new Constant_Block(zahlstring.toInt()));addblock(new Minus_Block());state=START;}
    else if (c=='*')            {addblock(new Constant_Block(zahlstring.toInt()));addblock(new Mal_Block());state=START;}
    else if (c=='/')            {addblock(new Constant_Block(zahlstring.toInt()));addblock(new Geteilt_Block());state=START;}
    else if (c=='(')            {addblock(new Constant_Block(zahlstring.toInt()));addblock(new Mal_Block());addblock(new Klammerauf_Block());state=KLAMMERAUF;}
    else if (c==')')            {addblock(new Constant_Block(zahlstring.toInt()));addblock(new Klammerzu_Block());state=KLAMMERZU;}
    else if(c=='T' || c=='t')   {addblock(new Constant_Block(zahlstring.toInt()));addblock(new Mal_Block());addblock(new Time_Block());state=TIME;}
    else if(c=='S' || c=='s')   {addblock(new Constant_Block(zahlstring.toInt()));addblock(new Mal_Block());addblock(new Integrate_Block());state=INTEGRATE;}
    else if(c=='D' || c=='d')   {addblock(new Constant_Block(zahlstring.toInt()));addblock(new Mal_Block());addblock(new Derivate_Block());state=DERIVATE;}
    else if(c>='0' && c<='9')   {zahlstring.append(c);state=ZAHL;}
    else if(c=='.')             {zahlstring.append(c);state=PUNKT;}
    else if(c==':')             {parameterstring="";state=PARAMETER;}
    else if(c=='\0')            {addblock(new Constant_Block(zahlstring.toInt()));state=ZAHL;}
    else                        {error=UNGUELTIG;}
    return(error);
}

DECODEERROR Rechenfeld::parameter(QChar c)
{
    ParameterViewer* parameterviewer=get_parameter();
    DECODEERROR error=NODECODEERROR;
    if (c=='+' && parameterviewer)      {addblock(parameterviewer);addblock(new Plus_Block());state=START;}
    else if (c=='-' && parameterviewer) {addblock(parameterviewer);addblock(new Minus_Block());state=START;}
    else if (c=='*' && parameterviewer) {addblock(parameterviewer);addblock(new Mal_Block());state=START;}
    else if (c=='/' && parameterviewer) {addblock(parameterviewer);addblock(new Geteilt_Block());state=START;}
    else if (c=='(' && parameterviewer) {addblock(parameterviewer);addblock(new Mal_Block());addblock(new Klammerauf_Block());state=KLAMMERAUF;}
    else if (c==')' && parameterviewer) {addblock(parameterviewer);addblock(new Klammerzu_Block());state=KLAMMERZU;}
    else if(c=='\0')                    {if (parameterviewer)addblock(parameterviewer);else error=MISSINGPARAMETER; state=PARAMETER;}
    else                                {parameterstring.append(c);}
    return(error);
}

DECODEERROR Rechenfeld::punkt(QChar c)
{
    DECODEERROR error=NODECODEERROR;
    if (c=='+')                 {addblock(new Constant_Block(zahlstring.toDouble()));addblock(new Plus_Block());state=START;}
    else if (c=='-')            {addblock(new Constant_Block(zahlstring.toDouble()));addblock(new Minus_Block());state=START;}
    else if (c=='*')            {addblock(new Constant_Block(zahlstring.toDouble()));addblock(new Mal_Block());state=START;}
    else if (c=='/')            {addblock(new Constant_Block(zahlstring.toDouble()));addblock(new Geteilt_Block());state=START;}
    else if (c=='(')            {addblock(new Constant_Block(zahlstring.toDouble()));addblock(new Mal_Block());addblock(new Klammerauf_Block());state=KLAMMERAUF;}
    else if (c==')')            {addblock(new Constant_Block(zahlstring.toDouble()));addblock(new Klammerzu_Block());state=KLAMMERZU;}
    else if(c=='T' || c=='t')   {addblock(new Constant_Block(zahlstring.toDouble()));addblock(new Mal_Block());addblock(new Time_Block());state=TIME;}
    else if(c=='S' || c=='s')   {addblock(new Constant_Block(zahlstring.toDouble()));addblock(new Mal_Block());addblock(new Integrate_Block());state=INTEGRATE;}
    else if(c=='D' || c=='d')   {addblock(new Constant_Block(zahlstring.toDouble()));addblock(new Mal_Block());addblock(new Derivate_Block());state=DERIVATE;}
    else if(c>='0' && c<='9')   {zahlstring.append(c);state=ZAHL;}
    else if(c=='\0')            {addblock(new Constant_Block(zahlstring.toDouble()));state=ZAHL;}
    else                        {error=UNGUELTIG;}
    return(error);
}

ParameterViewer* Rechenfeld::get_parameter()
{
    Karte* karte=NULL;
    Parameter* parameter=NULL;
    ParameterViewer* parameterviewer=NULL;
    karte= Control::findkarte(zahlstring.toInt());
    if (karte)
    {
        parameter=karte->find_parameter(parameterstring.toStdString().c_str());
        if (parameter)
        {
            activeparameter.push_back(parameter);
            activeparameter.unique();
            parameterviewer=new ParameterViewer(parameter);
        }
    }
    else parameterviewer=NULL;
    return parameterviewer;
}

Rechenblock* Rechenfeld::activeblock=NULL;
