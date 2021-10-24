#include "linker/linker.hpp"
#include "linker/object_file.hpp"
#include "linker/static_lib.hpp"
#include "linker/symbol.hpp"
#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <iostream>

namespace bfs = boost::filesystem;

/// Print the linker's states in the examples.
class ExampleLinkerStatePrinter
{
public:
    /// Constructor
    ExampleLinkerStatePrinter(Linker const &linker) : m_linker(linker)
    {
        std::cout << std::string(10U, '-') << std::endl;
    }

    void
    report_linker_states(size_t leading_spaces = 0)
    {
        std::string s(leading_spaces, ' ');
        std::cout << s << m_linker.str() << std::endl;
    }

    void
    print_actions(ObjectFileProcessing const &p, size_t leading_spaces = 0)
    {
        std::string s(leading_spaces, ' ');
        std::cout << s << p.str() << std::endl;
    }

    void
    print_actions(std::vector<ObjectFileProcessing> const &actions, size_t leading_spaces = 0)
    {
        for (auto const &a: actions)
        {
            print_actions(a, leading_spaces);
        }
    }

    /// Destructor
    ~ExampleLinkerStatePrinter()
    {
        std::cout << std::string(10U, '-') << std::endl;
    }

private:
    Linker const &m_linker;
};

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
        ExampleLinkerStatePrinter p(linker);

        p.print_actions(linker.link(simple_main));
        p.report_linker_states(4);

        p.print_actions(linker.link(simple_func));
        p.report_linker_states(4);

        std::set<Symbol> undefined = linker.report_undefined();
        EXPECT_TRUE(undefined.empty());
    }

    {
        // Linking `simple_func` and then `simple_main` can succeed, too,
        // because "these are object files, the linking order does not matter.
        // Object files are always taken into the link."
        Linker linker;
        ExampleLinkerStatePrinter p(linker);

        p.print_actions(linker.link(simple_func));
        p.report_linker_states(4);

        p.print_actions(linker.link(simple_main));
        p.report_linker_states(4);

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
        ExampleLinkerStatePrinter p(linker);

        p.print_actions(linker.link(simple_main));
        p.report_linker_states(4);

        p.print_actions(linker.link(libsimplefunc));
        p.report_linker_states(4);

        std::set<Symbol> undefined = linker.report_undefined();
        EXPECT_TRUE(undefined.empty());
    }

    {
        // But linking the static library `libsimplefunc.a` first would result
        // in the error "undefined reference to 'func'".
        Linker linker;
        ExampleLinkerStatePrinter p(linker);

        p.print_actions(linker.link(libsimplefunc));
        p.report_linker_states(4);

        p.print_actions(linker.link(simple_main));
        p.report_linker_states(4);

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
        ExampleLinkerStatePrinter p(linker);

        p.print_actions(linker.link(simple_main));
        p.report_linker_states(4);

        p.print_actions(linker.link(libbar_dep));
        p.report_linker_states(4);

        p.print_actions(linker.link(libfunc_dep));
        p.report_linker_states(4);

        std::set<Symbol> undefined = linker.report_undefined();
        EXPECT_EQ(undefined.size(), size_t(1));
        EXPECT_TRUE(undefined.find(Symbol("bar")) != undefined.end());
    }

    {
        // Quiz 1
        Linker linker;
        ExampleLinkerStatePrinter p(linker);

        p.print_actions(linker.link(simple_main));
        p.report_linker_states(4);

        p.print_actions(linker.link(libfunc_dep));
        p.report_linker_states(4);

        p.print_actions(linker.link(libbar_dep));
        p.report_linker_states(4);

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
        ExampleLinkerStatePrinter p(linker);

        p.print_actions(linker.link(simple_main));
        p.report_linker_states(4);

        p.print_actions(linker.link(libfunc_dep));
        p.report_linker_states(4);

        p.print_actions(linker.link(libbar_dep));
        p.report_linker_states(4);

        std::set<Symbol> undefined = linker.report_undefined();
        EXPECT_EQ(undefined.size(), size_t(1));
        EXPECT_TRUE(undefined.find(Symbol("frodo")) != undefined.end());
    }

    {
        Linker linker;
        ExampleLinkerStatePrinter p(linker);

        p.print_actions(linker.link(simple_main));
        p.report_linker_states(4);

        p.print_actions(linker.link(libbar_dep));
        p.report_linker_states(4);

        p.print_actions(linker.link(libfunc_dep));
        p.report_linker_states(4);

        std::set<Symbol> undefined = linker.report_undefined();
        EXPECT_EQ(undefined.size(), size_t(1));
        EXPECT_TRUE(undefined.find(Symbol("bar")) != undefined.end());
    }

    {
        // Quiz 2: Even providing `libbar_dep` twice wouldn't help.
        Linker linker;
        ExampleLinkerStatePrinter p(linker);

        p.print_actions(linker.link(simple_main));
        p.report_linker_states(4);

        p.print_actions(linker.link(libbar_dep));
        p.report_linker_states(4);

        p.print_actions(linker.link(libfunc_dep));
        p.report_linker_states(4);

        p.print_actions(linker.link(libbar_dep));
        p.report_linker_states(4);

        std::set<Symbol> undefined = linker.report_undefined();
        EXPECT_EQ(undefined.size(), size_t(1));
        EXPECT_TRUE(undefined.find(Symbol("frodo")) != undefined.end());
    }
}
