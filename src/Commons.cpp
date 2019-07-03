#include "Commons.h"
#include <QFont>
#include <QString>
#include <QFontMetrics>

int FontMaxSize(const QFont &font,
                int shape_w, int shape_h,
                const QString &str) {
  QFont tf(font);
  QFontMetrics fm(font);
  int f, l, m;
  l = 4096; f = 0;

  while (f < l) {
    m = (f+l) >> 1;
    tf.setPointSize(m);
    fm = QFontMetrics(tf);

    if (fm.height() > shape_h ||
        fm.horizontalAdvance(str) > shape_w) {
      l = m;
    } else {
      f = m+1;
    }
  }
  return l;
}
///////////////////////////////////////////////////////
