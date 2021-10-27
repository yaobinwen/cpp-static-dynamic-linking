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

LIBBOOST_LOG_SETUP_A=$(grep "libboost_log_setup.a" "$FLIST") || exit
echo "libboost_log_setup.a: $LIBBOOST_LOG_SETUP_A"

LIBBOOST_LOG_A=$(grep "libboost_log.a" "$FLIST") || exit
echo "libboost_log.a: $LIBBOOST_LOG_A"

nm -C -f posix "$LIBBOOST_LOG_SETUP_A" > \
  "$TMP_DIR/nm_libboost_log_setup.txt" || exit

nm -C -f posix "$LIBBOOST_LOG_A" > "$TMP_DIR/nm_libboost_log.txt" || exit

: ${SYMBOL_TYPES:="
  U
  TWnr
"}

: ${FILE_NAMES:="
  log_setup
  log
"}

for ST in ${SYMBOL_TYPES}; do
  for FN in ${FILE_NAMES}; do
    PYTHONPATH=./nm-parser/src \
      python3 -m nm_parser.nm_parser \
        -t "${ST}" \
        -i "${TMP_DIR}/nm_libboost_${FN}.txt" \
        -o "${TMP_DIR}/${FN}_${ST}_unsorted.txt" || exit

    sort -u "${TMP_DIR}/${FN}_${ST}_unsorted.txt" > \
      "${TMP_DIR}/${FN}_${ST}.txt"
  done
done

echo "-----------------------------------------------"
echo "libboost_log_setup.a depends on libboost_log.a:"
echo
comm -12 "${TMP_DIR}/log_setup_U.txt" "${TMP_DIR}/log_TWnr.txt"

echo
echo "-----------------------------------------------"
echo "libboost_log.a depends on libboost_log_setup.a:"
echo
comm -12 "${TMP_DIR}/log_setup_TWnr.txt" "${TMP_DIR}/log_U.txt"
