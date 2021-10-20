#include "linker/linker.hpp"
#include "linker/object_file.hpp"
#include "linker/static_lib.hpp"
#include "linker/symbol.hpp"
#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

TEST(demos, simple_example_both_object_files)
{
    ObjectFile simple_main(
        bfs::path("simplemain.o"),
        { Symbol("main") }, // exported
        { Symbol("func") }  // imported
    );

    ObjectFile simple_func(
        bfs::path("simplefunc.o"),
        { Symbol("func") }, // exported
        {}                  // imported
    );

    {
        // Linking `simple_main` and then `simple_func` can succeed.
        Linker linker;
        linker.link(simple_main);
        linker.link(simple_func);

        std::set<Symbol> undefined = linker.report_undefined();
        EXPECT_TRUE(undefined.empty());
    }

    {
        // Linking `simple_func` and then `simple_main` can succeed, too,
        // because "these are object files, the linking order does not matter.
        // Object files are always taken into the link."
        Linker linker;
        linker.link(simple_func);
        linker.link(simple_main);

        std::set<Symbol> undefined = linker.report_undefined();
        EXPECT_TRUE(undefined.empty());
    }
}

TEST(demos, simple_example_libsimplefunc_a)
{
    ObjectFile simple_main(
        bfs::path("simplemain.o"),
        { Symbol("main") }, // exported
        { Symbol("func") }  // imported
    );

    StaticLib libsimplefunc("libsimplefunc.a");
    ObjectFile simple_func(
        bfs::path("simplefunc.o"),
        { Symbol("func") }, // exported
        {}                  // imported
    );
    libsimplefunc.obj_files.push_back(simple_func);

    {
        // Linking the object file `simple_main.o` followed by the static
        // library `libsimplefunc.a` works.
        Linker linker;
        linker.link(simple_main);
        linker.link(libsimplefunc);

        std::set<Symbol> undefined = linker.report_undefined();
        EXPECT_TRUE(undefined.empty());
    }

    {
        // But linking the static library `libsimplefunc.a` first would result
        // in the error "undefined reference to 'func'".
        Linker linker;
        linker.link(libsimplefunc);
        linker.link(simple_main);

        std::set<Symbol> undefined = linker.report_undefined();
        EXPECT_EQ(undefined.size(), size_t(1));
        EXPECT_TRUE(undefined.find(Symbol("func")) != undefined.end());
    }
}

TEST(demos, simple_example_circular_deps_1)
{
    ObjectFile simple_main(
        bfs::path("simplemain.o"),
        { Symbol("main") }, // exported
        { Symbol("func") }  // imported
    );

    StaticLib libfunc_dep(bfs::path("libfunc_dep.a"));
    {
        ObjectFile func_dep(
            bfs::path("func_dep.o"),
            { Symbol("func") }, // exported
            { Symbol("bar") }   // imported
        );

        libfunc_dep.obj_files.push_back(func_dep);
    }

    StaticLib libbar_dep(bfs::path("libbar_dep.a"));
    {
        ObjectFile bar_dep(
            bfs::path("bar_dep.o"),
            { Symbol("bar") }, // exported
            { Symbol("func") } // imported
        );

        libbar_dep.obj_files.push_back(bar_dep);
    }

    {
        Linker linker;
        linker.link(simple_main);
        linker.link(libbar_dep);
        linker.link(libfunc_dep);

        std::set<Symbol> undefined = linker.report_undefined();
        EXPECT_EQ(undefined.size(), size_t(1));
        EXPECT_TRUE(undefined.find(Symbol("bar")) != undefined.end());
    }

    {
        Linker linker;
        linker.link(simple_main);
        linker.link(libfunc_dep);
        linker.link(libbar_dep);

        std::set<Symbol> undefined = linker.report_undefined();
        EXPECT_TRUE(undefined.empty());
    }
}

TEST(demos, simple_example_circular_deps_with_frodo)
{
    ObjectFile simple_main(
        bfs::path("simplemain.o"),
        { Symbol("main") }, // exported
        { Symbol("func") }  // imported
    );

    StaticLib libfunc_dep(bfs::path("libfunc_dep.a"));
    {
        ObjectFile func_dep(
            bfs::path("func_dep.o"),
            { Symbol("func") }, // exported
            { Symbol("bar") }   // imported
        );

        ObjectFile frodo_dep(
            bfs::path("frodo_dep.o"),
            { Symbol("frodo") }, // exported
            {}                   // imported
        );

        libfunc_dep.obj_files.push_back(func_dep);
        libfunc_dep.obj_files.push_back(frodo_dep);
    }

    StaticLib libbar_dep(bfs::path("libbar_dep.a"));
    {
        ObjectFile bar_dep(
            bfs::path("bar_dep.o"),
            { Symbol("bar") },                  // exported
            { Symbol("func"), Symbol("frodo") } // imported
        );

        libbar_dep.obj_files.push_back(bar_dep);
    }

    {
        Linker linker;
        linker.link(simple_main);
        linker.link(libfunc_dep);
        linker.link(libbar_dep);

        std::set<Symbol> undefined = linker.report_undefined();
        EXPECT_EQ(undefined.size(), size_t(1));
        EXPECT_TRUE(undefined.find(Symbol("frodo")) != undefined.end());
    }

    {
        Linker linker;
        linker.link(simple_main);
        linker.link(libbar_dep);
        linker.link(libfunc_dep);

        std::set<Symbol> undefined = linker.report_undefined();
        EXPECT_EQ(undefined.size(), size_t(1));
        EXPECT_TRUE(undefined.find(Symbol("bar")) != undefined.end());
    }
}
