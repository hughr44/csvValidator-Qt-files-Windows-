#include "mapping.h"
#include "ui_mapping.h"
#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QStringList>

int fieldCount = 0;
QStringList list;
int numFields;
QStringList fieldType;
QString fieldTypeNums; // not being used
MainWindow *mainParent;

Mapping::Mapping(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Mapping)
{
    ui->setupUi(this);

    connect(ui->pushButton,SIGNAL(released()),this,SLOT(next_pressed()));
    connect(ui->pushButton_2,SIGNAL(released()),this,SLOT(last_pressed()));
    connect(ui->pushButton_3,SIGNAL(released()),this,SLOT(finishedMapping_pressed()));

    connect(ui->radioButton,SIGNAL(clicked()),this,SLOT(notString_selected()));
    connect(ui->radioButton_2,SIGNAL(clicked()),this,SLOT(notString_selected()));
    connect(ui->radioButton_3,SIGNAL(clicked()),this,SLOT(notString_selected()));
    connect(ui->radioButton_4,SIGNAL(clicked()),this,SLOT(notString_selected()));
    connect(ui->radioButton_5,SIGNAL(clicked()),this,SLOT(string_selected()));

    ui->checkBox->setCheckable(false);
    ui->checkBox_2->setCheckable(false);
    ui->checkBox_3->setCheckable(false);
    ui->checkBox_4->setCheckable(false);
    ui->checkBox_5->setCheckable(false);
}

Mapping::~Mapping()
{
    delete ui;
}

void Mapping::performMapping(QStringList inputList, int num, MainWindow *parentPoint){
    numFields = num;
    list = inputList;
    ui->plainTextEdit->setPlainText(list[0]);
    qDebug() << list[0];
    mainParent = parentPoint;

    QString temp;
    for(int i=0; i<numFields-1; ++i){
        temp = temp + "00000000000,";
    }
    temp = temp + "00000000000";
    fieldType = temp.split(",");
}

