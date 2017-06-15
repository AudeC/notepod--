#include "mainwindow.h"
#include "ui_relations.h"

Relations::Relations(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Relations),
    m( (MainWindow*) parent ),
    relationCourante(nullptr)
{


     ui->setupUi(this);
     setWindowTitle("Nouvelle relation");
     setWindowIcon(QIcon(QApplication::applicationDirPath()+"/ressources/edit-set-5-256.png"));
     connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accepter()));


}


void Relations::accepter(){

    qDebug() << "acceptey";

    //relationCourante->setTitre(ui->editTitre->text());
    relationCourante->setDescription(ui->editDesc->text());
    relationCourante->setOrientation(ui->checkSens->isChecked());
}

Relations::~Relations()
{
    delete ui;
}

void Relations::ajouterCouple(){
   if(ui->comboCouples->currentText() != ui->comboCouples2->currentText()){
       try{
       relationCourante->ajouterCouple(m->getNotePtr(ui->comboCouples->currentText()), m->getNotePtr(ui->comboCouples2->currentText()), ui->editLabel->text());
        }catch(NOTES::NotesException e){
           qDebug() << "impossible";
       }

        afficherCouples();

    }
}

void Relations::enleverCouple(){
   relationCourante->enleverCouple(m->getNotePtr(ui->comboCouples->currentText()), m->getNotePtr(ui->comboCouples2->currentText()));
   qDebug() << "enleve";
   afficherCouples();
}

void Relations::afficherCouples(){
    ui->tabCouples->clear();
    for(NOTES::Relation::Couple c : relationCourante->getCouples()){
        QString str(c.getMb1().getId());
        str += " et ";
        str += c.getMb2().getId();
        str += " - ";
        str += c.getLabel();
        ui->tabCouples->addItem(str);

     }

}

void Relations::editerRel(QListWidgetItem * i)
{

    relationCourante = m->getRelation(i->text());

    ui->editTitre->setText(relationCourante->getTitre());
    ui->editTitre->setDisabled(true);
    ui->checkSens->setChecked(relationCourante->getOrientation());
    ui->editDesc->setText(relationCourante->getDescription());

    afficherCouples();

    connect(ui->btnAjouter, SIGNAL(clicked()), this, SLOT(ajouterCouple()));
    connect(ui->enlever, SIGNAL(clicked()), this, SLOT(enleverCouple()));

     ui->comboCouples->clear();
     ui->comboCouples2->clear();
       for(NOTES::Note* n : m->getNotes()){
           ui->comboCouples->addItem(n->getId());
           ui->comboCouples2->addItem(n->getId());

       }

    open();



}
