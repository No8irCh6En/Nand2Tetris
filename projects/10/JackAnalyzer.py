import sys
import re
import os

class TerminalToken:
    """
    """
    def __init__(self, category, value):
        self.value = value
        self.type = category
        self.is_terminal = True
        if value == '<':
            self.form = "<" + category + "> &lt; </" + category + ">"
        elif value == '>':
            self.form = "<" + category + "> &gt; </" + category + ">"
        elif value == '&':
            self.form = "<" + category + "> &amp; </" + category + ">"
        else:
            self.form = "<" + category + "> " + value + " </" + category + ">"

class UnTerminalToken:
    """
    """
    def __init__(self, value, is_head):
        self.value = value
        self.is_terminal = False
        if is_head:
            self.form = '<' + value + '>'
        else:
            self.form = '</' + value + '>'
            
class JackTokenizer:
    """
    """
    def __init__(self, filename):
        self.tokens = []
        self.codes = []
        self.strings = []
        self.index = 0
        self.keywords = ['class', 'constructor', 'function', 
		'method', 'field', 'static', 'var', 'int', 'char',
		'boolean', 'void', 'true', 'false', 'null', 'this',
		'let', 'do', 'if', 'else', 'while', 'return']
        self.symbols = ['{', '}', '(', ')', '[', ']', '.',',',
		 ';', '+', '-', '*', '/', '&', '|', '<', '>','=', '~']
        assert filename[-4:] == "jack"
        self.file_name = filename[:-5]
        
        with open(filename, 'r') as fileIn:
            multi_comments = False
            for line in fileIn:
                _line = line.strip()
                if _line.find('/*') != -1:
                    start_pos = _line.find('/*')
                    end_pos = _line.find('*/')
                    if end_pos == -1:
                        multi_comments = True
                        _line = _line[:start_pos]
                    else:
                        _line = _line[:start_pos] + _line[end_pos+2:]
                if multi_comments:
                    if _line.find('*/') == -1:
                        continue
                    _line = _line[_line.find('*/')+2 :]
                    multi_comments = False
                    
                if len(_line) == 0 or _line[:2] == '//':
                    continue
                if _line.find('//') != -1:
                    _line = _line[:_line.find('//')]
                
                self.codes.append(_line.strip())

    
    def tokenize(self):
        """
        """
        self.strings = []
        self.index = 0
        for code in self.codes:
            next_quote = 0
            while code[next_quote:].find('\"') != -1:
                start_pos = code[next_quote:].find('\"')
                end_pos = start_pos + 1 + code[start_pos+1:].find('\"')
                self.strings.append(code[start_pos+1:end_pos])
                code = code[:start_pos] + code[end_pos:] 
                # print(code)
                next_quote = start_pos + 1
            code_list = code.split()
            for word in code_list:
                self.handle_word(word)  

    
    
    def handle_word(self, word):
        """
        should consider cases like: find() and ";
        """
        if word[0] in self.symbols:
            self.tokens.append(TerminalToken("symbol", word[0]))
            if len(word) == 1:
                return
            self.handle_word(word[1:])
        
        elif word[0] == '\"':
            self.tokens.append(TerminalToken("stringConstant", self.strings[self.index]))
            self.index += 1
            if len(word) == 1:
                return
            self.handle_word(word[1:])
        elif word[0].isdecimal():
            _int = word
            for idx,ch in enumerate(word[1:],1):
                if ch.isdecimal() == False:
                    _int = word[:idx]
                    break
            self.tokens.append(TerminalToken("integerConstant", _int))
            if len(_int) == len(word):
                return
            self.handle_word(word[idx:])
            
        else:
            for keyword in self.keywords:
                if word.find(keyword) == 0:
                    self.tokens.append(TerminalToken("keyword", keyword))
                    if len(keyword) == len(word):
                        return
                    self.handle_word(word[len(keyword):])
                    return
            identifier = word
            for idx,ch in enumerate(word):
                if ch in self.symbols:
                    identifier = word[:idx]
                    break
            self.tokens.append(TerminalToken("identifier", identifier))
            if len(identifier) == len(word):
                return
            self.handle_word(word[idx:])
    
            
