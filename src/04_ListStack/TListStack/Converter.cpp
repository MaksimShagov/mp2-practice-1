#include "Converter.h"

std::string Converter::ConvertToPostfixForm(const std::string & _expression)
{
  std::string *_operands = new std::string[_expression.length()];
  std::string buffer;
  int lengthOfExpression = _expression.length();
  int countOfOperators = 0;
  int countOfOperands = 0;
  int countOfLeftBrackets = 0;
  int countOfRightBrackets = 0;
  char lastSymbol = 0;
  int indexOfOperand = 0;

  for (int i = 0; i < lengthOfExpression; i++)
  {
    if (buffer.length() != 0)
    {
      if ((_expression[i] == '+') || (_expression[i] == '-')
        || (_expression[i] == '*') || (_expression[i] == '/')
        || (_expression[i] == '(') || (_expression[i] == ')')
        || (_expression[i] == ' '))
      {
        countOfOperands++;
        (*stackOfOperands).Push(buffer);
        {
          int k = 0;
          while ((k < indexOfOperand) &&
            (_operands[k++] != _operands[indexOfOperand])) {
          };
          if (k == indexOfOperand)
            _operands[indexOfOperand++] = buffer;
        }
        buffer.clear();
      }
    }

    if (_expression[i] == ' ');
    else if ((_expression[i] != '+') && (_expression[i] != '-')
      && (_expression[i] != '*') && (_expression[i] != '/')
      && (_expression[i] != '(') && (_expression[i] != ')'))
    {
      if (countOfOperands == (countOfOperators + 1))
      {
        ExceptionWrongExpression ex(__LINE__, __FILE__);
        throw ex;
      }

      buffer.push_back(_expression[i]);
      if (i == (lengthOfExpression - 1))
      {
        countOfOperands++;
        (*stackOfOperands).Push(buffer);
        {
          int k = 0;
          while ((k < indexOfOperand) &&
            (_operands[k++] != _operands[indexOfOperand])) {
          };
          if (k == indexOfOperand)
            _operands[indexOfOperand++] = buffer;
        }
        buffer.clear();
      }
    }
    else if (_expression[i] == '(')
    {
      if ((lastSymbol != '+') && (lastSymbol != '-')
        && (lastSymbol != '*') && (lastSymbol != '/')
        && (lastSymbol != ' ') && (lastSymbol != '(')
        && (lastSymbol != 0))
      {
        ExceptionWrongExpression ex(__LINE__, __FILE__);
        throw ex;
      }
      countOfLeftBrackets++;
      (*operators).Push(_expression[i]);
    }
    else if (_expression[i] == ')')
    {
      if ((lastSymbol == '+') || (lastSymbol == '-')
        || (lastSymbol == '*') || (lastSymbol == '/')
        || (lastSymbol == '('))
      {
        ExceptionWrongExpression ex(__LINE__, __FILE__);
        throw ex;
      }
      countOfRightBrackets++;
      while ((*operators).Top() != '(')
      {
        std::string tmp;
        try
        {
          tmp.push_back((*operators).Top());
          (*operators).Pop();
          (*stackOfOperands).Push(tmp);
        }
        catch (ExceptionEmptyStack const& e)
        {
          ExceptionWrongExpression ex(__LINE__, __FILE__);
          throw ex;
        }
      }
      (*operators).Pop();
    }
    else if ((*operators).IsEmpty())
    {
      if (lastSymbol == 0)
      {
        ExceptionWrongExpression ex(__LINE__, __FILE__);
        throw ex;
      }
      (*operators).Push(_expression[i]);
      countOfOperators++;
    }
    else if (getPriorityOfOperator(_expression[i]) > getPriorityOfOperator((*operators).Top()))
    {
      (*operators).Push(_expression[i]);
      countOfOperators++;
    }
    else
    {
      while ((!(*operators).IsEmpty()) && ((*operators).Top() != '(')
        && (getPriorityOfOperator(_expression[i]) <= getPriorityOfOperator((*operators).Top())))
      {
        std::string tmp;
        tmp.push_back((*operators).Top());
        (*operators).Pop();
        (*stackOfOperands).Push(tmp);
      }
      (*operators).Push(_expression[i]);
      countOfOperators++;
    }

    if (_expression[i] != ' ')
      lastSymbol = _expression[i];
  }

  if ((countOfOperands != countOfOperators + 1)
    || (countOfLeftBrackets != countOfRightBrackets))
  {
    ExceptionWrongExpression e(__LINE__, __FILE__);
    throw e;
  }

  while (!(*operators).IsEmpty())
  {
    std::string tmp;
    tmp.push_back((*operators).Top());
    (*operators).Pop();
    (*stackOfOperands).Push(tmp);
  }

  TArrayStack<std::string> reverseStackOfOperands(lengthOfExpression);
  for (; !(*stackOfOperands).IsEmpty(); (*stackOfOperands).Pop())
    reverseStackOfOperands.Push((*stackOfOperands).Top());

  int startPosition = 0;

  while (!reverseStackOfOperands.IsEmpty())
  {
    startPosition = postfixForm.length();
    postfixForm.insert(startPosition, reverseStackOfOperands.Top());
    reverseStackOfOperands.Pop();
    buffer = ' ';
    postfixForm.insert(postfixForm.length(), buffer);
  }

  {
    Converter::operands = new std::string[countOfOperands + 1]; //1 for " ", then we can forgot count of operands
    for (int i = 0; i < countOfOperands; i++)
      Converter::operands[i] = _operands[i];
    Converter::operands[countOfOperands] = " "; //"end of vecror"
    delete[] _operands;
  }

  return postfixForm;
}

