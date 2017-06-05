#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
     fenAjout(new Ajout(this))
{
    ui->setupUi(this);

    // Menu - Ajout d'une note
    QAction *actionNouveau = new QAction("&Ajouter note", this);
    ui->menuAjout->addAction(actionNouveau);
    connect(actionNouveau, SIGNAL(triggered()), fenAjout, SLOT(open()));

    // Ajout des notes
    for(NOTES::Note i : getNotes())
    {
        ui->listeNotes->addItem(i.getId());
    }
    connect(ui->listeNotes, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(visualiserNote(QListWidgetItem*)));

    ui->visualisation->hide();
}

void MainWindow::ajouterNote(NOTES::Note *a){
    addNote(a);
    ui->listeNotes->addItem(a->getId());
}

void MainWindow::visualiserNote(QListWidgetItem * i){
    NOTES::Note& a = getNote(i->text());
    ui->editTitre->setText(a.getTitle());
    ui->editTexte->setText("Coucou !!!");
    ui->visualisation->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}
