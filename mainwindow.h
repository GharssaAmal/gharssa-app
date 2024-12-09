#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "materiel.h"
#include <QMainWindow>
#include "arduino.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Materiel materiel;
    Arduino ard;

private slots:
    void on_ajouter_Materiel_clicked();
    void on_supression_Materiel_clicked();
    void on_modifier_Materiel_clicked();
    void on_rechercher_Materiel_clicked();
    void on_trier_Materiel_clicked();
    void on_PDF_clicked();
    void genererStatistiquesMateriel(QSqlQueryModel* model);


    void on_mail_clicked();
    void on_qrcodegen_clicked();

    void on_statsButton_clicked();
    void on_qrcodegen_2_clicked();
    void on_ajouter_Materiel_3_clicked();
    void on_ajouter_Materiel_2_clicked();
};

#endif // MAINWINDOW_H
