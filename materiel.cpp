#include "materiel.h"
#include <iostream>
#include <QSqlError>
#include "materiel.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableView>

// Constructeur par défaut
Materiel::Materiel()
    : id(0), nom(""), etat(""), date_achat(""), duree_garantie(0), prix(0.0), quantite_disponible(0), duree_de_vie(0) {}

// Constructeur avec paramètres
Materiel::Materiel(int id, const  QString nom, const QString etat, const  QString date_achat,
                   int duree_garantie, double prix, int quantite_disponible, int duree_de_vie)
    : id(id), nom(nom), etat(etat), date_achat(date_achat), duree_garantie(duree_garantie),
      prix(prix), quantite_disponible(quantite_disponible), duree_de_vie(duree_de_vie) {}

// Getters
int Materiel::getId() const { return id; }
 QString Materiel::getNom() const { return nom; }
 QString Materiel::getEtat() const { return etat; }
 QString Materiel::getDateAchat() const { return date_achat; }
int Materiel::getDureeGarantie() const { return duree_garantie; }
double Materiel::getPrix() const { return prix; }
int Materiel::getQuantiteDisponible() const { return quantite_disponible; }
int Materiel::getDureeDeVie() const { return duree_de_vie; }

// Setters
void Materiel::setId(int id) { this->id = id; }
void Materiel::setNom(const  QString nom) { this->nom = nom; }
void Materiel::setEtat(const  QString etat) { this->etat = etat; }
void Materiel::setDateAchat(const  QString date_achat) { this->date_achat = date_achat; }
void Materiel::setDureeGarantie(int duree_garantie) { this->duree_garantie = duree_garantie; }
void Materiel::setPrix(double prix) { this->prix = prix; }
void Materiel::setQuantiteDisponible(int quantite_disponible) { this->quantite_disponible = quantite_disponible; }
void Materiel::setDureeDeVie(int duree_de_vie) { this->duree_de_vie = duree_de_vie; }



bool Materiel::ajouter() {
    // Création d'un objet requête SQL
    QSqlQuery query;

    // Conversion de l'identifiant (entier) en chaîne de caractères
    QString res = QString::number(id);

    // Préparation de la requête SQL d'insertion
    // Les valeurs seront liées plus tard pour éviter les injections SQL
    query.prepare("INSERT INTO MATERIEL (id, nom, etat, date_achat, duree_garantie, prix, quantite_disponible, duree_de_vie) "
                  "VALUES (:id, :nom, :etat, :date_achat, :duree_garantie, :prix, :quantite_disponible, :duree_de_vie)");

    // Liaison des valeurs aux paramètres de la requête
    query.bindValue(":id", res);
    query.bindValue(":nom", nom);
    query.bindValue(":etat", etat);
    query.bindValue(":date_achat", date_achat);
    query.bindValue(":duree_garantie", duree_garantie);
    query.bindValue(":prix", prix);
    query.bindValue(":quantite_disponible", quantite_disponible);
    query.bindValue(":duree_de_vie", duree_de_vie);

    // Exécution de la requête et retour du résultat
    return query.exec(); // Renvoie true si l'insertion a réussi, false sinon
}
QSqlQueryModel* Materiel::afficher() {
    // Create a new query model
    QSqlQueryModel* model = new QSqlQueryModel();

    // Set the SQL query to select all columns from the "materiel" table
    model->setQuery("SELECT * FROM MATERIEL");

    // Set the headers for each column in the model
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date d'achat"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Durée de garantie"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Prix"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Quantité disponible"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Durée de vie"));

    // Return the created model
    return model;
}
bool Materiel::supprimer(int id) {
    // Check if the record exists before attempting deletion
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM MATERIEL WHERE id = :id");
    checkQuery.bindValue(":id", id);

    if (checkQuery.exec() && checkQuery.next()) {
        if (checkQuery.value(0).toInt() == 0) {
            qDebug() << "Deletion failed: No record with ID" << id << "exists in the database.";
            return false;
        }
    } else {
        qDebug() << "Error in checking record existence:" << checkQuery.lastError().text();
        return false;
    }

    // Prepare the SQL delete statement
    QSqlQuery query;
    query.prepare("DELETE FROM MATERIEL WHERE id = :id");
    query.bindValue(":id", id);

    // Execute the query and check for success
    if (query.exec()) {
        qDebug() << "Record with ID" << id << "successfully deleted.";
        return true; // Deletion succeeded
    } else {
        qDebug() << "Deletion failed:" << query.lastError().text();
        return false; // Deletion failed
    }
}


bool Materiel::modifier(int id) {
    // Create an SQL query object
    QSqlQuery query;

    // Convert the id to QString (although it may not be necessary if using bindValue with an int)
    QString res = QString::number(id);

    // Prepare the SQL update statement
    query.prepare("UPDATE MATERIEL SET nom = :nom, etat = :etat, date_achat = :date_achat, "
                  "duree_garantie = :duree_garantie, prix = :prix, "
                  "quantite_disponible = :quantite_disponible, duree_de_vie = :duree_de_vie "
                  "WHERE id = :id");

    // Bind the values of the member variables to the query
    query.bindValue(":id", res);
    query.bindValue(":nom", nom);
    query.bindValue(":etat", etat);
    query.bindValue(":date_achat", date_achat);
    query.bindValue(":duree_garantie", duree_garantie);
    query.bindValue(":prix", prix);
    query.bindValue(":quantite_disponible", quantite_disponible);
    query.bindValue(":duree_de_vie", duree_de_vie);

    // Execute the query and return the result
    return query.exec(); // Returns true if update succeeded, false otherwise
}

QSqlQueryModel* Materiel::rechercher(const QString& searchQuery) {
    // Create a new query model to display the results
    QSqlQueryModel* model = new QSqlQueryModel();

    // Prepare the SQL query to search by id or name (nom)
    QSqlQuery query;

    // If the search query is numeric, assume it's an ID search
    if (searchQuery.toInt() != 0) {
        query.prepare("SELECT * FROM MATERIEL WHERE id = :searchQuery");
        query.bindValue(":searchQuery", searchQuery.toInt());
    } else {
        // Otherwise, search by name (nom)
        query.prepare("SELECT * FROM MATERIEL WHERE nom LIKE :searchQuery");
        query.bindValue(":searchQuery", "%" + searchQuery + "%");
    }

    // Execute the query
    query.exec();

    // Set the query results to the model
    model->setQuery(query);

    // Set the headers for each column
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date d'achat"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Durée de garantie"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Prix"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Quantité disponible"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Durée de vie"));

    return model;
}


QSqlQueryModel* Materiel::trier(const QString& critere, const QString& ordre) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QString query = QString("SELECT * FROM MATERIEL ORDER BY %1 %2").arg(critere, ordre);

    model->setQuery(query);

    // Définir les en-têtes des colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Etat"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date d'achat"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Durée Garantie"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Prix"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Quantité Disponible"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Durée de Vie"));

    return model;
}
QSqlQuery Materiel::prendre(int id)
{
    QSqlQuery query;
    query.prepare("select DATE_ACHAT, DUREE_GARANTIE from materiel where id=:id");
    query.bindValue(":id",id)
            ;
    query.exec();
    query.next();
    return query;
}
