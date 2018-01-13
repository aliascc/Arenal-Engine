
import os
import glob
import sys

dirToSearch = sys.argv[1]

os.chdir(dirToSearch)


oldLicense = ("/********************************************************\n"
              "*\n"
              "* Author: Carlos Chacon N.\n"
              "*\n"
              "* Copyright 2012-2015\n"
              "*\n"
              "*********************************************************/\n")


newLicense = ("/*\n"
              "* Copyright (c) 2018 <Carlos Chacon>\n"
              "* All rights reserved\n"
              "*\n"
              "* Licensed under the Apache License, Version 2.0 (the \"License\");\n"
              "* you may not use this file except in compliance with the License.\n"
              "* You may obtain a copy of the License at\n"
              "*\n"
              "* http://www.apache.org/licenses/LICENSE-2.0\n"
              "*\n"
              "* Unless required by applicable law or agreed to in writing, software\n"
              "* distributed under the License is distributed on an \"AS IS\" BASIS,\n"
              "* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
              "* See the License for the specific language governing permissions and\n"
              "* limitations under the License.\n"
              "*/\n")

#file_lic = []

exts = ["**/*.fx", "**/*.h", "**/*.cpp"]

for ext in exts:
    for fileName in glob.glob(ext, recursive=True):
        if "Compiled Materials" in fileName or "3rdParty" in fileName or "GeneratedFiles" in fileName:
            continue
        #file_lic.append(file)

        print("Fixing file: " + fileName)

        # Read in the file
        file = open(fileName, 'r')
        filedata = file.read()
        file.close()

        # Replace the target string
        newData = filedata.replace(oldLicense, newLicense)

        # Write the file out again
        file = open(fileName, 'w')
        file.write(newData)
        file.close()


