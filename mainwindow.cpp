#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox> // Pour afficher les messages
#include "materiel.h"  // Assurez-vous d'inclure la classe Materiel
#include <QSqlQueryModel>
#include <QTableView>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QObject>
#include <QDate>
#include <QDateTime>
#include <QDateEdit>
#include <iostream>
#include <QDebug>
#include <QFileDialog>
#include <QSqlError>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableView>
#include <QSslSocket>
 #include "qrcodegen.hpp"
#include <QtCharts/QChartView>
   #include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QMainWindow>
#include <QtCharts/QPieSeries>

 MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDoubleValidator *prixValidator = new QDoubleValidator(0, 1000000, 2, this);
       prixValidator->setNotation(QDoubleValidator::StandardNotation);
       ui->prix_materiel->setValidator(prixValidator);

       // Validateur pour les entiers (durée de garantie, durée de vie, quantité)
       QIntValidator *intValidator = new QIntValidator(0, 100000, this);
       ui->duree_garantie_materiel->setValidator(intValidator);
       ui->duree_de_vie_materiel->setValidator(intValidator);
       ui->quantite_materiel->setValidator(intValidator);
    // Connectez le bouton ajouter_materiel au slot on_ajouter_Materiel_clicked

       connect(ui->ajouter_Materiel, &QPushButton::clicked, this, &MainWindow::on_ajouter_Materiel_clicked);
       connect(ui->supression_Materiel, &QPushButton::clicked, this, &MainWindow::on_supression_Materiel_clicked);
       connect(ui->recherche, &QPushButton::clicked, this, &MainWindow::on_rechercher_Materiel_clicked);
        connect(ui->trier_Materiel, &QPushButton::clicked, this, &MainWindow::on_trier_Materiel_clicked);
        connect(ui->modifier_Materiel, &QPushButton::clicked, this, &MainWindow::on_modifier_Materiel_clicked);
        connect(ui->btnExporterPDF, &QPushButton::clicked, this, &MainWindow::on_PDF_clicked);
        ard.connect_arduino();







}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ajouter_Materiel_clicked()
{
    // Gather data from the UI fields
    int id = ui->id_materiel->text().toInt();
    QString nom = ui->nom_materiel->text();
    QString etat = ui->etat_materiel->text();
    QString date_achat = ui->date_achat_materiel->text();
    int duree_garantie = ui->duree_garantie_materiel->text().toInt();
    double prix = ui->prix_materiel->text().toDouble();
    int quantite_disponible = ui->quantite_materiel->text().toInt();
    int duree_de_vie = ui->duree_de_vie_materiel->text().toInt();
    // Validate fields
    if (nom.isEmpty()) {
           QMessageBox::warning(this, tr("Validation Error"), tr("Le nom du matériel ne peut pas être vide."));
           return;
       }
       if (etat.isEmpty()) {
           QMessageBox::warning(this, tr("Validation Error"), tr("L'état du matériel ne peut pas être vide."));
           return;
       }
       if (date_achat.isEmpty()) {
           QMessageBox::warning(this, tr("Validation Error"), tr("La date d'achat ne peut pas être vide."));
           return;
       }
    // Create a Materiel object with the collected data
    Materiel materiel(id, nom, etat, date_achat, duree_garantie, prix, quantite_disponible, duree_de_vie);
    ui->id_materiel->clear();
          ui->nom_materiel->clear();
          ui->etat_materiel->clear();
          ui->date_achat_materiel->clear();
          ui->duree_garantie_materiel->clear();
          ui->prix_materiel->clear();
          ui->quantite_materiel->clear();
          ui->duree_de_vie_materiel->clear();
    // Attempt to add the material to the database
    if (materiel.ajouter()) {
        // Refresh the table view to show the new entry
        ui->affichage->setModel(materiel.afficher());
        QMessageBox::information(this, tr("Success"), tr("Material added successfully."));
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Failed to add material. The ID might already exist."));
    }
}

