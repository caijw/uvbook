#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <uv.h>
#include <thread>
#include <iostream>

uv_loop_t *loop;
uv_async_t async;
int count = 0;

double percentage;

void fake_download(uv_work_t *req) {
    int size = *((int*) req->data);
    int downloaded = 0;
    while (1 || downloaded < size) {
        percentage = downloaded*100.0/size;
        async.data = (void*) &percentage;
        uv_async_send(&async);

        sleep(1);
        downloaded += (200+random())%1000; // can only download max 1000bytes/sec,
                                           // but at least a 200;
        count++;
        std::cout << "count: " << count << std::endl;
        if (count == 7) {
            uv_stop(loop);
            count = 0;
        }
    }
}

void after(uv_work_t *req, int status) {
    fprintf(stderr, "Download complete\n");
    uv_close((uv_handle_t*) &async, NULL);
}

void print_progress(uv_async_t *handle) {
    double percentage = *((double*) handle->data);
    fprintf(stderr, "Downloaded %.2f%%\n", percentage);
    std::cout << "uv_loop_alive: " << uv_loop_alive(loop) << std::endl;


}


void test() {
    int size = 10240;
    int downloaded = 0;
    printf("begin test\n");
    while (1 || downloaded < size) {
        percentage = downloaded*100.0/size;
        async.data = (void*) &percentage;
        uv_async_send(&async);
        downloaded += (200+random())%1000;
        sleep(1);
        count++;
        std::cout << "count: " << count << std::endl;
        if (count == 7) {
            uv_stop(loop);
            count = 0;
        }        
    }
    printf("end test\n");
}

int main() {
    printf("start main\n");
    // loop = uv_default_loop();
    loop = new uv_loop_t;
    uv_loop_init(loop);
    std::cout << "uv_loop_alive: " << uv_loop_alive(loop) << std::endl;
    uv_async_init(loop, &async, print_progress);
    std::cout << "uv_loop_alive: " << uv_loop_alive(loop) << std::endl;

    std::thread(test).detach();
    printf("start uv loop\n");
    std::cout << "uv_loop_alive: " << uv_loop_alive(loop) << std::endl;
    uv_run(loop, UV_RUN_DEFAULT);
    printf("end uv loop\n");
    std::cout << "uv_loop_alive: " << uv_loop_alive(loop) << std::endl;
    uv_run(loop, UV_RUN_DEFAULT);
    uv_loop_close(loop);
    free(loop);

    return 0;
}
