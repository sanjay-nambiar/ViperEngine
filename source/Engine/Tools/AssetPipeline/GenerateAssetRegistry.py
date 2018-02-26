#python 2.7

import json
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
        print 'Usage: python %s <asset_source_dir> <content_dir>' % (argv[0])
        exit(1)

def check_format_support(filename):
    filename_split = os.path.splitext(filename)
    if len(filename_split) == 2 and filename_split[1] in SUPPORTED_FILE_EXTENSIONS:
        return True
    return False

def generate_asset_list(source_dir, content_dir):
    asset_list = { 'count': 0 }
    assets = []
    for root, subdirs, files in os.walk(source_dir):
        for filename in [f for f in files if check_format_support(f)]:
            relative_path = root[len(source_dir):]
            source_file = os.path.join(root, filename)
            name = os.path.join(relative_path, os.path.splitext(filename)[0])
            package = content_dir + name + '.vasset'
            assets.append({ 'name' : name, 'package' : package, 'sourceFile' : source_file})
    asset_list['resources'] = assets
    asset_list['count'] = len(assets)
    return asset_list

def main(argv):
    check_commandline_args(argv)
    walk_dir = os.path.abspath(argv[1])
    content_dir = argv[2]
    asset_list = generate_asset_list(walk_dir, content_dir)
    with open(LIST_FILE, 'w') as out_file:
        out_file.write(json.dumps(asset_list, indent=4, sort_keys=True))

if __name__ == "__main__":
    main(sys.argv)
