#include "EntriesModel.h"

EntriesModel::EntriesModel(const Entries_t& data, QObject* parent)
    : QAbstractTableModel{parent}, _data{data}
{
}
//=================================================================================================
QVariant EntriesModel::headerData(int32_t section,
                                  Qt::Orientation orientation,
                                  int32_t role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    switch (orientation) // :)
    {
    case Qt::Horizontal:
        if (section == 0)
        {
            return QString(tr("Имя файла"));
        }
        else if (section == 1)
        {
            return QString(tr("Размер"));
        }
        break;
    case Qt::Vertical:
        return section+1;
        break;

    }
    return QVariant();
}
//=================================================================================================
int32_t EntriesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return _data.size();
}
//=================================================================================================
int32_t EntriesModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return 2;
}
//=================================================================================================
QVariant EntriesModel::data(const QModelIndex &index, int32_t role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
    {
        //auto& row = _data[index.row()];
        //return QVariant::fromValue((index.column() == 0)
        //                           ? row.first : QString::number(row.second));

        if (index.column() == 0)
        {
            return _data[index.row()].first;
        }
        else if (index.column() == 1)
        {
            return _data[index.row()].second;
        }
        break;
    }
    case Qt::DecorationRole:
        break;
    }
    return QVariant();
}
//=================================================================================================
/*Qt::ItemFlags EntriesModel::flags(const QModelIndex& index) const
{
    return Qt::ItemFlags();
}*/
