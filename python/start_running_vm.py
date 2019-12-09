import sys

import prlsdkapi
import prlsdkapi.prlsdk.consts as consts

prlsdkapi.init_server_sdk()

local = prlsdkapi.Server()
local.login("127.0.0.1", "root", "1q2w3e").wait()

target = None
for i in local.get_vm_list_ex(consts.PVTF_VM).wait():
    if i.get_name() != sys.argv[1]:
        continue

    target = i
    break

try:
    print target.start().wait()
    print target.start().wait()

except prlsdkapi.PrlSDKError, e:
    print e
    print prlsdkapi.Debug().prl_result_to_string(e.get_result())

