#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPixmap>
#include <QFileDialog>

#include "main.h"
#include "notesmanager.h"

class MainWindow : public QMainWindow, private NOTES::NotesManager
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    Ajout* fenAjout;
    Relations* fenRel;
    Corbeille* fenCorbeille;
    NOTES::Note* noteOuverte;
    QString fichier;
    bool viderCorbeilleExit;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void ajouterNote(NOTES::Note *a);
    using NOTES::NotesManager::getNotes;
    using NOTES::NotesManager::getNotePtr;

    Corbeille* getfenCorbeille() {return fenCorbeille;}
    using NotesManager::getRelation;
    using NotesManager::viderCorbeille;
    void writeSettings();
    friend class Corbeille;
public slots:
    void closeEvent(QCloseEvent *event);
    void visualiserNote(QListWidgetItem * a);
    void restaurerNote(QListWidgetItem * a);
    void affEcheance(bool);
    void affPriorite(bool);
    void chercherFichier();
    void demandeConfirmation();
    void ouvrirDenis();
    void sauvegarder();
    void ajouterRel();
    void supprimer();
    void saveSlot(){
        NotesManager::save();
    }
};


class Ajout : public QDialog
{
    Q_OBJECT

public:
    explicit Ajout(QWidget* parent = 0);
    ~Ajout();

private:
    Ui::Ajout *ui;
    MainWindow* m;

public slots :
    void accepter();
};

class Relations : public QDialog
{
    Q_OBJECT

public:
    explicit Relations(QWidget *parent = 0);
    ~Relations();

private:
    Ui::Relations *ui;
    MainWindow* m;
    NOTES::Relation* relationCourante;
    void afficherCouples();

public slots :
    void editerRel(QListWidgetItem *);
    void ajouterCouple();
    void enleverCouple();
    void accepter();
};

class Corbeille : public QDialog
{
    Q_OBJECT

public:
    explicit Corbeille(QWidget *parent = 0);
    void ajouter(NOTES::Note*);
    ~Corbeille();

private:
    Ui::Corbeille *ui;
    MainWindow* m;

signals:
    void activerBtn(bool);

public slots:
    void demRest();
    void restaurer();
    void viderCorbeille();    
};



#endif // MAINWINDOW_H
