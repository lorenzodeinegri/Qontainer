#include "listmodeladapter.h"

ListModelAdapter::ListModelAdapter(QObject * parent) :
    QAbstractListModel(parent),
    model(new Model()),
    nextInsert(DeepPointer<Material>()) {}

ListModelAdapter::~ListModelAdapter() {
    if (model != nullptr)
        delete model;
}

int ListModelAdapter::rowCount(const QModelIndex &) const {
    return static_cast<int>(model->count());
}

QVariant ListModelAdapter::data(const QModelIndex & adapterIndex, int role) const {
    if (adapterIndex.isValid() && adapterIndex.row() < rowCount()) {
        switch (role) {
        case Qt::DisplayRole:
            return QString::fromStdString((model->get(static_cast<unsigned int>(adapterIndex.row())))->getInfo());
        case Qt::DecorationRole:
            return QPixmap(QString::fromStdString((model->get(static_cast<unsigned int>(adapterIndex.row())))->getPhoto())).scaled(QSize(300, 300));
        case Qt::BackgroundRole:
            if (!(adapterIndex.row() % 2))
                return QBrush(QColor(225, 225, 225));
            else
                return QBrush(QColor(250, 250, 250));
        default:
            return QVariant();
        }
    }
    else
        return QVariant();
}

bool ListModelAdapter::insertRows(int begin, int count, const QModelIndex & parent) {
    beginInsertRows(parent, begin, begin + count - 1);
    model->add(nextInsert.operator->());
    endInsertRows();
    return true;
}

bool ListModelAdapter::removeRows(int begin, int count, const QModelIndex & parent) {
    beginRemoveRows(parent, begin, begin + count - 1);
    model->remove(static_cast<unsigned int>(begin));
    endRemoveRows();
    return true;
}

void ListModelAdapter::setNextInsert(Material * material) {
    nextInsert = material;
    if (material)
        delete material;
}

void ListModelAdapter::showDisplay(const QModelIndex & adapterIndex) const {
    if (adapterIndex.isValid() && adapterIndex.row() < rowCount()) {
        Details * details = new Details(model->get(static_cast<unsigned int>(adapterIndex.row())));
        details->setAttribute(Qt::WA_DeleteOnClose);
        details->show();
    }
}

void ListModelAdapter::showModify(const QModelIndex & adapterIndex) const {
    if (adapterIndex.isValid() && adapterIndex.row() < rowCount()) {
        Modifies * modifies = new Modifies(model->get(static_cast<unsigned int>(adapterIndex.row())), adapterIndex, adapterIndex);
        connect(modifies, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)));
        modifies->setAttribute(Qt::WA_DeleteOnClose);
        modifies->show();
    }
}

bool ListModelAdapter::findMatch(unsigned int index, const QRegExp & regularExpression, const QString & filter) const {
    if (filter == "Settore") {
        unsigned int sector = regularExpression.pattern().toUInt();
        if (sector)
            return model->get(index)->getSector() == sector;
        else
            return true;
    }
    else if (filter == "Autore")
        return QString::fromStdString(model->get(index)->getAuthor()).contains(regularExpression);
    else if (filter == "Titolo")
        return QString::fromStdString(model->get(index)->getTitle()).contains(regularExpression);
    else if (filter == "Luogo")
        return QString::fromStdString(model->get(index)->getPlace()).contains(regularExpression);
    else if (filter == "Materiale" && model->get(index)->getMaterialType() == "Opera d'arte")
        return QString::fromStdString(static_cast<ArtWork *>(model->get(index))->getMaterial()).contains(regularExpression);
    else if (filter == "Tecnica" && model->get(index)->getMaterialType() == "Opera d'arte")
        return QString::fromStdString(static_cast<ArtWork *>(model->get(index))->getTechnique()).contains(regularExpression);
    else if (filter == "Movimento Artistico" && model->get(index)->getMaterialType() == "Opera d'arte")
        return QString::fromStdString(static_cast<ArtWork *>(model->get(index))->getArtMovement()).contains(regularExpression);
    else if (filter == "Soggetto" && model->get(index)->getMaterialType() == "Opera d'arte")
        return QString::fromStdString(static_cast<ArtWork *>(model->get(index))->getSubject()).contains(regularExpression);
    else if (filter == "Lingua" && model->get(index)->getMaterialType() == "Opera letteraria")
        return QString::fromStdString(static_cast<LiteraryWork *>(model->get(index))->getLanguage()).contains(regularExpression);
    else if (filter == "Stile" && model->get(index)->getMaterialType() == "Opera letteraria")
        return QString::fromStdString(static_cast<LiteraryWork *>(model->get(index))->getStyle()).contains(regularExpression);
    else if (filter == "Forma" && model->get(index)->getType() == "Scultura")
        return QString::fromStdString(static_cast<Sculpture *>(model->get(index))->getShape()).contains(regularExpression);
    else if (filter == "Oggetto" && model->get(index)->getType() == "Atto")
        return QString::fromStdString(static_cast<Act *>(model->get(index))->getObject()).contains(regularExpression);
    else if (filter == "Destinatario" && model->get(index)->getType() == "Lettera")
        return QString::fromStdString(static_cast<Letter *>(model->get(index))->getAddressee()).contains(regularExpression);
    else
        return false;
}

