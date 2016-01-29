#include <string>
#include <stack>
#include <cctype>
#include <iostream>
#include <cassert>

using namespace std;

bool ltePrecedence(char op1, char op2);
bool evalChar(char op, const bool values[]);

int evaluate(string infix, const bool values[], string& postfix, bool& result)
// Evaluates a boolean expression
// Postcondition: If infix is a syntactically valid infix boolean
//   expression, then postfix is set to the postfix form of that
//   expression, result is set to the value of the expression (where
//   in that expression, each digit k represents element k of the
//   values array), and the function returns zero.  If infix is not a
//   syntactically valid expression, the function returns 1.  (In that
//   case, postfix may or may not be changed, but result must be
//   unchanged.)
{
	if (infix.empty())
		return 1;

	stack<char> ops;
	string post;
	int pos = 0;
	char prev = '(';
	char curr;

	while (pos != infix.size())
	{
		curr = infix[pos];
		switch (curr)
		{
		case '!':
		case '(':
			if (isdigit(prev)) //forgot about closing parenthesis you fool
				return 1;
			if (pos + 1 == infix.size())
				return 1;

			ops.push(curr);
			prev = curr;
			break;
		case ')':
			if (prev == '(' || prev == '|' || prev == '&' || prev == '!')
				return 1;
			if (ops.empty())
				return 1;

			while (ops.top() != '(')
			{
				post += ops.top();
				ops.pop();
			}
			ops.pop();
			prev = curr;
			break;
		case '|':
		case '&':
			if (!isdigit(prev) && prev != ')')
				return 1;
			if (pos + 1 == infix.size())
				return 1;

			while (!ops.empty() && ops.top() != '(' && ltePrecedence(curr, ops.top()))
			{
				post += ops.top();
				ops.pop();
			}
			ops.push(curr);
			prev = curr;
			break;
		default:
			if (isdigit(curr))
			{
				if (isdigit(prev) || prev == ')')
					return 1;

				post += curr;
				prev = curr;
			}
			else if (curr != ' ')
				return 1;
			break;
		}
		pos++;
	}
	while (!ops.empty())
	{
		if (ops.top() == '(')
			return 1;

		post += ops.top();
		ops.pop();
	}

	if (post.empty())
		return 1;

	pos = 0;
	while (pos != post.size())
	{
		curr = post[pos];
		if (curr == '!')
		{
			char op1 = ops.top();
			ops.pop();
			bool res1 = evalChar(op1, values);

			if (res1)
				ops.push('f');
			else
				ops.push('t');
		}
		else if (curr == '&' || curr == '|')
		{
			char op2 = ops.top();
			ops.pop();
			char op1 = ops.top();
			ops.pop();
			bool res2 = evalChar(op2, values);
			bool res1 = evalChar(op1, values);
			
			if (curr == '&')
				if (res2 && res1)
					ops.push('t');
				else
					ops.push('f');
			else if (curr == '|')
				if (res2 || res1)
					ops.push('t');
				else
					ops.push('f');
		}
		else
			ops.push(curr);
		pos++;
	}

	postfix = post;
	result = evalChar(ops.top(), values);
	return 0;
}

bool ltePrecedence(char op1, char op2)
{
	if (op1 == '|')
		return true;
	else if (op2 == '|')
		return false;
	if (op1 == '&')
		return true;
	else if (op2 == '&')
		return false;
	return true;
}

bool evalChar(char op, const bool values[])
{
	if (isdigit(op))
		return values[op - '0'];
	else
		if (op == 't')
			return true;
		else
			return false;
}

int main()
{
	bool ba[10] = {
		//  0      1      2      3      4      5      6      7      8      9
		true, true, true, false, false, false, true, false, true, false
	};
	string pf;
	bool answer;

	assert(evaluate("(0&5)(5&7)", ba, pf, answer) == 1);
	assert(evaluate("2", ba, pf, answer) == 0 && pf == "2" && answer);
	assert(evaluate("(3)", ba, pf, answer) == 0 && pf == "3" && !answer);
	assert(evaluate("2&(3)", ba, pf, answer) == 0 && pf == "23&" && !answer);
	assert(evaluate("0&!9", ba, pf, answer) == 0 && pf == "09!&" && answer);
	assert(evaluate("!(7|8)", ba, pf, answer) == 0 && pf == "78|!" && !answer);
	assert(evaluate("!7|8", ba, pf, answer) == 0 && pf == "7!8|" && answer);
	assert(evaluate("6|4&5", ba, pf, answer) == 0 && pf == "645&|" && answer);
	assert(evaluate("1&!(9|1&1|9) | !!!(9&1&9)", ba, pf, answer) == 0 && pf == "1911&|9|!&91&9&!!!|" && answer);
	assert(evaluate("b", ba, pf, answer) == 1);
	assert(evaluate("(0&1)))", ba, pf, answer) == 1);
	assert(evaluate("(0&1) & 3 && 3", ba, pf, answer) == 1);
	assert(evaluate("(0&1)3", ba, pf, answer) == 1);
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  answer);
	assert(evaluate("8|", ba, pf, answer) == 1);
	assert(evaluate("4 5", ba, pf, answer) == 1);
	assert(evaluate("01", ba, pf, answer) == 1);
	assert(evaluate("()", ba, pf, answer) == 1);
	assert(evaluate("2(9|8)", ba, pf, answer) == 1);
	assert(evaluate("2(&8)", ba, pf, answer) == 1);
	assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
	assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
		&& pf == "43!03&&|"  &&  !answer);
	assert(evaluate("", ba, pf, answer) == 1);
	assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9"  &&  !answer);
	ba[2] = false;
	ba[9] = true;
	assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9"  &&  answer);
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  !answer);
	cout << "Passed all tests" << endl;
}