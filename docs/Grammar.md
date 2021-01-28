# Raven Grammar Specifcation

```
program ::=
    ( <import-statement>    ||
      <type-redefinition>   ||
      <variable-definition>  ||
      <function-definition> || 
      <class-definition> )+

number-type ::=
    i8 || i16 || i32 || i64 || u8 || u16 ||
    u32 || u64 || f32 || f64 

number ::=
    [0-9]+ ( .[0-9]* )? ( <number-type> )? ||
    <hex-literal> || <binary-literal> ||
    <octal-literal>

hex-literal ::=
    ( 0x || 0X ) [0-9a-fA-F]+

binary-literal ::=
    ( 0b || 0B ) (0 || 1)+

octal-literal ::=
    0 [0-7]+

ident ::=
    [a-zA-Z_] ( [a-zA-Z0-9_] )*

char-literal ::=
    '<ascii-character>'

string-literal ::=
    "<ascii-character>+"

binary-operator ::=
    + || - || * || / || ^ || & || ( | ) ||
    = || % || < || > || <= || >= || ( || ) ||
    &&

expr ::=
    <number> || <ident> || 
    <expr> <binary-operator> <expr> ||
    <unary-operator> <expr> ||
    <func-call> || { (<expr>,)+ } ||
    <string-literal> || <char-literal> ||

function-call ::=
    <ident>(<parameter-list>)

method-call ::=
    (( this || <ident> ) .)+ <function-call>

body ::=
    ( <conditioal> || <variable-definition> ; || 
    <function-call> ; || <method-call> ; ||
    <return-statement> ; || <expr> ; )*

else-if ::=
    else if (<expr>+) { <body>* }

else ::=
    else { <body>* }

conditional ::=
    if (<expr>+) { <body>* } <else-if>* <else>?

type-ident ::=
    int8 || int16 || int32  || int64 ||
    uint8 || uint16 || uint32 || uint64 ||
    float32 || float64 || bool || <ident> ||
    char || string || & <type-ident> ||
    [ <type-ident> ]

assignment-operator ::=
    =  || += || -= || *= || /= || 
    != || &= || |=

import-statement ::=
    import <ident> ;

type-redefinition ::=
    type <ident> = <type-ident> ;

variable-definition ::=
    ( var || let ) <ident> ( : <type-ident> )? <assignemment>?

arg-list ::=
    (<ident>: <type-ident>,)*

parameter-list ::= 
    (<expr>,)*

function-definition ::=
    fn <ident>(<arg-list>) { <body> }

assignment ::=
    <assignment-operator> <expr>+ ;

access-modifier ::=
    public || protected || private

class-definition ::= 
    <access-modifier>? static? class <ident> ( ; || { <class-body> } )

class-body ::=
    ( <data-member> || <method-definition> || <constructor> )+

data-member ::=
    <access-modifier>? static? <variable-definition>

<method-definition> ::=
    <access-modifier>? static? <function-definition>

constructor ::=
    <ident>( <arg-list> ) { <function-body> }
```