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

    QAction *actionSave = new QAction("&Sauvegarder", this);
    ui->menuSauvegarder->addAction(actionSave);
    connect(actionSave, SIGNAL(triggered()), this, SLOT(saveSlot()));

    load();

    // Test
    //addNote(new NOTES::Article("wujah2", "Je suis un test", "Mon beau texte"));
    //addNote(new NOTES::Media("woojoh", "Mediapart", son, "Notepod c'est trop bien"));
   //addNote(new NOTES::Tache("hojjy", "BosserLO21", "Au boulot", 1));


    // Ajout des notes
    for(NOTES::Note* i : getNotes())
    {
        ui->listeNotes->addItem(i->getId());
    }
    connect(ui->listeNotes, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(visualiserNote(QListWidgetItem*)));

    ui->visualisation->hide();
}

void MainWindow::ajouterNote(NOTES::Note *a){
    addNote(a);
    ui->listeNotes->addItem(a->getId());
}

void MainWindow::visualiserNote(QListWidgetItem * i){

    // Cacher les parties spéciales
    ui->visuAction->hide();
    ui->visuCont->hide();
    ui->visuPrio->hide();
    ui->editAction->hide();
    ui->editTexte->hide();
    ui->editPrio->hide();

    NOTES::Note& a = getNote(i->text());
  a.visualiser(ui);


    ui->visualisation->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}