double Converter::Calculate(const std::string & _postfixForm, const Variables& _var)
{
  std::string tmp;
  int value;

  for (int i = 0, j = 0; i < _postfixForm.length(); i++)
  {
    if ((_postfixForm[i] != ' '))
    {
      tmp.push_back(_postfixForm[i]);
    }
    else
    {
      if ((tmp != "*") && (tmp != "/") && (tmp != "+") && (tmp != "-"))
      {
        int idx = 0;
        while ((idx < _var.countOfVariables) && (_var.variables[idx++] != tmp));
        idx--;
        (*result).Push(_var.values[idx]);
      }
      else if (tmp == "*")
      {
        double b = (*result).Top(); (*result).Pop();
        double a = (*result).Top(); (*result).Pop();
        (*result).Push(a * b);
      }
      else if (tmp == "/")
      {
        double b = (*result).Top(); (*result).Pop();
        double a = (*result).Top(); (*result).Pop();
        if (b == 0)
        {
          ExceptionDivisionByZero e(__LINE__, __FILE__);
          throw e;
        }
        (*result).Push(a / b);
      }
      else if (tmp == "+")
      {
        double b = (*result).Top(); (*result).Pop();
        double a = (*result).Top(); (*result).Pop();
        (*result).Push(a + b);
      }
      else if (tmp == "-")
      {
        double b = (*result).Top(); (*result).Pop();
        double a = (*result).Top(); (*result).Pop();
        (*result).Push(a - b);
      }
      tmp.clear();
    }
  }
  return (*result).Top();
}

int Converter::getPriorityOfOperator(const char _operator)
{
  if ((_operator == '*') || (_operator == '/')) return 3;
  if ((_operator == '+') || (_operator == '-')) return 2;
  if (_operator == ')') return 1;
  return 0;
}

Converter::Converter(bool _isTListStack)
{
  if (_isTListStack == true)
  {
    operators = new TListStack<char>();
    stackOfOperands = new TListStack<std::string>();
    result = new TListStack<double>();
  }
  else
  {
    operators = new TArrayStack<char>(1024);
    stackOfOperands = new TArrayStack<std::string>(1024);
    result = new TArrayStack<double>(1024);
  }
}

Converter::~Converter()
{
  delete[] operands;
  delete stackOfOperands;
  delete operators;
  delete result;
}