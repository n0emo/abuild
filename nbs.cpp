#include <unordered_map>
#include <unordered_set>
#define NBS_IMPLEMENTATION
#include "nbs.hpp"

using namespace nbs;
using namespace log;
using namespace os;

void build()
{
    c::CompileOptions options{
        .compiler = c::current_compiler(), .flags = {"-Wall"},
        // .include_paths = {Path(".")},
    };

    auto obj_proc = options.obj_cmd("hello.o", "hello.cpp").run_async();
    if (!obj_proc.await())
        return;
    auto exe_proc = options.exe_cmd("hello", {"hello.o"}).run_async();
    exe_proc.await();
}

void test_levels()
{
    std::unordered_map<std::string, std::unordered_set<std::string>> graph;
    graph.insert({"1", {"2", "3", "4"}});
    graph.insert({"2", {"4"}});
    graph.insert({"3", {"4"}});
    graph.insert({"4", {}});

    auto result = nbs::graph::tolopogical_levels<std::string>(graph, "1").value();

    for (const auto &pair : graph)
    {
        std::cout << pair.first << " ->";
        for (const auto &edge : pair.second)
        {
            std::cout << " " << edge;
        }
        std::cout << '\n';
    }

    for (size_t level = 0; level < result.size(); level++)
    {
        std::cout << level << ":";
        for (const auto &vertex : result[level])
        {
            std::cout << " " << vertex;
        }
        std::cout << '\n';
    }
    std::cout << std::endl;
}

int main(int argc, char **argv)
{
    log::info("Testing bfs");
    test_levels();
    return 0;

    os::Path build_path("build");
    info(("nbs" + build_path + "debug").str());
    self_update(argc, argv, __FILE__);
    info("Starting build");

    std::string subcommand;
    if (argc == 1 || (subcommand = argv[1]) == "build")
    {
        build();
    }
    else if (subcommand == "clean")
    {
        Cmd({"rm", "-f", "*.o", "hello"}).run_or_die("Error cleaning directory");
    }
    else if (subcommand == "run")
    {
        build();
        Cmd("./hello").run_or_die("Error executing hello");
    }

    return 0;
}
