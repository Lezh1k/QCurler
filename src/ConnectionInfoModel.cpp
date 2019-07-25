#include <vector>
#include <QColor>

#include "ConnectionInfoModel.h"
#include "CurlWorker.h"
#include "Commons.h"

static std::vector<QString> lst_headers = {
  "URL", "Time", "Speed"
};

enum columns {
  col_url = 0,
  col_time,
  col_speed
};

ConnectionInfoModel::ConnectionInfoModel(QObject *parent) :
  QAbstractTableModel(parent) {
}

ConnectionInfoModel::~ConnectionInfoModel() {

}
///////////////////////////////////////////////////////////

int ConnectionInfoModel::rowCount(const QModelIndex &parent) const {
  UNUSED(parent);
  return static_cast<int>(m_lstIRI.size());
}
///////////////////////////////////////////////////////////

int ConnectionInfoModel::columnCount(const QModelIndex &parent) const {
  UNUSED(parent);
  return static_cast<int>(lst_headers.size());
}
///////////////////////////////////////////////////////////

QVariant ConnectionInfoModel::data(const QModelIndex &index, int role) const {
  size_t ixrow = static_cast<size_t>(index.row());

  if (role == Qt::DecorationRole) {
    if (index.column() != col_url)
      return QVariant();
    QPixmap pm = QPixmap(m_lstIRI[ixrow].ir.img_path);
    if (pm.isNull())
      return QVariant();
    return pm.scaled(m_imageSize, m_imageSize, Qt::KeepAspectRatio);
  }

  if (role == Qt::DisplayRole) {
    if (index.column() == col_url)
      return m_lstIRI[ixrow].ir.name;
    if (index.column() == col_time)
      return m_lstIRI[ixrow].time_total;
    if (index.column() == col_speed)
      return m_lstIRI[ixrow].speed_info();
    return QVariant();
  }

  if (role == Qt::FontRole) {
    QFont f = m_font;
    int32_t sw = m_colWidth;
    int32_t sh = m_rowHeight;
    QString txt;
    if (index.column() == col_url) {
      txt = m_lstIRI[ixrow].ir.name;
      sw -= m_imageSize;
    } else if (index.column() == col_time) {
      txt = QString("%1").arg(m_lstIRI[ixrow].time_total);
    } else {
      txt = m_lstIRI[ixrow].speed_info();
    }
    int mfs = FontMaxSize(m_font, sw, sh, QString("%1W").arg(txt));
    if (mfs == 0)
      mfs = 1;
    f.setPointSize(mfs);
    return f;
  }

  if (role == Qt::BackgroundColorRole) {    
    if (index.column() == col_url)
      return QVariant();
    if (!m_lstIRI[ixrow].success)
      return QColor(0xff, 0, 0);

    double coeff = 255000.0 / m_lstIRI[ixrow].ir.timeout_ms;
    int r = static_cast<int>(coeff * m_lstIRI[ixrow].time_total);
    if (r > 0xff) r = 0xff;
    int g = 0xff - r;
    return QColor(r, g, 0);
  }

  if (role == Qt::TextAlignmentRole) {
    return Qt::AlignCenter;
  }

  return QVariant();
}

QVariant ConnectionInfoModel::headerData(int section,
                                         Qt::Orientation orientation,
                                         int role) const {
  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      return lst_headers[static_cast<size_t>(section)];
    }
    if (orientation == Qt::Vertical) {
      return section+1;
    }
  }
  return QVariant();
}
///////////////////////////////////////////////////////

void ConnectionInfoModel::setImageSize(int32_t rh) {
  m_imageSize = rh;
}
///////////////////////////////////////////////////////

void ConnectionInfoModel::setRowHeight(int32_t rh) {
  m_rowHeight = rh;
}
///////////////////////////////////////////////////////

void ConnectionInfoModel::setColWidth(int32_t cw) {
  m_colWidth = cw;
}
///////////////////////////////////////////////////////

void ConnectionInfoModel::resourceListUpdated(size_t size) {
  m_lstIRI = std::vector<InternetResourceInfo>(size);
}
///////////////////////////////////////////////////////

void ConnectionInfoModel::setFont(const QFont &font) {
  m_font = font;
}
///////////////////////////////////////////////////////

void ConnectionInfoModel::infoReceived(InternetResourceInfo info) {
  if (info.ir.ix >= m_lstIRI.size())
    return;
  m_lstIRI[info.ir.ix] = info;
  this->endResetModel();
}
///////////////////////////////////////////////////////////
