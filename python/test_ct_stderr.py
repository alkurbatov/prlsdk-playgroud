import subprocess

mbToWrite = 10
#args = ["prlctl", "exec", "200", "python", "/root/stdoutdata.py", "1000"]
args = ['prlctl', 'exec', '{aa9f9675-d947-45dc-91ff-11790b64474f}', 'python', '/root/stdoutdata.py', '10']
#args = ['/root/tmp/prlctl', 'exec', '{4c487110-e1f0-4bf5-ac52-9013871680b4}', '--login', 'root:1q2w3e@10.29.0.255', 'python', '/vz/tmp/tmp_D_xla/stdoutdata.py', '1']
#args = ['prlctl', 'exec', '{4c487110-e1f0-4bf5-ac52-9013871680b4}', '--login', 'root:1q2w3e@10.29.0.255', 'python', '/vz/tmp/tmp_D_xla/stdoutdata_1.py']
#args = ['prlctl', 'exec', '200', '--login', 'root:1q2w3e@10.30.28.98', 'python', '/root/stdoutdata.py', '2']

fd = subprocess.Popen(args, stderr = subprocess.PIPE)

nChar = 0
nBytesRead = 0
bToWrite = mbToWrite << 20
infoEvery = bToWrite / 20
buff = ''
moreInfo = False
readDesc = fd.stderr

def Fail(msg_):
    print msg_
    exit(-1)

def Info(msg_):
    print msg_

while nBytesRead < bToWrite:
    buff = readDesc.read(2 ** 20)
    if not len(buff):
        Fail('Pipe is closed after %i bytes, expected %i ' % (nBytesRead, 2 ** 20 * mbToWrite))

    for i in range(0, len(buff)):
        if nChar == 256:
            nChar = 0

        if ord(buff[i]) != nChar:
            while readDesc.read(2 ** 20):
                pass
            Fail('Chars a different,' +\
                ' %i bytes read' % int(nBytesRead))

        nChar += 1

    nBytesRead += len(buff)

    if infoEvery:           
        if nBytesRead % infoEvery == 0:
            Info('Reading from VM ....')

while readDesc.read(2 ** 20):
    moreInfo = True

if moreInfo:            
    Fail('There are more info in descriptor')

Info("Success")

