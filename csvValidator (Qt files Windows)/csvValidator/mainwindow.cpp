#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mapping.h"
#include "helpwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QStringList>
#include <QChar>

QStringList fieldsList;
int numFieldsGlob;
QStringList mappingStringList;
QString CSVText;
QString errorsFound;
int fileOption;
int expectedNumFields;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton,SIGNAL(released()),this,SLOT(openFile_pressed()));
    connect(ui->pushButton_2,SIGNAL(released()),this,SLOT(mapping_pressed()));
    connect(ui->pushButton_3,SIGNAL(released()),this,SLOT(display_pressed()));
    connect(ui->pushButton_4,SIGNAL(released()),this,SLOT(help_pressed()));
    connect(ui->pushButton_5,SIGNAL(released()),this,SLOT(newFile_pressed()));

    connect(ui->radioButton,SIGNAL(released()),this,SLOT(fileType_pressed()));
    connect(ui->radioButton_2,SIGNAL(released()),this,SLOT(fileType_pressed()));
    connect(ui->radioButton_3,SIGNAL(released()),this,SLOT(fileType_pressed()));
    connect(ui->radioButton_4,SIGNAL(released()),this,SLOT(fileType_pressed()));
    connect(ui->radioButton_5,SIGNAL(released()),this,SLOT(fileType_pressed()));

    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_5->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fileType_pressed(){
    ui->pushButton->show();
}

void MainWindow::openFile_pressed()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Open a file", "C://");
    QFile file(file_name);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "title", "file not open");
    }
    QTextStream in(&file);
    QString text = in.readAll();
    ui->plainTextEdit->setPlainText(text);
    CSVText = text;

    for(int i = 0; text[i]!='\0'; ++i){
        qDebug() << text[i];
    }

    QStringList list;
    QString line;
    int numFields = 0;
    for(int i = 0; text[i]!='\xa'; ++i){
        line = line + text[i];
        if(text[i]==','){
            ++numFields; //couting number of columns
        }
    }

    numFieldsGlob = numFields + 1;

    fieldsList = line.split(",");

    qDebug() << fieldsList[0];

    file.close();

    if(ui->radioButton->isChecked()){ // if order (mapping) display mapping button
        ui->pushButton_2->show();
        fileOption = 1; //orders
        expectedNumFields = -1;
    }
    else{ // if any other file type show display validation results button
        ui->pushButton_3->show();
    }
    if(ui->radioButton_2->isChecked()){
        fileOption = 2; //customers
        expectedNumFields = 13;
    }
    if(ui->radioButton_3->isChecked()){
        fileOption = 3; //products
        expectedNumFields = 24;
    }
    if(ui->radioButton_4->isChecked()){
        fileOption = 4; //jet products
        expectedNumFields = 29;
    }
    if(ui->radioButton_5->isChecked()){
        fileOption = 5; //inventory
        expectedNumFields = 9;
    }

    ui->radioButton->hide();
    ui->radioButton_2->hide();
    ui->radioButton_3->hide();
    ui->radioButton_4->hide();
    ui->radioButton_5->hide();

    // reset errors found string
    errorsFound = "";
}

void MainWindow::mapping_pressed()
{
    Mapping dia;
    dia.performMapping(fieldsList, numFieldsGlob, this);
    dia.setModal(true);
    dia.exec();
}

