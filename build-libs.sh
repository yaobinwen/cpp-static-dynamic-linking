build_static_lib() {
    local SOURCE_FILE_NAME

    test $# -eq 1 || {
        echo "Usage $0 SOURCE-FILE-NAME"
        return 1
    }

    SOURCE_FILE_NAME="$1"

    : "${SOURCE_FILE_NAME:?not set}"

    # Compile the `.cpp` file but do not link.
    g++ -c -fPIC -o "${SOURCE_FILE_NAME}.o" "${SOURCE_FILE_NAME}.cpp" \
        || return 1

    # Create the static archive.
    ar rvs "lib${SOURCE_FILE_NAME}.a" "${SOURCE_FILE_NAME}.o" || return 1
}

build_dynamic_lib() {
    local SOURCE_FILE_NAME

    test $# -eq 1 || {
        echo "Usage $0 SOURCE-FILE-NAME"
        return 1
    }

    SOURCE_FILE_NAME="$1"

    : "${SOURCE_FILE_NAME:?not set}"

    # Compile the `.cpp` file but do not link.
    g++ -c -fPIC -o "${SOURCE_FILE_NAME}.o" "${SOURCE_FILE_NAME}.cpp" \
        || return 1

    # Create the shared library.
    #
    # `-shared`:
    # Produce a shared object which can then be linked with other objects to
    # form an executable. Not all systems support this option. For predictable
    # results, you must also specify the same set of options used for
    # compilation ('-fpic', '-fPIC', or model suboptions) when you specify this
    # linker option.
    g++ -shared -fPIC -o "lib${SOURCE_FILE_NAME}.so" "${SOURCE_FILE_NAME}.o" \
        || return 1
}
