 #include "connection.h"
#include <QMainWindow>
#include "materiel.h"
#include <QSqlError>
#include <QSqlQuery>

Connection::Connection()
{

}

bool Connection::createconnection()
{
 db = QSqlDatabase::addDatabase("QODBC");
    bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("projet25");//inserer le nom de la source de données
db.setUserName("amal");//inserer nom de l'utilisateur
db.setPassword("oacle");//inserer mot de passe de cet utilisateur

if (db.open()) test=true;
return test;
if (!db.open()) {
qDebug() << "Database Error: " << db.lastError().text();
    // Handle the error, return false or show an error message
    return false;
}
}
