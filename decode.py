#!/usr/bin/env python3

import serial   # requires pySerial - 
import argparse
import time
#import pinpadio
import re
from Cryptodome.Cipher import DES3  # for MAC calculation - requires pycryptodomex (https://pypi.org/project/pycryptodomex/)

# NOTE: To install a Cryptodome, download the proper package e.g. for Python 3.5 and 64-bit Windows it is: pycryptodomex-3.8.0-cp35-cp35m-win_amd64.whl
# Then install with: pip install pycryptodomex-3.8.0-cp35-cp35m-win_amd64.whl


# NOTE: key is the same as in loadMACkey.py and in DOC180_089_EN_D_PP1000se_Reference_and_Programmers_Guide
key = '6AC292FAA1315B4D8234B3A3D7D5933A'
keyIndex = '3'

    

def substituteControlCharsNames(text):
    text = text.replace('<STX>', '\x02')
    text = text.replace('<ETX>', '\x03')
    text = text.replace('<SUB>', '\x1a')
    text = text.replace('<FS>', '\x1c')
    text = text.replace('<US>', '\x1f')
    return text

def filterMACChars(text):
    result = re.sub('(\x1f.{4})|[^A-Za-z\x1a\x1c]', '\\1', text)
    return result

def hexWithSpaces(t):
    return ' '.join(['{:02x}'.format(x) for x in t])

def calcMAC(text, key):
    if isinstance(key, str) :
        key = bytes.fromhex(key)

    # convert plaintext to bytes
    if isinstance(text, str) :
        text = bytes(text, 'ascii')
    
    # pad the plaintext
    if len(text) % 8:
        text += b'0' * (8 - len(text) % 8)

    #print('MAC plaintext=', hexWithSpaces(text))

    cipher = DES3.new(key, DES3.MODE_CBC, iv=bytes.fromhex('0000000000000000'))
    encryptedMsg = cipher.encrypt(text)
    
    # NOTE: Only first half of the first block is used
    result = encryptedMsg[-8:-4]
    return result.hex()


def calcMACWithFilter(text, key):
    #print('text for MAC=', repr(text))
    text = filterMACChars(text)
    #print('filtered text for MAC=', repr(text))
    mac = calcMAC(text, key)
    return mac

if __name__ == '__main__':
    speed = 0
    parity = 'E'
    timeout = None
    use_rtscts = False
    use_dsrdtr = False
    text = '<SUB>MESSAGE ONE 1<FS>MESSAGE TWO 2'
    #text = '<SUB>Test'
    text = substituteControlCharsNames(text)
    mac = calcMACWithFilter(text, key)
    print (mac,text)
    cmd = 'Z2<FS>' + keyIndex + mac + text
    cmd = substituteControlCharsNames(cmd)
    print (cmd)
