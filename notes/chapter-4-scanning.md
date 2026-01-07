# Scanning

## Some Terms
**Scanner/Lexer:** takes in raw source code characters and groups them into a series of chunks called tokens.<br>
**REPL:** An interactive prompt. **R**ead a line of input, **E**valuate it, **P**rint it, **L**oop and do it all over again.


## Lexemes and Tokens

**Lexeme:** A group of characters. Just the raw source code.<br>
**Token:** Now imagine that the raw source code (lexeme) gets some useful information added to it. That's a token. 

### Goal
1. Scan through the list of characters and group them together into the smallest sequences that still represent something.
2. Categorise those groups of characters so they represent something meaningful.

### Example
suppose a line of code: `var language = "lox";`<br><br>
Its Lexemes would be:<br>
`var` `language` `=` `"lox"` `;`<br>

### Token Types
What type of Lexeme did the scanner group?<br>We do this at the time of scanning rather than parsing. Faster this way.

### Location Calculation for Errors
Some necessary info that needs to be in our Token class.

#### Storing the necessary info
1. offset from the beginning of the source file to the beginning of the lexeme.
2. size of the lexeme.

#### Conversion
convert offset to the column and line number when there is a need to display it.

## Regular Expressions

### Lexical Grammar
Rules that determine how a language groups characters into lexemes. <br>
In our case its a regular expression => [a-zA-Z_][a-zA-Z_0-9]* For identifiers.

### Maximal Munch
Resolving the reserved words and identifiers issue using maximal munch.

#### What is maximal munch?
Imagine 2 words `or` and `orchid`.<br>
My scanner scans `orchid`. Now since it has more letters matching `orchid` and not `or`, this scanned word is an identifier, and not a reserved keyword.

### Challenges
#### Question 4.1 Python and Haskell are not regular languages. What does that mean, and why aren’t they?
**Ans.** A regular language is `finite`. It has a limited number of states, so it can be represented by finite-automata.<br>
A non-regular language, on the other hand has arbitrary length. Which means that it can have infinite states. For such languages, Context-Free Grammar (CFG) can be used to represent them.

One of the reasons Python and Haskell are non-regular because the parantheses can be nested infinite number of times. Arbitrary length.

#### Question 4.2 How does "space" change the way code is parsed in the follow languages: CoffeeScript, Ruby, and the C preprocessor?

**CoffeeScript**
- Indentation matters
- Space used instead of comma to separate object and array literals.
- Implied function calls. No parentheses. Whitespace is used to imply function calls. like `print "something"`

**Ruby**
- Implied function calls.
- distinguishing between unary negation/minus operator and binary subtraction operator. `a-b` vs `a - b`.

**C Preprocessor**
- Macro function definition. `#define FUNC(a, b) (a + b)` is correct, but `#define FUNC (a, b) (a + b)` is not a function, but an object.
- There are some more, but ehhhh... I am too lazy to type about them.

#### Question 4.3 What can keeping/reading comments and whitespaces be useful for?
I just searched it, and I really liked the first result.
- Code formatting/linting: Linters can read the comments and whitespaces and keep the code clean-looking.