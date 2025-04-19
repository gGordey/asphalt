// this is an expanded version of classicsmol interpreter, made so you can read this code and understand how exactely classicsmol works
// obviously it works a bit different because in original code I used logical expressions as 'if' statements
// but here, for readability I made code as understandable as I can.
// At the very end of this file you can read about some tricks I used to make the smaller and smaller
#include <stdio.h>
#define TOK(x) code[c+x] // doesnt exist in original code, here for more comfort
#define pint(l) (l>>8) % 16 + (l >> 16) % 16 * 10 + (l >> 24) % 16 * 100
// pint is made just to optimaze space usage, and dont repeat same code. It paeses 3 digits after control byte
#define PARSE(location) (c+location % 256 > 57 ? space[pint(location)] : pint(location))
// PARSE macro converts binary representation of nuber to decimal:
// 0x30323034 => 204 (go check ASCII table) 
// if the first symbol is bigger than 57 (57 is 0x39 or '9' symbol), macro will return what's inside that spot in space array
// P.S: actualy symbols read in this order: 3 2 1 4     why? because reading code that way saves space 
#define operator(o) PARSE(TOK(2))o PARSE(TOK(4)) // returns an expression with 'o' beeng operator (e.g. operator(%)  --->   5 % 2)
#define opname :TOK(3)>>24== // I am not going to use this macro here for goods of readabillily
#define W;while(d[c]!='NDCE') // this loop is used twice (reading from file and running code) so why dont we preprocessor it?
int c = 0, i = 0, ch_bff; // c <=> cursor_position
int code[9995];
int space[999];
int main(int argc, char** argv) { // here I decided to add file name as an argument so you dont have to name file 'main.smol' 
    if (argc == 1) {
        printf("no file given!");
        return -1;
    }
    FILE* f = fopen(*(argv+1), "r");
    // reading data from file
    // compressed version: 
    //  W(b=fgetc(f))>32&&(d[c]+=b<<24*i++)||(c+=i%4<1);
    while (TOK(0) != 'NDCE') {
        ch_bff = fgetc(f);
        if (ch_bff != ' ' && ch_bff != '\n') {
            TOK(0) += ch_bff << 24 * i++;
        } 
        else {
            if (i % 4 == 0) {
                ++c; 
            }
        }
    }
    fclose(f); // this line is kinda unnessesery, so original code doesnt have it
    c = 0;
    while (TOK(0) != 'NDCE') {
        // JMPI keyword
        // compressed version:
        //  $'MPIJ'&&P(1)&&(c=P(2)*5)
        if (TOK(0) == 'MPIJ' && PARSE(TOK(1)) > 0) { // Token is JMPI
            PARSE(TOK(1)) && (c = PARSE(TOK(2))*5);
        }
        // PRNT keyword
        // compressed:
        //  $'RNTP'&&printf(d[c+1]=='HARC'?"%c":"%d",P(2))
        if (TOK(0) == 'RNTP') { // Token is PRNT
            if (TOK(1) == 'HARC') // Next toket is CHAR
                printf("%c", PARSE(TOK(2)));
            else 
                printf("%d", PARSE(TOK(2)));
        }
        // SETV keyword
        // originaly:
        //  $'ETVS'&&(s[P(1)]=d[c+3]>>24==43?_(+)45?_(-)47?_(/)37?_(%)42?_(*)60?_(<)33?!P(2):0)
        // here  "_" is "operator" macro (a bit more optimized as you can see)
        if (TOK(0)=='ETVS') { // Token is SETV
            space[PARSE(TOK(1))] = 
            TOK(3)>>24 == '+' ? operator(+) :
            TOK(3)>>24 == '-' ? operator(-) :
            TOK(3)>>24 == '/' ? operator(/) :
            TOK(3)>>24 == '%' ? operator(%) :
            TOK(3)>>24 == '*' ? operator(*) :
            TOK(3)>>24 == '=' ? operator(==):
            TOK(3)>>24 == '&' ? operator(&&):
            TOK(3)>>24 == '<' ? operator(<) :
            TOK(3)>>24 == '>' ? operator(<) : // original code doesnt have this operator, but I added it here
            TOK(3)>>24 == '!' ? !PARSE(TOK(2)):
            0; // undefined operator returns 0 
        }
        / SETV keyword
        // small:
        //  $'ETVG'&&(s[P(1)]=s[P(2)]);
        if (TOK(0) =='ETVG') {
            /// .........
        }
        c += 5;
    }
}
/*

    Some of staff I used when golfing this code:

    First of all: Macros. They can replace code with with one symbol!
    For example "W" macro. It replaces `while(TOK(0)!=0x454e4443)` one symbol for 25. -24 bytes per one usage.
    Macro PARSE (P in original code) uses "pint", witch is another macro.
    TOK (T in original) is the most cammon macro is this code. It saves 4 bytes (in original code):
        T(1) is clearly less than *(d+c+1)  (d is code[])

    Second, You dont need to specify data type! compile will auto make it int! (yes, compile warns you but who cares)
    also you dont have to return value from int function.
    so this is the smallest counter we can get (25 bytes):
    c;main(){c++<10&&main();}

    This counter also have logical expression insted of `if` statement.
    It is third and last thing I want to talk about: Replacing `if` with `&&` and `?:`
        a && b && c && d
    lets have a look at this line: `a` will be checked is it true or not. If it is - `b` will be ckecked. If at some point expression is false
    we wont continue checking it.
    It can be writen like this:
        if (a) {
            if (b) {
                if (c) {
                    d;
                }
            }
        }
    `?:`  operator is simply `if` - `else`
    a ? b : c
    =
    if (a) {
        b
    }
    else {
        c
    }
*/