// Thank you to the folks at the C++ slack channel,
// along with @lewissbaker for the excellent literature
// (even though it took me a few days to be convinced
// it really was so).

#include <uv.h>

#include <iostream>
#include <coroutine>

#define DELAY 1000

int puv(int res, const char *message) {
	if (res < 0) {
		std::cerr << "error: " << message << ": " << uv_err_name(res) << ": " << uv_strerror(res) << std::endl;
		exit(1);
	}

	return res;
}

struct sleep {
	unsigned long _delay;
	explicit sleep(unsigned long delay) : _delay(delay) {}
};

class service {
public:
	struct awaiter;
	struct promise_type;
	using coro_handle = std::coroutine_handle<promise_type>;

	coro_handle _co;

	service(coro_handle co) : _co(co) {}
	service(const service &) = delete;
	service(service &&) = delete;

	awaiter operator co_await();
};

void on_sleep_done(uv_timer_t *timer) {
	auto co = std::coroutine_handle<>::from_address(timer->data);
	delete timer;
	co.resume();
}

struct final_awaitable {
	std::coroutine_handle<> _co;

	final_awaitable(std::coroutine_handle<> co) : _co(co) {}

	bool await_ready() noexcept { return false; }
	std::coroutine_handle<> await_suspend(std::coroutine_handle<>) noexcept {
		if (_co) {
			return _co;
		} else {
			return std::noop_coroutine();
		}
	}

	void await_resume() noexcept {}
};

struct service::promise_type {
	using coro_handle = std::coroutine_handle<promise_type>;

	uv_loop_t *_loop;
	std::coroutine_handle<> _continuation;

	promise_type(uv_loop_t *loop) : _loop(loop) {}

	auto get_return_object() { return coro_handle::from_promise(*this); }
	std::suspend_never initial_suspend() { return {}; }
	auto final_suspend() noexcept {
		return final_awaitable{ _continuation };
	}
	void return_void() {}

	template <typename T>
	auto && await_transform(T &&obj) const noexcept {
		return std::forward<T>(obj);
	}

	auto await_transform(sleep sleep_cmd) {
		uv_timer_t *timer = new uv_timer_t{};
		puv(uv_timer_init(_loop, timer), "failed to initialize timer");
		timer->data = coro_handle::from_promise(*this).address();
		puv(uv_timer_start(timer, &on_sleep_done, sleep_cmd._delay, 0), "failed to start timer");
		return std::suspend_always{};
	}

	void unhandled_exception() {
		std::terminate();
	}
};

struct service::awaiter {
	using coro_handle = std::coroutine_handle<service::promise_type>;

	coro_handle _co;

	awaiter(coro_handle co) : _co(co) {};
	bool await_ready() {
		return false;
	}

	auto await_suspend(std::coroutine_handle<> co_cont) {
		_co.promise()._continuation = co_cont;
		return true;
	}

	void await_resume() {}
};

service::awaiter service::operator co_await() {
	return { _co };
}

service third(uv_loop_t *) {
	std::cout << "4" << std::endl;
	co_await sleep(DELAY);
	std::cout << "5" << std::endl;
	co_await sleep(DELAY);
	std::cout << "6" << std::endl;
	co_await sleep(DELAY);
}

service second(uv_loop_t *loop) {
	std::cout << "2" << std::endl;
	co_await sleep(DELAY);
	std::cout << "3" << std::endl;
	co_await sleep(DELAY);
	for (int i = 0; i < 3; i++) {
		co_await third(loop);
	}
	std::cout << "7" << std::endl;
	co_await sleep(DELAY);
}

service service_main(uv_loop_t *loop) {
	std::cout << "1" << std::endl;
	co_await sleep(DELAY);
	co_await second(loop);
	std::cout << "8" << std::endl;
	co_await sleep(DELAY);
	std::cout << "9" << std::endl;
}

int main() {
	uv_loop_t loop;
	puv(uv_loop_init(&loop), "failed to initialize loop");

	service_main(&loop);
	int res = uv_run(&loop, UV_RUN_DEFAULT);

	std::cerr << "uv run complete; returned " << res << std::endl;

	return 0;
}
