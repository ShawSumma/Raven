# Parsing in Raven
---
## Expressions involving operators
An example of an expression using operators would be:

```(1 + 2) * 4 / 2```

This sort of expression would typically be seen on the right hand side of an assignment. In these expressions function calls would be considered a part of the AST but has been omitted in this previous example for clarity.

In Raven these expressions are transformed into reverse polish notation (postfix) using Dijkstra's [Shunting Yard Algorithm](https://en.wikipedia.org/wiki/Shunting-yard_algorithm). So for the case of the expression above the RPN would be:


```1 2 + 4 * 2 /```

In RPN there is no need for parentheses as each binay operator can only act on the previous two entries.

This process creates an Abstract Syntax Tree representation of the expression as it encounters new nodes.

## Expressions involving assignment
An assignment in Raven is signified by an assignment operator (e.g. `=`, `+=`, `-=`). The expression has two halves, a Left Hand Side and a Right Hand Side. On the LHS of an assignment is generally an identifer or memory location of which to store or mutate data as needed. On the RHS is usually an expression that may resemble the example given in the previous section.

As these expressions are split in two and one side having a well defined process for transformation, they could be split at the assignment operator with a indentifer or variable declation on the left and the AST generated from the section previous.

##