#ifndef KARTE_GUI_H
#define KARTE_GUI_H
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <mutex>
#include "karte.h"
class Karte;
class ListItem;

class Karte_GUI : public QGroupBox
{
    Q_OBJECT
public:
    Karte_GUI(Karte* karte);
    virtual ~Karte_GUI();
    QLabel* name;
    QComboBox* parameter;
    QComboBox* wert;
    Karte* karte;
    ListItem* listitem;
private:
    void set_wert_to_parameter(std::string name);
private slots:
    void parameter_activated(const QString& name);
    void wert_activated(const QString& name);
protected:
    void mousePressEvent(QMouseEvent *event) override;
};

class ListItem : public QListWidgetItem
{
public:
    ListItem(int n_index)
    {
        setData(3, n_index);
    }
private:

    bool operator<(const QListWidgetItem& item2) const
    {
        return data(3)<item2.data(3);
    }
};

#endif // KARTE_GUI_H
