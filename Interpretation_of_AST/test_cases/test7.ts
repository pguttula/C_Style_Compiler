a = 5;
if (a < 5) {
	a = 7;
	b = 2;
} else {
	Println(a);
	b = a + 2;
}
Println(a, b);

if (1 == 1) {
	Println(111);
}
if (1 == 2) {
	Println(121);
}

if (1 == 1) {
	Println(211);
}
else {
	Println(210);
}
if (1 == 2) {
	Println(221);
}
else {
	Println(220);
}

if (1 == 1) Println(311);
else {
	Println(310);
}
if (1 == 2) Println(321);
else {
	Println(320);
}

if (1 == 1) {
	Println(411);
}
else Println(410);
if (1 == 2) {
	Println(421);
}
else Println(420);
