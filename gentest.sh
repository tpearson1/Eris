#!/bin/sh

dir=`dirname $0`

if [ $# -ne 2 ]; then
  echo "Usage: ./gentest.sh <name> <package-name>"
  exit
fi

test_dir=$dir/mods/$2/tests
mkdir -p $test_dir

cat <<EOF > $test_dir/$1.cpp
#include <test/test.h>

TEST_CASE("Success", {})

EOF