bool ListModelAdapter::findDateMatch(unsigned int index, const date & begin, const date & end, const QString & filter) const {
    if (filter == "Data")
        return begin <= model->get(index)->getDate() && model->get(index)->getDate() <= end;
    else
        return false;
}

bool ListModelAdapter::findFloatMatch(unsigned int index, float begin, float end, const QString & filter) const {
    if (filter == "Valore Base")
        return begin <= model->get(index)->getBaseValue() && model->get(index)->getBaseValue() <= end;
    else if (filter == "Entrate")
        return begin <= model->get(index)->getIncome() && model->get(index)->getIncome() <= end;
    else if (filter == "Uscite")
        return begin <= model->get(index)->getExpense() && model->get(index)->getExpense() <= end;
    else
        return false;
}

bool ListModelAdapter::findIntegerMatch(unsigned int index, unsigned int begin, unsigned int end, const QString & filter) const {
    if (filter == "Restauri")
        return begin <= model->get(index)->getRestorations() && model->get(index)->getRestorations() <= end;
    else if (filter == "Prestiti")
        return begin <= model->get(index)->getLoans() && model->get(index)->getLoans() <= end;
    else
        return false;
}

bool ListModelAdapter::findBoolMatch(unsigned int index, bool value, const QString & filter) const {
    if (filter == "Proprieta")
        return model->get(index)->isProprietary() == value;
    else if (filter == "Stato")
        return model->get(index)->isDamaged() == value;
    else if (filter == "Disponibilita")
        return model->get(index)->isAvailable() == value;
    else if (filter == "Testo" && model->get(index)->getMaterialType() == "Opera letteraria")
        return static_cast<LiteraryWork *>(model->get(index))->isComplete() == value;
    else if (filter == "Scrittura" && model->get(index)->getMaterialType() == "Opera letteraria")
        return static_cast<LiteraryWork *>(model->get(index))->isHandwritten() == value;
    else if (filter == "Formato" && model->get(index)->getType() == "Dipinto")
        return static_cast<Picture *>(model->get(index))->isPhotography() == value;
    else
        return false;
}

bool ListModelAdapter::restore(const QModelIndex & adapterIndex) {
    if (adapterIndex.isValid() && adapterIndex.row() < rowCount()) {
        bool result = model->restore(static_cast<unsigned int>(adapterIndex.row()));
        if (result)
            emit dataChanged(adapterIndex, adapterIndex);
        return result;
    }
    else
        return false;
}

bool ListModelAdapter::lend(const QModelIndex & adapterIndex) {
    if (adapterIndex.isValid() && adapterIndex.row() < rowCount()) {
        bool result = model->lend(static_cast<unsigned int>(adapterIndex.row()));
        if (result)
            emit dataChanged(adapterIndex, adapterIndex);
        return result;
    }
    else
        return false;
}

float ListModelAdapter::calculateValue(const QModelIndex & adapterIndex) const {
    if (adapterIndex.isValid() && adapterIndex.row() < rowCount()) {
        return model->calculateValue(static_cast<unsigned int>(adapterIndex.row()));
    }
    else
        return 0.0f;
}

float ListModelAdapter::calculateProfit(const QModelIndex & adapterIndex) const {
    if (adapterIndex.isValid() && adapterIndex.row() < rowCount()) {
        return model->calculateProfit(static_cast<unsigned int>(adapterIndex.row()));
    }
    else
        return 0.0f;
}

float ListModelAdapter::calculateIncome(const QModelIndex & adapterIndex) const {
    if (adapterIndex.isValid() && adapterIndex.row() < rowCount()) {
        return (model->get(static_cast<unsigned int>(adapterIndex.row())))->getIncome();
    }
    else
        return 0.0f;
}

float ListModelAdapter::calculateExpense(const QModelIndex & adapterIndex) const {
    if (adapterIndex.isValid() && adapterIndex.row() < rowCount()) {
        return (model->get(static_cast<unsigned int>(adapterIndex.row())))->getExpense();
    }
    else
        return 0.0f;
}

float ListModelAdapter::calculateTotalValue() const {
    return model->calculateTotalValue();
}

float ListModelAdapter::calculateTotalProfit() const {
    return model->calculateTotalProfit();
}

float ListModelAdapter::calculateTotalIncome() const {
    return model->calculateTotalIncome();
}

float ListModelAdapter::calculateTotalExpense() const {
    return model->calculateTotalExpense();
}

void ListModelAdapter::loadFile(const std::string & filePath) {
    beginResetModel();
    model->loadFile(filePath);
    endResetModel();
}

void ListModelAdapter::saveFile(const std::string & filePath) const {
    model->saveFile(filePath);
}
