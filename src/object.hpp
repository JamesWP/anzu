#pragma once
#include <format>
#include <memory>
#include <string>
#include <variant>
#include <unordered_map>
#include <vector>
#include <span>
#include <filesystem>
#include <utility>

#include "utility/common.hpp"
#include "utility/value_ptr.hpp"

namespace anzu {

// Want these to be equivalent since we want uints available in the runtime but we also want
// to use it as indexes into C++ vectors which use size_t.
static_assert(std::is_same_v<std::uint64_t, std::size_t>);

struct type_name;

enum class type_fundamental : std::uint8_t
{
    null_type,
    bool_type,
    char_type,
    i32_type,
    i64_type,
    u64_type,
    f64_type,
    nullptr_type,
};

struct type_struct
{
    std::string            name;
    std::filesystem::path  module;
    std::vector<type_name> templates;
    auto operator==(const type_struct&) const -> bool = default;
};

struct type_array
{
    value_ptr<type_name> inner_type;
    std::size_t          count;
    auto operator==(const type_array&) const -> bool = default;
};

struct type_ptr
{
    value_ptr<type_name> inner_type;
    auto operator==(const type_ptr&) const -> bool = default;
};

struct type_span
{
    value_ptr<type_name> inner_type;
    auto operator==(const type_span&) const -> bool = default;
};

struct type_function_ptr
{
    std::vector<type_name> param_types;
    value_ptr<type_name>   return_type;
    auto operator==(const type_function_ptr&) const -> bool = default;
};

struct type_builtin
{
    std::string            name; // for printing only
    std::size_t            id;
    std::vector<type_name> args;
    value_ptr<type_name>   return_type;
    auto operator==(const type_builtin&) const -> bool = default;
};

struct type_bound_method
{
    std::vector<type_name> param_types;
    value_ptr<type_name>   return_type;
    std::string            name; // for printing only
    std::size_t            id;
    auto operator==(const type_bound_method&) const -> bool = default;
};

struct type_bound_method_template
{
    std::filesystem::path    module;
    type_struct              struct_name;
    std::string              name;
    auto operator==(const type_bound_method_template&) const -> bool = default;
};

struct type_arena
{
    auto operator==(const type_arena&) const -> bool = default;
};

// Represents a type as a value along with the compile-time value
struct type_type
{
    value_ptr<type_name> type_val;
    auto operator==(const type_type&) const -> bool = default;
};

struct type_function
{
    std::size_t            id;
    std::vector<type_name> param_types;
    value_ptr<type_name>   return_type;
    auto to_pointer() const -> type_name;
    auto operator==(const type_function&) const -> bool = default;
};

struct type_function_template
{
    std::filesystem::path    module;
    type_struct              struct_name;
    std::string              name;
    auto operator==(const type_function_template&) const -> bool = default;
};

struct type_struct_template
{
    std::filesystem::path module;
    std::string           name;
    auto operator==(const type_struct_template&) const -> bool = default;
};

struct type_module
{
    std::filesystem::path filepath;
    auto operator==(const type_module&) const -> bool = default;
};

struct type_ct_bool
{
    bool value;
    auto operator==(const type_ct_bool&) const -> bool = default;
};


struct type_name : public std::variant<
    type_fundamental,
    type_struct,
    type_array,
    type_ptr,
    type_span,
    type_arena,
    type_function_ptr,
    type_bound_method,
    type_bound_method_template,
    type_builtin,
    type_type,
    type_function,
    type_function_template,
    type_struct_template,
    type_module,
    type_ct_bool>
{
    using variant::variant;
    
    bool is_const = false;

    [[nodiscard]] auto is_fundamental() const -> bool;

    [[nodiscard]] auto is_ptr() const -> bool;
    [[nodiscard]] auto add_ptr() const -> type_name;
    [[nodiscard]] auto remove_ptr() const -> type_name;
 
    [[nodiscard]] auto add_const() const -> type_name;
    [[nodiscard]] auto remove_const() const -> type_name;

    [[nodiscard]] auto is_array() const -> bool;
    [[nodiscard]] auto add_array(std::size_t size) const -> type_name;
    [[nodiscard]] auto remove_array() const -> type_name;

    [[nodiscard]] auto is_span() const -> bool;
    [[nodiscard]] auto add_span() const -> type_name;
    [[nodiscard]] auto remove_span() const -> type_name;

    [[nodiscard]] auto is_function() const -> bool;
    [[nodiscard]] auto is_function_ptr() const -> bool;
    [[nodiscard]] auto is_builtin() const -> bool;
    [[nodiscard]] auto is_bound_method() const -> bool;
    [[nodiscard]] auto is_arena() const -> bool;

    [[nodiscard]] auto is_struct() const -> bool;
    [[nodiscard]] auto as_struct() const -> const type_struct&;

    [[nodiscard]] auto is_type_value() const -> bool;
    [[nodiscard]] auto is_module_value() const -> bool;
};

auto hash(const type_name& type) -> std::size_t;
auto hash(type_fundamental type) -> std::size_t;
auto hash(const type_struct& type) -> std::size_t;
auto hash(const type_array& type) -> std::size_t;
auto hash(const type_ptr& type) -> std::size_t;
auto hash(const type_span& type) -> std::size_t;
auto hash(const type_function_ptr& type) -> std::size_t;
auto hash(const type_builtin& type) -> std::size_t;
auto hash(const type_bound_method& type) -> std::size_t;
auto hash(const type_bound_method_template& type) -> std::size_t;
auto hash(const type_arena& type) -> std::size_t;
auto hash(const type_type& type) -> std::size_t;
auto hash(const type_function& type) -> std::size_t;
auto hash(const type_function_template& type) -> std::size_t;
auto hash(const type_struct_template& type) -> std::size_t;
auto hash(const type_module& type) -> std::size_t;
auto hash(const type_ct_bool& type) -> std::size_t;
auto hash(std::span<const type_name> types) -> std::size_t;

// Used for resolving template types. In the future could also be used for type aliases
using template_map = std::unordered_map<std::string, type_name>;

auto null_type() -> type_name;
auto nullptr_type() -> type_name;
auto bool_type() -> type_name;
auto char_type() -> type_name;
auto i32_type() -> type_name;
auto i64_type() -> type_name;
auto u64_type() -> type_name;
auto f64_type() -> type_name;
auto arena_type() -> type_name;
auto string_literal_type() -> type_name;

// Extracts the single inner type of the given t. Undefined if the given t is not a compound
// type with a single subtype.
auto inner_type(const type_name& t) -> type_name;

// Extracts the array size of the given type. Undefined if the given t is not an array
auto array_length(const type_name& t) -> std::size_t;

auto to_string(const type_name& type) -> std::string;
auto to_string(type_fundamental t) -> std::string;
auto to_string(const type_array& type) -> std::string;
auto to_string(const type_ptr& type) -> std::string;
auto to_string(const type_span& type) -> std::string;
auto to_string(const type_struct& type) -> std::string;
auto to_string(const type_function_ptr& type) -> std::string;
auto to_string(const type_builtin& type) -> std::string;
auto to_string(const type_bound_method& type) -> std::string;
auto to_string(const type_bound_method_template& type) -> std::string;
auto to_string(const type_arena& type) -> std::string;
auto to_string(const type_type& type) -> std::string;
auto to_string(const type_function& type) -> std::string;
auto to_string(const type_function_template& type) -> std::string;
auto to_string(const type_struct_template& type) -> std::string;
auto to_string(const type_module& type) -> std::string;
auto to_string(const type_ct_bool& type) -> std::string;

}

template <>
struct std::formatter<std::byte> : std::formatter<std::string>
{
    auto format(std::byte b, auto& ctx) const {
        const auto str = std::format("{:X}", static_cast<unsigned char>(b));
        return std::formatter<std::string>::format(str, ctx);
    }
};

template <>
struct std::formatter<anzu::type_name> : std::formatter<std::string>
{
    auto format(const anzu::type_name& type, auto& ctx) const {
        return std::formatter<std::string>::format(anzu::to_string(type), ctx);
    }
};

template<>
struct std::hash<anzu::type_name>
{
    auto operator()(const anzu::type_name& name) const -> std::size_t
    {
        return anzu::hash(name);
    }
};