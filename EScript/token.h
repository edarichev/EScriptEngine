/**
 * @file token.h
 * @brief Определение токенов
 */
#ifndef TOKEN_H
#define TOKEN_H

namespace escript {

/**
 * @brief Токен
 */
enum class Token
{
    Eof = 0, // Конец потока
    Break, // break
    Case, // case
    Catch, // catch
    Class, // class
    Const, // const
    Continue, // continue
    Debugger, // debugger
    Default, // default
    Delete, // delete
    Do, // do
    Else, // else
    Enum, // enum
    Export, // export
    Extends, // extends
    Finally, // finally
    For, // for
    Function, // function
    If, // if
    Implements, // implements
    Import, // import
    In, // in
    Instanceof, // instanceof
    Interface, // interface
    Let, // let
    New, // new
    Package, // package
    Private, // private
    Protected, // protected
    Public, // public
    Return, // return
    Static, // static
    Super, // super
    Switch, // switch
    This, // this
    Throw, // throw
    Try, // try
    Typeof, // typeof
    Var, // var
    Void, // void
    While, // while
    With, // with
    Yield, // yield
};

} // namespace escript

#endif // TOKEN_H