void MainWindow::submitMapping(QStringList mappingInfo)
{
    mappingStringList = mappingInfo;
    QString mappingString;

    for(int i=0; i<mappingStringList.size(); ++i){
        qDebug() << mappingStringList[i];
    }

    int count = 0;
    for(int i = 0; CSVText[i]!='\xa'; ++i){
        ++count;
    }
    ++count;

    int row = 0;
    bool endOF = false;
    bool fieldError = false;
    int lineLength;
    QString currentLine;
    QStringList currentLineList;
    QString strCharsMap;

    if(expectedNumFields>0){
        if(expectedNumFields == 13 && numFieldsGlob != 13){ //customers
            endOF = true;
            errorsFound = errorsFound + "The CSV file selected does not have the proper number of fields for a Customer Import CSV File" + '\xa';
        }
        if(expectedNumFields == 24 && numFieldsGlob != 24){ //products
            endOF = true;
            errorsFound = errorsFound + "The CSV file selected does not have the proper number of fields for a Product Import CSV File" + '\xa';
        }
        if(expectedNumFields == 29 && numFieldsGlob != 29){ //jet products
            endOF = true;
            errorsFound = errorsFound + "The CSV file selected does not have the proper number of fields for a Jet Product Import CSV File" + '\xa';
        }
        if(expectedNumFields == 9 && numFieldsGlob != 9){ //inventory
            endOF = true;
            errorsFound = errorsFound + "The CSV file selected does not have the proper number of fields for a Inventory Import CSV File" + '\xa';
        }
    }

    // for checking fields inside of quotation marks " "
    // fields inside quotion marks can contain commas , which would throw off .split(",");
    while( !endOF ){
        currentLine = "";
        while( CSVText[count]!='\xa' && CSVText[count]!='\0' ){
            currentLine = currentLine + CSVText[count];
            ++count;
        }
        ++count;
        qDebug() << currentLine;
        if(currentLine==""){
            endOF = true;
        }
        else{
            // validate the line based on mapping

            /*
             need to check for fields containging commas ,
             in fields inside of quotation marks " "
            */
            lineLength = 0;
            for(int i=0; currentLine[i]!='\0'; ++i){
                ++lineLength;
            }

            // start of the validation for quoted fields

            bool eofQuote = false;
            int quoteStart = 0;
            int quoteEnd = 0;
            int numQuotedFields = 0;
            int fieldTrack = 0;
            int quoteStartIndexes[numFieldsGlob];
            int quoteEndIndexes[numFieldsGlob];
            for(int i=0; i<lineLength; ++i){
                eofQuote = false;
                quoteStart = 0;
                quoteEnd = 0;

                //qDebug() << "1";

                if( (i==0 && currentLine[i]=='"') || (i!=0 && currentLine[i]=='"' && currentLine[i-1]==',' && currentLine[i+1]!='\0') ){ // if first field is in " ", possibly contains commas
                    quoteStart = i;
                    fieldTrack = i;
                    while(!eofQuote){
                        if(currentLine[fieldTrack]=='\0'){
                            eofQuote = true;
                        }
                        if(currentLine[fieldTrack]=='"' && currentLine[fieldTrack+1]==','){
                            eofQuote = true;
                            quoteEnd = fieldTrack;
                            ++numQuotedFields;
                            i = fieldTrack + 1;
                        }
                        if(currentLine[fieldTrack]=='"' && currentLine[fieldTrack+1]=='\0'){
                            eofQuote = true;
                            quoteEnd = fieldTrack;
                            ++numQuotedFields;
                            i = fieldTrack + 1;
                        }
                        ++fieldTrack;
                    }
                    if(quoteEnd>0){
                        quoteStartIndexes[numQuotedFields-1] = quoteStart;
                        quoteEndIndexes[numQuotedFields-1] = quoteEnd;
                    }
                }
                /*
                else if(currentLine[i]=='"' && currentLine[i-1]==',' && currentLine[i+1]!='\0'){

                }*/
            }

            //qDebug() << quoteStartIndexes[0];
            //qDebug() << quoteEndIndexes[0];
            //qDebug() << numQuotedFields;

            currentLineList.clear();
            QString tempString = "";
            QStringList tempStringList;

            qDebug() << "Good here 1";

            if(numQuotedFields>0){
                tempStringList.clear();

                qDebug() << "Good here 2";

                for(int i=0; i<numQuotedFields; ++i){
                    tempString = "";
                    if(quoteStartIndexes[i] == 0){
                        for(int j=0; j < quoteEndIndexes[0]; ++j){
                            tempString = tempString + currentLine[j];
                        }

                        currentLineList.append(tempString);
                    }
                    else{
                        // append fields to currentLineList up to quoteStartIndexes[i]
                        // put quoted field into tempString
                        // append tempString to currentLineList
                        if(i==0){

                            qDebug() << "Good here 3";
                            // (quoteStartIndexes[0]-1)
                            for(int j=0; j<(quoteStartIndexes[0]-1); ++j){
                                tempString = tempString + currentLine[j];
                            }

                            qDebug() << "Good here 4";

                            tempStringList = tempString.split(",");

                            qDebug() << "Good here 5";

                            foreach (QString iter, tempStringList) {
                                currentLineList.append(iter);
                            }

                            qDebug() << "Good here 6";

                            tempString = "";
                            for(int j=quoteStartIndexes[0]; j<=quoteEndIndexes[0]; ++j){
                                tempString = tempString + currentLine[j];
                            }

                            qDebug() << tempString;

                            currentLineList.append(tempString);
                        }
                        else{
                            for(int j=(quoteEndIndexes[i-1] + 1); i<quoteStartIndexes[i]; ++j){
                                tempString = tempString + currentLine[j];
                            }
                            tempStringList = tempString.split(",");
                            foreach (QString iter, tempStringList) {
                                currentLineList.append(iter);
                            }
                            tempString = "";
                            for(int j=quoteStartIndexes[i]; j<=quoteEndIndexes[i]; ++j){
                                tempString = tempString + currentLine[j];
                            }
                            currentLineList.append(tempString);
                        }
                    }
                }
                // if currentLine[( quoteEndIndexes[numQuotedFields - 1] ) + 1] != '\0'
                // aka the last quoted field added is not the last field overall
                // append the rest of the fields in the end of currentLine to currentLineList
                tempStringList.clear();
                tempString = "";
                if(currentLine[( quoteEndIndexes[numQuotedFields - 1] ) + 1] != '\0'){
                    for(int i=(( quoteEndIndexes[numQuotedFields - 1] ) + 2); currentLine[i]!='\0'; ++i){
                        tempString = tempString + currentLine[i];
                    }
                    tempStringList = tempString.split(",");
                    foreach (QString iter, tempStringList) {
                        currentLineList.append(iter);
                    }
                }
            }
            else{
                currentLineList = currentLine.split(",");
            }

            // end of the validation for quoted fields


            ++row;

            if(numFieldsGlob == currentLineList.size()){ // checking if # of fields in current line is = to # of headers

            for(int i = 0; i < numFieldsGlob; ++i){
                qDebug() << currentLineList[i];
                qDebug() << mappingStringList[i];

                mappingString = mappingStringList[i];

                fieldError = false;

                if(currentLineList[i].isEmpty()){
                    if(mappingString[5] == '1'){
                        fieldError = true;
                    }
                }
                else{
                    if(mappingString[0] == '1'){
                        fieldError = invalidInt(currentLineList[i]);
                        qDebug() << "Validating int";
                    } //int
                    if(mappingString[1] == '1'){
                        fieldError = invalidDec(currentLineList[i]);
                        qDebug() << "Validating dec";
                    } //decimal
                    if(mappingString[2] == '1'){
                        fieldError = invalidDate(currentLineList[i]);
                        qDebug() << "Validating date";
                    } //date
                    if(mappingString[3] == '1'){
                        fieldError = invalidBool(currentLineList[i]);
                        qDebug() << "Validating bool";
                    } //bool
                    if(mappingString[4] == '1'){
                        strCharsMap = "";
                        strCharsMap = strCharsMap + mappingString[6] + mappingString[7] + mappingString[8] + mappingString[9] + mappingString[10];

                        fieldError = invalidString(currentLineList[i], strCharsMap);
                        qDebug() << "Validating string";
                        qDebug() << strCharsMap;
                    } //string
                }

                if(fieldError){
                    errorsFound = errorsFound + "Error found in " + fieldsList[i] + " field " + "row " + QString::number(row) + " column " + QString::number(i + 1) + '\xa';
                }
            }

            } // close bracket for checking field #
            else{
                errorsFound = errorsFound + "Error found in row " + QString::number(row) + ", number of fields in row does not match the number of headers provided" + '\xa';
            }

        }
    }

    ui->pushButton_3->show();
}

