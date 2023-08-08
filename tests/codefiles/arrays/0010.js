x = NaN;

let a = [1,2,3,4,56,76,4,5,2,8]
for (const key in a) {
	if (Number.isNaN(x) || x < a[key])
		x = a[key];
}
console.log("type=" + typeof x + "\nvalue=" + x);

