#ifndef MAPPING_H
#define MAPPING_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class Mapping;
}

class Mapping : public QDialog
{
    Q_OBJECT

public:
    explicit Mapping(QWidget *parent = 0);
    ~Mapping();
    void performMapping(QStringList, int, MainWindow *);

private:
    Ui::Mapping *ui;

private slots:
    void next_pressed();
    void last_pressed();
    void finishedMapping_pressed();

    void notString_selected();
    void string_selected();
};

#endif // MAPPING_H