void MainWindow::display_pressed()
{
    // if file option == 1 , then file was an order (mapping), validation already occurred in mapping_pressed()
    // if any other file must still validate w/o any mapping, based on CSV import templates from ShipStation

    if(fileOption == 1){
        if(errorsFound.isEmpty()){
            errorsFound = "No errors found";
        }

        errorsFound = "CSV Validation Results: \xa" + errorsFound;
        ui->plainTextEdit->setPlainText(errorsFound);
        ui->plainTextEdit->update();
        ui->plainTextEdit->repaint();
    }
    if(fileOption == 2){
        //validate customer csv
        // 13 total fields
        QStringList mapPattern = {"00001000001", "00001000001", "00001000001", "00001000001", "00001001110", "00001011010",
                                  "00001000001", "00001000001", "00001000001", "00001010000", "00001010000", "10000000000", "00001010000"}; // testing for CSV Test1 file

        this->submitMapping(mapPattern);

        if(errorsFound.isEmpty()){
            errorsFound = "No errors found";
        }

        errorsFound = "CSV Validation Results: \xa" + errorsFound;
        ui->plainTextEdit->setPlainText(errorsFound);
        ui->plainTextEdit->update();
        ui->plainTextEdit->repaint();
    }
    if(fileOption == 3){
        //validate product csv
        // 24 total fields

        QStringList mapPattern = {"00001000001", "00001000001", "00001000001", "01000000000", "00001000001", "00001000001", "00001000001",
                                  "00001000001", "00001000001", "00001000001", "00001000001", "01000000000", "10000000000", "00001010000",
                                  "00001000001", "10000000000", "00001000001", "01000000000", "01000000000", "01000000000", "00001000001",
                                  "00010000000", "00001000001", "00010000000"};

        this->submitMapping(mapPattern);

        if(errorsFound.isEmpty()){
            errorsFound = "No errors found";
        }

        errorsFound = "CSV Validation Results: \xa" + errorsFound;
        ui->plainTextEdit->setPlainText(errorsFound);
        ui->plainTextEdit->update();
        ui->plainTextEdit->repaint();
    }
    if(fileOption == 4){
        //validate jet product csv
        // 29 total fields
        QStringList mapPattern = {"00001000001", "00001000001", "00001000001", "01000000000", "00001000001", "00001000001", "00001000001",
                                  "00001000001", "00001000001", "00001000001", "00001000001", "01000000000", "10000000000", "00001010000",
                                  "00001000001", "10000000000", "00001000001", "01000000000", "01000000000", "01000000000", "00001000001",
                                  "00010000000", "00001000001", "00001000001", "00001000001", "01000000000", "10000000000", "00001000001",
                                  "00001000001"};

        this->submitMapping(mapPattern);

        if(errorsFound.isEmpty()){
            errorsFound = "No errors found";
        }

        errorsFound = "CSV Validation Results: \xa" + errorsFound;
        ui->plainTextEdit->setPlainText(errorsFound);
        ui->plainTextEdit->update();
        ui->plainTextEdit->repaint();
    }
    if(fileOption == 5){
        //validate inventory csv
        // 9 total fields
        QStringList mapPattern = {"00001000001", "00001000001", "00001000001", "00001000001", "00001000001", "00001000001", "10000000000",
                                  "10000000000", "01000000000"};

        this->submitMapping(mapPattern);

        if(errorsFound.isEmpty()){
            errorsFound = "No errors found";
        }

        errorsFound = "CSV Validation Results: \xa" + errorsFound;
        ui->plainTextEdit->setPlainText(errorsFound);
        ui->plainTextEdit->update();
        ui->plainTextEdit->repaint();
    }

    // show the validate another file button
    ui->pushButton_5->show();
}