void MainWindow::on_modifier_Materiel_clicked() {
    // Retrieve data from the input fields and convert them to appropriate types
    int id = ui->id_materiel->text().toInt();
    QString nom = ui->nom_materiel->text();
    QString etat = ui->etat_materiel->text();
    QString date_achat = ui->date_achat_materiel->text();
    QString duree_garantie = ui->duree_garantie_materiel->text();
    double prix = ui->prix_materiel->text().toDouble();
    int quantite_disponible = ui->quantite_materiel->text().toInt();
    int duree_de_vie = ui->duree_de_vie_materiel->text().toInt();

    // Log input values for debugging
    qDebug() << "Attempting modification with ID:" << id;
    qDebug() << "New values - Nom:" << nom << ", Etat:" << etat
             << ", Date d'achat:" << date_achat << ", Durée de garantie:" << duree_garantie
             << ", Prix:" << prix << ", Quantité disponible:" << quantite_disponible
             << ", Durée de vie:" << duree_de_vie;
    ui->id_materiel->clear();
          ui->nom_materiel->clear();
          ui->etat_materiel->clear();
          ui->date_achat_materiel->clear();
          ui->duree_garantie_materiel->clear();
          ui->prix_materiel->clear();
          ui->quantite_materiel->clear();
          ui->duree_de_vie_materiel->clear();

    // Create a Materiel object with the updated information
   Materiel materiel(id, nom, etat, date_achat, duree_garantie.toInt(), prix, quantite_disponible, duree_de_vie);
    // Attempt to modify the material record and log the result
    bool test = materiel.modifier(id);
    qDebug() << "Modifier function result:" << test;

    // Display a message based on the success of the modification
    if (test) {
        // Refresh the table view to reflect changes
        ui->affichage->setModel(materiel.afficher());
        QMessageBox::information(this, QObject::tr("OK"),
                                 QObject::tr("Modification effectuée\nCliquez sur Cancel pour quitter."),
                                 QMessageBox::Cancel);
    } else {
       QMessageBox::critical(this, QObject::tr("Erreur"),
                            QObject::tr("La modification n'a pas pu être effectuée.\nCliquez sur Cancel pour quitter."),
                           QMessageBox::Cancel);
    }
}

void MainWindow::on_supression_Materiel_clicked() {
    // Convert the text from the id_supp field to an integer
    int id = ui->id_supp->text().toInt();

    // Create a Materiel object and set its ID for deletion
    Materiel materiel;
    materiel.setId(id);

    // Call the supprimer() method of the Materiel object
    bool test = materiel.supprimer(materiel.getId());

    // Display a message based on the success of the deletion
    if (test) {
        // Refresh the table view only if the deletion succeeded
        ui->affichage->setModel(materiel.afficher());
        QMessageBox::information(this, QObject::tr("OK"),
                                 QObject::tr("Suppression effectuée\n"
                                             "Cliquez sur Annuler pour quitter."),
                                 QMessageBox::Cancel);
    } else {
        QMessageBox::critical(this, QObject::tr("Not OK"),
                              QObject::tr("Suppression non effectuée. Assurez-vous que l'ID existe.\n"
                                          "Cliquez sur Annuler pour quitter."),
                              QMessageBox::Cancel);
    }
    ui->id_materiel->clear();
          ui->nom_materiel->clear();
          ui->etat_materiel->clear();
          ui->date_achat_materiel->clear();
          ui->duree_garantie_materiel->clear();
          ui->prix_materiel->clear();
          ui->quantite_materiel->clear();
          ui->duree_de_vie_materiel->clear();
}

void MainWindow::on_rechercher_Materiel_clicked() {
    // Get the search query (either id or nom)
    QString GA_searchParam = ui->comboBox_rech->currentText();
    QString searchQuery = ui->id_rech->text();

    // Create a Materiel object to perform the search
    Materiel materiel;

    // Call the rechercher function
    QSqlQueryModel* model = materiel.rechercher(searchQuery);

    // Display the search results in the table view
    ui->affichage->setModel(model);
}


