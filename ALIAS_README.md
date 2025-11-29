# Alias Functionality in cxxopts

## Overview
This implementation adds alias functionality to the cxxopts library, allowing users to create alternative names for options.

## Usage

### Adding Aliases
```cpp
Options options("test", "Test alias functionality");
options.add_option("", "h", "help", "Print help", value<bool>());
options.add_option("", "o", "output", "Output file", value<std::string>());

// Add aliases
options.add_alias("h", "?");
options.add_alias("help", "info");
options.add_alias("o", "out");
options.add_alias("output", "target");
```

### Retrieving Aliases
```cpp
// Get aliases for an option
std::vector<std::string> aliases_h = options.get_aliases("h");
std::vector<std::string> aliases_output = options.get_aliases("output");
```

### Removing Aliases
```cpp
// Remove an alias
options.remove_alias("?");
options.remove_alias("info");
```

### Parsing with Aliases
```cpp
const char* argv[] = {"test", "-?", "--target", "test.txt", nullptr};
int argc = 4;

ParseResult result = options.parse(argc, argv);

// Original option names work
assert(result["h"].as<bool>() == true);
assert(result["output"].as<std::string>() == "test.txt");

// Aliases also work
assert(result["?"].as<bool>() == true);
assert(result["target"].as<std::string>() == "test.txt");
```

### Help Text with Aliases
```cpp
std::string help_text = options.help();
// Help text will include aliases like "(aliases: ?)" for the help option
```

## Features

1. **Alias Resolution**: Aliases are resolved to their original option names during parsing.
2. **Cycle Detection**: The implementation detects cycles in alias chains and throws an exception.
3. **Conflict Detection**: Conflicts with existing options or aliases are detected and throw an exception.
4. **Alias Display**: Aliases are displayed in the help text.

## Testing

A test file `test_aliases.cpp` is included to verify the alias functionality. To run the tests:
1. Install a C++ compiler (like MinGW or MSVC)
2. Compile the test file: `g++ -std=c++11 -Iinclude test_aliases.cpp -o test_aliases.exe`
3. Run the test: `test_aliases.exe`

## Implementation Details

- **Options Class**: Added methods for adding, removing, and getting aliases, as well as resolving aliases.
- **OptionParser Class**: Added alias resolution logic in the parse method.
- **Help Text Generation**: Modified to include aliases in the help output.
- **Data Structures**: Added maps to store alias relationships between options and aliases.
