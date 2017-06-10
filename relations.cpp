#include "mainwindow.h"
#include "ui_relations.h"

Relations::Relations(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Relations),
    m( (MainWindow*) parent )
{
    ui->setupUi(this);
}

Relations::~Relations()
{
    delete ui;
}

void Relations::editerRel(QListWidgetItem * i)
{

    NOTES::Relation r = m->getRelation(i->text());

    ui->editTitre->setText(r.getTitre());
    ui->checkSens->setChecked(r.getOrientation());
    ui->editDesc->setText(r.getDescription());
    ui->tabCouples->clearContents();
    for(NOTES::Relation::Couple c : r.getCouples()){
        ui->tabCouples->insertRow( ui->tabCouples->rowCount() );
         ui->tabCouples->setItem( ui->tabCouples->rowCount() , 0, new QTableWidgetItem(c.getMb1().getTitle()));
         ui->tabCouples->setItem( ui->tabCouples->rowCount() , 1, new QTableWidgetItem(c.getMb2().getTitle()));
         ui->tabCouples->setItem( ui->tabCouples->rowCount() , 2, new QTableWidgetItem(c.getLabel()));


     }

    open();



}
