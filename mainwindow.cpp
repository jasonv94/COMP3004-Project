#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = new DBManager();
    masterMenu = new Menu("MAIN MENU", {"USER","NEW SESSION","HISTORY"}, nullptr);
    mainMenu = masterMenu;
    initMenu(masterMenu);
    currentMenu = ui->menuWidget;
    currentMenu->addItems(masterMenu->getMenuItems());
    currentMenu->setCurrentRow(0);
    bool subMenu = false;
    recordings = db->getRecordings();
    for (int x = 0; x < 2; x++) {
        qDebug()<<recordings[x]->get_therapyName();
        userRecordings += recordings[x]->string_record()+"-"+recordings[x]->get_date()+ "-" +recordings[x]->get_therapyName()+" "+recordings[x]->get_sessionTime()+" "+
                                         recordings[x]->get_frequency() + " " + recordings[x]->get_intensity();
    }
    for(int x=0; x<2; x++){
        qDebug()<<userRecordings[x];
    }
    connect(ui->upButton, &QPushButton::pressed, this, &MainWindow::navigateUpMenu);
    connect(ui->downButton, &QPushButton::pressed, this, &MainWindow::navigateDownMenu);
    connect(ui->okButton, &QPushButton::pressed, this, &MainWindow::navigateSubMenu);
    connect(ui->alphaButton, &QPushButton::pressed, this, &MainWindow::alphaPressed);
    connect(ui->betaButton, &QPushButton::pressed, this, &MainWindow::betaPressed);
    connect(ui->deltaButton, &QPushButton::pressed, this, &MainWindow::deltaPressed);
    connect(ui->thetaButton, &QPushButton::pressed, this, &MainWindow::thetaPressed);
    connect(ui->selectButton, &QPushButton::pressed, this, &MainWindow::startSession);


    QPixmap pix(":/img/images/device.PNG");
    int w = ui->rmb->width();
    int h = ui->rmb->height();
    ui->rmb->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));
//    ui->userMenu.setVisible(false);
//    ui->mainMenu.setVisible(false);
//    ui->groupMenu.setVisible(false);
}

void MainWindow::initMenu(Menu* x){
    Menu* sessionMenu = new Menu("SESSION INFO", {"TYPE: ","TIME: ","FREQUENCY: "}, x);
    Menu* timeMenu = new Menu("TIME SELECTION MENU", {"20:00","45:00","Custom Time"}, sessionMenu);
    Menu* userMenu = new Menu("USER SELECTION MENU", {"User 1","User 2","User 3"}, x);
    Menu* history = new Menu("HISTORY",userRecordings, x);
    Menu* customTime = new Menu("CUSTOM TIME",{},timeMenu);

    x->addChildMenu(userMenu);
    x->addChildMenu(sessionMenu);
    x->addChildMenu(history);
    sessionMenu->addChildMenu(timeMenu);
    timeMenu->addChildMenu(customTime);
}

void MainWindow::navigateUpMenu() {

    int nextIndex = currentMenu->currentRow() - 1;

    if (nextIndex < 0) {
        nextIndex = currentMenu->count() - 1;
    }
    qDebug()<<masterMenu->getMenuItems()[nextIndex];
    currentMenu->setCurrentRow(nextIndex);
}


void MainWindow::navigateDownMenu() {

    int nextIndex = currentMenu->currentRow() + 1;

    if (nextIndex > currentMenu->count() - 1) {
        nextIndex = 0;
    }
    //qDebug()<<masterMenu->getMenuItems()[nextIndex];

    currentMenu->setCurrentRow(nextIndex);
}

