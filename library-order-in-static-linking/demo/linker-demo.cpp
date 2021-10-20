#include "linker/linker.hpp"
#include "linker/object_file.hpp"
#include "linker/static_lib.hpp"
#include "linker/symbol.hpp"
#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

TEST(demos, static_lib_rescanning)
{
    ObjectFile main(
        bfs::path("main.o"),
        { Symbol("main") }, // exported
        { Symbol("func1") } // imported
    );

    StaticLib A("libA.a");
    {
        ObjectFile A_subsubfunc1(
            bfs::path("A_subsubfunc1.o"),
            { Symbol("subsubfunc1") }, // exported
            {}                         // imported
        );

        ObjectFile A_subfunc1(
            bfs::path("A_subfunc1.o"),
            { Symbol("subfunc1") }, // exported
            {}                      // imported
        );

        ObjectFile A_subfunc2(
            bfs::path("A_subfunc2.o"),
            { Symbol("subfunc2") },                     // exported
            { Symbol("subsubfunc1"), Symbol("B_work") } // imported
        );

        ObjectFile A_subfunc3(
            bfs::path("A_subfunc3.o"),
            { Symbol("subfunc3") }, // exported
            {}                      // imported
        );

        ObjectFile A_func1(
            bfs::path("A_func1.o"),
            { Symbol("func1") },                       // exported
            { Symbol("subfunc1"), Symbol("subfunc2") } // imported
        );

        ObjectFile A_func2(
            bfs::path("A_func2.o"),
            { Symbol("func2") },   // exported
            { Symbol("subfunc3") } // imported
        );

        A.obj_files.push_back(A_subsubfunc1);
        A.obj_files.push_back(A_subfunc1);
        A.obj_files.push_back(A_subfunc2);
        A.obj_files.push_back(A_subfunc3);
        A.obj_files.push_back(A_func1);
        A.obj_files.push_back(A_func2);
    }

    StaticLib B("libB.a");
    {
        ObjectFile B_o(
            bfs::path("B.o"),
            { Symbol("B_work") }, // exported
            {}                    // imported
        );

        B.obj_files.push_back(B_o);
    }

    {
        // Link in the correct order and we will have no undefined symbols.
        Linker linker;
        linker.link(main);
        linker.link(A);
        linker.link(B);

        std::set<Symbol> undefined = linker.report_undefined();
        EXPECT_TRUE(undefined.empty());
    }

    {
        // Link in the wrong order and we will have the symbol "B_work"
        // unresolved.
        Linker linker;
        linker.link(main);
        linker.link(B);
        linker.link(A);

        std::set<Symbol> undefined = linker.report_undefined();
        EXPECT_EQ(undefined.size(), size_t(1));
        EXPECT_TRUE(undefined.find(Symbol("B_work")) != undefined.end());
    }
}
