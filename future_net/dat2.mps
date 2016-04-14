NAME	case
ROWS
 N	obj
 E	C(I.001)
 E	C(O.001)
 E	C(I.002)
 E	C(O.002)
 E	C(I.003)
 E	C(O.003)
 E	C(I.004)
 E	C(O.004)
COLUMNS
 X(000)		obj		1
 X(000)		C(O.001)	1
 X(000)		C(I.002)	1
 X(001)		obj		2
 X(001)		C(O.001)	1
 X(001)		C(I.003)	1
 X(002)		obj		1
 X(002)		C(O.001)	1
 X(002)		C(I.004)	1
 X(003)		obj		3
 X(003)		C(O.003)	1
 X(003)		C(I.002)	1
 X(004)		obj		1
 X(004)		C(O.004)	1
 X(004)		C(I.002)	1
 X(005)		obj		1
 X(005)		C(O.003)	1
 X(005)		C(I.004)	1
 X(006)		obj		1
 X(006)		C(O.004)	1
 X(006)		C(I.003)	1
RHS
 rhs		C(I.001)	0
 rhs		C(O.001)	1
 rhs		C(I.002)	1
 rhs		C(O.002)	0
 rhs		C(I.003)	1
 rhs		C(O.003)	1
 rhs		C(I.004)	1
 rhs		C(O.004)	1
BOUNDS
 BV bnd		X(000)
 BV bnd		X(001)
 BV bnd		X(002)
 BV bnd		X(003)
 BV bnd		X(004)
 BV bnd		X(005)
 BV bnd		X(006)
ENDATA