[![Build Status](https://travis-ci.org/jacob-hegna/duplicate-finder.svg?branch=master)](https://travis-ci.org/jacob-hegna/duplicate-finder)

# Duplicate Finder
Duplicate finder is a program to find and delete exact duplicates of other files in a give folder.  It uses sha256 to determine matches.

## Intended use case
I intend for this program to be used on competitive debate teams to reduce the file of shared folders.  Keeping shared folders small is especially important when using services such as dropbox or google drive with limited free storage.

## Install
The only external library used is Boost Filesystem, and if you don't have that installed you can usually grab that through a package manager.

```
git clone https://github.com/jacob-hegna/duplicate-finder.git
cd duplicate-finder
make
make install
```

## Usage
```
duplicate-finder [folder] [log name]
```