
#include "ui_ajout.h"
#include "mainwindow.h"


Ajout::Ajout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ajout),
    m( (MainWindow*)parent)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accepter()));

}

void Ajout::accepter(){
    // Quand on a appuyé sur "Ok"
  std::cout << "Connexion établie \n " ;

    try{
    m->ajouterNote(new NOTES::Note(ui->editId->text(), ui->editTitre->text()));
    ui->editId->clear();
    ui->editTitre->clear();
    } catch(NOTES::NotesException &e){
       std::cout << "Erreur ! " << e.getInfo();
    }

}

Ajout::~Ajout()
{
    delete ui;
}
