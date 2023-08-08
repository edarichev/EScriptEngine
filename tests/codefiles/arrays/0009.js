x = NaN;

let a = [1,2,3,4,56,76,4,5,2,8]
for (const b of a) {
	if (Number.isNaN(x) || x < b)
		x = b;
}
console.log("type=" + typeof x + "\nvalue=" + x);