void Mapping::next_pressed(){
    QString tempPattern = "00000000000";

    if(fieldCount < numFields){
        // set mapping pattern based of checked radio buttons and boxes
        // if radio button 1 checked tempPattern[0] = '1';
        if(ui->radioButton->isChecked()){
            tempPattern[0] = '1';
        }
        if(ui->radioButton_2->isChecked()){
            tempPattern[1] = '1';
        }
        if(ui->radioButton_3->isChecked()){
            tempPattern[2] = '1';
        }
        if(ui->radioButton_4->isChecked()){
            tempPattern[3] = '1';
        }
        if(ui->radioButton_5->isChecked()){
            tempPattern[4] = '1';
        }
        if(ui->radioButton_6->isChecked()){
            tempPattern[5] = '1';
        }
        if(ui->checkBox->isChecked()){
            tempPattern[6] = '1';
        }
        if(ui->checkBox_2->isChecked()){
            tempPattern[7] = '1';
        }
        if(ui->checkBox_3->isChecked()){
            tempPattern[8] = '1';
        }
        if(ui->checkBox_4->isChecked()){
            tempPattern[9] = '1';
        }
        if(ui->checkBox_5->isChecked()){
            tempPattern[10] = '1';
        }

        fieldType[fieldCount] = tempPattern;

        qDebug() << fieldType[fieldCount];

        //increment to next field index
        ++fieldCount;

        //displaying next field name
        if(fieldCount < numFields){
            ui->plainTextEdit->setPlainText(list[fieldCount]);
            ui->plainTextEdit->update();
            ui->plainTextEdit->repaint();
        }

        // reset all buttons to unchecked
        ui->radioButton->setAutoExclusive(false);
        ui->radioButton->setChecked(false);
        ui->radioButton->setAutoExclusive(true);
        ui->radioButton->update();
        ui->radioButton->repaint();

        ui->radioButton_2->setAutoExclusive(false);
        ui->radioButton_2->setChecked(false);
        ui->radioButton_2->setAutoExclusive(true);
        ui->radioButton_2->update();
        ui->radioButton_2->repaint();

        ui->radioButton_3->setAutoExclusive(false);
        ui->radioButton_3->setChecked(false);
        ui->radioButton_3->setAutoExclusive(true);
        ui->radioButton_3->update();
        ui->radioButton_3->repaint();

        ui->radioButton_4->setAutoExclusive(false);
        ui->radioButton_4->setChecked(false);
        ui->radioButton_4->setAutoExclusive(true);
        ui->radioButton_4->update();
        ui->radioButton_4->repaint();

        ui->radioButton_5->setAutoExclusive(false);
        ui->radioButton_5->setChecked(false);
        ui->radioButton_5->setAutoExclusive(true);
        ui->radioButton_5->update();
        ui->radioButton_5->repaint();

        ui->radioButton_6->setAutoExclusive(false);
        ui->radioButton_6->setChecked(false);
        ui->radioButton_6->setAutoExclusive(true);
        ui->radioButton_6->update();
        ui->radioButton_6->repaint();

        ui->checkBox->setChecked(false);
        ui->checkBox->update();
        ui->checkBox->repaint();

        ui->checkBox_2->setChecked(false);
        ui->checkBox_2->update();
        ui->checkBox_2->repaint();

        ui->checkBox_3->setChecked(false);
        ui->checkBox_3->update();
        ui->checkBox_3->repaint();

        ui->checkBox_4->setChecked(false);
        ui->checkBox_4->update();
        ui->checkBox_4->repaint();

        ui->checkBox_5->setChecked(false);
        ui->checkBox_5->update();
        ui->checkBox_5->repaint();

        ui->checkBox->setCheckable(true);
        ui->checkBox_2->setCheckable(true);
        ui->checkBox_3->setCheckable(true);
        ui->checkBox_4->setCheckable(true);
        ui->checkBox_5->setCheckable(true);

        // check buttons for next part
        if(fieldCount < numFields){
            tempPattern = fieldType[fieldCount];

            if(tempPattern[0] == '1'){
                ui->radioButton->setAutoExclusive(false);
                ui->radioButton->setChecked(true);
                ui->radioButton->setAutoExclusive(true);
                ui->radioButton->update();
                ui->radioButton->repaint();
            }
            if(tempPattern[1] == '1'){
                ui->radioButton_2->setAutoExclusive(false);
                ui->radioButton_2->setChecked(true);
                ui->radioButton_2->setAutoExclusive(true);
                ui->radioButton_2->update();
                ui->radioButton_2->repaint();
            }
            if(tempPattern[2] == '1'){
                ui->radioButton_3->setAutoExclusive(false);
                ui->radioButton_3->setChecked(true);
                ui->radioButton_3->setAutoExclusive(true);
                ui->radioButton_3->update();
                ui->radioButton_3->repaint();
            }
            if(tempPattern[3] == '1'){
                ui->radioButton_4->setAutoExclusive(false);
                ui->radioButton_4->setChecked(true);
                ui->radioButton_4->setAutoExclusive(true);
                ui->radioButton_4->update();
                ui->radioButton_4->repaint();
            }
            if(tempPattern[4] == '1'){
                ui->radioButton_5->setAutoExclusive(false);
                ui->radioButton_5->setChecked(true);
                ui->radioButton_5->setAutoExclusive(true);
                ui->radioButton_5->update();
                ui->radioButton_5->repaint();
            }
            if(tempPattern[5] == '1'){
                ui->radioButton_6->setAutoExclusive(false);
                ui->radioButton_6->setChecked(true);
                ui->radioButton_6->setAutoExclusive(true);
                ui->radioButton_6->update();
                ui->radioButton_6->repaint();
            }
            if(tempPattern[6] == '1'){
                //ui->checkBox->setAutoExclusive(false);
                ui->checkBox->setChecked(true);
                //ui->checkBox->setAutoExclusive(true);
                ui->checkBox->update();
                ui->checkBox->repaint();
            }
            if(tempPattern[7] == '1'){
                //ui->checkBox_2->setAutoExclusive(false);
                ui->checkBox_2->setChecked(true);
                //ui->checkBox_2->setAutoExclusive(true);
                ui->checkBox_2->update();
                ui->checkBox_2->repaint();
            }
            if(tempPattern[8] == '1'){
                //ui->checkBox_3->setAutoExclusive(false);
                ui->checkBox_3->setChecked(true);
                //ui->checkBox_3->setAutoExclusive(true);
                ui->checkBox_3->update();
                ui->checkBox_3->repaint();
            }
            if(tempPattern[9] == '1'){
                //ui->checkBox_4->setAutoExclusive(false);
                ui->checkBox_4->setChecked(true);
                //ui->checkBox_4->setAutoExclusive(true);
                ui->checkBox_4->update();
                ui->checkBox_4->repaint();
            }
            if(tempPattern[10] == '1'){
                //ui->checkBox_5->setAutoExclusive(false);
                ui->checkBox_5->setChecked(true);
                //ui->checkBox_5->setAutoExclusive(true);
                ui->checkBox_5->update();
                ui->checkBox_5->repaint();
            }

            if(tempPattern[4] == '1'){
                ui->checkBox->setCheckable(true);
                ui->checkBox_2->setCheckable(true);
                ui->checkBox_3->setCheckable(true);
                ui->checkBox_4->setCheckable(true);
                ui->checkBox_5->setCheckable(true);
            }
            else{
                ui->checkBox->setCheckable(false);
                ui->checkBox_2->setCheckable(false);
                ui->checkBox_3->setCheckable(false);
                ui->checkBox_4->setCheckable(false);
                ui->checkBox_5->setCheckable(false);
            }
        }
    }

    if(fieldCount == numFields){
        ui->plainTextEdit->setPlainText("No fields remaining. Click Submit Mapping to finish");
        ui->plainTextEdit->update();
        ui->plainTextEdit->repaint();

        ui->checkBox->setCheckable(false);
        ui->checkBox_2->setCheckable(false);
        ui->checkBox_3->setCheckable(false);
        ui->checkBox_4->setCheckable(false);
        ui->checkBox_5->setCheckable(false);
    }
}

