#ifndef KARTE_GUI_H
#define KARTE_GUI_H
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include "karte.h"
class Karte;

class Karte_GUI : public QGroupBox
{
    Q_OBJECT
public:
    Karte_GUI(Karte* karte);
    QLabel* name;
    QComboBox* parameter;
    QComboBox* wert;
    Karte* karte;
private:
    void set_wert_to_parameter(std::string name);
private slots:
    void parameter_activated(const QString& name);
    void wert_activated(const QString& name);
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