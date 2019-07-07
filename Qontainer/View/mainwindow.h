#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "material.h"
#include "filehandler.h"
#include "listmodeladapter.h"
#include "sortfilterproxymodel.h"

#include "menu.h"
#include "insert.h"
#include "interact.h"
#include "list.h"
#include "searchcombobox.h"

#include <QWidget>

class MainWindow : public QWidget {
    Q_OBJECT

private:
    QString filePath;

    SearchComboBox * searchComboBox;
    List * list;

    ListModelAdapter * listModelAdapter;
    SortFilterProxyModel * filterProxyModel;

    Menu * menu;
    Insert * insert;
    Interact * interact;


    void load();

private slots:
    void loadFile();
    void saveFile();
    void saveFileName();
    void insertShow() const;
    void interactShow() const;
    void about();

    void insertMaterial(Material *) const;

    void searchRegularExpressions(const QString &) const;
    void searchDates(const QDate &, const QDate &) const;
    void searchFloats(float, float) const;
    void searchIntegers(unsigned int, unsigned int) const;
    void searchBools(bool) const;
    void details(const QModelIndexList &);
    void modifies(const QModelIndexList &);
    void restore(const QModelIndexList &);
    void lend(const QModelIndexList &);
    void calculateValue(const QModelIndexList &);
    void calculateProfit(const QModelIndexList &);
    void calculateIncome(const QModelIndexList &);
    void calculateExpense(const QModelIndexList &);
    void calculateTotalValue(const QModelIndexList &);
    void calculateTotalProfit(const QModelIndexList &);
    void calculateTotalIncome(const QModelIndexList &);
    void calculateTotalExpense(const QModelIndexList &);
    void deletes(const QModelIndexList &);

protected:
    virtual void closeEvent(QCloseEvent *) override;

public:
    explicit MainWindow(QWidget * = nullptr);
};

#endif // MAINWINDOW_H
