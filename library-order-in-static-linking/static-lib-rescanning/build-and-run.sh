#!/bin/sh

set -ex

# Create a temporary directory.
TMP_D="$(mktemp -d)"

cleanup() {
  rm -fr "$TMP_D" || echo "WARNING: cleanup() failed" >&2
}
trap cleanup EXIT INT TERM

# Build the static library A.
: ${A_SRC_FNAMES:="
  A_subsubfunc1
  A_subfunc1
  A_subfunc2
  A_subfunc3
  A_func1
  A_func2
"}

for FN in ${A_SRC_FNAMES}; do
  echo "Compiling $FN.c"
  gcc -c "$FN.c" -o "$FN.o"
done

A_OBJ_FILES="$TMP_D/A_obj_files.txt"
touch "${A_OBJ_FILES}"
for FN in ${A_SRC_FNAMES}; do
  echo "$FN.o" >> "${A_OBJ_FILES}"
done

cat "${A_OBJ_FILES}"
ar rvs libA.a $(cat "${A_OBJ_FILES}")

# Build the static library B.
gcc -c "B.c" -o "B.o"
ar rvs libB.a "B.o"

# Build the main program.
gcc -c main.c
gcc main.o -o main1.exe -L. -lA -lB

# Build the main program but using the wrong linking order.
gcc main.o -o main2.exe -L. -lB -lA
