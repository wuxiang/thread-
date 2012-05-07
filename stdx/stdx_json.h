#ifndef __STDX_JSON_H
#define __STDX_JSON_H


// C++ 98 header files
#include <vector>
#include <string>

// stdx header files
#include "stdx_string.h"

namespace stdx {


#define hexdigit(x) (((x) <= '9') ? (x) - '0' : ((x) & 7) + 9)
#define error_ptr(error) ((void*)error)
#define is_error(ptr) ((unsigned long)ptr > (unsigned long)-4000L)


/* supported object types */

typedef enum json_type {
    json_type_null,
    json_type_boolean,
    json_type_double,
    json_type_int,
    json_type_object,
    json_type_array,
    json_type_string
} json_type;


class json_node
{
private:
    enum json_type m_type;

public:
    int _ref_count;

public:
    json_node(enum json_type type) : m_type(type), _ref_count(1)
    { }

    virtual ~json_node()
    { }

    virtual bool get_boolean() const
    { return false; }

    virtual int get_int() const
    { return 0; }

    virtual double get_double() const
    { return 0.0; }

    virtual std::string get_string() const
    { return this->to_json_string(); }

//  virtual json_array* get_array() const
//  { return NULL; }

//  virtual json_object* get_table() const
//  { return NULL; }

    virtual std::string to_json_string() const = 0;
};


class json_null : public json_node
{
public:
    json_null() : json_node(json_type_null)
    { }

    virtual bool get_boolean() const
    { return false; }

    virtual int get_int() const
    { return 0; }

    virtual double get_double() const
    { return 0.0; }

    virtual std::string get_string() const
    { return this->to_json_string(); }

//  virtual json_array* get_array() const
//  { return NULL; }

//  virtual json_object* get_table() const
//  { return NULL; }

    virtual std::string to_json_string() const
    {
        return "null";
    }
};

class json_boolean : public json_node
{
private:
    bool m_val;

public:
    json_boolean(bool val) : json_node(json_type_boolean), m_val(val)
    { }

    virtual bool get_boolean() const
    { return m_val; }

    virtual int get_int() const
    { return static_cast<int>(m_val); }

    virtual double get_double() const
    { return m_val; }

    virtual std::string get_string() const
    { return this->to_json_string(); }

//  virtual json_array* get_array() const
//  { return NULL; }

//  virtual json_object* get_table() const
//  { return NULL; }

    virtual std::string to_json_string() const
    {
        return m_val ? "true" : "false";
    }
};

class json_int : public json_node
{
private:
    int m_val;

public:
    json_int(int val) : json_node(json_type_int), m_val(val)
    { }

    virtual bool get_boolean() const
    { return m_val != 0; }

    virtual int get_int() const
    { return m_val; }

    virtual double get_double() const
    { return m_val; }

    virtual std::string get_string() const
    { return this->to_json_string(); }

//  virtual json_array* get_array() const
//  { return NULL; }

//  virtual json_object* get_table() const
//  { return NULL; }

    virtual std::string to_json_string() const
    {
        return to_string(m_val);
    }
};

class json_double : public json_node
{
private:
    double m_val;

public:
    json_double(double val) : json_node(json_type_double), m_val(val)
    { }

    virtual bool get_boolean() const
    { return m_val != 0; }

    virtual int get_int() const
    { return static_cast<int>(m_val); }

    virtual double get_double() const
    { return m_val; }

    virtual std::string get_string() const
    { return this->to_json_string(); }

//  virtual json_array* get_array() const
//  { return NULL; }

//  virtual json_object* get_table() const
//  { return NULL; }

    virtual std::string to_json_string() const
    {
        return to_string(m_val);
    }
};

class json_string : public json_node
{
private:
    std::string m_val;

public:
    json_string(const std::string& val) : json_node(json_type_string), m_val(val)
    { }

    virtual bool get_boolean() const
    { return !m_val.empty(); }

    virtual int get_int() const
    {
        int val = stdx::from_string<int>(m_val);
        return val;
    }

    virtual double get_double() const
    {
        return stdx::from_string<double>(m_val);
    }

