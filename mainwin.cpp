#include "mainwin.h"
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

Widget::Widget(QWidget *parent)
    : QWidget(parent), m_svgView(new SvgView)
{
    //directory name box and button
    m_dirName = new QLineEdit(this);
    m_btnBrowse = new QPushButton("Browse...",this);
    QHBoxLayout *layout1 = new QHBoxLayout();
    layout1->addWidget(m_dirName);
    layout1->addWidget(m_btnBrowse);

    //search box and button
    m_tagEdit = new QLineEdit(this);
    m_btnSearch = new QPushButton("Search", this);
    QHBoxLayout *layout2 = new QHBoxLayout();
    layout2->addWidget(m_tagEdit);
    layout2->addWidget(m_btnSearch);

    //file/folder tree + search area
    m_dirTree = new QTreeView(this);
    QVBoxLayout *layout3 = new QVBoxLayout();

    layout3->addWidget(m_dirTree);
    layout3->addLayout(layout2);

    //svg view box and tag display area
    //m_svgView = new QGraphicsView(this);
    m_imgTags = new QLineEdit(this);
    m_imgTags->setText("all tags here");
    QVBoxLayout *layout4 = new QVBoxLayout();
    layout4->addWidget(m_svgView);
    layout4->addWidget(m_imgTags);

    //left/rigth
    QHBoxLayout *layout5 = new QHBoxLayout();
    layout5->addLayout(layout3);
    layout5->addLayout(layout4);


    //top/bottom
    QVBoxLayout *layoutMain = new QVBoxLayout();
    layoutMain->addLayout(layout1);
    layoutMain->addLayout(layout5);


    setLayout(layoutMain);
    setWindowTitle("SVG Library Manager");

    //signal/slot connections
    connect(m_btnBrowse, SIGNAL(clicked()), this, SLOT(onBtnBrowse()));
    connect(this, SIGNAL(dirSelected(QString)), this, SLOT(onDirSelected(QString)));
    //connect(m_dirTree, SIGNAL(clicked(QModelIndex)), this, SLOT(onCLickFileName(QModelIndex)));
    //connect(m_dirTree, SIGNAL(pressed(QModelIndex)), this, SLOT(onCLickFileName(QModelIndex)));

    //QItemSelectionModel* selectionModel = m_dirTree->selectionModel();
    //connect(selectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onCLickFileName(QModelIndex, QModelIndex)) );


    m_fsModel = 0;
    onDirSelected(tr("/Users/Alamgir/StockImages"));
}

Widget::~Widget()
{

}

//Browse button handler
void Widget::onBtnBrowse()
{
    QFileDialog folderDlg;
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose repo directory"),
                                                    QString("/"),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

   if(!dir.isEmpty()) { // if a directory is choosen
       emit  dirSelected(dir);
   }

}

// a new directory is provided
void Widget::onDirSelected(QString dir)
{
    if(m_fsModel)
        delete m_fsModel;

    m_dirName->setText(dir);

    m_fsModel = new QFileSystemModel(this);
    m_fsModel->setRootPath(dir);
    m_fsModel->setNameFilters(QStringList()<<"*.svg");
    m_fsModel->setNameFilterDisables(false);


    //m_fsModel->removeColumn(3);
    //m_fsModel->removeColumn(2);


    m_dirTree->setModel(m_fsModel);
    m_dirTree->setRootIndex(m_fsModel->index(dir));
    m_dirTree->setSelectionMode(QAbstractItemView::SingleSelection);
    for (int i = 1; i < m_fsModel->columnCount(); ++i)
        m_dirTree->hideColumn(i);

    QItemSelectionModel* selectionModel = m_dirTree->selectionModel();
    connect(selectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onCLickFileName(QModelIndex, QModelIndex)) );

}

//when a file is selected by clicking
void Widget::onCLickFileName(QModelIndex m, QModelIndex old)
{
    // TreeView clicked
    // 1. We need to extract path
    // 2. Set that path into our ListView

   // Get the full path of the item that's user clicked on
   QString modelPath = m_fsModel->fileInfo(m).absoluteFilePath();

   //show as tag for now
   m_imgTags->setText(modelPath);
   loadFile(modelPath);

   Q_UNUSED(old)
}

bool Widget::loadFile(const QString &fileName)
{
    if (!QFileInfo::exists(fileName) || !m_svgView->openFile(fileName)) {
       // QMessageBox::critical(this, tr("Open SVG File"),
       //                       tr("Could not open file '%1'.").arg(QDir::toNativeSeparators(fileName)));
        return false;
    }

/*
    if (!fileName.startsWith(":/")) {
        m_currentPath = fileName;
        setWindowFilePath(fileName);
        const QSize size = m_svgView->svgSize();
        const QString message =
            tr("Opened %1, %2x%3").arg(QFileInfo(fileName).fileName()).arg(size.width()).arg(size.width());
        //statusBar()->showMessage(message);
        QMessageBox::information(this, "Info", message);
    }

    m_outlineAction->setEnabled(true);
    m_backgroundAction->setEnabled(true);
    */

    //const QSize availableSize = QApplication::desktop()->availableGeometry(this).size();
   // const QSize availSize = m_svgView->size();
    //resize(m_svgView->sizeHint().expandedTo(availSize / 4) + QSize(80, 80 + 20));

    return true;
}
