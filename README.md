# Lexical Analyzer

In this project, I implemented a lexical analyzer (scanner) for a specific programming language. The primary function of the lexical analyzer is to read the source code written in the language and convert it into a sequence of tokens, which are the basic building blocks of the language. Here's an overview of the key functionalities and the types of tokens the lexical analyzer can identify:

### Key Functionalities:

1. **Whitespace Handling**: Whitespaces are used to separate tokens and are not part of the tokens themselves.
2. **Comments**: Lines starting with a tilde (~) are treated as comments and ignored by the scanner, except when inside string literals or character literals.
3. **Token Types**:
   - **Brackets**: Recognizes single-character tokens for `()[]{}`.
   - **Number Literals**: Identifies both integer and floating-point numbers, treating all as a `NUMBER` token.
   - **Boolean Literals**: Recognizes `true` and `false` as `BOOLEAN` tokens.
   - **Character Literals**: Single Unicode characters enclosed in single quotes, identified as `CHAR` tokens.
   - **String Literals**: Sequences of characters enclosed in double quotes, identified as `STRING` tokens.
   - **Keywords**: Recognizes specific keywords (`define`, `let`, `cond`, `if`, `begin`) as their respective tokens.
   - **Identifiers**: Sequences that match the regular expression for identifiers and are not keywords, identified as `IDENTIFIER` tokens.
### Output
- The scanner outputs each token in the given code with their type and position with row and column no.
### Error Handling:
- The scanner outputs an error message if it encounters a lexically incorrect token, specifying the position and description of the error.
