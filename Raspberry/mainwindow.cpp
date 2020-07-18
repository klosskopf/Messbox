#include "mainwindow.h"
#include "gpio.h"

mainWindow::mainWindow()
{
    Gpio::init();

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

    QVBoxLayout* timeframelayout = new QVBoxLayout();
    QLabel* timeframelabel = new QLabel("Timeframe [s]");
    timeframe = new QLineEdit;
    connect(timeframe, SIGNAL (editingFinished()),this, SLOT (handletimframe()));
    timeframe->setText(QString::number(Control::timeframe));
    timeframe->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    timeframelayout->addWidget(timeframelabel);
    timeframelayout->addWidget(timeframe);

    QVBoxLayout* samplelayout = new QVBoxLayout();
    QLabel* samplelabel = new QLabel("Samplefreq. [Hz]");
    sample = new QLineEdit;
    connect(sample, SIGNAL (editingFinished()),this, SLOT (handlesample()));
    sample->setToolTip("Sample Frequenz in Hz");
    sample->setText(QString::number(Control::samplefreq));
    sample->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    samplelayout->addWidget(samplelabel);
    samplelayout->addWidget(sample);

    savebutton = new QPushButton("Save");
    savebutton->setFont(QFont("Arial", 12));
    savebutton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    graph = new QChart();
    graph->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    anzeigeserie = new QLineSeries;
    aenderserie = new QLineSeries;
    graph->addSeries(anzeigeserie);

    graphview = new QChartView();
    graphview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    graphview->setChart(graph);

    rechenfeld = new Rechenfeld();
    rechenfeld->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->addWidget(parameterauswahl,0,0,10,3);
    mainLayout->addWidget(startstopbutton,0,3,1,1);
    mainLayout->addWidget(modebutton,1,3,1,1);
    mainLayout->addLayout(timeframelayout,2,3,1,1);
    mainLayout->addLayout(samplelayout,3,3,1,1);
    mainLayout->addWidget(savebutton,5,3,1,1);
    mainLayout->addWidget(graphview,0,4,6,6);
    mainLayout->addWidget(rechenfeld,6,3,4,7);
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
    //setWindowState(Qt::WindowFullScreen);

    graphersteller = new Graphersteller(this);
    QObject::connect(graphersteller,&Graphersteller::create_graph,this,&mainWindow::create_graph);
}

void mainWindow::handlestartstopbutton()
{
    if(Control::zustand == MESS)
    {
        Control::zustand = STOP;
        Post::send_stop();
        Gpio::set_led(LED_STOP);
        startstopbutton->setText("Stop");
        startstopbutton->setStyleSheet("background-color: rgb(0, 255, 0); color: rgb(0, 0, 0)");
    }
    else
    {
        Control::zustand = MESS;
        Control::datenmutex.lock();
        for (Karte* karte : Control::Kartenset)
        {
            for (Parameter* param : *(karte->parameter))
            {
                param->delete_daten();
            }
        }
        Control::datenmutex.unlock();
        Gpio::set_led(LED_RUN);
        if (Control::modus==CONT)
            Post::send_start_kont();
        else
            Post::send_start_startstop();
        startstopbutton->setText("misst...");
        startstopbutton->setStyleSheet("background-color: rgb(255, 0, 0); color: rgb(0, 0, 0)");
    }
}

void mainWindow::handlemodebutton()
{
    if(Control::modus == STARTSTOP)
    {
        Control::modus = CONT;
        modebutton->setText("Cont.");
    }
    else
    {
        Control::modus = STARTSTOP;
        modebutton->setText("Start/Stop");
    }
    if(Control::zustand==MESS)
    {
        if (Control::modus==CONT)
            Post::send_start_kont();
        else
            Post::send_start_startstop();
    }
}

void mainWindow::handletimframe()
{
    float n_timeframe = std::atof(timeframe->text().toUtf8());
    if (n_timeframe)
    {
        Control::timeframe=n_timeframe;
    }
    else
    {
        timeframe->setText(QString::number(Control::timeframe));
    }
}

void mainWindow::handlesample()
{
    float n_sample = std::atof(sample->text().toUtf8());
    if (n_sample)
    {
        Control::samplefreq=n_sample;
    }
    else
    {
        sample->setText(QString::number(Control::samplefreq));
    }
    Post::send_set_sample_freq(n_sample);
}

void mainWindow::create_graph(QLineSeries* n_serie)
{
    for (QAbstractSeries * series : graph->series())
    {
        graph->removeSeries(series);
    }
    QLineSeries* old_serie=anzeigeserie;
    anzeigeserie = n_serie;
    aenderserie=old_serie;
    graph->addSeries(anzeigeserie);
    graph->createDefaultAxes();

}
