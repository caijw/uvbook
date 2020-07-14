#include <uv.h>
#include <iostream>

uv_loop_t *uv_loop_;
uv_async_t *async;

int main() {
    uv_loop_ = new uv_loop_t;
    async = new uv_async_t;
    uv_loop_init(uv_loop_);
    uv_async_init(uv_loop_, async, nullptr);
    // uv_close((uv_handle_t*) async, NULL);
    uv_run(uv_loop_, UV_RUN_DEFAULT);
}