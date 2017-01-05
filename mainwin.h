#ifndef MAINWIN_H
#define MAINWIN_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeView>
#include <QFileSystemModel>
#include <QGraphicsView>
#include <QLabel>

#include "svgview.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
signals:
    void dirSelected(QString d);

private slots:
    void onBtnBrowse();
    void onDirSelected(QString dir);
    void onCLickFileName(QModelIndex, QModelIndex);
    bool loadFile(const QString &fileName);

private:
    QLineEdit *m_dirName; // directory input box
    QPushButton *m_btnBrowse; // directory browse button

    QTreeView  *m_dirTree; // directory tree
    QFileSystemModel *m_fsModel; // file system model to access files/directories
    QLineEdit     *m_tagEdit; // tag name to search for
    QPushButton   *m_btnSearch; // search button

   // QGraphicsView *m_svgView; // SVG viewing area
    QLineEdit         *m_imgTags; // tags related to the image

    SvgView *m_svgView;
    QString m_currentPath;


};

#endif // MAINWIN_H
