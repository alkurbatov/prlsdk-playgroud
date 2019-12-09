import sys, time

import prlsdkapi
import prlsdkapi.prlsdk.consts as consts

def main(target):
    prlsdkapi.init_server_sdk()

    local = prlsdkapi.Server()
#    local.login("127.0.0.1", "root", "1q2w3e").wait()
    job = local.login("127.0.0.1", "root", "1q2w3e")

    if job.get_status() == consts.PJS_FINISHED:
        print "Finished"

    job = local.create_vm_backup(target, force_operation = True, \
        reserved_flags = 0, nTargetPort = 0, strDescription = "", \
        sTargetHost = "localhost", backup_flags = 2048, sTargetSessionId = "")

    time.sleep(2)

#    while (job.get_status() == consts.PJS_RUNNING):
#        print "Running"

    if job.get_status() == consts.PJS_FINISHED:
        print "Finished"

# target -> VM uuid
if __name__ == "__main__":
    sys.exit(main(sys.argv[1]))

