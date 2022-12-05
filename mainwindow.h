#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSpinBox>
#include <QListWidget>
#include <QGraphicsView>
#include <QTime>
#include <QTimer>
#include <QStatusBar>
#include <QVector>
#include <QtGlobal>
#include <QMainWindow>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include "menu.h"
#include "history.h"
#include "dbmanager.h"
#include "user.h"
#include "therapy.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void changePowerStatus();
    bool testSelect = false;
    bool timeSelect = false;
    bool sessionStart = false;


private:
    Ui::MainWindow *ui;
    int currentTimerCount;
    DBManager* db;
    QVector<History*> recordings;
    QStringList userRecordings;
    Menu* masterMenu;
    Menu* mainMenu;
    Therapy* currentSession;
    User* CurrentUser;
    void initMenu(Menu *);
    void updateMenu(const QString selectedMenuItem, const QStringList menuItems);
    QListWidget *currentMenu;
    void initTimer(QTimer*);
    void updateTimer();
private slots:
    //void updateTimer();
    void navigateDownMenu();
    void navigateUpMenu();
    void navigateSubMenu();
    void alphaPressed();
    void betaPressed();
    void thetaPressed();
    void deltaPressed();
    void startSession();
};
#endif // MAINWINDOW_H
