#!/bin/sh

# Create the temporary directory.
TMP_DIR=$(mktemp --directory) || exit

_cleanup() {
  rm -fr "$T" || echo "WARNING: cleanup() failed" >&2
}
trap _cleanup EXIT INT TERM

# Determine the path of the lib files.
FLIST="$TMP_DIR/libboost-log1.65-dev.txt"
dpkg -L libboost-log1.65-dev > "$FLIST" || exit

LIBBOOST_LOG_A=$(grep "libboost_log.a" "$FLIST") || exit

# Cache the result of the symbols.
nm -C -f posix "$LIBBOOST_LOG_A" > "$TMP_DIR/all_symbols.txt"

IFS=
while read -r line; do
   grep "$line" "$TMP_DIR/all_symbols.txt"
done < ./undefined-symbols.txt