void MainWindow::on_trier_Materiel_clicked() {
    QString GA_sortParam = ui->comboBox_trie->currentText(); // Get the selected sort parameter

    QSqlQueryModel *sortedModel = new QSqlQueryModel();

    if (GA_sortParam == "Etat") {
        // Tri de l'état en ordre alphabétique de A à Z
        sortedModel->setQuery("SELECT * FROM Materiel ORDER BY etat ASC");
    } else if (GA_sortParam == "Date d'achat") {
        // Tri des dates d'achat de la première date à la dernière date (ordre croissant)
        sortedModel->setQuery("SELECT * FROM Materiel ORDER BY date_achat ASC");
    } else {
        QMessageBox::warning(this, "Erreur", "Paramètre de tri non valide.");
        return;
    }

    ui->affichage->setModel(sortedModel);
}
void MainWindow::on_PDF_clicked() {
    // Boîte de dialogue pour choisir l'emplacement d'enregistrement
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer sous", "", "Fichiers CSV (*.csv)");

    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucun emplacement sélectionné pour l'exportation.");
        return;
    }

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        // Écrire les en-têtes des colonnes
        out << "\"id\",\"nom\",\"etat\",\"date_achat\",\"quantite_disponible\",\"duree_de_vie\",\"duree_garantie\",\"prix\"\n";

        // Préparer la requête pour récupérer les données de matériel
        QSqlQuery query;
        query.prepare("SELECT id, nom, etat, date_achat, quantite_disponible, duree_de_vie, duree_garantie, prix FROM materiel");

        // Exécuter la requête et écrire chaque ligne dans le fichier CSV
        if (query.exec()) {
            while (query.next()) {
                int id = query.value("id").toInt();
                QString nom = query.value("nom").toString();
                QString etat = query.value("etat").toString();
                QString date_achat = query.value("date_achat").toString();
                int quantite_disponible = query.value("quantite_disponible").toInt();
                int duree_de_vie = query.value("duree_de_vie").toInt();
                int duree_garantie = query.value("duree_garantie").toInt();
                double prix = query.value("prix").toDouble();

                out << "\"" << id << "\","
                    << "\"" << nom << "\","
                    << "\"" << etat << "\","
                    << "\"" << date_achat << "\","
                    << "\"" << quantite_disponible << "\","
                    << "\"" << duree_de_vie << "\","
                    << "\"" << duree_garantie << "\","
                    << "\"" << prix << "\"\n";
            }
            QMessageBox::information(this, "Exportation réussie", "Les données ont été exportées avec succès en CSV.");
        } else {
            QMessageBox::warning(this, "Erreur", "Échec de la récupération des données de matériel : " + query.lastError().text());
        }

        file.close();
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier pour l'écriture.");
    }
}
void MainWindow::genererStatistiquesMateriel(QSqlQueryModel* model)
{
    QPieSeries *series = new QPieSeries();

    qreal totalPrix = 0;
    qreal totalQuantite = 0;

    // Calculer les totaux
    for (int row = 0; row < model->rowCount(); ++row) {
        qreal prix = model->data(model->index(row, 4)).toDouble(); // Colonne du prix
        qreal quantite = model->data(model->index(row, 6)).toDouble(); // Colonne de la quantité disponible

        totalPrix += prix;
        totalQuantite += quantite;
    }

    if (totalPrix > 0) {
        series->append("Prix total (" + QString::number(totalPrix, 'f', 2) + ")", totalPrix);
    }
    if (totalQuantite > 0) {
        series->append("Quantité totale (" + QString::number(totalQuantite, 'f', 0) + ")", totalQuantite);
    }

    // Créer le graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des matériels par prix et quantité disponible");
    chart->legend()->setAlignment(Qt::AlignRight);

    // Définir les polices
    QFont font;
    font.setPixelSize(16);
    chart->setTitleFont(font);
    chart->legend()->setFont(font);

    // Afficher le graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(800, 600);
    chartView->show();
}

