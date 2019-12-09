import sys

import prlsdkapi
import prlsdkapi.prlsdk.consts as consts

prlsdkapi.init_server_sdk()

local = prlsdkapi.Server()
local.login("127.0.0.1", "root", "1q2w3eQAZ").wait()

# sys.argv[1] - VM name.
# sys.argv[2] - new hdd size.

target = None
for i in local.get_vm_list_ex(consts.PVTF_VM).wait():
    if i.get_name() != sys.argv[1]:
        continue

    target = i
    break

target.lock(0)
target.begin_edit()

cfg = target.get_config()
hdd0 = cfg.get_dev_by_type(consts.PDE_HARD_DISK, 0)

hdd0.resize_image(int(sys.argv[2]), consts.PACF_NON_INTERACTIVE_MODE).wait()

target.commit_ex(consts.PVCF_WAIT_FOR_APPLY).wait()
target.unlock(0)

