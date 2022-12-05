#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSpinBox>
#include <QListWidget>
#include <QGraphicsView>
#include <QTime>
#include <QStatusBar>
#include <QVector>
#include <QtGlobal>
#include <QMainWindow>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include "menu.h"
#include "dbmanager.h"
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
    Menu* masterMenu;
    Menu* mainMenuOG;
    Menu* sessionMenu;
    Menu* historyMenu;
    QString timeString;
    void updateMenu(const QString selectedMenuItem, const QStringList menuItems);
    QListWidget *currentMenu;
    bool powerStatus;
    void initializeTimer(QTimer*);
    DBManager* db;
    QVector<History*> recordings;
    QStringList userRecordings;
    QTimer *timer;


private slots:
    void updateTimer();
    void navigateDownMenu();
    void navigateUpMenu();
    void navigateSubMenu();
    void alphaPressed();
    void betaPressed();
    void thetaPressed();
    void deltaPressed();
    void deleteData();
    void sessionWindow();
};
#endif // MAINWINDOW_H
