#ifndef __STDX_EXPR_H
#define __STDX_EXPR_H


#include <unistd.h>
#include <sys/times.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <iosfwd>
#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include <stack>
#include <sstream>
#include <stdexcept>

// stdx header files
//#include "stdx/stdx_streambuf.h"

struct expression_error : public std::runtime_error
{
    explicit expression_error(const std::string& what)
        : std::runtime_error(what)
    { }
};

enum calc_tokener
{
    TOKENER_NAME,
    TOKENER_NUMBER,
    TOKENER_END,
    TOKENER_PLUS = '+',
    TOKENER_MINUS = '-',
    TOKENER_MUL = '*',
    TOKENER_DIV = '/',
    TOKENER_PRINT = ';',
    TOKENER_ASSIGN = '=',
    TOKENER_LP = '(',
    TOKENER_RP = ')',
    TOKENER_LB = '[',
    TOKENER_RB = ']',
};

class expression
{
private:
    calc_tokener curr_tok_;
    double value_;
    std::string identifier_;
    std::map<std::string,double> table_;
    std::istream* input_;
    std::stack<bool> expr_type_; // int is true, and double is false

public:
    expression(std::istream* input, const std::map<std::string,double>& tbl)
        : curr_tok_(TOKENER_PRINT), value_(0.0),
          input_(input)
    {
    //  table_["pi"] = 3.1415926535897932385;
    //  table_["e"] = 2.7182818284590452354;
        table_.insert(tbl.begin(), tbl.end());
    }

    double error(const std::string& s)
    {
        throw expression_error(s);
        return 1;
    }

    inline calc_tokener get_token();
    inline double prim(bool get);
    inline double term(bool get);
    inline double expr(bool get);
    inline double execute();
};

calc_tokener expression::get_token()
{
    char ch = 0;
    do
    {
        if (!(*input_).get(ch))
            return curr_tok_ = TOKENER_END;
    } while (ch != '\n' && isspace(ch));
    switch (ch)
    {
    case '\0':
        return curr_tok_ = TOKENER_END;

    case ';':
    case '\n':
        return curr_tok_ = TOKENER_PRINT;

    case '+': case '-': case '*': case '/':
    case '(': case ')': case '=':
    case '[': case ']':
        return curr_tok_ = calc_tokener(ch);

    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    case '.':
        (*input_).putback(ch);
        (*input_) >> value_;
        if (!expr_type_.empty() && expr_type_.top())
            value_ = static_cast<int>(value_);
        return curr_tok_ = TOKENER_NUMBER;

    default:
        if (isalpha(ch))
        {
            identifier_ = ch;
            while ((*input_).get(ch) && isalnum(ch))
                identifier_.push_back(ch);
            (*input_).putback(ch);
            return curr_tok_ = TOKENER_NAME;
        }
        error("bad token");
        return curr_tok_ = TOKENER_PRINT;
    }
}

double expression::prim(bool get)
{
    if (get)
        get_token();

    switch (curr_tok_)
    {
    case TOKENER_NUMBER:
        {
            double v = value_;
            get_token();
            return v;
        }
        break;
    case TOKENER_NAME:
        {
            double& v = table_[identifier_];
            if (get_token() == TOKENER_ASSIGN)
                v = expr(true);
            if (!expr_type_.empty() && expr_type_.top())
                v = static_cast<int>(v);
            return v;
        }
        break;
    case TOKENER_MINUS:
        return -prim(true);
        break;
    case TOKENER_LP:
        {
            expr_type_.push(false);
            double e = expr(true);
            if (curr_tok_ != TOKENER_RP)
                return error(") expected");
            expr_type_.pop();
            get_token();
            return e;
        }
        break;
    case TOKENER_LB:
        {
            expr_type_.push(true);
            double e = expr(true);
            if (curr_tok_ != TOKENER_RB)
                return error("] expected");
            e = static_cast<int>(e);
        //  std::cout << "out ] " << e << std::endl;
            expr_type_.pop();
            get_token();
            return e;
        }
        break;
    default:
        return error("primary expected");
    }
}

double expression::term(bool get)
{
    double left = prim(get);
    while (true)
    {
        switch (curr_tok_)
        {
        case TOKENER_MUL:
            left *= prim(true);
            break;
        case TOKENER_DIV:
            if (double d = prim(true))
            {
                left /= d;
                break;
            }
            return error("divide by 0");
        default:
            return left;
            break;
        }
    }
}

