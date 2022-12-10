#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QDateTime>
#include <QDebug>
#include <QSqlQuery>
#include <QList>
#include <QApplication>
#include "history.h"


/* Class Purpose:
 * Manages interactions between application and database
 * Creates database, adds records and deletes
 *
 */

class DBManager {

public:
    const QString DATE_FORMAT = "yyyy-MM-dd hh:mm";
    static const QString DATABASE_PATH;
    QVector<History*> getRecordings(int pid);// get recordings based on user
    History *getRecord(int pid, int rid);//get
    DBManager();
    bool addRecord(int pid,QString therapyName,QString sessionTime,QString frequency, int intensity);// add record to database
    bool deleteRecords(int pid);//delete based on user



private:
    QSqlDatabase oasisDB;// database initializer
    bool addRecord(const QString& tableName, const QString& name, const QDateTime& time, int powerLevel, int duration);//add record to database
    bool DBInit();//initialize database

};

#endif // DBMANAGER_H

