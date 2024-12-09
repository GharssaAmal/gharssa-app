#ifndef MATERIEL_H
#define MATERIEL_H

#include <string>
#include <QSqlQuery>
#include <QSqlQueryModel>

#include <QString>


class Materiel {
private:
    int id;
    QString nom;
    QString etat;
    QString date_achat;
    int duree_garantie;
    double prix;
    int quantite_disponible;
    int duree_de_vie;

public:
    // Constructeur par défaut
    Materiel();

    // Constructeur avec paramètres
   /* Materiel(int id,  QString nom,   QString etat,   QString date_achat,
             int duree_garantie, double prix, int quantite_disponible, int duree_de_vie);*/
    Materiel(int id, const QString nom, const QString etat, const QString date_achat,
                int duree_garantie, double prix, int quantite_disponible, int duree_de_vie);

    // Getters
    int getId() const;
    QString getNom() const;
  QString getEtat() const;
    QString getDateAchat() const;
    int getDureeGarantie() const;
    double getPrix() const;
    int getQuantiteDisponible() const;
    int getDureeDeVie() const;

    // Setters
    void setId(int id);
    void setNom(const  QString nom);
    void setEtat(const  QString etat);
    void setDateAchat(const  QString date_achat);
    void setDureeGarantie(int duree_garantie);
    void setPrix(double prix);
    void setQuantiteDisponible(int quantite_disponible);
    void setDureeDeVie(int duree_de_vie);

    // Méthodes CRUD
    bool ajouter();
    bool supprimer(int id);
    bool modifier(int id);
    QSqlQueryModel* afficher();
    QSqlQueryModel* rechercher(const QString& searchQuery);
    QSqlQueryModel* trier(const QString& critere, const QString& ordre);

 void genererStatistiquesMateriel(QSqlQueryModel* model);
 QSqlQuery prendre(int id);

};


#endif // MATERIEL_H

