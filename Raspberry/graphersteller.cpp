#include "graphersteller.h"
#include "control.h"

void Graphersteller::graph_thread()
{
    while(1)
    {
        draw_graph();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void Graphersteller::draw_graph()
{
    if(Control::xAchse && Control::yAchse)
    {
        uint32_t newesty=Control::yAchse->newest();
        uint32_t newestx=Control::xAchse->newest();

        uint32_t newestime=(newestx > newesty) ? newesty : newestx;
        uint32_t number = Control::samplefreq * Control::timeframe;
        for (Kennliniendaten* datum : Control::kennlinie)
        {
            delete datum;
        }
        Control::kennlinie.clear();
        gui->aenderserie->clear();
        for (uint32_t time=newestime; time>0 && time>(newestime-number); time--)
        {
            double xvalue=Control::xAchse->get_data(time);
            double yvalue=Control::yAchse->get_data(time);
            Control::kennlinie.push_back(new Kennliniendaten(xvalue, yvalue));
            gui->aenderserie->append(xvalue,yvalue);
        }
        emit create_graph(gui->aenderserie);
    }
}
