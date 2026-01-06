## Future Adds
- operators like bitwise shift, modulo, conditional operators.
- do while loops.
- for-in for each loops;
- I/O

## Some Terms
Expression: produce a value.<br>
Statement: produce an effect.<br>
First Class: real values. Functions in lox are first class. You can get their reference and store them, use them or whatever.

## Challenges
### Question 3.1 Any edge cases in lox?
**Ans.** I honestly do not know what to look for when the question says "something not covered in the chapter. Find edge cases". I tried running some code for 5 min and the closest thing I could find was 
- **division by 0**. Instead of givingan error (as one would expect) it just stored the value "Infinity" in the variable. I hope this is one of the correct answers.
- **escape sequence**. I cannot seem to use `\n` or `\t` in string literals as escape sequence characters. Instead it just prints them as 2 characters. Strange.

### Question 3.2 abababababa

### Question 3.3 missing features?
**Ans.** Arrays. I don't think its a part of the standard library, I cannot seem to figure out a way to make them either, but they are very useful. The ability to make arrays, and take it to higher dimensions is very important I believe. 
