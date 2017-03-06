#!/bin/sh

dir=`dirname $0`

if [ $# -ne 4 ]; then
  echo "Usage: ./genpackage.sh <name> <author> <playable> <uses-c++>"
  exit
fi

# Uses C++?
if [ "$4" == "true" ]; then
  mkdir -p $dir/mods/$1/include/$1
  mkdir -p $dir/mods/$1/src
else
  if [ "$3" == "true" ]; then
    echo "A package that doesn't use C++ can't be playable"
    exit
  fi

  mkdir -p $dir/mods/$1
fi

cat <<EOF > $dir/mods/$1/package.json
{
  "author": "$2",
  "version": "0.0.1",
  "playable": $3,
  "uses-c++": $4
}
EOF
