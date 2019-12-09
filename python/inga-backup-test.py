#!/usr/bin/python

import prlsdkapi
from prlsdkapi import consts
import sys
from urlparse import urlparse
from time import sleep

UUID_LEN = 36

code_type = {
    consts.PVT_VM: "VM",
    consts.PVT_CT: "CT",
    "UNKNOWN": "UN"
}

def init():
    try:
        prlsdkapi.init_server_sdk()
        server = prlsdkapi.Server()
        server.login_local().wait()
        return server
    except RuntimeError, e:
        print(str(e))
        exit(-1)

def callback_func(hEvent, vm_uuid):
    #print("Arglist: ")
    #print(hEvent, vm_uuid)
    #return
    event = prlsdkapi.Event(hEvent)
    if event.get_issuer_id() != vm_uuid:
        return

    evt_type = event.get_type()
    if (evt_type == events_map["PET_DSP_EVT_BACKUP_PROGRESS_CHANGED"] or evt_type == events_map["PET_DSP_EVT_RESTORE_PROGRESS_CHANGED"]):
        progress = event.get_param_by_name("progress_changed").to_string()
        idx = event.get_param_by_name("device_index").to_string()
        print("Backup hdd.%s progress: %s%%" % (idx, progress))
    elif evt_type == events_map["PET_DSP_EVT_BACKUP_STARTED"]:
        print("Backup started")
    elif evt_type == events_map["PET_DSP_EVT_CREATE_BACKUP_FINISHED"]:
        print("Backup finished")
    elif evt_type == events_map["PET_DSP_EVT_BACKUP_CANCELLED"]:
        print("Backup cancelled")
#   elif evt_type == events_map["PET_DSP_EVT_VM_MESSAGE"]:
#       print (event.get_err_string())

def parse_url(url):
    # Protocol is needed for urlparse to work
    if url.find("://") == -1:
        url = "http://" + url
    url = urlparse(url)
    host = url.hostname if url.hostname is not None else ""
    port = url.port if url.port is not None else 0
    user = url.username if url.username is not None else ""
    passwd = url.password if url.password is not None else ""
    return (host, port, user, passwd)

def normalize_uuid(uuid):
    if len(uuid) == UUID_LEN:
        return ("{" + uuid + "}")
    else:
        return uuid

def get_vm_by_name_or_uuid(server, vm_name, vmtype = consts.PVTF_CT | consts.PVTF_VM):
    for vm in server.get_vm_list_ex(vmtype).wait():
        if vm.get_name() == vm_name:
            return vm
    else:
        for vm in server.get_vm_list_ex(vmtype).wait():
            vm_name = normalize_uuid(vm_name)
            if vm.get_uuid() == vm_name:
                return vm
    print ("VE %s was not found" % vm_name)
    sys.exit(-1)

def create_backup(server, ve_name):
    ve = get_vm_by_name_or_uuid(server, ve_name)

    ve_name = ve.get_name()
    ve_uuid = ve.get_uuid()
    ve_type = code_type[ve.get_vm_type()]

    remote_host, remote_port, remote_user, remote_passwd = parse_url('')
    session_id, description = '', ''
    flags = 0
    flags |= consts.PBT_INCREMENTAL

    print ("Backing up the %s %s " % (ve_type, ve_name))

    server.reg_event_handler(callback_func, ve_uuid)
    #print(reg_handle)
    backupJob = server.create_vm_backup(ve_uuid, remote_host, remote_port, session_id, description, flags, 0, True)
    res = backupJob.wait()
    try:
        print("The VM has been successfully backed up with backup id %s" % res.get_param_by_index(1).get_backup_uuid())
    except IndexError:
        print ("Something went wrong during the backup creation")
    #callback.unregister_event_handler(server.handle, callback_func, ve_uuid)
    server.unreg_event_handler(callback_func, ve_uuid)

try:
    server = init()
    ve_name = sys.argv[1]
    create_backup(server, ve_name)
except prlsdkapi.PrlSDKError, e:
    print(str(e))
except IndexError:
    print("Please specify VE name/uuid")

