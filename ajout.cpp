
#include "ui_ajout.h"
#include "mainwindow.h"


Ajout::Ajout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ajout),
    m( (MainWindow*)parent)
{
    ui->setupUi(this);
    setWindowTitle("Nouvelle note");
    setWindowIcon(QIcon(QApplication::applicationDirPath()+"/ressources/edit-set-5-256.png"));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accepter()));


}

void Ajout::accepter(){
    // Quand on a appuyé sur "Ok"
  std::cout << "Connexion établie \n " ;

    try{
      QString classe = ui->editType->currentText();
      if(classe == "Article"){
             m->ajouterNote(new NOTES::Article(ui->editId->text(), ui->editTitre->text()));
      }else if(classe == "Media"){
             m->ajouterNote(new NOTES::Media(ui->editId->text(), ui->editTitre->text()));
      }else if(classe == "Tache"){
             m->ajouterNote(new NOTES::Tache(ui->editId->text(), ui->editTitre->text()));
      } else m->ajouterNote(new NOTES::Note(ui->editId->text(), ui->editTitre->text()));


        ui->editId->clear();
        ui->editTitre->clear();
    } catch(NOTES::NotesException &e){
       qDebug() << "Erreur ! ";
    }

}

Ajout::~Ajout()
{
    delete ui;
}
