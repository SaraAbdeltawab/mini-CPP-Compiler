# Programming-Language-using-Lex-and-Yacc
Design and implementation of a simple programming language using the Lex and Yacc compiler generating package.
<h2>To Run assembler version Steps:</h2>
<ul>
<li type="1">bison --yacc parser.y -d</li>
<li type="1">flex lex.l</li>
<li type="1">g++ -c symboltable.cpp</li>
<li type="1">gcc -c lex.yy.c y.tab.c executionassembly.c</li>
<li type="1">g++ symboltable.o executionassembly.o lex.yy.o y.tab.o</li>
<li type="1">run the generated .exe file</li>
</ul>


<h2>To Run interpreter version Steps:</h2>
<ul>
<li type="1">bison --yacc parser.y -d</li>
<li type="1">flex lex.l</li>
<li type="1">g++ -c symboltable.cpp</li>
<li type="1">gcc -c lex.yy.c y.tab.c executioninterpreter.c</li>
<li type="1">g++ symboltable.o executioninterpreter.o lex.yy.o y.tab.o</li>
<li type="1">run the generated .exe file</li>
</ul>

<h2>To Run from GUI:</h2>
<ul>
<li type="1">npm install</li>
<li type="1">npm start</li>
</ul>