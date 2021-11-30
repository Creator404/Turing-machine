#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <cstring>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

char a[7][31][5];
//第一个维度：n个总语法
//第二个维度：某语法中的句子有几个
//第三个维度：五个字符信息，分别如下：
//状态now 读入字符 状态future 改变字符 移动
//1 0n1n
//2 进制转换
//3 减法
int Numforsentence[7];//某语法中转换规则个数
char database[20000];
char datadeal[20000];
int grammernow = 0;
int position;
char settext[20000];
char statenow;
int totGrammer;

void MainWindow::on_pushButton_clicked()//读入txt
{
    database[0] = '\0';
    QFile file("in.txt");
    if (!file.open(QIODevice::ReadOnly | QFile::Text)){
        return;
    }
    char *database1 = file.readAll().data();
    memcpy(database,database1,20000);
    int check = 0;
    totGrammer = 1;
    int littlecheck = 1;
    for(int i = 0;i < strlen(database);i++)
    {
        if(database[i] == '\n'){
            if(database[i+1] == '\n'){
                Numforsentence[totGrammer] = littlecheck;
                littlecheck = 0;
                totGrammer++;
            }
            else{
                littlecheck++;
            }
            continue;
        }
        if(database[i] == ' '){
            continue;
        }
        a[totGrammer][littlecheck][check] = database[i];
        check++;
        check %= 5;
    }
    Numforsentence[totGrammer] = littlecheck;
    QFile file1("info.txt");
    if (!file1.open(QIODevice::ReadOnly | QFile::Text)){
        return;
    }
    char *database2 = file1.readAll().data();
    ui->textBrowser_2->setText(database2);
    QMessageBox::information(NULL,"Congratulations","Initialized successfully.",QMessageBox::Ok);
}

void MainWindow::on_pushButton_2_clicked()//判断txt中某个语法
{
    if(!ui->lineEdit->text().isEmpty()){
        QString dealtext = ui->lineEdit->text();
        QByteArray ba = dealtext.toLatin1();
        char *database1 = ba.data();
        for(int i = 0;i < strlen(database1);i++)
        {
            datadeal[i] = database1[i];
        }
        datadeal[strlen(database1)] = '\0';
    }
    else{
        ui->lineEdit->setFocus();
        return;
    }
    if(datadeal[0] - '0' > totGrammer || datadeal[0] < '1' || strlen(datadeal) != 1){
        QMessageBox::information(NULL,"Warning","您的输入不合法",QMessageBox::Ok);
        return;
    }
    grammernow = datadeal[0] - '0';
    QMessageBox::information(NULL,"Congratulations","Grammer has been chosen successfully.",QMessageBox::Ok);
}

void MainWindow::on_pushButton_3_clicked()//读入句子
{
    if(!ui->lineEdit_2->text().isEmpty()){
        QString dealtext = ui->lineEdit_2->text();
        QByteArray ba = dealtext.toLatin1();
        char *database1 = ba.data();
        for(int i = 0;i < strlen(database1);i++)
        {
            datadeal[i+1] = database1[i];
        }
        //后续处理，在句子两段加B，且添加'\0'保证数组能自动识别句子长度
        datadeal[strlen(database1)+1] = 'B';
        datadeal[strlen(database1)+2] = '\0';
        datadeal[0] = 'B';
    }
    else{
        ui->lineEdit->setFocus();
        return;
    }
    position = 1;
    statenow = '0';
    QMessageBox::information(NULL,"Congratulations","Recieve data successfully.",QMessageBox::Ok);
}

void MainWindow::on_pushButton_4_clicked()//依次执行
{
    int trigger;
    if(statenow != '#'){
        trigger = 0;
        for(int i = 1;i <= Numforsentence[grammernow];i++)
        {
            qDebug()<<a[grammernow][i][0]<<a[grammernow][i][1];
            if(a[grammernow][i][0] == statenow && a[grammernow][i][1] == datadeal[position]){
                statenow = a[grammernow][i][2];
                datadeal[position] = a[grammernow][i][3];
                if(a[grammernow][i][4] == 'R'){
                    if(position == strlen(datadeal) - 1){
                        datadeal[position+1] = 'B';
                        datadeal[position+2] = '\0';
                    }
                    position++;
                }
                else{
                    if(a[grammernow][i][4] == 'L'){
                        if(position == strlen(datadeal) - 1){
                            datadeal[position+1] = 'B';
                            datadeal[position+2] = '\0';
                        }
                        position--;
                    }
                }
                trigger = 1;
                break;
            }
        }
        if(!trigger){
            QMessageBox::information(NULL,"Warning","该输入不合法",QMessageBox::Ok);
            return;
        }
    }
    else{
        QMessageBox::information(NULL,"Congratulations","已完成转换",QMessageBox::Ok);
        return;
    }
    settext[0] = '\0';
    memcpy(settext,datadeal,20000);
    int pp = strlen(settext)+1;
    settext[pp-1] = '\n';
    for(int i = 1;i <= position;i++)
    {
        settext[pp] = ' ';
        pp++;
    }
    settext[pp] = '*';
    settext[pp+1] = '\0';
    ui->textBrowser->setText(settext);
}
