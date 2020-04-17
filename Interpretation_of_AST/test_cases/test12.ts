a = 0;
while (a < 10) {
	b = 0;
	c = 0;
	while (b < a) {
		b = b + 1;
		c = c + b;
	}
	Println(c);
	a = a + 1;
}
