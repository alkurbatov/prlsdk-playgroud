#include <iostream>

#include <pthread.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#include <libvzctl.h>

void* create_ct(void*) {
    struct vzctl_env_create_param param = {};

    struct vzctl_env_param *env = vzctl2_alloc_env_param();
    bzero(param.ctid, sizeof(ctid_t));
    param.layout = 5;
    param.ostmpl = const_cast<char*>("centos-7");

    int ret = vzctl2_env_create(env, &param, 0);
    if (ret != 0) {
        std::cerr << "Failed to create a CT: " << ret << std::endl;
        return NULL;
    }

    std::cout << "A CT was created?" << std::endl;

    return NULL;
}

void* create_ct_ex(void*) {
    system("vzctl create 6197b322-3d34-4306-92bf-105426a2f9a1 --config ubfbzdudaaaaa1uo5qwq5iotkatyxbaa --name TestX");

    std::cout << "A CT was created!" << std::endl;
    return NULL;
}

void* list_cts(void*) {
    int count = 0;

    while(count == 0) {
        usleep(100);

        struct vzctl_ids* ctids = vzctl2_alloc_env_ids();

        if (ctids == NULL) {
            std::cerr << "Failed to allocate env ids, no memory?" << std::endl;
            return NULL;
        }

        //count = vzctl2_get_env_ids_by_state(ctids, ENV_STATUS_EXISTS);
        count = vzctl2_get_env_ids_by_state(ctids, ENV_STATUS_RUNNING);
    }

    if (count < 0) {
        std::cerr << "Failed to get list of CTs: " << count << std::endl;
        return NULL;
    }

    std::cout << "CTs found: " << count << std::endl;
    //system("ls /etc/vz/conf/6197b322-3d34-4306-92bf-105426a2f9a1.conf");

    //int layout = vzctl2_env_layout_version("/vz/private/6197b322-3d34-4306-92bf-105426a2f9a1");
    //std::cout << "Layout version is " << layout << std::endl;

    return NULL;
}

int main() {
    pthread_t create_ct_thread, list_ct_thread;

    //pthread_create(&create_ct_thread, NULL, create_ct_ex, NULL);
    pthread_create(&list_ct_thread, NULL, list_cts, NULL);

    //pthread_join(create_ct_thread, NULL);
    pthread_join(list_ct_thread, NULL);

    return 0;
}

