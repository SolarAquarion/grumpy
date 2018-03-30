// This is a sample extension to GrumpyChat
// You can load this using command /grumpy.script /path/to/file.js
// Use this file as a reference to functions available

// These functions are all mandatory
function ext_init()
{
    grumpy_log("Hello there");
    return true;
}

function ext_is_working()
{
    return true;
}

function ext_get_name()
{
    return "Sample extension";
}

function ext_get_desc()
{
    return "This is just a sample extension, it doesn't do much useful";
}

function ext_get_version()
{
    return "1.0.0";
}

function ext_get_author()
{
    return "Petr Bena";
}

// function reference
//
// grumpy_debug_log(text, verbosity) // writes to debug log
// grumpy_error_log(text) // writes to error log
// grumpy_log(text) // writes to current scrollback
