#ifndef COMMONS_H
#define COMMONS_H

#include <mutex>
#define UNUSED(x) ((void)x)

class QFont;
class QString;

int FontMaxSize(const QFont &font,
                int shape_w, int shape_h,
                const QString &str);

struct MutexLocker {
  std::mutex &mut;
  MutexLocker() = delete;

  explicit MutexLocker(std::mutex &m) : mut(m) {
    mut.lock();
  }
  ~MutexLocker() { mut.unlock();}
};

#endif // COMMONS_H
