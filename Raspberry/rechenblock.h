#ifndef RECHENBLOCK_H
#define RECHENBLOCK_H
#include <list>
#include <stdint.h>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QString>
class Node;

class Rechenblock : public QWidget
{
    Q_OBJECT
public:
    Rechenblock(int eingaengenr);
    virtual ~Rechenblock();
    std::list<Rechenblock*> eingaenge;
    virtual double get_data(uint32_t time)=0;
    virtual Rechenblock* copy()=0;
    virtual uint32_t newest();
    bool on_kombinationsfeld;

    std::list<Node*> nodes;

    QWidget* widget;
    const int eingaengenr;

protected:
    void setup_Rechenblock(QWidget*);
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void delete_this(Rechenblock*);
};

class Node : public QLabel
{
    Q_OBJECT
public:
    Node(Rechenblock* n_parent):QLabel(), parent(n_parent)
    {
        parent->nodes.push_back(this);
        setPixmap(QPixmap(":images/node.png"));
        setAttribute(Qt::WA_DeleteOnClose);
        //setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    }
    virtual ~Node(){};
private:
    Rechenblock* parent;

};

#endif // RECHENBLOCK_H
