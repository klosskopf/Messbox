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

    QGridLayout* minmaxlayout = new QGridLayout();
    maxxlabel=new QLabel("xmax:0");
    minxlabel=new QLabel("xmin:0");
    maxylabel=new QLabel("ymax:0");
    minylabel=new QLabel("ymin:0");
    minmaxlayout->addWidget(maxxlabel,0,0,1,1);
    minmaxlayout->addWidget(minxlabel,0,1,1,1);
    minmaxlayout->addWidget(maxylabel,1,0,1,1);
    minmaxlayout->addWidget(minylabel,1,1,1,1);

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
    graph->addSeries(anzeigeserie);

    graphview = new QChartView();
    graphview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    graphview->setChart(graph);

    axisx = new QValueAxis;
    axisx->setRange(1,-1);
 //   axisx->setTickCount(13);
    axisx->setLabelFormat("%.5f");

    axisy = new QValueAxis;
    axisy->setRange(1,-1);
  //  axisy->setTickCount(6);
    axisy->setLabelFormat("%.5f");

    graph->setAxisX(axisx,anzeigeserie);
    graph->setAxisY(axisy,anzeigeserie);

    rechenfeld = new Rechenfeld();
    rechenfeld->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->addWidget(parameterauswahl,0,0,10,3);
    mainLayout->addWidget(startstopbutton,0,3,1,1);
    mainLayout->addWidget(modebutton,1,3,1,1);
    mainLayout->addLayout(timeframelayout,2,3,1,1);
    mainLayout->addLayout(samplelayout,3,3,1,1);
    mainLayout->addLayout(minmaxlayout,4,3,1,1);
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
}

void mainWindow::handlestartstopbutton()
{
    if(Control::zustand == MESS)
    {
        Control::stop();
    }
    else
    {
        Control::start();
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
        Control::start();
    }
}

void mainWindow::handletimframe()
{
    float n_timeframe = timeframe->text().toFloat();
    if (n_timeframe)
    {
        rechenfeld->rechenfeld_mutex.lock();
        Control::timeframe=n_timeframe;
        rechenfeld->rechenfeld_mutex.unlock();
        graphersteller->clear_graph();
    }
    else
    {
        timeframe->setText(QString::number(Control::timeframe));
    }
}

void mainWindow::handlesample()
{
    float n_sample = sample->text().toFloat();
    if (n_sample)
    {
        rechenfeld->rechenfeld_mutex.lock();
        if (n_sample < 0.09)
            Control::samplefreq=0.1;
        else if (n_sample > 10000)
            Control::samplefreq=1000;
        else
            Control::samplefreq=n_sample;
        rechenfeld->rechenfeld_mutex.unlock();
        graphersteller->clear_graph();
    }
    sample->setText(QString::number(Control::samplefreq));

    Post::send_set_sample_freq(n_sample);
    if(Control::zustand==MESS)
    {
       Control::start();
    }
}
