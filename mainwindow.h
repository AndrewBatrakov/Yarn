#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtSql>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void createPanel();
    void readSettings();
    void writeSettings();
    void createActions();
    void createMenu();
    void aboutProgramm();
    void splitterCreate();
    void createContextMenu();

    void viewTemplateTable(QString);
    void sortTable(int);
    void updatePanel(QModelIndex);

    void addRecordOfTable();
    void deleteRecordOfTable();
    void editRecordOfTable();
    void copyRecordOfTable();

    void viewColor();
    void viewMaterial();
    void viewStructure();
    void viewYarn();
    void viewUnit();
    void viewFirma();
    void viewJournal();

    void putBaseProcedure();
    void getBaseProcedure();
    void searchProcedure();
    void readItem();

private:
    QVBoxLayout *mainLayout;
    QListWidget *listWidget;
    QWidget *panel;
    QSplitter *splitter;

    QTableView *tableView;
    QLabel *tableLabel;
    QSqlRelationalTableModel *templateModel;

    QPushButton *addRecordButton;
    QPushButton *editRecordButton;
    QPushButton *deleteRecordButton;

    QAction *addAction;
    QAction *deleteAction;
    QAction *editAction;
    QAction *copyAction;

    QMenu *fileMenu;
    //QAction *prefixAction;
    QAction *exitAction;

    QMenu *referenceMenu;
    QAction *yarnAction;
    QAction *structureAction;
    QAction *materialAction;
    QAction *colorAction;
    QAction *productAction;
    QAction *unitAction;
    QAction *firmaAction;
    QAction *journalAction;

    //QMenu *documentMenu;

    //QMenu *reportMenu;

    QMenu *serviceMenu;
    //QAction *ademantAction;
    //QAction *ftpAction;
    QAction *searchAction;
    QAction *putAction;
    QAction *getAction;
    //QAction *vaccumAction;

    QMenu *helpMenu;
    QAction *aboutQtAction;
    QAction *aboutProgAction;

    //int addressInt, portInt;
    bool setFilter;
    QString filterTable;

protected:
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);
};

#endif // MAINWINDOW_H
