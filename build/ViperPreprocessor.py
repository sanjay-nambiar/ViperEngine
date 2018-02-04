''' Does custom preprocessor substituions before C++ preprocessor is invoked.
'''

import fnmatch
import os
import re
import sys
import zlib


# Constants
USAGE_STRING = "Usage: python %(scriptName)s <directory>"
CPP_FILE_PATTERN = '*.cpp'

# Handlers
def StringIdProcessor(regexMatch):
    '''Handles StringID macro. Returns CRC32 value of string literal input'''
    return zlib.crc32(regexMatch.group(1)) & 0xffffffff


HANDLERS = {
    re.compile(r'StringID\("([^"]*)"\)'): StringIdProcessor
}

# find replace in files of type
def findReplace(directory, filePattern):
    for path, dirs, files in os.walk(os.path.abspath(directory)):
        for filename in fnmatch.filter(files, filePattern):
            filepath = os.path.join(path, filename)
            content = ''
            with open(filepath) as codeFile:
                content = codeFile.read()
                for reg, handler in HANDLERS.iteritems():
                    content = reg.sub(lambda match: handler(match), content)
            with open(filepath, 'w') as codeFile:
                codeFile.write(content)


# Check commandline arguments
if len(sys.argv) != 2:
    print USAGE_STRING % {'scriptName' : sys.argv[0]}
    exit(1)

findReplace(sys.argv[1], CPP_FILE_PATTERN)
