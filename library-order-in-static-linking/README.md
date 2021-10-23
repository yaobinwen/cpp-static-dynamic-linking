# Library order in static linking

## 1. Overview

This directory is essentially an appendix to [1]. The original article uses text to describe the linking process while I translate the text into code which can hopefully help the readers understand the article more easily.

The subsequent sections of this document generally follows the proceeding of [1]. I will describe the part of [1] that the current section is written for.

The most important take-away:

> If object or library AA needs a symbol from library BB, then AA should come before library BB in the command-line invocation of the linker.

## 2. Static Library Re-scanning

[1] says:

> Finally, if _any_ of the objects in the library has been included in the link, the library is rescanned again - it's possible that symbols imported by the included object can be found in other objects within the same library.

The author doesn't tell more details about how the re-scanning is actually done, nor do the examples later in the article show much of this procedure. So my questions are:
- Is the static library re-scanned only once? Or is it re-scanned as many times as needed?
- For each object file inside the static library that was skipped previously but now is being re-scanned, how are the exported and imported symbols handled?

Therefore, I wrote `static-lib-rescanning` to test how the re-scanning possibly works.

The entire program consists of three parts:
- The static library `A` which consists of the following object files:
  - `func1.o`
  - `func2.o`
  - `subfunc1.o`
  - `subfunc2.o`
  - `subfunc3.o`
  - `subsubfunc1.o`
- The static library `B` which consists of the following object files:
  - `B.o`
- The `main` program.

The chain of invocation from `main` is as follows:
- `main` calls:
  - `func1` which calls:
    - `subfunc1` which calls nothing else
    - `subfunc2` which calls:
      - `subsubfunc1` which calls nothing else
      - `B_work` which calls nothing else

However, in the static library file `libA.a`, the object files are arranged in the following order:
- `subsubfunc1.o`
- `subfunc1.o`
- `subfunc2.o`
- `subfunc3.o`
- `func1.o`
- `func2.o`

This invocation chain and the object file arrangement create **two levels of needs** to discover which object files are actually needed by the final program. This can be explained as follows:
- When `libA.a` is scanned for the first time, the only requested symbol is `func1` that comes from `main.o`.
- In the first scan of `libA.a`, all the `sub...` object files will be skipped because, as [1] points out, none of them exports a symbol that's being requested.
- If the re-scanning happens only once, then the linker would need to add all the other object files to the link in order to include `subsubfunc1.o` correctly. Otherwise, if the linker still goes with the "see and add" strategy, `subsubfunc1.o` would still be skipped because it is the first object file to be scanned and it doesn't export any symbol that is being requested at this time. As a result, the object files `subfunc3.o` and `func2.o` would be added, too.
- But if the re-scanning happens as many times as needed, then the linker wouldn't need to add all of the object files but could still add them once they are actually needed. In this case, `subfunc3` and `func2` wouldn't be in the final program.

Therefore, by checking the symbol table of the final program, we can determine if the re-scanning happens once or multiple times.

By running `nm -s main1.exe`, I can get the following symbol table:

```
0000000000201010 B __bss_start
0000000000000663 T B_work
0000000000201010 b completed.7698
                 w __cxa_finalize@@GLIBC_2.2.5
0000000000201000 D __data_start
0000000000201000 W data_start
0000000000000520 t deregister_tm_clones
00000000000005b0 t __do_global_dtors_aux
0000000000200df8 t __do_global_dtors_aux_fini_array_entry
0000000000201008 D __dso_handle
0000000000200e00 d _DYNAMIC
0000000000201010 D _edata
0000000000201018 B _end
00000000000006e4 T _fini
00000000000005f0 t frame_dummy
0000000000200df0 t __frame_dummy_init_array_entry
00000000000008fc r __FRAME_END__
000000000000061f T func1
0000000000200fc0 d _GLOBAL_OFFSET_TABLE_
                 w __gmon_start__
00000000000006f4 r __GNU_EH_FRAME_HDR
00000000000004b8 T _init
0000000000200df8 t __init_array_end
0000000000200df0 t __init_array_start
00000000000006f0 R _IO_stdin_used
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
00000000000006e0 T __libc_csu_fini
0000000000000670 T __libc_csu_init
                 U __libc_start_main@@GLIBC_2.2.5
00000000000005fa T main
0000000000000560 t register_tm_clones
00000000000004f0 T _start
000000000000063a T subfunc1
0000000000000641 T subfunc2
000000000000065c T subsubfunc1
0000000000201010 D __TMC_END__
```

