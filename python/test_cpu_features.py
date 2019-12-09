import prlsdkapi
import prlsdkapi.prlsdk.consts as consts

prlsdkapi.init_server_sdk()

srv = prlsdkapi.Server()
srv.login("127.0.0.1", "root", "1q2w3e").wait()

cfg = srv.get_common_prefs().wait().get_param_by_index(0)

srv.common_prefs_begin_edit()

f = cfg.get_cpu_features_mask_ex()

m = (
    consts.PCFE_FEATURES,
    consts.PCFE_EXT_FEATURES,
    consts.PCFE_EXT_00000007_EBX,
    consts.PCFE_EXT_80000001_ECX,
    consts.PCFE_EXT_80000001_EDX,
    consts.PCFE_EXT_80000007_EDX,
    consts.PCFE_EXT_80000008_EAX)

for i in m:
    f.set_value(i, 0xff)

cfg.set_cpu_features_mask_ex(f)

f = cfg.get_cpu_features_mask_ex()

for i in m:
    print "Mask %d - Value %02x" % (i, f.get_value(i))

f = cfg.get_cpu_features_ex()
for i in m:
    print "Supported Feature %d - Value %02x" % (i, f.get_value(i))