    virtual std::string get_string() const
    { return this->m_val; }

//  virtual json_array* get_array() const
//  { return NULL; }

//  virtual json_object* get_table() const
//  { return NULL; }

    virtual std::string to_json_string() const
    {
        std::string jsonstr;
        jsonstr += "\"";
        stdx::stdx_escape_string(jsonstr, m_val);
        jsonstr += "\"";
        return jsonstr;
    }
};

class json_array : public json_node
{
private:
    typedef std::vector<json_node*>::size_type   size_type;
    std::vector<json_node*> m_vec;

public:
    json_array() : json_node(json_type_array)
    { }

    ~json_array()
    {
        for (std::vector<void*>::size_type i = 0; i < m_vec.size(); ++i)
        {
            if (m_vec[i])
            {
                delete m_vec[i];
                m_vec[i] = NULL;
            }
        }
    }

    json_node*
    at(size_type i) const
    {
        if (i >= m_vec.size())
            return NULL;
        return m_vec[i];
    }

    int
    insert(size_type idx, json_node *val)
    {
        if (idx >= m_vec.size())
            m_vec.resize(idx+1, NULL);
        if (m_vec[idx])
            delete m_vec[idx];
        m_vec[idx] = val;
        return 0;
    }

    int push_back(json_node *val)
    {
        return this->insert(m_vec.size(), val);
    }

    int size() const
    {
        return m_vec.size();
    }

    virtual bool get_boolean() const
    { return false; }

    virtual int get_int() const
    { return 0; }

    virtual double get_double() const
    { return 0.0; }

    virtual std::string get_string() const
    { return this->to_json_string(); }

//  virtual json_array* get_array() const
//  { return NULL; }

//  virtual json_object* get_table() const
//  { return NULL; }

    virtual std::string to_json_string() const
    {
        int len = this->size();
        std::string buf;
        buf += "[";
        for (int i = 0; i < len; i++)
        {
            // separator
            if (i)
            {
                buf += ", ";
            }
            else
            {
                buf += " ";
            }
            // array element
            json_node *node = (json_node*)this->at(i);
            if (node == NULL)
            {
                buf += "null";
            }
            else
            {
                buf += node->to_json_string();
            }
        }
        buf += " ]";
        return buf;
    }
};

class json_object : public json_node
{
private:
    typedef std::pair<std::string, json_node*>  json_object_entry;
    typedef std::vector<json_object_entry >     json_object_vector;

public:
    json_object_vector m_vec;

public:
    json_object() : json_node(json_type_object)
    { }

    ~json_object()
    {
        for (json_object_vector::iterator it = m_vec.begin();
                it != m_vec.end(); ++it)
        {
            delete it->second;
        }
    }

    int insert(const std::string& key, json_node* val)
    {
        m_vec.push_back(std::make_pair(key, val));
        return 0;
    }

    int erase(const std::string& key)
    {
        
        for (json_object_vector::iterator it = m_vec.begin();
                it != m_vec.end(); ++it)
        {
            if (it->first == key)
            {
                delete it->second;
                m_vec.erase(it);
                break;
            }
        }
        return 0;
    }

    json_node* find(const std::string& key)
    {
        for (json_object_vector::iterator it = m_vec.begin();
                it != m_vec.end(); ++it)
        {
            if (it->first == key)
            {
                return it->second;
            }
        }
        return NULL;
    }

    virtual bool get_boolean() const
    { return false; }

    virtual int get_int() const
    { return 0; }

    virtual double get_double() const
    { return 0.0; }

    virtual std::string get_string() const
    { return this->to_json_string(); }

//  virtual json_array* get_array() const
//  { return NULL; }

//  virtual json_object* get_table() const
//  { return NULL; }