void Mapping::last_pressed(){
    QString tempPattern = "00000000000";

    if(fieldCount > 0){
        // set mapping pattern based of checked radio buttons and boxes
        // if radio button 1 checked tempPattern[0] = '1';

        if(fieldCount < numFields){
            if(ui->radioButton->isChecked()){
                tempPattern[0] = '1';
            }
            if(ui->radioButton_2->isChecked()){
                tempPattern[1] = '1';
            }
            if(ui->radioButton_3->isChecked()){
                tempPattern[2] = '1';
            }
            if(ui->radioButton_4->isChecked()){
                tempPattern[3] = '1';
            }
            if(ui->radioButton_5->isChecked()){
                tempPattern[4] = '1';
            }
            if(ui->radioButton_6->isChecked()){
                tempPattern[5] = '1';
            }
            if(ui->checkBox->isChecked()){
                tempPattern[6] = '1';
            }
            if(ui->checkBox_2->isChecked()){
                tempPattern[7] = '1';
            }
            if(ui->checkBox_3->isChecked()){
                tempPattern[8] = '1';
            }
            if(ui->checkBox_4->isChecked()){
                tempPattern[9] = '1';
            }
            if(ui->checkBox_5->isChecked()){
                tempPattern[10] = '1';
            }

            fieldType[fieldCount] = tempPattern;

            qDebug() << fieldType[fieldCount];
        }

        // decrement to last field
        --fieldCount;

        //displaying next field name
        ui->plainTextEdit->setPlainText(list[fieldCount]);
        ui->plainTextEdit->update();
        ui->plainTextEdit->repaint();

        // reset all buttons to unchecked
        ui->radioButton->setAutoExclusive(false);
        ui->radioButton->setChecked(false);
        ui->radioButton->setAutoExclusive(true);
        ui->radioButton->update();
        ui->radioButton->repaint();

        ui->radioButton_2->setAutoExclusive(false);
        ui->radioButton_2->setChecked(false);
        ui->radioButton_2->setAutoExclusive(true);
        ui->radioButton_2->update();
        ui->radioButton_2->repaint();

        ui->radioButton_3->setAutoExclusive(false);
        ui->radioButton_3->setChecked(false);
        ui->radioButton_3->setAutoExclusive(true);
        ui->radioButton_3->update();
        ui->radioButton_3->repaint();

        ui->radioButton_4->setAutoExclusive(false);
        ui->radioButton_4->setChecked(false);
        ui->radioButton_4->setAutoExclusive(true);
        ui->radioButton_4->update();
        ui->radioButton_4->repaint();

        ui->radioButton_5->setAutoExclusive(false);
        ui->radioButton_5->setChecked(false);
        ui->radioButton_5->setAutoExclusive(true);
        ui->radioButton_5->update();
        ui->radioButton_5->repaint();

        ui->radioButton_6->setAutoExclusive(false);
        ui->radioButton_6->setChecked(false);
        ui->radioButton_6->setAutoExclusive(true);
        ui->radioButton_6->update();
        ui->radioButton_6->repaint();

        //ui->checkBox->setAutoExclusive(false);
        ui->checkBox->setChecked(false);
        //ui->checkBox->setAutoExclusive(true);
        ui->checkBox->update();
        ui->checkBox->repaint();

        //ui->checkBox_2->setAutoExclusive(false);
        ui->checkBox_2->setChecked(false);
        //ui->checkBox_2->setAutoExclusive(true);
        ui->checkBox_2->update();
        ui->checkBox_2->repaint();

        //ui->checkBox_3->setAutoExclusive(false);
        ui->checkBox_3->setChecked(false);
        //ui->checkBox_3->setAutoExclusive(true);
        ui->checkBox_3->update();
        ui->checkBox_3->repaint();

        //ui->checkBox_4->setAutoExclusive(false);
        ui->checkBox_4->setChecked(false);
        //ui->checkBox_4->setAutoExclusive(true);
        ui->checkBox_4->update();
        ui->checkBox_4->repaint();

        //ui->checkBox_5->setAutoExclusive(false);
        ui->checkBox_5->setChecked(false);
        //ui->checkBox_5->setAutoExclusive(true);
        ui->checkBox_5->update();
        ui->checkBox_5->repaint();

        ui->checkBox->setCheckable(true);
        ui->checkBox_2->setCheckable(true);
        ui->checkBox_3->setCheckable(true);
        ui->checkBox_4->setCheckable(true);
        ui->checkBox_5->setCheckable(true);

        // check buttons for next part
        tempPattern = fieldType[fieldCount];

        if(tempPattern[0] == '1'){
            ui->radioButton->setAutoExclusive(false);
            ui->radioButton->setChecked(true);
            ui->radioButton->setAutoExclusive(true);
            ui->radioButton->update();
            ui->radioButton->repaint();
        }
        if(tempPattern[1] == '1'){
            ui->radioButton_2->setAutoExclusive(false);
            ui->radioButton_2->setChecked(true);
            ui->radioButton_2->setAutoExclusive(true);
            ui->radioButton_2->update();
            ui->radioButton_2->repaint();
        }
        if(tempPattern[2] == '1'){
            ui->radioButton_3->setAutoExclusive(false);
            ui->radioButton_3->setChecked(true);
            ui->radioButton_3->setAutoExclusive(true);
            ui->radioButton_3->update();
            ui->radioButton_3->repaint();
        }
        if(tempPattern[3] == '1'){
            ui->radioButton_4->setAutoExclusive(false);
            ui->radioButton_4->setChecked(true);
            ui->radioButton_4->setAutoExclusive(true);
            ui->radioButton_4->update();
            ui->radioButton_4->repaint();
        }
        if(tempPattern[4] == '1'){
            ui->radioButton_5->setAutoExclusive(false);
            ui->radioButton_5->setChecked(true);
            ui->radioButton_5->setAutoExclusive(true);
            ui->radioButton_5->update();
            ui->radioButton_5->repaint();
        }
        if(tempPattern[5] == '1'){
            ui->radioButton_6->setAutoExclusive(false);
            ui->radioButton_6->setChecked(true);
            ui->radioButton_6->setAutoExclusive(true);
            ui->radioButton_6->update();
            ui->radioButton_6->repaint();
        }
        if(tempPattern[6] == '1'){
            //ui->checkBox->setAutoExclusive(false);
            ui->checkBox->setChecked(true);
            //ui->checkBox->setAutoExclusive(true);
            ui->checkBox->update();
            ui->checkBox->repaint();
        }
        if(tempPattern[7] == '1'){
            //ui->checkBox_2->setAutoExclusive(false);
            ui->checkBox_2->setChecked(true);
            //ui->checkBox_2->setAutoExclusive(true);
            ui->checkBox_2->update();
            ui->checkBox_2->repaint();
        }
        if(tempPattern[8] == '1'){
            //ui->checkBox_3->setAutoExclusive(false);
            ui->checkBox_3->setChecked(true);
            //ui->checkBox_3->setAutoExclusive(true);
            ui->checkBox_3->update();
            ui->checkBox_3->repaint();
        }
        if(tempPattern[9] == '1'){
            //ui->checkBox_4->setAutoExclusive(false);
            ui->checkBox_4->setChecked(true);
            //ui->checkBox_4->setAutoExclusive(true);
            ui->checkBox_4->update();
            ui->checkBox_4->repaint();
        }
        if(tempPattern[10] == '1'){
            //ui->checkBox_5->setAutoExclusive(false);
            ui->checkBox_5->setChecked(true);
            //ui->checkBox_5->setAutoExclusive(true);
            ui->checkBox_5->update();
            ui->checkBox_5->repaint();
        }

        if(tempPattern[4] == '1'){
            ui->checkBox->setCheckable(true);
            ui->checkBox_2->setCheckable(true);
            ui->checkBox_3->setCheckable(true);
            ui->checkBox_4->setCheckable(true);
            ui->checkBox_5->setCheckable(true);
        }
        else{
            ui->checkBox->setCheckable(false);
            ui->checkBox_2->setCheckable(false);
            ui->checkBox_3->setCheckable(false);
            ui->checkBox_4->setCheckable(false);
            ui->checkBox_5->setCheckable(false);
        }
    }
}

