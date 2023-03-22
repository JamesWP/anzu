#pragma once
#include <string>
#include <format>
#include <source_location>
#include <vector>

namespace anzu {

enum class token_type
{
    ampersand_ampersand,
    ampersand,
    arrow,
    bang_equal,
    bang,
    bar_bar,
    bar,
    character,
    colon_equal,
    colon,
    comma,
    dot,
    eof,
    equal_equal,
    equal,
    float64,
    greater_equal,
    greater,
    identifier,
    int32,
    int64,
    kw_assert,
    kw_bool,
    kw_break,
    kw_char,
    kw_continue,
    kw_default,
    kw_delete,
    kw_else,
    kw_f64,
    kw_false,
    kw_for,
    kw_function,
    kw_i32,
    kw_i64,
    kw_if,
    kw_import,
    kw_in,
    kw_loop,
    kw_new,
    kw_null,
    kw_return,
    kw_sizeof,
    kw_struct,
    kw_true,
    kw_typeof,
    kw_u64,
    kw_while,
    left_brace,
    left_bracket,
    left_paren,
    less_equal,
    less,
    minus,
    percent,
    plus,
    right_brace,
    right_bracket,
    right_paren,
    semicolon,
    slash,
    star,
    string,
    uint64,
};

struct token
{
    std::string_view text;
    std::size_t      line;
    std::size_t      col;
    token_type       type;

    [[noreturn]] void error(std::string_view msg) const;

    template <typename... Args>
    [[noreturn]] void error(std::string_view msg, Args&&... args) const
    {
        error(std::format(msg, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void assert(bool condition, std::string_view msg, Args&&... args) const
    {
        if (!condition) error(std::format(msg, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void assert_eq(const auto& lhs, const auto& rhs, std::string_view msg, Args&&... args) const
    {
        if (lhs != rhs) {
            const auto user_msg = std::format(msg, std::forward<Args>(args)...);
            error("{}: expected {}, got {}", user_msg, rhs, lhs);
        }
    }

    template <typename... Args>
    void assert_type(token_type tt, std::string_view msg, Args&&... args) const
    {
        if (tt != type) {
            const auto user_msg = std::format(msg, std::forward<Args>(args)...);
            error("{}: expected token type '{}', got '{}'", user_msg, tt, type);
        }
    }
};

auto print_token(token tok) -> void;
auto to_string(token_type tt) -> std::string_view;
    
}

template <> struct std::formatter<anzu::token_type> : std::formatter<std::string_view> {
    auto format(const anzu::token_type& tt, auto& ctx) {
        return std::formatter<std::string_view>::format(to_string(tt), ctx);
    }
};