void MainWindow::on_mail_clicked()
{
    QString serveur = "smtp.gmail.com";
                 int port = 587; // Port pour SMTP avec STARTTLS

                 // Informations de l'expéditeur et du destinataire
                 QString expediteur = "amal.gharssa2501@gmail.com";
                 QString destinataire = ui->dest->text();

                 QString subject = "Alerte de Stock";

                 // Détails du message
                  QString corps = "Le stock a atteint le seuil minimal veuillez vous le consulter le plut tot possible ";

                 // Connexion au serveur SMTP
                 QSslSocket socket;
                 socket.connectToHost(serveur, port);
                 if (!socket.waitForConnected()) {
                     qDebug() << "Échec de connexion au serveur SMTP:" << socket.errorString();
                     return;
                 }

                 // Attente de la réponse du serveur
                 if (!socket.waitForReadyRead()) {
                     qDebug() << "Délai d'attente de réponse du serveur SMTP:" << socket.errorString();
                     return;
                 }
                 qDebug() << socket.readAll(); // Afficher la réponse initiale du serveur

                 // Envoi de la commande EHLO
                 socket.write("EHLO localhost\r\n");
                 if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
                     qDebug() << "Délai d'attente de réponse du serveur SMTP (EHLO):" << socket.errorString();
                     return;
                 }
                 qDebug() << socket.readAll(); // Afficher la réponse après EHLO

                 // Envoi de la commande STARTTLS
                 socket.write("STARTTLS\r\n");
                 if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
                     qDebug() << "Délai d'attente de réponse du serveur SMTP (STARTTLS):" << socket.errorString();
                     return;
                 }
                 qDebug() << socket.readAll(); // Afficher la réponse après STARTTLS

                 // Démarrer le chiffrement
                 socket.startClientEncryption();
                 if (!socket.waitForEncrypted()) {
                     qDebug() << "Échec du chiffrement:" << socket.errorString();
                     return;
                 }

                 // Envoi de la commande AUTH LOGIN
                 socket.write("AUTH LOGIN\r\n");
                 if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
                     qDebug() << "Délai d'attente de réponse du serveur SMTP (AUTH LOGIN):" << socket.errorString();
                     return;
                 }
                 qDebug() << socket.readAll(); // Afficher la réponse après AUTH LOGIN

                 // Envoi du nom d'utilisateur encodé en Base64
                 socket.write(QByteArray().append(expediteur.toUtf8()).toBase64() + "\r\n");
                 if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
                     qDebug() << "Délai d'attente de réponse du serveur SMTP (Nom d'utilisateur):" << socket.errorString();
                     return;
                 }
                 qDebug() << socket.readAll(); // Afficher la réponse après l'envoi du nom d'utilisateur

                 // Envoi du mot de passe encodé en Base64
                 socket.write(QByteArray().append("tueg fvoq uzdt pcow").toBase64() + "\r\n");
                 if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
                     qDebug() << "Échec de l'authentification SMTP (Mot de passe):" << socket.errorString();
                     return;
                 }
                 qDebug() << socket.readAll(); // Afficher la réponse après l'envoi du mot de passe

                 // Envoi de la commande MAIL FROM
                 socket.write("MAIL FROM:<" + expediteur.toUtf8() + ">\r\n");
                 if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
                     qDebug() << "Délai d'attente de réponse du serveur SMTP (MAIL FROM):" << socket.errorString();
                     return;
                 }
                 qDebug() << socket.readAll(); // Afficher la réponse après MAIL FROM

                 // Envoi de la commande RCPT TO
                 socket.write("RCPT TO:<" + destinataire.toUtf8() + ">\r\n");
                 if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
                     qDebug() << "Délai d'attente de réponse du serveur SMTP (RCPT TO):" << socket.errorString();
                     return;
                 }
                 qDebug() << socket.readAll(); // Afficher la réponse après RCPT TO

                 // Envoi de la commande DATA
                 socket.write("DATA\r\n");
                 if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
                     qDebug() << "Délai d'attente de réponse du serveur SMTP (DATA):" << socket.errorString();
                     return;
                 }
                 qDebug() << socket.readAll(); // Afficher la réponse après DATA

                 // Envoi des en-têtes et du corps du message
                 socket.write("From: " + expediteur.toUtf8() + "\r\n");
                 socket.write("To: " + destinataire.toUtf8() + "\r\n");
                 socket.write("Subject: " + subject.toUtf8() + "\r\n");
                 socket.write("\r\n");
                 socket.write(corps.toUtf8() + "\r\n");
                 socket.write(".\r\n");
                 if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
                     qDebug() << "Échec d'envoi des données du mail:" << socket.errorString();
                     return;
                 }
                 qDebug() << socket.readAll(); // Afficher la réponse après l'envoi des données du mail

                 // Envoi de la commande QUIT
                 socket.write("QUIT\r\n");
                 if (!socket.waitForBytesWritten()) {
                     qDebug() << "Échec d'envoi de la commande QUIT:" << socket.errorString();
                     return;
                 }

                 // Fermeture de la connexion
                 socket.close();
                 QMessageBox::information(this, "Succès", "L'email a été envoyé avec succès à " + destinataire);

}