    virtual std::string to_json_string() const
    {
        std::string buf;
        buf += "{";

        // Iterate through all keys and values of an object (ANSI C Safe)
        const json_object *table = this;
        bool with_colon = false;
        
        for (json_object_vector::const_iterator it = table->m_vec.begin();
                it != table->m_vec.end(); ++it)
        {
            const std::string& key = it->first;

            if (with_colon)
                buf += ",";
            buf += " \"";
            stdx::stdx_escape_string(buf, key);
            buf += "\": ";

            json_node *val = it->second;
            if (val == NULL)
                buf += "null";
            else
                buf += val->to_json_string();
            with_colon = true;
        }

        buf += " }";

        return buf;
    }
};


const std::string json_null_str = "null";
const std::string json_true_str = "true";
const std::string json_false_str = "false";


/*static const char* json_tokener_errors[] = {
    "success",
    "continue",
    "nesting to deep",
    "unexpected end of data",
    "unexpected character",
    "null expected",
    "boolean expected",
    "number expected",
    "array value separator ',' expected",
    "quoted object property name expected",
    "object property name separator ':' expected",
    "object value separator ',' expected",
    "invalid string sequence",
    "expected comment",
};*/

enum json_tokener_error {
    json_tokener_success,
    json_tokener_continue,
    json_tokener_error_depth,
    json_tokener_error_parse_eof,
    json_tokener_error_parse_unexpected,
    json_tokener_error_parse_null,
    json_tokener_error_parse_boolean,
    json_tokener_error_parse_number,
    json_tokener_error_parse_array,
    json_tokener_error_parse_object_key_name,
    json_tokener_error_parse_object_key_sep,
    json_tokener_error_parse_object_value_sep,
    json_tokener_error_parse_string,
    json_tokener_error_parse_comment
};

enum json_tokener_state {
    json_tokener_state_eatws,
    json_tokener_state_start,
    json_tokener_state_finish,
    json_tokener_state_null,
    json_tokener_state_comment_start,
    json_tokener_state_comment,
    json_tokener_state_comment_eol,
    json_tokener_state_comment_end,
    json_tokener_state_string,
    json_tokener_state_string_escape,
    json_tokener_state_escape_unicode,
    json_tokener_state_boolean,
    json_tokener_state_number,
    json_tokener_state_array,
    json_tokener_state_array_add,
    json_tokener_state_array_sep,
    json_tokener_state_object_field_start,
    json_tokener_state_object_field,
    json_tokener_state_object_field_end,
    json_tokener_state_object_value,
    json_tokener_state_object_value_add,
    json_tokener_state_object_sep
};

/* reference counting functions */

struct json_tokener_srec
{
    enum json_tokener_state state;
    enum json_tokener_state saved_state;
    class json_node *obj;
    class json_node *current;
    std::string obj_field_name;

    json_tokener_srec() : state(json_tokener_state_eatws),
        saved_state(json_tokener_state_eatws), obj(NULL), current(NULL)
    { }

    void reset()
    {
        state = json_tokener_state_eatws;
        saved_state = json_tokener_state_start;
        current = NULL;
        obj_field_name.clear();
    }
};

#define JSON_TOKENER_MAX_DEPTH 32

struct json_tokener
{
    int m_depth;
    bool m_double;
    std::ptrdiff_t m_err;
    std::string::size_type st_pos;
    int char_offset;
    unsigned int ucs_char;
    char quote_char;
    std::string m_buf;
    struct json_tokener_srec stack[JSON_TOKENER_MAX_DEPTH];

    json_tokener() :
        m_depth(0), m_double(false), m_err(0),
        st_pos(0), char_offset(0), ucs_char(0), quote_char('\0')
    {
        this->reset();
    }

    ~json_tokener()
    {
        this->reset();
    }

    void reset()
    {
        for (int i = m_depth; i >= 0; --i)
            reset_level(i);
        m_depth = 0;
        m_err = json_tokener_success;
    }

    void reset_level(int m_depth)
    {
        this->stack[m_depth].reset();
    }

    // peeks at the current char and stores it in dest.
    inline bool
    PEEP_CHAR(char& dest, char c, int len)
    {
        if (this->char_offset == len)
        {
            // sets tok->m_err and returns false if no more chars.
            if (this->m_depth == 0 &&
                this->stack[this->m_depth].state == json_tokener_state_eatws &&
                this->stack[this->m_depth].saved_state == json_tokener_state_finish)
            {
                this->m_err = json_tokener_success;
            }
            else
            {
                this->m_err = json_tokener_continue;
            }
            return false;
        }
        else
        {
            dest = c;
            return true;
        }
    }