void MainWindow::help_pressed(){
    helpWindow help;
    help.setModal(true);
    help.exec();
}

void MainWindow::newFile_pressed(){
    ui->setupUi(this);

    connect(ui->pushButton,SIGNAL(released()),this,SLOT(openFile_pressed()));
    connect(ui->pushButton_2,SIGNAL(released()),this,SLOT(mapping_pressed()));
    connect(ui->pushButton_3,SIGNAL(released()),this,SLOT(display_pressed()));
    connect(ui->pushButton_4,SIGNAL(released()),this,SLOT(help_pressed()));
    connect(ui->pushButton_5,SIGNAL(released()),this,SLOT(newFile_pressed()));

    connect(ui->radioButton,SIGNAL(released()),this,SLOT(fileType_pressed()));
    connect(ui->radioButton_2,SIGNAL(released()),this,SLOT(fileType_pressed()));
    connect(ui->radioButton_3,SIGNAL(released()),this,SLOT(fileType_pressed()));
    connect(ui->radioButton_4,SIGNAL(released()),this,SLOT(fileType_pressed()));
    connect(ui->radioButton_5,SIGNAL(released()),this,SLOT(fileType_pressed()));

    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_5->hide();
}

// returns false for valid, true for invalid
bool MainWindow::invalidInt(QString field){
    bool inValid = false;

    for(int i=0; field[i]!='\0'; ++i){
        if(!field[i].isDigit()){
            inValid = true;
        }
    }

    return inValid;
}

bool MainWindow::invalidDec(QString field){
    bool inValid = false;

    int numNonDigits = 0;
    bool containsDecimal = false;
    int decimalLoc = 0;

    for(int i=0; field[i]!='\0'; ++i){
        if(!field[i].isDigit()){
            ++numNonDigits;
        }
        if(field[i] == '.'){
            containsDecimal = true;
            decimalLoc = i;
        }
    }

    if(numNonDigits >= 2){
        inValid = true;
    }
    else if(!containsDecimal && numNonDigits > 0){
        inValid = true;
    }

    return inValid;
}

