#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>

#include "main.h"
#include "notesmanager.h"

class MainWindow : public QMainWindow, private NOTES::NotesManager
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    Ajout* fenAjout;
    NOTES::Note* noteOuverte;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void ajouterNote(NOTES::Note *a);
    void visualiserUI(NOTES::Note& n) const { n.visualiser(ui); }

public slots:
    void visualiserNote(QListWidgetItem * a);
    void restaurerNote(QListWidgetItem * a);
    void sauvegarder();
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


#endif // MAINWINDOW_H
