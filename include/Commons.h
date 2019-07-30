#ifndef COMMONS_H
#define COMMONS_H

#include <mutex>
#include <functional>
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

struct Defer {
  std::function<void()> finalize;
  Defer() = delete;
  Defer(std::function<void()> &&fin) : finalize(fin) {}
  ~Defer() { finalize(); }
};

#endif // COMMONS_H
