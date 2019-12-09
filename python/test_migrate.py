import sys, time
import threading

import prlsdkapi
import prlsdkapi.prlsdk.consts as consts

class Migrate(threading.Thread):
    def __init__(self, job):
        threading.Thread.__init__(self)

        self.job = job

    def run(self):
        print "Migrating..."
        self.job.wait()

class Listen(threading.Thread):
    def __init__(self, job):
        threading.Thread.__init__(self)

        self.job = job

    def run(self):
        while self.job.get_status() != consts.PJS_FINISHED:
            print "%d%%" % self.job.get_progress()

            time.sleep(2)

def main(target):
    prlsdkapi.init_server_sdk()

    distant = prlsdkapi.Server()
    distant.login("10.30.25.115", "root", "1q2w3e").wait()

    local = prlsdkapi.Server()
    local.login("127.0.0.1", "root", "1q2w3e").wait()

    # PVMT_SWITCH_TEMPLATE : applable just in clone mode - lets to clone remotely VM to template
    # PVMT_CHANGE_SID      : for Windows VMs/containers lets to change SID after remote clone
    # PVMT_CLONE_MODE      : preserves source VM
    flags = consts.PVMT_SWITCH_TEMPLATE | consts.PVMT_CHANGE_SID | consts.PVMT_CLONE_MODE

    #for vm in local.get_vm_list_ex(consts.PVTF_VM).wait():
    for vm in distant.get_vm_list_ex(consts.PVTF_VM).wait():
        if vm.get_name() != target:
            continue

        #job = vm.migrate(distant, "", flags)
        job = vm.migrate(local, "", flags)

        mthread = Migrate(job)
        mthread.start()

        # lthread = Listen(job)
        # lthread.start()

        mthread.join()
        # lthread.join()

        print "Finished!"
        break

if __name__ == "__main__":
    sys.exit(main(sys.argv[1]))