    inline char
    ADVANCE_CHAR(std::string::size_type& idx, char c)
    {
    //  std::cout << std::string(20,'<') << "(" << idx << ", " << (tok)->char_offset << ")" << std::string(20, '>') << std::endl;
        ++idx;
        this->char_offset++;
        return c;
    }

    inline class json_node*
    json_tokener_parse_ex(const std::string& strtok, int len);
};

inline class json_node*
json_tokener::json_tokener_parse_ex(const std::string& strtok, int len)
{
    struct json_tokener *tok = this;
    class json_node *obj = NULL; // object in up layer
    char c = '\1';

    tok->char_offset = 0;
    tok->m_err = json_tokener_success;

    std::string::size_type idx = 0;
    while (tok->PEEP_CHAR(c, strtok[idx], len))
    {

redo_char:
        json_tokener_state &state = tok->stack[tok->m_depth].state;
        json_tokener_state &saved_state = tok->stack[tok->m_depth].saved_state;

        switch (state) {

        case json_tokener_state_eatws:
            /* advance until we change state */
            while (isspace(c))
            {
                if ((!tok->ADVANCE_CHAR(idx, c)) || (!tok->PEEP_CHAR(c, strtok[idx], len)))
                    goto out;
            }
            if (c == '/')
            {
                tok->m_buf.clear();
                tok->m_buf += c;
                state = json_tokener_state_comment_start;
            }
            else
            {
                state = saved_state;
                goto redo_char;
            }
            break;

        case json_tokener_state_start:
            switch(c)
            {
            case '{':
                state = json_tokener_state_eatws;
                saved_state = json_tokener_state_object_field_start;
                tok->stack[tok->m_depth].current = new json_object;
                break;
            case '[':
                state = json_tokener_state_eatws;
                saved_state = json_tokener_state_array;
                tok->stack[tok->m_depth].current = new json_array;
                break;
            case 'N':
            case 'n':
                state = json_tokener_state_null;
                tok->m_buf.clear();
                tok->st_pos = 0;
                goto redo_char;
            case '"':
            case '\'':
                state = json_tokener_state_string;
                tok->m_buf.clear();
                tok->quote_char = c;
                break;
            case 'T':
            case 't':
            case 'F':
            case 'f':
                state = json_tokener_state_boolean;
                tok->m_buf.clear();
                tok->st_pos = 0;
                goto redo_char;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '-':
                state = json_tokener_state_number;
                tok->m_buf.clear();
                tok->m_double = false;
                goto redo_char;
            default:
                tok->m_err = json_tokener_error_parse_unexpected;
                goto out;
            }
            break;

        case json_tokener_state_finish:
            if (tok->m_depth == 0)
                goto out;
            obj = tok->stack[tok->m_depth].current;
            tok->reset_level(tok->m_depth);
            tok->m_depth--;
            goto redo_char;

        case json_tokener_state_null:
            tok->m_buf += c;
            if (strncasecmp(json_null_str.c_str(), tok->m_buf.c_str(),
                        std::min(tok->st_pos+1, json_null_str.size())) == 0)
            {
                if (tok->st_pos == json_null_str.size())
                {
                    tok->stack[tok->m_depth].current = NULL;
                    saved_state = json_tokener_state_finish;
                    state = json_tokener_state_eatws;
                    goto redo_char;
                }
            }
            else
            {
                tok->m_err = json_tokener_error_parse_null;
                goto out;
            }
            tok->st_pos++;
            break;

        case json_tokener_state_comment_start:
            if (c == '*')
            {
                state = json_tokener_state_comment;
            }
            else if (c == '/')
            {
                state = json_tokener_state_comment_eol;
            }
            else
            {
                tok->m_err = json_tokener_error_parse_comment;
                goto out;
            }
            tok->m_buf += c;
            break;

        case json_tokener_state_comment:
            {
                /* advance until we change state */
                int idx_start = idx;
                while (c != '*')
                {
                    if (!tok->ADVANCE_CHAR(idx, c) || !tok->PEEP_CHAR(c, strtok[idx], len))
                    {
                        tok->m_buf.append(strtok.substr(idx_start, idx-idx_start));
                        goto out;
                    } 
                }
                tok->m_buf.append(strtok.substr(idx_start, 1+idx-idx_start));
                state = json_tokener_state_comment_end;
            }
            break;

        case json_tokener_state_comment_eol:
            {
                /* advance until we change state */
                int idx_start = idx;
                while (c != '\n')
                {
                    if (!tok->ADVANCE_CHAR(idx, c) || !tok->PEEP_CHAR(c, strtok[idx], len))
                    {
                        tok->m_buf.append(strtok.substr(idx_start, idx-idx_start));
                        goto out;
                    }
                }
                tok->m_buf.append(strtok.substr(idx_start, idx-idx_start));
            //  std::cout << "json_tokener_comment: " << tok->m_buf << std::endl;
                state = json_tokener_state_eatws;
            }
            break;

        case json_tokener_state_comment_end:
            tok->m_buf += c;
            if (c == '/')
            {
            //  std::cout << "json_tokener_comment: " << tok->m_buf << std::endl;
                state = json_tokener_state_eatws;
            }
            else
            {
                state = json_tokener_state_comment;
            }
            break;

        case json_tokener_state_string:
            {
                /* advance until we change state */
                int idx_start = idx;
                while (true)
                {
                    if (c == tok->quote_char)
                    {
                        tok->m_buf.append(strtok.substr(idx_start, idx-idx_start));
                        tok->stack[tok->m_depth].current = new json_string(tok->m_buf);
                        saved_state = json_tokener_state_finish;
                        state = json_tokener_state_eatws;
                        break;
                    }
                    else if (c == '\\')
                    {
                        tok->m_buf.append(strtok.substr(idx_start, idx-idx_start));
                        saved_state = json_tokener_state_string;
                        state = json_tokener_state_string_escape;
                        break;
                    }
                    if (!tok->ADVANCE_CHAR(idx, c) || !tok->PEEP_CHAR(c, strtok[idx], len))
                    {
                        tok->m_buf.append(strtok.substr(idx_start, idx-idx_start));
                        goto out;
                    }
                }
            }
            break;

        case json_tokener_state_string_escape:
            switch(c)
            {
            case '"':
            case '\\':
            case '/':
                tok->m_buf += c;
                state = saved_state;
                break;
            case 'b':
            case 'n':
            case 'r':
            case 't':
                if(c == 'b') tok->m_buf.append("\b", 1);
                else if(c == 'n') tok->m_buf.append("\n", 1);
                else if(c == 'r') tok->m_buf.append("\r", 1);
                else if(c == 't') tok->m_buf.append("\t", 1);
                state = saved_state;
                break;
            case 'u':
                tok->ucs_char = 0;
                tok->st_pos = 0;
                state = json_tokener_state_escape_unicode;
                break;
            default:
                tok->m_err = json_tokener_error_parse_string;
                goto out;
            }
            break;

        case json_tokener_state_escape_unicode:
            /* Note that the following code is inefficient for handling large
             * chunks of extended chars, calling append() once
             * for each multi-byte character of input.
             */
            {
                /* advance until we change state */
                while (true)
                {
                    if (json_hex_chars.find(c) != std::string::npos)
                    {
                        tok->ucs_char += ((unsigned int)hexdigit(c) << ((3-tok->st_pos++)*4));
                        if (tok->st_pos == 4)
                        {
                            unsigned char utf_out[3];
                            if (tok->ucs_char < 0x80)
                            {
                                utf_out[0] = tok->ucs_char;
                                tok->m_buf.append((char*)utf_out, 1);
                            }
                            else if (tok->ucs_char < 0x800)
                            {
                                utf_out[0] = 0xc0 | (tok->ucs_char >> 6);
                                utf_out[1] = 0x80 | (tok->ucs_char & 0x3f);
                                tok->m_buf.append((char*)utf_out, 2);
                            }
                            else
                            {
                                utf_out[0] = 0xe0 | (tok->ucs_char >> 12);
                                utf_out[1] = 0x80 | ((tok->ucs_char >> 6) & 0x3f);
                                utf_out[2] = 0x80 | (tok->ucs_char & 0x3f);
                                tok->m_buf.append((char*)utf_out, 3);
                            }
                            state = saved_state;
                            break;
                        }
                    }
                    else
                    {
                        tok->m_err = json_tokener_error_parse_string;
                        goto out;
                    }
                    if (!tok->ADVANCE_CHAR(idx, c) || !tok->PEEP_CHAR(c, strtok[idx], len))
                        goto out;
                }
            }
            break;

        case json_tokener_state_boolean:
            tok->m_buf += c;
            if (strncasecmp(json_true_str.c_str(), tok->m_buf.c_str(),
                        std::min(tok->st_pos+1, json_true_str.size())) == 0)
            {
                if (tok->st_pos == json_true_str.size())
                {
                    tok->stack[tok->m_depth].current = new json_boolean(true);
                    saved_state = json_tokener_state_finish;
                    state = json_tokener_state_eatws;
                    goto redo_char;
                }
            }
            else if (strncasecmp(json_false_str.c_str(), tok->m_buf.c_str(),
                        std::min(tok->st_pos+1, json_false_str.size())) == 0)
            {
                if (tok->st_pos == json_false_str.size())
                {
                    tok->stack[tok->m_depth].current = new json_boolean(false);
                    saved_state = json_tokener_state_finish;
                    state = json_tokener_state_eatws;
                    goto redo_char;
                }
            }
            else
            {
                tok->m_err = json_tokener_error_parse_boolean;
                goto out;
            }
            tok->st_pos++;
            break;

        case json_tokener_state_number:
            {
                /* advance until we change state */
                int idx_start = idx;
                int case_len = 0;
                while (c && json_number_chars.find(c) != std::string::npos)
                {
                    ++case_len;
                    if (c == '.' || c == 'e')
                        tok->m_double = true;
                    if (!tok->ADVANCE_CHAR(idx, c) || !tok->PEEP_CHAR(c, strtok[idx], len))
                    {
                        tok->m_buf.append(strtok.substr(idx_start, case_len));
                        goto out;
                    }
                }
                if (case_len > 0)
                    tok->m_buf.append(strtok.substr(idx_start, case_len));
            }
            {
            //  if (!tok->m_double && sscanf(tok->m_buf.c_str(), "%d", &numi) == 1)
                if (!tok->m_double) // integer type
                {
                    int numi = stdx::from_string<double>(tok->m_buf);
                    tok->stack[tok->m_depth].current = new json_int(numi);
                }
                else if (tok->m_double)
                {
                    double numd = stdx::from_string<double>(tok->m_buf);
                    tok->stack[tok->m_depth].current = new json_double(numd);
                }
                else
                {
                    tok->m_err = json_tokener_error_parse_number;
                    goto out;
                }
                saved_state = json_tokener_state_finish;
                state = json_tokener_state_eatws;
                goto redo_char;
            }
            break;

        case json_tokener_state_array:
            if (c == ']')
            {
                saved_state = json_tokener_state_finish;
                state = json_tokener_state_eatws;
            }
            else
            {
                if (tok->m_depth >= JSON_TOKENER_MAX_DEPTH-1)
                {
                    tok->m_err = json_tokener_error_depth;
                    goto out;
                }
                state = json_tokener_state_array_add;
                tok->m_depth++;
                tok->reset_level(tok->m_depth);
                goto redo_char;
            }
            break;

        case json_tokener_state_array_add:
            dynamic_cast<json_array*>(tok->stack[tok->m_depth].current)->push_back(obj);
            saved_state = json_tokener_state_array_sep;
            state = json_tokener_state_eatws;
            goto redo_char;

        case json_tokener_state_array_sep:
            if (c == ']')
            {
                saved_state = json_tokener_state_finish;
                state = json_tokener_state_eatws;
            }
            else if (c == ',')
            {
                saved_state = json_tokener_state_array;
                state = json_tokener_state_eatws;
            }
            else
            {
                tok->m_err = json_tokener_error_parse_array;
                goto out;
            }
            break;

        case json_tokener_state_object_field_start:
            if (c == '}')
            {
                saved_state = json_tokener_state_finish;
                state = json_tokener_state_eatws;
            }
            else if (c == '"' || c == '\'')
            {
                tok->quote_char = c;
                tok->m_buf.clear();
                state = json_tokener_state_object_field;
            }
            else
            {
                tok->m_err = json_tokener_error_parse_object_key_name;
                goto out;
            }
            break;

        case json_tokener_state_object_field:
            {
                /* advance until we change state */
                int idx_start = idx;
                while (true)
                {
                    if (c == tok->quote_char)
                    {
                        tok->m_buf.append(strtok.substr(idx_start, idx-idx_start));
                        tok->stack[tok->m_depth].obj_field_name = tok->m_buf;
                        saved_state = json_tokener_state_object_field_end;
                        state = json_tokener_state_eatws;
                        break;
                    }
                    else if (c == '\\')
                    {
                        tok->m_buf.append(strtok.substr(idx_start, idx-idx_start));
                        saved_state = json_tokener_state_object_field;
                        state = json_tokener_state_string_escape;
                        break;
                    }
                    if (!tok->ADVANCE_CHAR(idx, c) || !tok->PEEP_CHAR(c, strtok[idx], len)) {
                        tok->m_buf.append(strtok.substr(idx_start, idx-idx_start));
                        goto out;
                    }
                }
            }
            break;

        case json_tokener_state_object_field_end:
            if (c == ':')
            {
                saved_state = json_tokener_state_object_value;
                state = json_tokener_state_eatws;
            }
            else
            {
                tok->m_err = json_tokener_error_parse_object_key_sep;
                goto out;
            }
            break;

        case json_tokener_state_object_value:
            if (tok->m_depth >= JSON_TOKENER_MAX_DEPTH-1)
            {
                tok->m_err = json_tokener_error_depth;
                goto out;
            }
            state = json_tokener_state_object_value_add;
            tok->m_depth++;
            tok->reset_level(tok->m_depth);
            goto redo_char;

        case json_tokener_state_object_value_add:
            dynamic_cast<json_object*>(tok->stack[tok->m_depth].current)->insert(tok->stack[tok->m_depth].obj_field_name, obj);
            tok->stack[tok->m_depth].obj_field_name.clear();
            saved_state = json_tokener_state_object_sep;
            state = json_tokener_state_eatws;
            goto redo_char;

        case json_tokener_state_object_sep:
            if (c == '}')
            {
                saved_state = json_tokener_state_finish;
                state = json_tokener_state_eatws;
            }
            else if (c == ',')
            {
                saved_state = json_tokener_state_object_field_start;
                state = json_tokener_state_eatws;
            }
            else
            {
                tok->m_err = json_tokener_error_parse_object_value_sep;
                goto out;
            }
            break;

        }
        if (!tok->ADVANCE_CHAR(idx, c))
            goto out;
    } /* while(PEEP_CHAR) */

out:
    if (!c)
    {
        /* We hit an eof char (0) */
        if (tok->stack[tok->m_depth].state != json_tokener_state_finish &&
            tok->stack[tok->m_depth].saved_state != json_tokener_state_finish)
            tok->m_err = json_tokener_error_parse_eof;
    }

    if (tok->m_err == json_tokener_success)
        return tok->stack[tok->m_depth].current;
//  std::cout << "json_tokener_parse_ex: error " << json_tokener_errors[tok->m_err]
//       << " at offset " << tok->char_offset << std::endl;
    return NULL;
}

inline json_node* json_tokener_parse(const std::string& strtok)
{
    json_tokener tok;
    class json_node* obj = tok.json_tokener_parse_ex(strtok, -1);
    if (tok.m_err != json_tokener_success)
        obj = (class json_node*)error_ptr(-tok.m_err);
    return obj;
}

} // namespace stdx

#endif // __STDX_JSON_H

// vim:set tabstop=4 shiftwidth=4 expandtab:
