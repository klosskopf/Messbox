#include "mainwindow.h"

mainWindow::mainWindow()
{
    setGeometry(0,0,1280,720);

    centralwidget = new QWidget(this);

    menubar= new QMenuBar;

    parameterauswahl = new Parameterauswahl();

    startstopbutton = new QPushButton("Start");
    connect(startstopbutton, SIGNAL (pressed()),this, SLOT (handlestartstopbutton()));
    startstopbutton->setAutoFillBackground(true);
    startstopbutton->setFont(QFont("Arial", 15,QFont::Bold));
    startstopbutton->setStyleSheet("background-color: rgb(0, 255, 0); color: rgb(0, 0, 0)");
    startstopbutton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    modebutton = new QPushButton("Start/Stop");
    connect(modebutton, SIGNAL (pressed()),this, SLOT (handlemodebutton()));
    modebutton->setFont(QFont("Arial", 12));
    modebutton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    timeframe = new QLineEdit;
    timeframe->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    savebutton = new QPushButton("Save");
    savebutton->setFont(QFont("Arial", 12));
    savebutton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    graph = new QChart;
    QLineSeries *series = new QLineSeries(graph);
    for(float f=0; f<10; f+=0.001)
    series->append(f,f*f);
    series->setName("Daten!!!");
    graph->addSeries(series);

    graph->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    graphview = new QChartView;
    graphview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    graphview->setChart(graph);

    kombinationsfeld = new QGroupBox;
    kombinationsfeld->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->addWidget(parameterauswahl,0,0,10,3);
    mainLayout->addWidget(startstopbutton,0,3,1,1);
    mainLayout->addWidget(modebutton,1,3,1,1);
    mainLayout->addWidget(timeframe,2,3,1,1);
    mainLayout->addWidget(savebutton,5,3,1,1);
    mainLayout->addWidget(graphview,0,4,6,6);
    mainLayout->addWidget(kombinationsfeld,6,3,4,7);
    for (int i=0;i<10;i++)
    {
        mainLayout->setRowStretch(i,10);
        mainLayout->setColumnStretch(i,10);
    }
    // Add the previous two inner layouts


    // Set the outer layout as a main layout
    // of the widget
    centralwidget->setLayout(mainLayout);

    this->setCentralWidget(centralwidget);

    // Window title
    setWindowTitle("Messbox2000");
    setWindowState(Qt::WindowFullScreen);

    control = new Control(this);
    QObject::connect(control,&Control::let_create_karte,this->parameterauswahl,&Parameterauswahl::create_karte);
    QObject::connect(control,&Control::let_delete_karte,this->parameterauswahl,&Parameterauswahl::delete_karte);

    Karte* testKarte = new Karte(this,2,"test");
    emit control->let_create_karte(testKarte);
}

void mainWindow::handlestartstopbutton()
{
    if(startstopbutton->text()=="Start")
    {
        startstopbutton->setText("Stop");
        startstopbutton->setStyleSheet("background-color: rgb(255, 0, 0); color: rgb(0, 0, 0)");
    }
    else
    {
        startstopbutton->setText("Start");
        startstopbutton->setStyleSheet("background-color: rgb(0, 255, 0); color: rgb(0, 0, 0)");
    }
}

void mainWindow::handlemodebutton()
{
    if(modebutton->text()=="Start/Stop")
    {
        modebutton->setText("Cont.");
    }
    else
    {
        modebutton->setText("Start/Stop");
    }
}
