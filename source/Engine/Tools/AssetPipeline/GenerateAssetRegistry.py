#python 2.7

import os
import sys

SUPPORTED_FILE_EXTENSIONS = [
    '.jpg',
    '.png',
    '.dds',
    '.tga',
    '.bmp',
    '.psd',
    '.gif',
    '.obj',
    '.mtl',
    '.dae',
    '.blend',
    '.3ds',
    '.gltf',
    '.glb',
    '.fbx'
]

LIST_FILE = 'list.temp'

def check_commandline_args(argv):
    if len(argv) != 3:
        print 'Usage: python %s <asset_source_dir> <game_content_dir>' % (argv[0])
        exit(1)

def check_format_support(filename):
    filename_split = os.path.splitext(filename)
    if len(filename_split) == 2 and filename_split[1] in SUPPORTED_FILE_EXTENSIONS:
        return True
    return False

def generate_asset_list(root_dir, list_file):
    with open(list_file, 'w') as out_file:
        for root, subdirs, files in os.walk(root_dir):
            for filename in [f for f in files if check_format_support(f)]:
                out_file.write(os.path.join(root, filename) + '\n')

def main(argv):
    check_commandline_args(argv)
    walk_dir = os.path.abspath(argv[1])
    generate_asset_list(walk_dir, LIST_FILE)

if __name__ == "__main__":
    main(sys.argv)
