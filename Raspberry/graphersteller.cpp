#include "graphersteller.h"
#include "control.h"
#include <QtDebug>

Graphersteller::Graphersteller(mainWindow* n_gui)
{
    gui=n_gui;
    firsttimepoint=0;
    nexttimepoint=0;
    maxx=maxy=minx=miny=0;
    QObject::connect(this,&Graphersteller::create_graph_send,this,&Graphersteller::create_graph_rec);
}

void Graphersteller::graph_thread()
{
    while(1)
    {
        emit create_graph_send();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Graphersteller::clear_graph()
{
    gui->rechenfeld->rechenfeld_mutex.lock();
    buffer.clear();
    gui->anzeigeserie->replace(buffer);
    maxx=maxy=minx=miny=0;
    firsttimepoint=0;
    nexttimepoint=0;
    gui->maxxlabel->setText("xmax:0");
    gui->minxlabel->setText("xmin:0");
    gui->maxylabel->setText("ymax:0");
    gui->minylabel->setText("ymin:0");
    gui->rechenfeld->rechenfeld_mutex.unlock();
}

void Graphersteller::create_graph_rec()
{
    if(Control::xAchse && Control::yAchse)
    {
        gui->rechenfeld->rechenfeld_mutex.lock();
  //      qDebug()<<"graphersteller nimmt rechenfeld";
        uint32_t newesty=Control::yAchse->newest();
        uint32_t newestx=Control::xAchse->newest();

        uint32_t newestime=(newestx > newesty) ? newesty : newestx;
        uint32_t number = Control::samplefreq * Control::timeframe;
        if (Control::modus == CONT)
        {
            if (newestime>=number)
            {
                if (newestime-number < nexttimepoint)
                {   //remove a part of the points
                    if(firsttimepoint+number<newestime)
                    {
                        buffer.remove(0, newestime-firsttimepoint-number+1);
                        firsttimepoint=newestime-number+1;
                    }
                }
                else
                {   //remove all points
                    buffer.clear();
                    firsttimepoint=newestime-number+1;
                    nexttimepoint=firsttimepoint;
                }
            }
            for(;nexttimepoint<=newestime;nexttimepoint++)
            {
                double xvalue=Control::xAchse->get_data(nexttimepoint);
                double yvalue=Control::yAchse->get_data(nexttimepoint);
                buffer.push_back(QPointF(xvalue,yvalue));

            }
            minx=maxx=buffer.front().x();
            miny=maxy=buffer.front().y();
            for (QPointF point : buffer)
            {
                if (point.x() > maxx) maxx=point.x();
                if (point.y() > maxy) maxy=point.y();
                if (point.x() < minx) minx=point.x();
                if (point.y() < miny) miny=point.y();
            }

            QString maxx_s=QString(QString::number(maxx));
            QString minx_s=QString(QString::number(minx));
            QString maxy_s=QString(QString::number(maxy));
            QString miny_s=QString(QString::number(miny));
            Control::gui->maxxlabel->setText(QString("xmax:").append(maxx_s));
            Control::gui->minxlabel->setText(QString("xmin:").append(minx_s));
            Control::gui->maxylabel->setText(QString("ymax:").append(maxy_s));
            Control::gui->minylabel->setText(QString("ymin:").append(miny_s));

            double deltax=0.1*(maxx-minx)+0.000001;
            double deltay=0.1*(maxy-miny)+0.000001;

            gui->anzeigeserie->replace(buffer);
            gui->axisx->setRange(minx-deltax,maxx+deltax);
            gui->axisy->setRange(miny-deltay,maxy+deltay);
            gui->rechenfeld->rechenfeld_mutex.unlock();
        }
        else
        {
            for(;nexttimepoint<=newestime;nexttimepoint++)
            {
                if((number<1000) || nexttimepoint%(number/1000)==0)
                {
                    double xvalue=Control::xAchse->get_data(nexttimepoint);
                    double yvalue=Control::yAchse->get_data(nexttimepoint);
                    buffer.push_back(QPointF(xvalue,yvalue));
                    if (xvalue > maxx) maxx=xvalue;
                    if (yvalue > maxy) maxy=yvalue;
                    if (xvalue < minx) minx=xvalue;
                    if (yvalue < miny) miny=yvalue;
                }
            }

            QString maxx_s=QString(QString::number(maxx));
            QString minx_s=QString(QString::number(minx));
            QString maxy_s=QString(QString::number(maxy));
            QString miny_s=QString(QString::number(miny));
            maxx_s.resize(5);
            minx_s.resize(5);
            maxy_s.resize(5);
            miny_s.resize(5);
            Control::gui->maxxlabel->setText(QString("xmax:").append(maxx_s));
            Control::gui->minxlabel->setText(QString("xmin:").append(minx_s));
            Control::gui->maxylabel->setText(QString("ymax:").append(maxy_s));
            Control::gui->minylabel->setText(QString("ymin:").append(miny_s));

            double deltax=0.1*(maxx-minx)+0.000001;
            double deltay=0.1*(maxy-miny)+0.000001;

            gui->anzeigeserie->replace(buffer);
            gui->axisx->setRange(minx-deltax,maxx+deltax);
            gui->axisy->setRange(miny-deltay,maxy+deltay);
            gui->rechenfeld->rechenfeld_mutex.unlock();
        }
        gui->graph->update();
    }
}




/*   for (Kennliniendaten* datum : Control::kennlinie)
   {
       delete datum;
   }
   Control::kennlinie.clear();
   gui->aenderserie->clear();
   uint32_t starttime=newestime<number ? 0 : newestime-number;
   for (uint32_t time=starttime; time<=newestime ; time++)
   {

       Control::kennlinie.push_back(new Kennliniendaten(xvalue, yvalue));
       gui->aenderserie->append(xvalue,yvalue);
   }*/
