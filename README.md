
# Welcome to Asphalt!

  

**Asphalt** is an interpreted Turing-complete esoteric programming language, written in **C**.

Its interpreter is made to be as small as I can make it so language would still be readable (I managed to do it in 500 bytes)
Code itself is in ***src/small_main.c*** but you can find readable version with explanations here: ***src/main-manual.c*** (outdated, not ready yet, but still contains lots of info)
If you want to compile Asphalts interpreter locally *(for some reason)*, I would recommend you using `gcc` because other compilers may not work *(g++ doesn't compile it)*

**By the way, you can find Brainfuck, written in Asphalt in *examples/brainfuck.asph* (to run it you will need to rename it to "a")**


# How to run

You can run only code from file named "a", placed within the same folder as an interpreter itself
Just run the executable and hope for the best!

# Syntax

Worth mentioning:
----------------

Asphalt was designed to have *really small interpreter*, so it have *really weird rules*, do not let them scare you!

### Asphalt is democratic language!

You can do whatever you want, as far as

1. each line contains **5 words, 4 symbols each**,
2. **There is a line, witch starts with `ENDC` token** 
3. **THERE ARE NO EMPTY LINES**

Asphalt has 5 commands:

```
  PRNT
  JMPI
  SETV
  GETV
  ENDC
```

Command names should be the first word of the line. Otherwise they will be skipped.
Everything else will be considered *comment*. **Comments must also follow rules above.**

`ENDC` forces interpreter to stop reading code so **EVERYTHING** after it, will be considered as *comment*

Concept of *the void*
----------------------
Keyword `SETV` lets you change data on *the void* (array of 999 ints (yes not bytes but 32-bit integers))
Example of using `SETV`:

```
SETV 0001 0010 ++++ 0008

```
here we write `18` to second address.  (*the void* is 0-indexed) 
second token is the address on *the void*. 
Third token represents first operand, Fourth token is an operator and the last is second operand.
For the Operator to be recognised by Asphalt, *Second* symbol should be that operator
(why? -never ask me anything about Asphalt, I don't know how it works):
`SETV 0000 0012 +-/* 0013` - this is *minus* operator

To use data from *the void* you use `v`-syntax:
```
SETV 0010 v001 ++++ 0012
```
This line will put **sum** of *data from 2 address* and *12* to the 10-th address in *the void*

`v` followed by an address on *the void*, gives you data inside that address!
(actually, it works a bit differently - first byte is *control byte*,
if its representation according to **ASCII table** is greater than 57 (57 is '9' symbol), number will be taken as address
otherwise - actual number (control byte doesn't count) 
-it just would be so nice if everyone just used  `v`!)

Operators
--------------

```
SETV 0001 0010 ++++ 0008
SETV 0001 0010 (+.. 0008
SETV 0001 0010 %+$@ 0008
SETV 0001 0010 a+bc 0008
SETV 0001 0010 1+;( 0008
```

All this lines do the same, and it is true for all operators!
But for good readability **repeat same operator 4 times** (just like first example)

Asphalt supports following operators:
`+` plus
`-` minus
`/` divition
`*` multiplication
`%` mod
`<` less than
`!` logical NOT

You might have noticed that *Greater than, Logical AND, Logical OR, Is Equal To* are nowhere to be found.
That is because:
`a > b` is **the same** as `!(a <= b)`  
`a > b --->  !(a <= b)  --->  !(a < b OR a == b)` - We can implement that in our code!

*Logical OR* is missing. But that is not a problem at all, because for data to be **true** it just need not to be zero. 
`a || b` in Asphalt is `a + b`
Following this, *Logical AND* is `a * b` (because if you think about it, `0 && 1 => 0`; `0 * 1 => 0`;)

*Is Equal To* can be created like this:
`a == b` => `!(a - b)` because if `a` is actually equal to `b`, than `a - b = 0`, `!0 = 1`.

Finally, *logical NOT* is *unary* operator so second operand can be anything (doesn't matter):

```
SETV 0000 v123 !!!! abcd
```

### SETV cool feature

You can use data inside address **as address for another operation**:

```
SETV 0012 0005 ++++ 0000
SETV v012 0001 ++++ 0008
```

This line will put `9` into address `5`. If it doesn't make sense for you: First we put `5+0` to address `12`,  then we access value in `12` and get `5`. And finally we put `1+8` to `5`. 

## Print to console

Keyword `PRNT`(shorten from **P**rint **R**int **N**t **T**) is the only way to interact with user at runtime.
There are two types of formatting output:
```
PRNT CHAR 0118 .... ....
and_ sec_ _is_ prnt numb
PRNT numb 0118 .... ....
```
I use *UPPERCASE*	for keywords (the only right way) and *lowercase* for comments, and other stuff that interpreter doesn't care about. (2 line is comment)
So if next token after **PRNT** is **CHAR**, 3-rd token will be parsed and printed as ASCII symbol.
*First line will print `v` to console*
And if second token is something else, 3rd token will be printed as decimal integer.
*Second line will print `118` to console*
`skip` in the example is to fill up the line to 5 words (***always remember the rules***) and it is better to change it to "`" symbols as it is one of the smallest symbols, and code will be easier to read. (I will do it in next examples)

And we can also combine *the void* and **PRNT**:

```
PRNT numb v001 .... ....
```

## Complexity

**JMPI** is the 3rd keyword. Its syntax is very easy:
JMPI + number (0-> jump will not happend, else it will jump) + number (line of code to jump, lines are 0-indexed)

```
JMPI 0001 0003 .... ....
```

This line will always jump on the 3rd line of code when reached
We can use *the void* as any of the number (or both)

## Pointers and how to use them

Last keyword is `GETV`
It allows you to use addresses as Pointers

It is kinda hard to explain, but I'll try.

This is how `SETV` works under the hood:
`space(Parse(Token(1))) = Parse(Token(2)) 'operator' Parse(Token(4));`
`Parse` can return both number and data inside address (based on control byte)

Now `GETV` implementation:
`space(Parse(Token(1))) = space(Parse(Token(2)))`

Here is code example:
```
SETV 0001 0123 ++++ 0000
SETV 0002 0032 ++++ 0000
SETV 0003 0999 ++++ 0000
SETV 0004 0001 ++++ 0000
.... .... .... .... ....
SETV 0005 v005 ++++ 0001
SETV 0006 v005 <<<< 0004
PRNT numb v005 .... ....
PRNT CHAR 0032 .... ....
JMPI v006 0005 .... loop
ENDC .... .... .... ....
```
this code will print numbers from 1 to 4

Next example outputs "123 32 999 1":
```
SETV 0001 0123 ++++ 0000
SETV 0002 0032 ++++ 0000
SETV 0003 0999 ++++ 0000
SETV 0004 0001 ++++ 0000
.... .... .... .... ....
SETV 0005 v005 ++++ 0001
SETV 0006 v005 <<<< 0004
GETV 0007 v005 .... ....
PRNT numb v007 .... ....
PRNT CHAR 0032 .... ....
JMPI v006 0005 .... loop
ENDC .... .... .... ....
```

I really hope you got the point!
