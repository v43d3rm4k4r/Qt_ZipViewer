#pragma once

#include <QAbstractTableModel>

#include "EntriesManager.h"

class EntriesModel : public QAbstractTableModel
{
    Q_OBJECT

    Entries_t _data;

public:
    explicit EntriesModel(const Entries_t& data, QObject* parent = nullptr);
    virtual ~EntriesModel() = default;

    QVariant headerData(int32_t section,
                        Qt::Orientation orientation,
                        int32_t role) const override;

    int32_t rowCount(const QModelIndex& parent)    const override;
    int32_t columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index,
                  int32_t role = Qt::DisplayRole)  const override;
};
