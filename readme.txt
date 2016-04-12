1.本答案引用了LPSolve Ver5.5 开源库，开源库代码在code/lib文件夹，编译后的静态库为code/liblpsolve55.a,未作修改。
(a)开源库版权信息：
lpsolve citation data
----------------------
Description     : Open source (Mixed-Integer) Linear Programming system
Language        : Multi-platform, pure ANSI C / POSIX source code, Lex/Yacc based parsing
Official name   : lp_solve (alternatively lpsolve)
Release data    : Version 5.1.0.0 dated 1 May 2004
Co-developers   : Michel Berkelaar, Kjell Eikland, Peter Notebaert
Licence terms   : GNU LGPL (Lesser General Public Licence)
Citation policy : General references as per LGPL
                  Module specific references as specified therein

(b)开源库下载地址：
https://sourceforge.net/projects/lpsolve/

2.可执行文件未用sdk编译，而是使用了batch55.sh，附在压缩文件根目录中。
3.code/lib文件夹还包含了自己编写的头文件defines.h。
4.可执行文件执行后，将在当前目录创建logs.txt，是原应在stdout输出的内容重定向得到。