Since `subfunc3` and `func2` are not included in the symbol table, we can say the re-scanning happens as many times as needed. The following table shows the detailed procedure of the multiple re-scanning(**note** that this is just a **possible** algorithm to process the files because I have not read the source code yet):

| Step | Library | File | Handling | Exported Symbols | Imported Symbols | Remarks |
|:----:|:-------:|:-----:|:--------:|:-----------------|:-----------------|:--------|
| 1 | N/A | `main.o` | Scanned | `main` | `func1` | `main.o` is an object file so it's always processed. |
| 2 | `libA.a` | `subsubfunc1.o` | Skipped | `main` | `func1` | Doesn't export any symbols that are being requested. |
| 3 | `libA.a` | `subfunc1.o` | Skipped | `main` | `func1` | Doesn't export any symbols that are being requested. |
| 4 | `libA.a` | `subfunc2.o` | Skipped | `main` | `func1` | Doesn't export any symbols that are being requested. |
| 5 | `libA.a` | `subfunc3.o` | Skipped | `main` | `func1` | Doesn't export any symbols that are being requested. |
| 6 | `libA.a` | `func1.o` | Scanned | `main` <br/> `func1` | `subfunc1` <br/> `subfunc2` | Exports `func1` that is being requested. |
| 7 | `libA.a` | `func2.o` | Skipped | `main` <br/> `func1` | `subfunc1` <br/> `subfunc2` | Doesn't export any symbols that are being requested. |
| 8 | `libA.a` | `subsubfunc1.o` | Skipped | `main` <br/> `func1` | `subfunc1` <br/> `subfunc2` | Doesn't export any symbols that are being requested. |
| 9 | `libA.a` | `subfunc1.o` | Scanned | `main` <br/> `func1` <br/> `subfunc1` | `subfunc2` | Exports `subfunc1` that is being requested. |
| 10 | `libA.a` | `subfunc2.o` | Scanned | `main` <br/> `func1` <br/> `subfunc1` <br/> `subfunc2` | `subsubfunc1` <br/> `B_work` | Exports `subfunc2` that is being requested. |
| 11 | `libA.a` | `subfunc3.o` | Skipped | `main` <br/> `func1` <br/> `subfunc1` <br/> `subfunc2` | `subsubfunc1` <br/> `B_work` | Doesn't export any symbols that are being requested. |
| 12 | `libA.a` | `func1.o` | Skipped | `main` <br/> `func1` <br/> `subfunc1` <br/> `subfunc2` | `subsubfunc1` <br/> `B_work` | Already been scanned. |
| 13 | `libA.a` | `func2.o` | Skipped | `main` <br/> `func1` <br/> `subfunc1` <br/> `subfunc2` | `subsubfunc1` <br/> `B_work` | Doesn't export any symbols that are being requested. |
| 14 | `libA.a` | `subsubfunc1.o` | Scanned | `main` <br/> `func1` <br/> `subfunc1` <br/> `subfunc2` <br/> `subsubfunc1` | `B_work` | Exports `subsubfunc1` that is being requested. |
| 15 | `libA.a` | `subfunc1.o` | Skipped | `main` <br/> `func1` <br/> `subfunc1` <br/> `subfunc2` <br/> `subsubfunc1` | `B_work` | Already been scanned. |
| 16 | `libA.a` | `subfunc2.o` | Skipped | `main` <br/> `func1` <br/> `subfunc1` <br/> `subfunc2` <br/> `subsubfunc1` | `B_work` | Already been scanned. |
| 17 | `libA.a` | `subfunc3.o` | Skipped | `main` <br/> `func1` <br/> `subfunc1` <br/> `subfunc2` <br/> `subsubfunc1` | `B_work` | Already been scanned. |
| 18 | `libA.a` | `func1.o` | Skipped | `main` <br/> `func1` <br/> `subfunc1` <br/> `subfunc2` <br/> `subsubfunc1` | `B_work` | Already been scanned. |
| 19 | `libA.a` | `func2.o` | Skipped | `main` <br/> `func1` <br/> `subfunc1` <br/> `subfunc2` <br/> `subsubfunc1` | `B_work` | Doesn't export any symbols that are being requested. |
| 20 | `libA.a` | `subsubfunc1.o` | Skipped | `main` <br/> `func1` <br/> `subfunc1` <br/> `subfunc2` <br/> `subsubfunc1` | `B_work` | Already been scanned. |
| 21 | `libA.a` | `subfunc1.o` | Skipped | `main` <br/> `func1` <br/> `subfunc1` <br/> `subfunc2` <br/> `subsubfunc1` | `B_work` | Already been scanned. |
| 22 | `libA.a` | `subfunc2.o` | Skipped | `main` <br/> `func1` <br/> `subfunc1` <br/> `subfunc2` <br/> `subsubfunc1` | `B_work` | Already been scanned. |
| 23 | `libA.a` | `subfunc3.o` | Skipped | `main` <br/> `func1` <br/> `subfunc1` <br/> `subfunc2` <br/> `subsubfunc1` | `B_work` | Doesn't export any symbols that are being requested. |
| 24 | `libA.a` | `func1.o` | Skipped | `main` <br/> `func1` <br/> `subfunc1` <br/> `subfunc2` <br/> `subsubfunc1` | `B_work` | Already been scanned. |
| 25 | `libA.a` | `func2.o` | Skipped | `main` <br/> `func1` <br/> `subfunc1` <br/> `subfunc2` <br/> `subsubfunc1` | `B_work` | Doesn't export any symbols that are being requested. |
| 26 | `libB.a` | `B.o` | Scanned | `main` <br/> `func1` <br/> `subfunc1` <br/> `subfunc2` <br/> `subsubfunc1` <br/> `B_work` || Exports `B_work` that is being requested. |

