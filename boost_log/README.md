# README

## Overview

This code in `main.cpp` is copied from the Stack Overflow question [1].

The `CMakeLists.txt` is copied from [1] with minor changes to fix the linking error and I have written the result as the answer [2].

To fully understand why the static linking didn't work when only `${Boost_LIBRARIES}` was used, see the sibling directory `library-order-in-static-linking`.

## Undefined Symbols

When linking the library with `${Boost_LIBRARIES}` in CMake, the undefined symbols are listed in the file `undefined-symbols.txt`. These symbols are requested by `libboost_log_setup.a` and are defined in `libboost_log.a`.

The script `undefined-symbols-types.sh` prints the types of the undefined symbols in `libboost_log.a`. The output is in `undefined-symbols-types.txt`. According to the output, these undefined symbols have these types:
- `T`
- `W`
- `n`
- `r`

**Note** that the type `n` is not defined in [`nm(1)` on Ubuntu 18.04](http://manpages.ubuntu.com/manpages/bionic/man1/nm.1.html) (but is defined from Ubuntu 20.04 and onwards).

Running the shell `depends.sh` can list the symbols that `libboost_log_setup.a` depends on `libboost_log.a` and vice versa, as follows. This also shows that `libboost_log_setup.a` depends on `libboost_log.a` so `libboost_log_setup.a` should be specified before ``libboost_log.a`` in the linking order.

```
-----------------------------------------------
libboost_log_setup.a depends on libboost_log.a:

boost::log::v2s_mt_posix::attribute_value_set::find(boost::log::v2s_mt_posix::attribute_name) const
boost::log::v2s_mt_posix::aux::attach_attribute_name_info(boost::exception&, boost::log::v2s_mt_posix::attribute_name const&)
boost::log::v2s_mt_posix::aux::code_convert_impl(wchar_t const*, unsigned long, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&, unsigned long, std::locale const&)
boost::log::v2s_mt_posix::aux::futex_based_event::futex_based_event()
boost::log::v2s_mt_posix::aux::futex_based_event::set_signalled()
boost::log::v2s_mt_posix::aux::once_block_sentry::rollback()
boost::log::v2s_mt_posix::aux::threadsafe_queue_impl::create(boost::log::v2s_mt_posix::aux::threadsafe_queue_impl::node_base*)
boost::log::v2s_mt_posix::parse_error::~parse_error()
boost::log::v2s_mt_posix::parse_error::throw_(char const*, unsigned long, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)
boost::log::v2s_mt_posix::sinks::file::aux::make_collector(boost::filesystem::path const&, unsigned long, unsigned long, unsigned long)
boost::log::v2s_mt_posix::sinks::file::rotation_at_time_point::rotation_at_time_point(boost::date_time::weekdays, unsigned char, unsigned char, unsigned char)
boost::log::v2s_mt_posix::sinks::syslog_backend::~syslog_backend()
boost::log::v2s_mt_posix::sinks::text_file_backend::consume(boost::log::v2s_mt_posix::record_view const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)
boost::log::v2s_mt_posix::sinks::text_file_backend::enable_final_rotation(bool)
boost::log::v2s_mt_posix::sinks::text_file_backend::set_file_name_pattern_internal(boost::filesystem::path const&)
boost::log::v2s_mt_posix::sinks::text_file_backend::set_open_mode(std::_Ios_Openmode)
boost::log::v2s_mt_posix::sinks::text_file_backend::set_rotation_size(unsigned long)
boost::log::v2s_mt_posix::sinks::text_file_backend::~text_file_backend()

-----------------------------------------------
libboost_log.a depends on libboost_log_setup.a:
```

**NOTE:** I haven't figured out why the list that `depends.sh` outputs is shorter than what linker reports undefined (in `undefined-symbols.txt`).

## References

- [1] ["Compiling boost::log with static linking doesn't work when including init_from_settings.hpp"](https://stackoverflow.com/q/53551754/630364)
- [2] [My answer to [1]](https://stackoverflow.com/a/69647325/630364)