void Mapping::finishedMapping_pressed(){
    QString tempPattern = "00000000000";

    if(fieldCount < numFields){
        if(ui->radioButton->isChecked()){
            tempPattern[0] = '1';
        }
        if(ui->radioButton_2->isChecked()){
            tempPattern[1] = '1';
        }
        if(ui->radioButton_3->isChecked()){
            tempPattern[2] = '1';
        }
        if(ui->radioButton_4->isChecked()){
            tempPattern[3] = '1';
        }
        if(ui->radioButton_5->isChecked()){
            tempPattern[4] = '1';
        }
        if(ui->radioButton_6->isChecked()){
            tempPattern[5] = '1';
        }
        if(ui->checkBox->isChecked()){
            tempPattern[6] = '1';
        }
        if(ui->checkBox_2->isChecked()){
            tempPattern[7] = '1';
        }
        if(ui->checkBox_3->isChecked()){
            tempPattern[8] = '1';
        }
        if(ui->checkBox_4->isChecked()){
            tempPattern[9] = '1';
        }
        if(ui->checkBox_5->isChecked()){
            tempPattern[10] = '1';
        }

        fieldType[fieldCount] = tempPattern;
        qDebug() << fieldType[fieldCount];
    }

    mainParent->submitMapping(fieldType);

    done(0);
}

