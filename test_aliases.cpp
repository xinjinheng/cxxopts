#include "cxxopts.hpp"
#include <iostream>
#include <cassert>

using namespace cxxopts;

int main() {
  // Test 1: Basic alias functionality
  try {
    Options options("test_aliases", "Test for alias functionality");
    options.add_option("", "h", "help", "Print help", value<bool>());
    options.add_option("", "v", "version", "Print version", value<bool>());
    options.add_option("", "o", "output", "Output file", value<std::string>());

    // Add aliases
    options.add_alias("h", "?");
    options.add_alias("help", "info");
    options.add_alias("o", "out");
    options.add_alias("output", "target");

    // Verify aliases
    auto aliases_h = options.get_aliases("h");
    assert(aliases_h.size() == 1);
    assert(aliases_h[0] == "?");

    auto aliases_help = options.get_aliases("help");
    assert(aliases_help.size() == 1);
    assert(aliases_help[0] == "info");

    auto aliases_o = options.get_aliases("o");
    assert(aliases_o.size() == 1);
    assert(aliases_o[0] == "out");

    auto aliases_output = options.get_aliases("output");
    assert(aliases_output.size() == 1);
    assert(aliases_output[0] == "target");

    std::cout << "Test 1 passed: Basic alias functionality\n";
  } catch (const std::exception& e) {
    std::cerr << "Test 1 failed: " << e.what() << std::endl;
    return 1;
  }

  // Test 2: Alias resolution
  try {
    Options options("test_aliases", "Test for alias resolution");
    options.add_option("", "a", "alpha", "Alpha option", value<bool>());
    options.add_alias("a", "b");
    options.add_alias("b", "c");
    options.add_alias("c", "d");

    assert(options.resolve_alias("a") == "a");
    assert(options.resolve_alias("b") == "a");
    assert(options.resolve_alias("c") == "a");
    assert(options.resolve_alias("d") == "a");

    std::cout << "Test 2 passed: Alias resolution\n";
  } catch (const std::exception& e) {
    std::cerr << "Test 2 failed: " << e.what() << std::endl;
    return 1;
  }

  // Test 3: Alias conflict detection
  try {
    Options options("test_aliases", "Test for alias conflict detection");
    options.add_option("", "x", "xray", "Xray option", value<bool>());
    options.add_option("", "y", "yank", "Yank option", value<bool>());

    options.add_alias("x", "y"); // This should throw
    std::cerr << "Test 3 failed: Expected exception for alias conflict\n";
    return 1;
  } catch (const std::invalid_argument& e) {
    if (std::string(e.what()).find("conflicts with existing option") == std::string::npos) {
      std::cerr << "Test 3 failed: Unexpected error message: " << e.what() << std::endl;
      return 1;
    }
    std::cout << "Test 3 passed: Alias conflict detection\n";
  } catch (const std::exception& e) {
    std::cerr << "Test 3 failed: Unexpected exception type: " << e.what() << std::endl;
    return 1;
  }

  // Test 4: Cycle detection
  try {
    Options options("test_aliases", "Test for cycle detection");
    options.add_option("", "a", "alpha", "Alpha option", value<bool>());
    options.add_option("", "b", "beta", "Beta option", value<bool>());
    options.add_option("", "c", "gamma", "Gamma option", value<bool>());

    options.add_alias("a", "b");
    options.add_alias("b", "c");
    options.add_alias("c", "a"); // This should throw
    std::cerr << "Test 4 failed: Expected exception for cycle\n";
    return 1;
  } catch (const std::invalid_argument& e) {
    if (std::string(e.what()).find("Cycle detected") == std::string::npos) {
      std::cerr << "Test 4 failed: Unexpected error message: " << e.what() << std::endl;
      return 1;
    }
    std::cout << "Test 4 passed: Cycle detection\n";
  } catch (const std::exception& e) {
    std::cerr << "Test 4 failed: Unexpected exception type: " << e.what() << std::endl;
    return 1;
  }

  // Test 5: Alias removal
  try {
    Options options("test_aliases", "Test for alias removal");
    options.add_option("", "f", "file", "File option", value<std::string>());
    options.add_alias("f", "input");
    options.add_alias("f", "in");

    assert(options.get_aliases("f").size() == 2);

    options.remove_alias("input");
    auto aliases_f = options.get_aliases("f");
    assert(aliases_f.size() == 1);
    assert(aliases_f[0] == "in");

    options.remove_alias("in");
    assert(options.get_aliases("f").empty());

    std::cout << "Test 5 passed: Alias removal\n";
  } catch (const std::exception& e) {
    std::cerr << "Test 5 failed: " << e.what() << std::endl;
    return 1;
  }

  // Test 6: Parse with aliases
  try {
    Options options("test_aliases", "Test for parsing with aliases");
    options.add_option("", "v", "version", "Print version", value<bool>()->default_value("false"));
    options.add_option("", "o", "output", "Output file", value<std::string>());

    options.add_alias("v", "V");
    options.add_alias("output", "target");

    const char* argv[] = {"test_aliases", "-V", "--target", "test.txt", nullptr};
    int argc = 4;

    ParseResult result = options.parse(argc, argv);

    assert(result["v"].as<bool>() == true);
    assert(result["V"].as<bool>() == true);
    assert(result["version"].as<bool>() == true);

    assert(result["o"].as<std::string>() == "test.txt");
    assert(result["output"].as<std::string>() == "test.txt");
    assert(result["target"].as<std::string>() == "test.txt");

    std::cout << "Test 6 passed: Parse with aliases\n";
  } catch (const std::exception& e) {
    std::cerr << "Test 6 failed: " << e.what() << std::endl;
    return 1;
  }

  // Test 7: Help with aliases
  try {
    Options options("test_aliases", "Test for help with aliases");
    options.add_option("", "h", "help", "Print help", value<bool>());
    options.add_option("", "o", "output", "Output file", value<std::string>());

    options.add_alias("h", "?");
    options.add_alias("output", "target");

    std::string help_text = options.help();
    assert(help_text.find("aliases: ?") != std::string::npos);
    assert(help_text.find("aliases: target") != std::string::npos);

    std::cout << "Test 7 passed: Help with aliases\n";
  } catch (const std::exception& e) {
    std::cerr << "Test 7 failed: " << e.what() << std::endl;
    return 1;
  }

  std::cout << "All tests passed!\n";
  return 0;
}
