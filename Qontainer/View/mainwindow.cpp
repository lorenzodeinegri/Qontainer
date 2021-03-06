#include "mainwindow.h"

void MainWindow::setDirectories() const {
    QString applicationDir = QCoreApplication::applicationDirPath();
    QDir photoDirectory(applicationDir + "/Photos");
    if (!photoDirectory.exists()) {
        photoDirectory.mkdir(applicationDir + "/Photos");
    }

    QDir dataDirectory(applicationDir + "/Data");
    if (!dataDirectory.exists()) {
        dataDirectory.mkdir(applicationDir + "/Data");
    }
}

void MainWindow::load() {
    QString path = QCoreApplication::applicationDirPath() + "/Data/data.xml";
    QFile file(path);
    if (file.exists()) {
        try {
            listModelAdapter->loadFile(path.toStdString());
            filePath = path;
            setWindowTitle("Museo - " + filePath);
        }
        catch (FileException exception) {
            QMessageBox::critical(this,
                                  "Caricamento file: " + file.fileName(),
                                  QString::fromStdString(exception.getException()),
                                  QMessageBox::Ok,
                                  QMessageBox::NoButton,
                                  QMessageBox::NoButton);
        }
    }
    else
        loadFile();
}

void MainWindow::loadFile() {
    QString file = QFileDialog::getOpenFileName(this, "Selezionare il file da caricare", QCoreApplication::applicationDirPath() + "/Data", "XML (*.xml) ;; Tutti i file (*)");
    if (!file.isEmpty()) {
        if (!file.contains(".xml"))
            file.append(".xml");
        try {
            listModelAdapter->loadFile(file.toStdString());
            filePath = file;
            setWindowTitle("Museo - " + filePath);
        }
        catch (FileException exception) {
            QMessageBox::critical(this,
                                  "Caricamento file: " + file,
                                  QString::fromStdString(exception.getException()),
                                  QMessageBox::Ok,
                                  QMessageBox::NoButton,
                                  QMessageBox::NoButton);
        }
    }
}

void MainWindow::saveFile() {
    if (!filePath.isEmpty()) {
        try {
            listModelAdapter->saveFile(filePath.toStdString());
            // Secondo salvataggio per aggiornare la sessione recuperata all'avvio successivo
            listModelAdapter->saveFile((QCoreApplication::applicationDirPath() + "/Data/data.xml").toStdString());
        } catch (FileException exception) {
            QMessageBox::critical(this,
                                  "Salvataggio file: " + filePath,
                                  QString::fromStdString(exception.getException()),
                                  QMessageBox::Ok,
                                  QMessageBox::NoButton,
                                  QMessageBox::NoButton);
        }
    }
    else
        saveFileName();
}

void MainWindow::saveFileName() {
    QString file = QFileDialog::getSaveFileName(this, "Selezionare il file in cui salvare", QCoreApplication::applicationDirPath() + "/Data", "XML (*.xml) ;; Tutti i file (*)");
    if (!file.isEmpty()) {
        if (!file.contains(".xml"))
            file.append(".xml");
        try {
            listModelAdapter->saveFile(file.toStdString());
            filePath = file;
            setWindowTitle("Museo - " + filePath);
        }
        catch (FileException exception) {
            QMessageBox::critical(this,
                                  "Salvataggio file: " + file,
                                  QString::fromStdString(exception.getException()),
                                  QMessageBox::Ok,
                                  QMessageBox::NoButton,
                                  QMessageBox::NoButton);
        }
    }
}

void MainWindow::insertShow() const {
    insert->setVisible(true);
    interact->setVisible(false);
}

void MainWindow::interactShow() const {
    insert->setVisible(false);
    interact->setVisible(true);
}

void MainWindow::about() {
    QMessageBox::about(this,
                       "Informazioni sull'applicazione",
                       "Progetto per il corso di programmazione ad oggetti\nLorenzo Dei Negri - 1161729");
}