Therefore, the process can be described as follows:
- Finally, if _any_ of the object files in the library has been included in the link, the library is re-scanned.
- In the re-scanning, if _any_ of the object files in the library is included in the link, the library is re-scanned again. This keeps going on until no new object file is included in the link.

## 3. Demo

The linker library in the sub-directory `demo/linker` is an implementation of the linking process that is explained in the section "2. Static Library Re-scanning".

The demo `demo/linker-demo.cpp` implements the experiment in `static-lib-rescanning` to show how the linker works.

The demo `demo/simple-examples.cpp` implements the examples in [1].

## 4. Other Takeaways

[1] mentions two other options in [2] that can help resolve the dependency issue:

- `--start-group archives --end-group`:

> The specified archives are searched repeatedly until no new undefined references are
> created.  Normally, an archive is searched only once in the order that it is specified
> on the command line.  If a symbol in that archive is needed to resolve an undefined
> symbol referred to by an object in an archive that appears later on the command line,
> the linker would not be able to resolve that reference.  By grouping the archives,
> they all be searched repeatedly until all possible references are resolved.
>
> Using this option has a significant performance cost.  It is best to use it only when
> there are unavoidable circular references between two or more archives.

- `--undefined`

> Force symbol to be entered in the output file as an undefined symbol.  Doing this may,
> for example, trigger linking of additional modules from standard libraries.  -u may be
> repeated with different option arguments to enter additional undefined symbols.  This
> option is equivalent to the "EXTERN" linker script command.
>
> If this option is being used to force additional modules to be pulled into the link,
> and if it is an error for the symbol to remain undefined, then the option
> `--require-defined` should be used instead.

## 5. References

- [1] [Library order in static linking](https://eli.thegreenplace.net/2013/07/09/library-order-in-static-linking)
- [2] [`ld(1)` manpage](https://manpages.ubuntu.com/manpages/bionic/man1/ld.1.html)
