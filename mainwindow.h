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

private:
    Ui::MainWindow *ui;
    Menu* masterMenu;
    Menu* mainMenuOG;
    Menu* sessionMenu;
    Menu* historyMenu;
    void updateMenu(const QString selectedMenuItem, const QStringList menuItems);
    QListWidget *currentMenu;
    bool powerStatus;
    DBManager* db;
    QVector<History*> recordings;
    QStringList userRecordings;

private slots:
    void navigateDownMenu();
    void navigateUpMenu();
    void navigateSubMenu();
    void alphaPressed();
    void betaPressed();
    void thetaPressed();
    void deltaPressed();
    void deleteData();
};
#endif // MAINWINDOW_H
