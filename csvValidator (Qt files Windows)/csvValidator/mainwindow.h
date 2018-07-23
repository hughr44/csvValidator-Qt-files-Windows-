#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void submitMapping(QStringList);
    bool invalidInt(QString);
    bool invalidDec(QString);
    bool invalidDate(QString);
    bool invalidBool(QString);
    bool invalidString(QString, QString);

private:
    Ui::MainWindow *ui;

private slots:
    void openFile_pressed();
    void mapping_pressed();
    void display_pressed();
    void help_pressed();
    void newFile_pressed();
    void fileType_pressed();
};

#endif // MAINWINDOW_H