bool MainWindow::invalidDate(QString field){
    bool inValid = false;

    int numSlashes = 0;
    QString month = "";
    QString day = "";
    QString year = "";

    for(int i=0; field[i]!='\0'; ++i){
        if(field[i] == '/'){
            ++numSlashes;
        }
        else if(numSlashes == 0){
            month = month + field[i];
        }
        else if(numSlashes == 1){
            day = day + field[i];
        }
        else{
            year = year + field[i];
        }
    }

    bool isAllDigits = true;
    for(int i = 0; month[i]!='\0'; ++i){
        if(!month[i].isDigit()){
            isAllDigits = false;
        }
    }
    for(int i = 0; day[i]!='\0'; ++i){
        if(!day[i].isDigit()){
            isAllDigits = false;
        }
    }
    for(int i = 0; year[i]!='\0'; ++i){
        if(!year[i].isDigit()){
            isAllDigits = false;
        }
    }

    int monthNum;
    int dayNum;
    int yearNum;
    if(isAllDigits){
        monthNum = month.toInt();
        dayNum = day.toInt();
        yearNum = year.toInt();
        if(monthNum > 12 || monthNum <= 0){
            inValid = true;
        }
        if(dayNum > 31 || dayNum <= 0){
            inValid = true;
        }
        if(year.length()!=2 && year.length()!=4){ //making sure year is either YY or YYYY
            inValid = true;
        }
        if(monthNum == 1 || monthNum == 3 || monthNum == 5 || monthNum == 7 || monthNum == 8 || monthNum == 10 || monthNum == 12){
            if(dayNum > 31){
                inValid = true;
            }
        }
        else if(monthNum == 4 || monthNum == 6 || monthNum == 9 || monthNum == 11){
            if(dayNum > 30){
                inValid = true;
            }
        }
        else{
            if(yearNum % 4 == 0){
                if(dayNum > 29){
                    inValid = true;
                }
            }
            else{
                if(dayNum > 28){
                    inValid = true;
                }
            }
        }
    }
    else{
        inValid = true;
    }

    if(numSlashes!=2){
        inValid = true;
    }

    return inValid;
}

bool MainWindow::invalidBool(QString field){
    bool inValid = false;

    int fieldLen = 0;

    QChar fieldArr[field.length() + 1];
    for(int i=0; field[i]!='\0'; ++i){
        // this deals with trailing and leading white spaces
        /*
        if(field[i] != ' '){
            fieldArr[fieldLen] = tolower(field[i]);
            ++fieldLen;
        }
        */
        fieldArr[fieldLen] = field[i].toLower();
        ++fieldLen;
    }
    fieldArr[fieldLen] = '\0';

    char fstr[] = {'f', 'a', 'l', 's', 'e'};
    char tstr[] = {'t', 'r', 'u', 'e'};

    bool isTrue = true;
    bool isFalse = true;

    if(fieldLen == 5){
        for(int i=0; fieldArr[i]!='\0'; ++i){
            if(fieldArr[i] != fstr[i]){
                isFalse = false;
            }
            qDebug() << fieldArr[i] << " " << fstr[i] << endl;
        }
    }

    if(fieldLen == 4){
        for(int i=0; fieldArr[i]!='\0'; ++i){
            if(fieldArr[i] != tstr[i]){
                isTrue = false;
            }
            qDebug() << fieldArr[i] << " " << tstr[i] << endl;
        }
    }

    if(fieldLen == 5 && !isFalse){
        inValid = true;
    }
    if(fieldLen == 4 && !isTrue){
        inValid = true;
    }
    if(fieldLen != 5 && fieldLen != 4){
        inValid = true;
    }

    return inValid;
}

bool MainWindow::invalidString(QString field, QString charsAllowed){
    bool inValid = false;

    if(charsAllowed[4] == '1'){
        inValid = false;
    }
    else{
        if(charsAllowed[0] == '0'){ //if letters are NOT allowed
            // if any letters then inValid = true
            for(int i=0; field[i]!='\0'; ++i){
                if(field[i].isLetter()){
                    inValid = true;
                }
            }
        }
        if(charsAllowed[1] == '0'){ //if digits are NOT allowed
            // if any digits then inValid = true
            for(int i=0; field[i]!='\0'; ++i){
                if(field[i].isDigit()){
                    inValid = true;
                }
            }
        }
        if(charsAllowed[2] == '0'){ //if spaces are NOT allowed
            // if any spaces then inValid = true
            for(int i=0; field[i]!='\0'; ++i){
                if(field[i].isSpace()){
                    inValid = true;
                }
            }
        }
        if(charsAllowed[3] == '0'){ //if symbols are NOT allowed
            // if any symbols then inValid = true
            // if any symbols = any char not a letter AND not a digit AND not a space
            for(int i=0; field[i]!='\0'; ++i){
                if(!field[i].isLetter() && !field[i].isDigit() && !field[i].isSpace()){
                    inValid = true;
                }
            }
        }
    }

    return inValid;
}
