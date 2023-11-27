#include <functional>

typedef std::function<void(int)> resolve_cb;
typedef std::function<void(resolve_cb)> exec_func;

class Promise {
  private:
    exec_func _func;

  public:
    Promise(exec_func func) {
        _func = func;
    }
    int await() {
        return 0;
    }
};