void Mapping::notString_selected(){
    ui->checkBox->setCheckable(true);
    ui->checkBox_2->setCheckable(true);
    ui->checkBox_3->setCheckable(true);
    ui->checkBox_4->setCheckable(true);
    ui->checkBox_5->setCheckable(true);

    ui->checkBox->setChecked(false);
    ui->checkBox_2->setChecked(false);
    ui->checkBox_3->setChecked(false);
    ui->checkBox_4->setChecked(false);
    ui->checkBox_5->setChecked(false);

    ui->checkBox->update();
    ui->checkBox->repaint();
    ui->checkBox_2->update();
    ui->checkBox_2->repaint();
    ui->checkBox_3->update();
    ui->checkBox_3->repaint();
    ui->checkBox_4->update();
    ui->checkBox_4->repaint();
    ui->checkBox_5->update();
    ui->checkBox_5->repaint();

    ui->checkBox->setCheckable(false);
    ui->checkBox_2->setCheckable(false);
    ui->checkBox_3->setCheckable(false);
    ui->checkBox_4->setCheckable(false);
    ui->checkBox_5->setCheckable(false);
}

void Mapping::string_selected(){
    ui->checkBox->setCheckable(true);
    ui->checkBox_2->setCheckable(true);
    ui->checkBox_3->setCheckable(true);
    ui->checkBox_4->setCheckable(true);
    ui->checkBox_5->setCheckable(true);
}
