import sys, os
import time

import prlsdkapi
import prlsdkapi.prlsdk.consts as consts

def main(name):
    prlsdkapi.init_server_sdk()

    local = prlsdkapi.Server()
    local.login("127.0.0.1", "root", "1q2w3e").wait()

    target = None

    for vm in local.get_vm_list_ex(consts.PVTF_VM).wait():
        if vm.get_name() != name:
            continue

        target = vm
        break

    if not target:
        print "Failed to find Vm named %s" % name
        return

    try:
        #config = target.get_config()

        target.start().wait()
        time.sleep(10)

        #cmd = "prlctl internal {%s} dbgdump --name mem.dmp" % config.get_uuid()
        print "Dump"
        cmd = "prlctl internal {7cdda423-0868-4d1e-a81b-1790efce5508} dbgdump --name mem.dmp"
        os.system(cmd)

        print "Suspend"
        target.suspend().wait()
        time.sleep(10)

        print "Resume"
        target.resume().wait()
        time.sleep(10)

        target.stop().wait()

    except prlsdkapi.PrlSDKError, e:
        print "Error: %s" % e
        return

if __name__ == "__main__":
    sys.exit(main(sys.argv[1]))

