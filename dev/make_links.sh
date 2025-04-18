#!/usr/bin/env sh

# creates symbolic links for development help with IDEs + Makefile
# this is supposed to be executed from the root of the repo

ln -s ./dev/.vscode .vscode
ln -s ./dev/.zed .zed
ln -s ./dev/.repomix .repomix
ln -s ./dev/Makefile Makefile
