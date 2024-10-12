#!/bin/bash


# # Assign the filename
# filename="config.make"

# # Take the search string
# search='../../..'

# # Take the replace string
# replace='/Applications/openFrameworks/'

# double "" let variables (like $search) inside intact while single '' ignore even variable names
# sed -i '' "s+$search+$replace+" $filename


sed -i '' 's+${workspaceRoot}/../../../+/Applications/openFrameworks/+' .vscode/c_cpp_properties.json

sed -i '' "s+../../..+/Applications/openFrameworks/+" config.make

sed -i '' "s+../../..+/Applications/openFrameworks/+" Makefile

sed -i '' 's+${workspaceRoot}/../../../../+/Applications/openFrameworks/+' *.code-workspace



