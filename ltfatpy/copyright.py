# -*- coding: utf-8 -*-
from __future__ import print_function, division
import time
import os
import sys
import fileinput


def findFiles(directory, files=[]):
    """scan a directory for py, pyx, pxd extension files."""
    for filename in os.listdir(directory):
        path = os.path.join(directory, filename)
        if os.path.isfile(path) and (path.endswith(".py") or
                                     path.endswith(".pyx") or
                                     path.endswith(".pxd")):
            files.append(path)
        elif os.path.isdir(path):
            findFiles(path, files)
    return files


def fileUnStamping(filename):
    """ Remove stamp from a file """
    is_stamp = False
    for line in fileinput.input(filename, inplace=1):
        if line.find("# COPYRIGHT #") != -1:
            is_stamp = not is_stamp
        elif not is_stamp:
            print(line, end="")

def fileRemoveSharps(filename):
    """ Remove sharps from a file """
    removeLine = False
    for line in fileinput.input(filename, inplace=1):
        if  line.startswith("# -*- coding: utf-8 -*-"):
            print(line, end="")
            removeLine = True
        elif removeLine and  line.startswith("#"):
            pass
        else:
            removeLine = False
            print(line, end="")

def fileRemoveBlankLines(filename):
    """ Remove undesirable blank lines from a file """
    removeLine = False
    for line in fileinput.input(filename, inplace=1):
        if  line.startswith("# -*- coding: utf-8 -*-"):
            print(line, end="")
            removeLine = True
        elif removeLine and not line.strip():
            pass
        else:
            removeLine = False
            print(line, end="")

def fileStamping(filename, stamp):
    """ Write a stamp on a file

    WARNING : The stamping must be done on an default utf8 machine !
    """
    old_stamp = False  # If a copyright already exist over write it.
    for line in fileinput.input(filename, inplace=1):
        if line.find("# COPYRIGHT #") != -1:
            old_stamp = not old_stamp
        elif line.startswith("# -*- coding: utf-8 -*-"):
            print(line, end="")
            print(stamp, end="")
        elif not old_stamp:
            print(line, end="")


def getStamp(date, ltfatpy_version, ltfat_version):
    """ Return the corrected formated stamp """
    stamp = open("copyrightstamp.txt").read()
    stamp = stamp.replace("DATE", date)
    stamp = stamp.replace("LTFATPY_VERSION", ltfatpy_version)
    stamp = stamp.replace("LTFAT_VERSION", ltfat_version)
    stamp = stamp.replace('\n', '\n# ')
    stamp = "# " + stamp
    stamp = stamp.replace("# \n", "#\n")
    return stamp.strip()[:-1]


def getVersionsAndDate():
    """ Return (date, ltfatpy_version, ltfat_version) """
    v_text = open('VERSION').read().strip()
    v_text_formted = '{"' + v_text.replace('\n', '","').replace(':', '":"')
    v_text_formted += '"}'
    v_dict = eval(v_text_formted)
    return (time.strftime("%Y"), v_dict['ltfatpy'],
            v_dict['ltfat'])


def writeCopyrightRst(date, ltfatpy_version, ltfat_version):
    stamp = ""
    for line in open("copyrightstamp.txt"):
        if "# COPYRIGHT #" not in line:
            stamp += line
    stamp = stamp.replace("DATE", date)
    stamp = stamp.replace("LTFATPY_VERSION", ltfatpy_version)
    stamp = stamp.replace("LTFAT_VERSION", ltfat_version)
    docfile = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                           "doc", "copyright.rst")
    f = open(docfile, 'w')
    f.write(stamp)
    f.close()


def writeStamp():
    """ Write a copyright stamp on all files """
    (date, ltfatpy_version, ltfat_version) = getVersionsAndDate()
    stamp = getStamp(date, ltfatpy_version, ltfat_version)
    files = findFiles(os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                   "ltfatpy"))
    for filename in files:
        fileStamping(filename, stamp)
    fileStamping("setup.py", stamp)
    writeCopyrightRst(date, ltfatpy_version, ltfat_version)


def eraseStamp():
    """ Erase a copyright stamp from all files """
    files = findFiles(os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                   "ltfatpy"))
    for filename in files:
        fileUnStamping(filename)
    fileUnStamping("setup.py")

def removeSharps():
    """ Remove undesirable sharps from all files """
    files = findFiles(os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                   "ltfatpy"))
    for filename in files:
        fileRemoveSharps(filename)
    fileRemoveSharps("setup.py")

def removeBlankLines():
    """ Remove undesirable blank lines from all files """
    files = findFiles(os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                   "ltfatpy"))
    for filename in files:
        fileRemoveBlankLines(filename)

def usage(arg):
    print("Usage :")
    print("\tpython %s stamping" % arg)
    print("\tpython %s unstamping" % arg)


if __name__ == "__main__":
    if len(sys.argv) == 1:
        usage(sys.argv[0])
    elif len(sys.argv) == 2:
        if sys.argv[1].startswith("unstamping"):
            eraseStamp()
        elif sys.argv[1].startswith("stamping"):
            writeStamp()
        elif sys.argv[1].startswith("blanklines"):
            removeBlankLines()
        else:
            usage(sys.argv[0])
    else:
        usage(sys.argv[0])
