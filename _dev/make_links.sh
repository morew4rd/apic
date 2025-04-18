#!/usr/bin/env sh

# creates symbolic links for development help with IDEs + Makefile
# this is supposed to be executed from the root of the repo


rm ./.vscode
ln -s ./_dev/.vscode ./.vscode

rm ./.zed
ln -s ./_dev/.zed ./.zed

rm ./.repomix
ln -s ./_dev/.repomix ./.repomix

rm ./Makefile
ln -s ./_dev/Makefile ./Makefile
