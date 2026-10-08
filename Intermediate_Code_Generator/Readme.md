# Intermediate_Code_Generator
## Date : 28/9
## Algorithm:

Start.
Read the arithmetic expression into expr.

Initialize the temporary variable counter:

temp = 1
Process parentheses:
Find the innermost ( and ).
Extract the expression inside them.
Remove the parentheses.
Repeat until no parentheses remain.
Process multiplication (*):
Search the expression for *.
Identify its left and right operands.

Generate:

t1 = left * right
Replace left * right in the expression with t1.
Increment the temporary counter.
Repeat until no * remains.
Process division (/) in the same way.
Process addition (+) in the same way.
Process subtraction (-) in the same way.

For every operation, print the generated three-address instruction:

temp = operand1 operator operand2
Continue until the complete expression has been reduced to a single result.
Stop.

## Output :
<img width="487" height="178" alt="image" src="https://github.com/user-attachments/assets/510dc428-1384-4cbd-9df9-6375fcf18b7a" />