void MainWindow::navigateSubMenu() {

    int index = currentMenu->currentRow();
    qDebug()<< index;
    if (index < 0) return;
    qDebug()<<masterMenu->getMenuItems()[index];
    if(masterMenu->getMenuItems()[index] == "USER"){
        masterMenu = masterMenu->get(index);
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "NEW SESSION"){
        masterMenu = masterMenu->get(index);
        //qDebug()<<masterMenu->getMenuItems()[1] + "something";
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "HISTORY"){
        masterMenu = masterMenu->get(index);
        updateMenu(masterMenu->getName(),userRecordings);
    }else if(masterMenu->getMenuItems()[index] == "TIME: "){
        masterMenu = masterMenu->get(0);
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "User 1"){
        masterMenu = masterMenu->getParent();
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "User 2"){
        masterMenu = masterMenu->getParent();
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "User 3"){
        masterMenu = masterMenu->getParent();
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "20:00"){
        masterMenu = masterMenu->getParent();
        masterMenu->addToMenu(1,"20:00");
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "45:00"){
        masterMenu = masterMenu->getParent();
        masterMenu->addToMenu(1,"45:00");
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "Custom Time"){
        masterMenu = masterMenu->get(0);
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getName() == "CUSTOM TIME"){

    }

}
//remeber to increment down
void MainWindow::alphaPressed(){
    if(masterMenu->getName() != "SESSION INFO"){
        return;
    }
    ui->progressBar->setValue(2);
    masterMenu->addToMenu(0,"Alpha Session");
    masterMenu->addToMenu(2,"9-11 Hz");
    updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    qDebug()<<masterMenu->getMenuItems()[0];
}
void MainWindow::betaPressed(){
    if(masterMenu->getName() != "SESSION INFO"){
        return;
    }
    ui->progressBar->setValue(4);
    masterMenu->addToMenu(0,"Beta 1 Session");
    masterMenu->addToMenu(2,"12-15 Hz");
    updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    qDebug()<<masterMenu->getMenuItems()[0];

}
void MainWindow::deltaPressed(){
    if(masterMenu->getName() != "SESSION INFO"){
        return;
    }
    ui->progressBar->setValue(6);
    masterMenu->addToMenu(0,"Delta Session");
    masterMenu->addToMenu(2,"2.5-5 Hz");
    updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    qDebug()<<masterMenu->getMenuItems()[0];

}

void MainWindow::thetaPressed(){
    if(masterMenu->getName() != "SESSION INFO"){
        return;
    }
    ui->progressBar->setValue(8);
    masterMenu->addToMenu(0,"Theta Session");
    masterMenu->addToMenu(2,"6-8 Hz");
    updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    qDebug()<<masterMenu->getMenuItems()[0];

}
void MainWindow::startSession(){
    if(masterMenu->getName() != "SESSION INFO"){
        return;
    }else if(masterMenu->getMenuItems()[0].length() < 7 || masterMenu->getMenuItems()[1].length() < 7 || masterMenu->getMenuItems()[2].length() < 11){
        return;
    }else{
        //qDebug()<<masterMenu->getMenuItems()[0].last(masterMenu->getMenuItems()[0].length()-7);
        //qDebug()<<masterMenu->getMenuItems()[1].last(masterMenu->getMenuItems()[1].length()-7).first(masterMenu->getMenuItems()[1].length()-10).toInt();
        //qDebug()<<masterMenu->getMenuItems()[2].last(masterMenu->getMenuItems()[2].length()-12);
        //qDebug()<<ui->progressBar->value();
        currentSession = new Therapy(masterMenu->getMenuItems()[0].last(masterMenu->getMenuItems()[0].length()-7),ui->progressBar->value(),masterMenu->getMenuItems()[2].last(masterMenu->getMenuItems()[2].length()-12), 60 * masterMenu->getMenuItems()[1].last(masterMenu->getMenuItems()[1].length()-7).first(masterMenu->getMenuItems()[1].length()-10).toInt());
        initTimer(currentSession->get_duration());
    }
}
void MainWindow::updateMenu(const QString selectedMenuItem, const QStringList menuItems) {

    currentMenu->clear();
    currentMenu->addItems(menuItems);
    currentMenu->setCurrentRow(0);
    //mainMenu = sessionMenu;

    //ui->menuLabel->setText(selectedMenuItem);
}
void MainWindow::initTimer(QTimer* timer){
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
    timer->start(1000);
}
void MainWindow::updateTimer(){
    currentSession->set_time(currentSession->get_length() - 1);
    int timeLeft = currentSession->get_length();
    masterMenu->addToMenu(1,QString::number(timeLeft/60) + ((timeLeft%60 < 10) ? + ":0" + QString::number(timeLeft%60) : + ":" + QString::number(timeLeft%60)));
    updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
}
MainWindow::~MainWindow()
{
    delete ui;
}
