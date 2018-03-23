#python 2.7

import argparse
import json
import os
import sys


################### Constants ############################

ASSET_TYPES = {
    '.jpg': 'texture',
    '.png': 'texture',
    '.dds': 'texture',
    '.tga': 'texture',
    '.bmp': 'texture',
    '.psd': 'texture',
    '.gif': 'texture',
    #'.mtl': 'material',  # mtl files should not be included since these are processed as part of the obj
    '.obj': 'model',
    '.dae': 'model',
    '.blend': 'model',
    '.3ds': 'model',
    '.gltf': 'model',
    '.glb': 'model',
    '.fbx': 'model'
}
ASSET_FILE_EXTENSION = '.vasset'
LIST_FILE = 'list.temp'
TEMP_DIRECTORY = '.temp'
TEMP_FILE_NAME = '.tempFile'


################### Main ############################

def main():
    commands = {'registry': generate_asset_list, 'relative': relative_path_to_resource}
    options = vars(parse_commandline_args())
    command = options['command']
    options.pop('command')
    commands[command](**options)


def parse_commandline_args():
    parser = argparse.ArgumentParser(description='ViperEngine asset registry helper')
    sub_parsers = parser.add_subparsers(help='Commands help', dest='command')
    # registry command command sub parser
    parser_registry = sub_parsers.add_parser('registry', help='Generate asset registry list file')
    parser_registry.add_argument('resources', help='Resources directory path')
    parser_registry.add_argument('content', help='Content directory path')
    # relative command sub parser
    parser_relative = sub_parsers.add_parser('relative', help='Get content directory based resource details from relative path')
    parser_relative.add_argument('path', help='Relative path of the asset')
    parser_relative.add_argument('base', help='Base of the relative path')
    parser_relative.add_argument('resources', help='Resources directory path')
    parser_relative.add_argument('content', help='Content directory path')
    return parser.parse_args()


################### Commands ############################

def generate_asset_list(content, resources):
    asset_list = {'count': 0}
    textures = []
    models = []
    for root, subdirs, files in os.walk(resources):
        for filename in [f for f in files if check_format_support(f)]:
            relative_path = root[len(resources):]
            source_file = os.path.join(root, filename)
            split = os.path.splitext(filename)
            # get resource details 
            package_name = os.path.join(relative_path, split[0])
            extension = split[1]
            package_path = content + package_name + ASSET_FILE_EXTENSION
            type = ASSET_TYPES[extension]
            # append to correct resource list
            resource_list = textures if type == 'texture' else models
            resource_list.append({
                'packageName': package_name, 
                'packageDir': os.path.dirname(package_path), 
                'resourceDir': os.path.dirname(source_file), 
                'resourceFile': source_file
            })
            # create directories and file
            mkdir_recursive(os.path.dirname(package_path))
            open(package_path, 'w').close()
    asset_list['textures'] = textures
    asset_list['models'] = models
    asset_list['count'] = len(textures) + len(models)
    asset_list['contentDirectoryPath'] = content
    asset_list['contentDirectory'] = os.path.basename(content)
    asset_list['tempDirectory'] = os.path.sep + TEMP_DIRECTORY + os.path.sep
    mkdir_recursive(os.path.join(content, TEMP_DIRECTORY))
    with open(LIST_FILE, 'w') as out_file:
        out_file.write(json.dumps(asset_list, indent=4, sort_keys=True))


def relative_path_to_resource(path, base, content, resources):
    with open(TEMP_FILE_NAME, "w") as temp_file:
        resourceFile = os.path.abspath(os.path.join(base, path))
        packageName = os.path.relpath(resourceFile, resources)
        resource = {
            'packageName': os.path.splitext(os.path.sep + packageName)[0], 
            'packageDir': os.path.dirname(os.path.join(content, packageName)), 
            'resourceDir': os.path.dirname(resourceFile), 
            'resourceFile': resourceFile
        }
        temp_file.write(json.dumps(resource, indent=4, sort_keys=True))


################### Helper methods ############################

def check_format_support(filename):
    filename_split = os.path.splitext(filename)
    if len(filename_split) == 2 and filename_split[1] in ASSET_TYPES:
        return True
    return False


def mkdir_recursive(dir_path):
    sub_path = os.path.dirname(dir_path)
    if sub_path and not os.path.exists(sub_path):
        mkdir_recursive(sub_path)
    if dir_path and not os.path.exists(dir_path):
        os.mkdir(dir_path)


################### Invoke main ################################

if __name__ == "__main__":
    main()
