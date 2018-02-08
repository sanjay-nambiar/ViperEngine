import sys
import subprocess
import re
from os import listdir, remove
from os.path import isfile, join, realpath, dirname


# Constants
USAGE_STRING = "Usage: python %(scriptName)s <ShaderDirectory> <OutputDirectory>"
INCLUDE_PATTERN = re.compile(r'^#include\s+"([^"]+)"')
SHADER_EXTENSIONS = ['vert', 'tesc', 'tese', 'geom', 'frag', 'comp']
GLSL_VALIDATOR = "glslangValidator.exe"
TEMP_FILE = "temp.bin"
VALIDATOR_COMMANDLINE = "%(program)s -w -G %(outputFile)s -o " + TEMP_FILE


# pre-processor logic
def processile(inputFile, outputFile):
    content = inputFile.readlines()
    for line in content:
        result = INCLUDE_PATTERN.search(line)
        if result:
            headerFileName = join(SHADER_SOURCE_DIR, result.group(1))
            with open(headerFileName) as headerFile:
                processFile(headerFile, outputFile)
        else:
            outputFile.write(line)


# Check commandline arguments
if len(sys.argv) != 3:
    print USAGE_STRING % {"scriptName" : sys.argv[0]}
    exit(1)


# Initialize argument variables
VALIDATOR_PROGRAM = join(dirname(realpath(__file__)), GLSL_VALIDATOR)
SHADER_SOURCE_DIR = sys.argv[1]
OUTPUT_DIR = sys.argv[2]


# Get all shader files (extensions specified as a constant above)
shaderFiles = [f for f in listdir(SHADER_SOURCE_DIR) if isfile(join(SHADER_SOURCE_DIR, f)) and f.rpartition('.')[-1] in SHADER_EXTENSIONS]

# Validate all shader files
for shaderFile in shaderFiles:
    inputFileName = join(SHADER_SOURCE_DIR, shaderFile)
    outputFileName = join(OUTPUT_DIR, shaderFile)

    with open(inputFileName) as inputFile, open(outputFileName, "w") as outputFile:
        processFile(inputFile, outputFile)
    
    returnCode = subprocess.call(VALIDATOR_COMMANDLINE % {"program" : VALIDATOR_PROGRAM, "outputFile" : outputFileName})
    if returnCode != 0:
        remove(TEMP_FILE)
        exit(1)

remove(TEMP_FILE)
