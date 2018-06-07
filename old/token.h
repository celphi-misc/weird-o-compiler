#ifndef TOKEN_H__
#define TOKEN_H__

enum yytokentype {
    SEM = 0x0000,   // ;
    ID  = 0x0001,   // identifier
    VAR = 0x0002,   // var
    FUN = 0x0003,   // function
    ASN = 0x0004,   // =
    RTN = 0x0005,   // return

    IF  = 0x0010,   // if
    ELS = 0x0011,   // else
    SWH = 0x0012,   // switch
    CAS = 0x0013,   // case
    DFT = 0x0014,   // default

    FOR = 0x0020,   // for
    WHL = 0x0021,   // while
    DO  = 0x0022,   // do
    BRK = 0x0023,   // break
    CTN = 0x0024,   // continue
    GTO = 0x0025,   // goto

    FLS = 0x0030,   // false
    TRU = 0x0031,   // true

    // Literals
    INT = 0x0050,   // int
    FLT = 0x0051,   // float
    STR = 0x0052,   // string

    // ()
    LPAREN = 0x0060,
    RPAREN = 0x0061,
    // []
    LBRACKET = 0x0062,
    RBRACKET = 0x0063,
    // {}
    LBRACE = 0x0064,
    RBRACE = 0x0065,

    // Arithmetics
    ADD = 0x0070,
    SUB = 0x0071,
    MUL = 0x0072,
    DIV = 0x0073,
    MOD = 0x0074,
    ADD_ASN = 0x0170,
    SUB_ASN = 0x0171,
    MUL_ASN = 0x0172,
    DIV_ASN = 0x0173,
    MOD_ASN = 0x0174,
    INC = 0x0270,
    DEC = 0x0271,
    // Bitwise
    B_AND = 0x0075,
    B_XOR = 0x0076,
    B_OR  = 0x0077,
    B_R   = 0x0078,
    B_L   = 0x0079,
    B_NOT = 0x007A,
    B_AND_ASN = 0x0175,
    B_XOR_ASN = 0x0176,
    B_OR_ASN  = 0x0177,
    B_R_ASN   = 0x0178,
    B_L_ASN   = 0x0179,

    // Comparison
    LEQ = 0x0080,
    GEQ = 0x0081,
    LE  = 0x0082,
    GT  = 0x0083,
    EQ  = 0x0084,
    NEQ = 0x0085,
    // Logical
    AND = 0x0086,
    OR  = 0x0087,
    NOT = 0x0088,

    // Special
    // PRT = 0x0090,
    DOT = 0x0091,   // .
    CLN = 0x0092,   // :
    QUE = 0x0093,   // ?
    CMA = 0x0094,   // ,
};

#endif