void MainWindow::on_qrcodegen_clicked()
{
    // Obtenez le modèle de sélection de la QTableView
    QItemSelectionModel *selectionModel = ui->affichage->selectionModel();
    if (!selectionModel->hasSelection()) {
        QMessageBox::warning(this, "No Selection", "Please select a supplier first.");
        return;
    }

    // Obtenez l'index de la ligne sélectionnée
    QModelIndex selectedIndex = selectionModel->currentIndex();
    int selectedRow = selectedIndex.row();

    // Obtenez le modèle associé à la QTableView
    QAbstractItemModel *model = ui->affichage->model();

    // Récupérer les informations ID, nom, état, durée/quantité, et prix à partir des colonnes correspondantes
    QString id = model->data(model->index(selectedRow, 0)).toString();  // Colonne 0 : ID
    QString nom = model->data(model->index(selectedRow, 1)).toString(); // Colonne 1 : Nom
    QString etat = model->data(model->index(selectedRow, 2)).toString(); // Colonne 2 : État
    QString dureeQuantite = model->data(model->index(selectedRow, 4)).toString(); // Colonne 4 : Durée/Quantité
    QString prix = model->data(model->index(selectedRow, 5)).toString(); // Colonne 5 : Prix

    // Vérifier que les informations sont valides
    if (id.isEmpty() || nom.isEmpty() || etat.isEmpty() || dureeQuantite.isEmpty() || prix.isEmpty()) {
        QMessageBox::warning(this, "Invalid Data", "Some required information is missing for the selected item.");
        return;
    }

    // Créer une chaîne de texte avec les informations pour le QR code
    QString qrData = QString("ID: %1\nNom: %2\nÉtat: %3\nDurée: %4\nPrix: %5")
                         .arg(id, nom, etat, dureeQuantite, prix);

    // Générer le QR code
    using namespace qrcodegen;
    QrCode qr = QrCode::encodeText(qrData.toUtf8().data(), QrCode::Ecc::MEDIUM);
    int sz = qr.getSize();
    QImage im(sz, sz, QImage::Format_RGB32);
    QRgb black = qRgb(0, 0, 0);
    QRgb white = qRgb(255, 255, 255);

    // Remplir l'image du QR code avec les modules (pixels)
    for (int y = 0; y < sz; y++) {
        for (int x = 0; x < sz; x++) {
            im.setPixel(x, y, qr.getModule(x, y) ? black : white);
        }
    }

    // Afficher le QR code dans qrcodeLabel
    ui->qrcodeLabel->setPixmap(QPixmap::fromImage(im.scaled(125, 125, Qt::KeepAspectRatio, Qt::FastTransformation)));
}

void MainWindow::on_statsButton_clicked()
{
    QSqlQueryModel* model= materiel.afficher();
    MainWindow::genererStatistiquesMateriel(model);
}


void MainWindow::on_qrcodegen_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_ajouter_Materiel_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_ajouter_Materiel_2_clicked()
{
    QSqlQuery query=materiel.prendre(ui->id_materiel_2->text().toInt());
        qDebug()<<QDate::fromString(query.value(0).toString(),"dd/MM/yyyy").daysTo(QDate::currentDate());

    if(QDate::fromString(query.value(0).toString(),"dd/MM/yyyy").daysTo(QDate::currentDate())>query.value(1).toString().toInt())
    {
        QMessageBox::information(this, "Garantie", "La durée de garantie est terminée !");

        QString chr="ssss";
        ard.write_to_arduino(chr.toUtf8());
    } else {
           QMessageBox::information(this, "Garantie", "La durée de garantie est encore disponible !");
       }
   }