double expression::expr(bool get)
{
    double left = term(get);
    while (true)
    {
        switch (curr_tok_)
        {
        case TOKENER_PLUS:
            left += term(true);
            break;
        case TOKENER_MINUS:
            left -= term(true);
            break;
        default:
            return left;
            break;
        }
    }
}

double expression::execute()
{
    double result = 0.0;
    while (*input_)
    {
        get_token();
        if (curr_tok_ == TOKENER_END)
            break;
        if (curr_tok_ == TOKENER_PRINT)
            continue;
        result = expr(false);
    }
    return result;
}

inline size_t find_identifiers(const std::string& strexpr, std::vector<std::string>& ids)
{
//  std::auto_ptr<std::istream> input(new std::istringstream(strexpr));

    const std::string alpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string alnum = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    size_t pos = 0;
    while (pos != std::string::npos)
    {
        std::string id;
        pos = strexpr.find_first_of(alpha, pos);
        if (pos == std::string::npos)
            break;
        size_t next = strexpr.find_first_not_of(alnum, pos);
        if (next == std::string::npos)
            id = strexpr.substr(pos);
        else
            id = strexpr.substr(pos, next - pos);
        if (std::find(ids.begin(), ids.end(), id) == ids.end())
            ids.push_back(id);
        pos = next;
        std::cout << id << std::endl;
    }
    return ids.size();
}

/*
static bool parse_params(const std::string& strexpr, std::deque<bool>& iports)
{
    const std::string alpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string alnum = alpha + "0123456789";
    const std::string symbol = alpha + "()[]";

    std::stack<bool> type_stack;
    std::vector<std::string> ids;

    size_t pos = 0;
    while (pos != std::string::npos)
    {
        std::string id;
        pos = strexpr.find_first_of(symbol, pos);
        if (pos == std::string::npos)
            break;
        if (strexpr[pos] == '[' || strexpr[pos] == '(')
        {
            bool float_type = (strexpr[pos] == '(');
            type_stack.push(float_type);
            ++pos;
        }
        else if (strexpr[pos] == ']' || strexpr[pos] == ')')
        {
            type_stack.pop();
            ++pos;
        }
        else
        {
            size_t next = strexpr.find_first_not_of(alnum, pos);
            if (next == std::string::npos)
                id = strexpr.substr(pos);
            else
                id = strexpr.substr(pos, next - pos);
            if (std::find(ids.begin(), ids.end(), id) == ids.end())
            {
                ids.push_back(id);
                if (type_stack.empty() || type_stack.top())
                    iports.push_back(true);
                else
                    iports.push_back(false);
            }
            pos = next;
        }
        std::cout << id << std::endl;
    }
    return ids.size();
}
*/

inline size_t identifier_count(const std::string& strexpr)
{
    std::vector<std::string> ids;
    return find_identifiers(strexpr, ids);
}

inline double calculate(const std::string& strexpr, const std::vector<double>& vals)
{
    std::map<std::string,double> args;
    std::vector<std::string> ids;
    if (find_identifiers(strexpr, ids) != vals.size())
        throw expression_error("variable and parameter don't match.");
    for (size_t i = 0; i < ids.size(); ++i)
    {
        std::cout << ids[i] << "," << vals[i] << std::endl;
        args.insert(std::make_pair(ids[i], vals[i]));
    }

    double result = 0.0;
    std::istream* input = new std::istringstream(strexpr);
    expression ex(input, args);
    result = ex.execute();
    delete input;
    return result;
}

/*
int main(int argc, char* argv[])
{
    const std::string strexpr = "x*y*(x+y)*y";
    try
    {
        std::vector<double> vals;
        vals.push_back(1.0);
        vals.push_back(2.0);
        double result = calculate(strexpr, vals);
        std::cout << result << '\n';
    }
    catch (expression_error& e)
    {
        std::cerr << "invalid expressin: " << strexpr << std::endl;
        std::cerr << "error information: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "error happened in expressin: " << strexpr << std::endl;
    }
    return 0;
}
*/


#endif // __STDX_EXPR_H

// ex:set tabstop=4 shiftwidth=4 expandtab:
