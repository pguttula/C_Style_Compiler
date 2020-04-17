a = 5;
{
	{
		b = 6;
		{
			c = 7;
		}
		Println(b, c);
	}
	a = c;
	{
		Println(a, b);
	}
	{
		b = c;
	}
	c = a;
}
Println(c, 5);