void MainWindow::insertMaterial(Material * material) {
    listModelAdapter->setNextInsert(material);
    filterProxyModel->insertRows(filterProxyModel->rowCount(), 1);
    list->clearSelection();
    list->selectionModel()->clearCurrentIndex();
    list->selectionModel()->select(filterProxyModel->index(listModelAdapter->rowCount() - 1, 0), QItemSelectionModel::Select);
    QMessageBox::information(this,
                             "Inserisci materiale",
                             "Materiale inserito!",
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
}

void MainWindow::searchRegularExpressions(const QString & expression) const {
    QRegExp regularExpression(expression, Qt::CaseInsensitive, QRegExp::Wildcard);
    filterProxyModel->setFilterRegExp(regularExpression);
}

void MainWindow::searchDates(const QDate & first, const QDate & last) const {
    filterProxyModel->setDateValues(first.toString("dd/MM/yyyy").toStdString(), last.toString("dd/MM/yyyy").toStdString());
}

void MainWindow::searchFloats(float first, float last) const {
    filterProxyModel->setFloatValues(first, last);
}

void MainWindow::searchIntegers(unsigned int first, unsigned int last) const {
    filterProxyModel->setIntegerValues(first, last);
}

void MainWindow::searchBools(bool value) const {
    filterProxyModel->setBoolValue(value);
}

void MainWindow::details(const QModelIndexList & modelIndexList) {
    if (!modelIndexList.isEmpty()) {
        filterProxyModel->showDisplay(modelIndexList.first());
    }
    else
        QMessageBox::warning(this,
                             "Dettagli materiale",
                             "Nessun materiale selezionato!",
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
}

void MainWindow::modifies(const QModelIndexList & modelIndexList) {
    if (!modelIndexList.isEmpty()) {
        filterProxyModel->showModify(modelIndexList.first());
    }
    else
        QMessageBox::warning(this,
                             "Modifica materiale",
                             "Nessun materiale selezionato!",
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
}

void MainWindow::restore(const QModelIndexList & modelIndexList) {
    if (!modelIndexList.isEmpty()) {
        if (filterProxyModel->restore(modelIndexList.first()))
            QMessageBox::information(this,
                                     "Restaura materiale",
                                     "Materiale in restauro!",
                                     QMessageBox::Ok,
                                     QMessageBox::NoButton,
                                     QMessageBox::NoButton);
        else
            QMessageBox::warning(this,
                                 "Restaura materiale",
                                 "Materiale non in restauro!\nIl materiale selezionato non risulta disponibile!",
                                 QMessageBox::Ok,
                                 QMessageBox::NoButton,
                                 QMessageBox::NoButton);
    }
    else
        QMessageBox::warning(this,
                             "Restaura materiale",
                             "Nessun materiale selezionato!",
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
}

void MainWindow::lend(const QModelIndexList & modelIndexList) {
    if (!modelIndexList.isEmpty()) {
        if (filterProxyModel->lend(modelIndexList.first()))
            QMessageBox::information(this,
                                     "Presta materiale",
                                     "Materiale in prestito!",
                                     QMessageBox::Ok,
                                     QMessageBox::NoButton,
                                     QMessageBox::NoButton);
        else
            QMessageBox::warning(this,
                                 "Presta materiale",
                                 "Materiale non in prestito!\nIl materiale selezionato non risulta disponibile!",
                                 QMessageBox::Ok,
                                 QMessageBox::NoButton,
                                 QMessageBox::NoButton);
    }
    else
        QMessageBox::warning(this,
                             "Presta materiale",
                             "Nessun materiale selezionato!",
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
}

void MainWindow::calculateValue(const QModelIndexList & modelIndexList) {
    if (!modelIndexList.isEmpty()) {
        QString value = QString().setNum(filterProxyModel->calculateValue(modelIndexList.first()), 'f', 2);
        QMessageBox::information(this,
                                 "Calcola valore",
                                 "Valore: " + value,
                                 QMessageBox::Ok,
                                 QMessageBox::NoButton,
                                 QMessageBox::NoButton);
    }
    else
        QMessageBox::warning(this,
                             "Calcola valore",
                             "Nessun elemento selezionato!",
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
}

void MainWindow::calculateProfit(const QModelIndexList & modelIndexList) {
    if (!modelIndexList.isEmpty()) {
        QString value = QString().setNum(filterProxyModel->calculateProfit(modelIndexList.first()), 'f', 2);
        QMessageBox::information(this,
                                 "Calcola profitto",
                                 "Profitto: " + value,
                                 QMessageBox::Ok,
                                 QMessageBox::NoButton,
                                 QMessageBox::NoButton);
    }
    else
        QMessageBox::warning(this,
                             "Calcola profitto",
                             "Nessun elemento selezionato!",
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
}

void MainWindow::calculateIncome(const QModelIndexList & modelIndexList) {
    if (!modelIndexList.isEmpty()) {
        QString value = QString().setNum(filterProxyModel->calculateIncome(modelIndexList.first()), 'f', 2);
        QMessageBox::information(this,
                                 "Calcola entrate",
                                 "Entrate: " + value,
                                 QMessageBox::Ok,
                                 QMessageBox::NoButton,
                                 QMessageBox::NoButton);
    }
    else
        QMessageBox::warning(this,
                             "Calcola entrate",
                             "Nessun elemento selezionato!",
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
}

void MainWindow::calculateExpense(const QModelIndexList & modelIndexList) {
    if (!modelIndexList.isEmpty()) {
        QString value = QString().setNum(filterProxyModel->calculateExpense(modelIndexList.first()), 'f', 2);
        QMessageBox::information(this,
                                 "Calcola uscite",
                                 "Uscite: " + value,
                                 QMessageBox::Ok,
                                 QMessageBox::NoButton,
                                 QMessageBox::NoButton);
    }
    else
        QMessageBox::warning(this,
                             "Calcola uscite",
                             "Nessun elemento selezionato!",
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
}

void MainWindow::calculateTotalValue() {
    QString value = QString().setNum(filterProxyModel->calculateTotalValue(), 'f', 2);
    QMessageBox::information(this,
                             "Valore totale",
                             "Valore: " + value,
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
}

void MainWindow::calculateTotalProfit() {
    QString value = QString().setNum(filterProxyModel->calculateTotalProfit(), 'f', 2);
    QMessageBox::information(this,
                             "Profitto totale",
                             "Profitto: " + value,
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
}

void MainWindow::calculateTotalIncome() {
    QString value = QString().setNum(filterProxyModel->calculateTotalIncome(), 'f', 2);
    QMessageBox::information(this,
                             "Entrate totali",
                             "Entrate: " + value,
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
}

void MainWindow::calculateTotalExpense() {
    QString value = QString().setNum(filterProxyModel->calculateTotalExpense(), 'f', 2);
    QMessageBox::information(this,
                             "Uscite totali",
                             "Uscite: " + value,
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
}

void MainWindow::deletes(const QModelIndexList & modelIndexList) {
    if (!modelIndexList.isEmpty()) {
        filterProxyModel->removeRows(modelIndexList.first().row(), 1);
    }
    else
        QMessageBox::warning(this,
                             "Rimuovi materiale",
                             "Nessun elemento selezionato!",
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
}

void MainWindow::closeEvent(QCloseEvent * event) {
    int choice = QMessageBox::question(this,
                                       "Chiusura applicazione",
                                       "Salvare i dati prima di chiudere l'applicazione?",
                                       QMessageBox::Yes,
                                       QMessageBox::No,
                                       QMessageBox::Cancel);

    if (choice == QMessageBox::Yes) {
        saveFile();
        event->accept();
        QCoreApplication::quit();
    }
    else if (choice == QMessageBox::No) {
        event->accept();
        QCoreApplication::quit();
    }
    else
        event->ignore();
}

MainWindow::MainWindow(QWidget * parent) :
    QWidget(parent),
    filePath(QCoreApplication::applicationDirPath() + "/Data/data.xml"),
    searchComboBox(new SearchComboBox(this)),
    list(new List(this)),
    listModelAdapter(new ListModelAdapter(this)),
    filterProxyModel(new SortFilterProxyModel(searchComboBox, this)),
    menu(new Menu(this)),
    insert(new Insert(this)),
    interact(new Interact(list, searchComboBox, this))
{
    setWindowIcon(QIcon(":/Icons/icon.png"));
    setWindowTitle("Museo");

    setDirectories();
    load();

    filterProxyModel->setSourceModel(listModelAdapter);
    list->setModel(filterProxyModel);

    QHBoxLayout * mainLayout = new QHBoxLayout(this);
    mainLayout->setMenuBar(menu);
    mainLayout->addWidget(insert);
    mainLayout->addWidget(interact);

    insertShow();

    setLayout(mainLayout);

    connect(menu, SIGNAL(load()), this, SLOT(loadFile()));
    connect(menu, SIGNAL(save()), this, SLOT(saveFile()));
    connect(menu, SIGNAL(saveName()), this, SLOT(saveFileName()));
    connect(menu, SIGNAL(exit()), this, SLOT(close()));
    connect(menu, SIGNAL(insert()), this, SLOT(insertShow()));
    connect(menu, SIGNAL(interact()), this, SLOT(interactShow()));
    connect(menu, SIGNAL(about()), this, SLOT(about()));

    connect(insert, SIGNAL(insertMaterial(Material *)), this, SLOT(insertMaterial(Material *)));

    connect(interact, SIGNAL(searchRegularExpressions(const QString &)), this, SLOT(searchRegularExpressions(const QString &)));
    connect(interact, SIGNAL(searchDates(const QDate &, const QDate &)), this, SLOT(searchDates(const QDate &, const QDate &)));
    connect(interact, SIGNAL(searchFloats(float, float)), this, SLOT(searchFloats(float, float)));
    connect(interact, SIGNAL(searchIntegers(unsigned int, unsigned int)), this, SLOT(searchIntegers(unsigned int, unsigned int)));
    connect(interact, SIGNAL(searchBools(bool)), this, SLOT(searchBools(bool)));
    connect(interact, SIGNAL(details(const QModelIndexList &)), this, SLOT(details(const QModelIndexList &)));
    connect(interact, SIGNAL(modifies(const QModelIndexList &)), this, SLOT(modifies(const QModelIndexList &)));
    connect(interact, SIGNAL(restore(const QModelIndexList &)), this, SLOT(restore(const QModelIndexList &)));
    connect(interact, SIGNAL(lend(const QModelIndexList &)), this, SLOT(lend(const QModelIndexList &)));
    connect(interact, SIGNAL(calculateValue(const QModelIndexList &)), this, SLOT(calculateValue(const QModelIndexList &)));
    connect(interact, SIGNAL(calculateProfit(const QModelIndexList &)), this, SLOT(calculateProfit(const QModelIndexList &)));
    connect(interact, SIGNAL(calculateIncome(const QModelIndexList &)), this, SLOT(calculateIncome(const QModelIndexList &)));
    connect(interact, SIGNAL(calculateExpense(const QModelIndexList &)), this, SLOT(calculateExpense(const QModelIndexList &)));
    connect(interact, SIGNAL(calculateTotalValue()), this, SLOT(calculateTotalValue()));
    connect(interact, SIGNAL(calculateTotalProfit()), this, SLOT(calculateTotalProfit()));
    connect(interact, SIGNAL(calculateTotalIncome()), this, SLOT(calculateTotalIncome()));
    connect(interact, SIGNAL(calculateTotalExpense()), this, SLOT(calculateTotalExpense()));
    connect(interact, SIGNAL(deletes(const QModelIndexList &)), this, SLOT(deletes(const QModelIndexList &)));
}

QSize MainWindow::sizeHint() const
{
    return QSize(1200, 600);
}
