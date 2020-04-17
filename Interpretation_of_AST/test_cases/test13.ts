a = 0;
d = 0;
while (a < 10) {
	if (d == 0) {
		d = 1;
		b = 0;
		c = 0;
		e = 0;
		while (b < a) {
			b = b + 1;
			if (e == 0) {
				c = c + b;
				e = 1;
			}
			else {
				e = 0;
			}
		}
		Println(c);
	}
	else {
		d = 0;
	}
	a = a + 1;
}
