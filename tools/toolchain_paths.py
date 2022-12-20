import subprocess
import json
import os
import argparse
import sys


def create_script(sdkfolder, add_to_path, file):

    assert (os.path.isdir(sdkfolder)), "%s is not a directory!" % (sdkfolder)

    cc_clang_path = "%s/bin/clang" % (sdkfolder)
    cxx_clang_path = "%s/bin/clang++" % (sdkfolder)

    assert (os.path.isfile(cc_clang_path)), "%s is not a file!" % (cc_clang_path)
    assert (os.path.isfile(cxx_clang_path)), "%s is not a file!" % (cxx_clang_path)

    file.write('export CC=%s\n' % (cc_clang_path))
    file.write('export CXX=%s\n' % (cxx_clang_path))

    if sys.platform == 'darwin':
        file.write('export CFLAGS="-isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk"\n')

    if add_to_path:
        file.write('export PATH="%s/bin/:$PATH"\n' % (sdkfolder))


if __name__ == '__main__':

    parser = argparse.ArgumentParser()

    parser.add_argument('--recipe', type=str, required=True, help="Recipe of the Synsense-LLVM compiler.")
    parser.add_argument('--add-to-path', action="store_true",
                        help="Add toolchain path to $PATH environment variable (default False).")
    parser.add_argument('--output', type=str, default="stdout",
                        help="Path of the environment activation script (default STDOUT).")
    parser.add_argument('--conan-command', type=str, default="conan",
                        help="Path to the desired conan command (optional, default=\"conan\")")

    args = parser.parse_args()

    raw_results = subprocess.check_output(
        [args.conan_command, 'info', args.recipe, '--paths', '--json']).decode("utf-8")

    results = json.loads(raw_results)

    assert(len(results) != 0), 'Cannot find recipe for: %s!' % (args.recipe)
    assert(len(results) == 1), 'Only one package for %s should be found!' % (args.recipe)

    file = sys.stdout if args.output == "stdout" else open(args.output, 'w')

    create_script(results[0]['package_folder'], args.add_to_path, file)
