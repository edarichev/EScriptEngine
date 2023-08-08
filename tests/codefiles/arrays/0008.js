x = NaN;

let a = [1,2,3,4,56,76,4,5,2,8]
for (i = 0; i < a.length; i++) {
	if (Number.isNaN(x) || x < a[i])
		x = a[i];
}
console.log("type=" + typeof x + "\nvalue=" + x);

