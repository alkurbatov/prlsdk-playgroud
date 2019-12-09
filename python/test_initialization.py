import time

import prlsdkapi
import prlsdkapi.prlsdk.consts as consts

prlsdkapi.init_server_sdk()
srv = prlsdkapi.Server()

job = srv.login("127.0.0.1", "root", "1q2w3e")

if job.get_status() == consts.PJS_RUNNING:
    print "Running"

time.sleep(2)

if job.get_status() == consts.PJS_FINISHED:
    print "Finished"

