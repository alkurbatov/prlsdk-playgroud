import sys, os
import random, time
import threading

import prlsdkapi
import prlsdkapi.prlsdk.consts as consts

max_timeout = 4

class Report(threading.Thread):
    def __init__(self, id, vm):
        threading.Thread.__init__(self)

        self.id = id
        self.vm = vm

    def run(self):
        for i in xrange(1, 10):
            print "#%d Reporting..." % self.id
            cmd = "prlctl problem-report %s --dump > test.tgz" % self.vm.get_config().get_name()
            os.system(cmd)
            time.sleep(5)

class Dump(threading.Thread):
    def __init__(self, id, vm):
        threading.Thread.__init__(self)

        self.id = id
        self.vm = vm

    def run(self):
        for i in xrange(1, 8):
            self.vm.start().wait()
            time.sleep(10)

            cmd = "prlctl internal %s dbgdump --name mem.dmp" % self.vm.get_config().get_uuid()
            os.system(cmd)

            self.vm.suspend().wait()
            timeout = random.randrange(0, max_timeout)
            print "#%d Wait after suspend: %d sec" % (self.id, timeout)
            time.sleep(timeout)

            self.vm.resume().wait()
            timeout = random.randrange(0, max_timeout)
            print "#%d Wait after resume: %d sec" % (self.id, timeout)
            time.sleep(timeout)

            self.vm.stop().wait()

class Configure(threading.Thread):
    def __init__(self, id, vm):
        threading.Thread.__init__(self)

        self.id = id
        self.vm = vm
        self.mem = (192, 256, 272, 368, 512, 768, 1024, 1512, 2048, 4096)

    def run(self):
        for i in xrange(1, 20):
            new_count = random.randrange(1, 8)
            print "#%d: New CPU count should be %d" % (self.id, new_count)

            new_size = self.mem[ random.randrange(0, len(self.mem) - 1) ]
            print "#%d: New RAM size should be %d" % (self.id, new_size)

            config = self.vm.get_config()
            self.vm.begin_edit().wait()

            config.set_ram_size(new_size)
            config.set_cpu_count(new_count)

            self.vm.commit().wait()

            self.vm.start().wait()
            time.sleep(5)

            self.vm.stop().wait()
            timeout = random.randrange(0, max_timeout)
            print "#%d Wait after stop: %d sec" % (self.id, timeout)
            time.sleep(timeout)

def main():
    prlsdkapi.init_server_sdk()

    local = prlsdkapi.Server()
    local.login("127.0.0.1", "root", "1q2w3e").wait()

    targets = []
    names = ["ssVM_1", "ssVM_2", "ssVM_3", "ssVM_4", "ssVM_5", "ssVM_6", "Ubuntu"]

    for vm in local.get_vm_list_ex(consts.PVTF_VM).wait():
        if not vm.get_name() in names:
            continue

        targets.append(vm)
        print "Got %s" % vm.get_name()

    if not targets or len(targets) != len(names):
        print "Failed to find all needed Vms!"
        return

    try:
        cthread1 = Configure(1, targets[0])
        cthread2 = Configure(2, targets[1])
        cthread3 = Report(3, targets[2])
        cthread4 = Configure(4, targets[3])
        cthread5 = Configure(5, targets[4])
        cthread6 = Report(6, targets[5])
        cthread7 = Dump(6, targets[6])

        cthread1.start()
        cthread2.start()
        cthread3.start()
        cthread4.start()
        cthread5.start()
        cthread6.start()
        cthread7.start()

        cthread1.join()
        cthread2.join()
        cthread3.join()
        cthread4.join()
        cthread5.join()
        cthread6.join()
        cthread7.join()

    except prlsdkapi.PrlSDKError, e:
        print "Exception caught! Error: %s" % e
        return

    print "Done!"

if __name__ == "__main__":
    sys.exit(main())

