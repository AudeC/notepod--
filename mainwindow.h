#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include "notesmanager.h"

namespace Ui {
class MainWindow;
class Ajout;
}

class MainWindow;
class Ajout;

class MainWindow : public QMainWindow, private NOTES::NotesManager
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    Ajout* fenAjout;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void ajouterNote(NOTES::Note *a);


public slots:
    void visualiserNote(QListWidgetItem * a);

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
