// вернуть объект
const stats = (x, y) => (
{
	average: (x + y) / 2,
	distance: Math.abs(x - y)
}
);
x = stats(1, 9).average + stats(1, 9).distance;
console.log("type=" + typeof x, "\nvalue="+x);