class CompilationEngile:
    """
    """
    def __init__(self, tokens, outfile):
        self.tokens = tokens
        self.outfile = outfile
        self.size = len(tokens)
        self.outTokens = []
        self.cur_pos = 0
        self.classDec = ['static', 'field']
        self.funcDec = ['constructor', 'function', 'method']
        self.statements = ['let', 'if', 'while', 'do', 'return']
        self.op = ['+', '-', '*', '/', '&', '|', '<', '>', '=']
        self.unary_op =['-', '~']
        
    
    def run(self):
        """
        The out structure must be Class
        """
        self.outTokens = []
        self.cur_pos = 0
        self.CompileClass() 

        
    def next(self):
        """
        like operator++ in C++
        """
        if self.cur_pos >= self.size:
            return None
        _next = self.tokens[self.cur_pos]
        self.cur_pos += 1
        return _next
        
    def peek(self):
        """
        like operator++(int) in C++
        """
        if self.cur_pos >= self.size:
            return None
        _next = self.tokens[self.cur_pos]
        return _next
    
    def add(self, token):
        self.outTokens.append(token)
        
    
    def CompileClass(self):
        self.add(UnTerminalToken('class', True)) # <class>
        self.add(self.next()) # class
        self.add(self.next()) # className
        self.add(self.next()) # {
        while self.peek() is not None and self.peek().value in self.classDec:
            self.CompileClassVarDec()
        while self.peek() is not None and self.peek().value in self.funcDec:
            self.CompileSubroutine()
        self.add(self.next()) # }
        self.add(UnTerminalToken('class', False)) # </class>
            
            
    def CompileClassVarDec(self):
        self.add(UnTerminalToken('classVarDec', True)) 
        self.add(self.next()) # (static | field)
        self.add(self.next()) # type
        self.add(self.next()) # varName
        while self.peek().value == ',':
            self.add(self.next()) # ,
            self.add(self.next()) # varName
        self.add(self.next()) # ;
        self.add(UnTerminalToken('classVarDec', False))
        
    def CompileSubroutine(self):
        self.add(UnTerminalToken('subroutineDec', True))
        self.add(self.next()) # (constructor | function | method)
        self.add(self.next()) # (void | type)
        self.add(self.next()) # subroutineName
        self.add(self.next()) # '('
        self.CompileParameterList()
        self.add(self.next()) # ')'
        
        self.add(UnTerminalToken('subroutineBody', True))
        self.add(self.next()) # '{'
        while self.peek().value == 'var':
            self.CompileVarDec()
        self.CompileStatements()
        self.add(self.next()) # '}'
        self.add(UnTerminalToken('subroutineBody', False))
        self.add(UnTerminalToken('subroutineDec', False))
        
        
    def CompileParameterList(self):
        self.add(UnTerminalToken('parameterList', True))
        if self.peek().value != ')':
            self.add(self.next()) # type
            self.add(self.next()) # varName
        while self.peek().value == ',':
            self.add(self.next()) # ,
            self.add(self.next()) # type
            self.add(self.next()) # varName
        self.add(UnTerminalToken('parameterList', False))
    
    def CompileVarDec(self):
        self.add(UnTerminalToken('varDec', True))
        self.add(self.next()) # var
        self.add(self.next()) # type
        self.add(self.next()) # varName
        while self.peek().value == ',':
            self.add(self.next()) # ,
            self.add(self.next()) # varName
        self.add(self.next()) # ;
        self.add(UnTerminalToken('varDec', False))
        
        
    def CompileStatements(self):
        self.add(UnTerminalToken('statements', True))
        while self.peek().value in self.statements:
            v = self.peek().value
            if v == 'if':
                self.CompileIf()
            elif v == 'let':
                self.CompileLet()
            elif v == 'while':
                self.CompileWhile()
            elif v == 'do':
                self.CompileDo()
            else:
                self.CompileReturn()
        self.add(UnTerminalToken('statements', False))
        

    def CompileDo(self):
        self.add(UnTerminalToken('doStatement', True))
        self.add(self.next()) # do
        self.add(self.next()) # subroutineCall
        if self.peek().value == '(':
            self.add(self.next()) # '('
            self.CompileExpressionList()
            self.add(self.next()) # ')'
        elif self.peek().value == '.':
            self.add(self.next()) # '.'
            self.add(self.next()) # subroutineName
            self.add(self.next()) # (
            self.CompileExpressionList()
            self.add(self.next()) # )
        self.add(self.next()) # ;
        self.add(UnTerminalToken('doStatement', False))
    
    
    def CompileLet(self):
        self.add(UnTerminalToken('letStatement', True))
        self.add(self.next()) # let
        self.add(self.next()) # varName
        if self.peek().value == '[':
            self.add(self.next()) # '['
            self.CompileExpression()
            self.add(self.next()) # ']'
        self.add(self.next()) # =
        self.CompileExpression()
        self.add(self.next()) # ;
        self.add(UnTerminalToken('letStatement', False))
    
    
    def CompileWhile(self):
        self.add(UnTerminalToken('whileStatement', True))
        self.add(self.next()) # while
        self.add(self.next()) # (
        self.CompileExpression()
        self.add(self.next()) # )
        self.add(self.next()) # {
        self.CompileStatements()
        self.add(self.next()) # }
        self.add(UnTerminalToken('whileStatement', False))
        
    
    def CompileReturn(self):
        self.add(UnTerminalToken('returnStatement', True))
        self.add(self.next()) # return
        if self.peek().value != ';':
            self.CompileExpression()
        self.add(self.next()) # ;
        self.add(UnTerminalToken('returnStatement', False))
        
    
    def CompileIf(self):
        self.add(UnTerminalToken('ifStatement', True))
        self.add(self.next()) # if
        self.add(self.next()) # (
        self.CompileExpression()
        self.add(self.next()) # )
        self.add(self.next()) # {
        self.CompileStatements()
        self.add(self.next()) # }
        if self.peek().value == 'else':
            self.add(self.next()) # else
            self.add(self.next()) # {
            self.CompileStatements()
            self.add(self.next()) # }
        self.add(UnTerminalToken('ifStatement', False))
        
        
    def CompileExpression(self):
        self.add(UnTerminalToken('expression', True))
        self.CompileTerm()
        while self.peek().value in self.op:
            self.add(self.next()) # op
            self.CompileTerm()
        self.add(UnTerminalToken('expression', False))
        
        
    def CompileTerm(self):
        self.add(UnTerminalToken('term', True))	
        if self.peek().value == '(':
            self.add(self.next()) # (
            self.CompileExpression()
            self.add(self.next()) # )
        elif self.peek().value in self.unary_op:
            self.add(self.next()) # - or ~
            self.CompileTerm()
        else:
            self.add(self.next())
            if self.peek().value == '[':
                self.add(self.next()) # '['
                self.CompileExpression()
                self.add(self.next()) # ']'
            elif self.peek().value == '(':
                self.add(self.next()) # '('
                self.CompileExpressionList()
                self.add(self.next()) # ')'
            elif self.peek().value == '.':
                self.add(self.next()) # '.'
                self.add(self.next()) # subroutineName
                self.add(self.next()) # (
                self.CompileExpressionList()
                self.add(self.next()) # )
        self.add(UnTerminalToken('term', False))
        
        
    def CompileExpressionList(self):
        self.add(UnTerminalToken('expressionList', True))
        if self.peek().value != ')':
            self.CompileExpression()
            while self.peek().value == ',':
                self.add(self.next()) # ,
                self.CompileExpression()
        self.add(UnTerminalToken('expressionList', False))


    def save(self):
        with open(self.outfile,'w') as fileOut:
            for token in self.outTokens:
                fileOut.write(token.form + '\n')
    
    
    
class JackAnalyzer:
    """
    """
    def __init__(self, inputFile):
        assert inputFile[-4:] == "jack"
        self.input_file = inputFile
        self.file_name = inputFile[:-5]
        self.output_file = self.file_name + '_alz.xml'
        
        
    def analyze(self):
        tokenizer = JackTokenizer(self.input_file)
        # print(tokenizer.codes)
        print("JackTokenizer initialized.")
        tokenizer.tokenize()
        # for token in tokenizer.tokens:
        #     print(token.form)
        print("Tokenization completed.")
        compileEngine = CompilationEngile(tokenizer.tokens, self.output_file)
        print("CompileEngine initialized.")
        compileEngine.run()
        # for otoken in compileEngine.outTokens:
        #     print(otoken.form)
        print("Compilization completed.")
        compileEngine.save()
        print("Data saved.")
        
        
def main():
    fileIn = sys.argv[1]
    JackAnalyzer(fileIn).analyze()
    

if __name__ == '__main__':
    main()
    